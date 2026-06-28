#!/usr/bin/env python3
"""
generate_coding_standard.py

Gera as secoes derivadas de _governance/CODING_STANDARD.md e os arquivos
firmware/src/<modulo>/<modulo>_config.h a partir de spec/firmware_constants.json.

Modos de uso:
  python scripts/generate_coding_standard.py
      Gera e escreve as secoes no documento e os arquivos _config.h. Valida cascata.

  python scripts/generate_coding_standard.py --check
      So valida: verifica cascata (valores vs specs), sync do documento e sync dos
      arquivos _config.h. Nao escreve nada. Retorna exit code 1 se houver divergencia.

Cascata: para cada constante em firmware_constants.json, le o valor real
no spec JSON e compara com o campo 'valor'. Qualquer divergencia e erro.

Sem hardcoded: todos os nomes, valores e estruturas vem de firmware_constants.json.
"""

import json
import re
import sys
from collections import defaultdict
from pathlib import Path

REPO_ROOT = Path(__file__).parent.parent
CONSTANTS_FILE = REPO_ROOT / "spec" / "firmware_constants.json"
CODING_STANDARD = REPO_ROOT / "_governance" / "CODING_STANDARD.md"

MARKER_BEGIN = "<!-- BEGIN GENERATED:{key} -->"
MARKER_END   = "<!-- END GENERATED:{key} -->"


# ---------------------------------------------------------------------------
# Resolucao de caminho JSON (ex: "mapeamento_gpios[0].gpio")
# ---------------------------------------------------------------------------

def resolver_campo(dados, caminho):
    """
    Navega em um objeto JSON usando notacao de caminho.
    Suporta:
      chave simples:        "threshold"
      chave aninhada:       "deteccao.threshold"
      indice de array:      "mapeamento_gpios[0].gpio"
      combinado:            "timings.intervalo_ms.valor_padrao"
    """
    tokens = re.split(r'\.', caminho)
    atual = dados
    for token in tokens:
        m = re.match(r'^(\w+)\[(\d+)\]$', token)
        if m:
            chave = m.group(1)
            idx   = int(m.group(2))
            atual = atual[chave][idx]
        else:
            atual = atual[token]
    return atual


# ---------------------------------------------------------------------------
# Validacao de cascata
# ---------------------------------------------------------------------------

def validar_cascata(constantes):
    """
    Para cada constante DERIVADO, le o valor real no spec JSON
    e compara com o campo 'valor' em firmware_constants.json.
    Retorna lista de erros.
    """
    erros = []
    spec_cache = {}

    for c in constantes:
        spec_arquivo = c["spec_arquivo"]
        spec_campo   = c["spec_campo"]
        valor_mapeado = c["valor"]

        spec_path = REPO_ROOT / spec_arquivo
        if not spec_path.exists():
            erros.append(
                f"  ERRO   {c['constante']}: spec '{spec_arquivo}' nao encontrado"
            )
            continue

        if spec_arquivo not in spec_cache:
            with open(spec_path, encoding="utf-8") as f:
                spec_cache[spec_arquivo] = json.load(f)

        try:
            valor_real = resolver_campo(spec_cache[spec_arquivo], spec_campo)
        except (KeyError, IndexError, TypeError) as e:
            erros.append(
                f"  ERRO   {c['constante']}: campo '{spec_campo}' nao encontrado "
                f"em '{spec_arquivo}' ({e})"
            )
            continue

        if valor_real != valor_mapeado:
            erros.append(
                f"  ERRO   {c['constante']}: valor mapeado={repr(valor_mapeado)} "
                f"mas spec diz={repr(valor_real)} "
                f"({spec_arquivo}#{spec_campo})"
            )

    return erros


# ---------------------------------------------------------------------------
# Geracao de conteudo markdown
# ---------------------------------------------------------------------------

def formatar_valor(valor, tipo_c):
    if valor is None:
        return "(determinar na implementacao)"
    if isinstance(valor, str):
        return f'`"{valor}"`'
    return str(valor)


def gerar_arvore_diretorios(modulos):
    """Gera arvore de diretorios firmware/src/ a partir da lista de modulos."""
    linhas = [
        "```",
        "firmware/",
        "├── platformio.ini",
        "├── src/",
        "│   ├── main.cpp",
    ]
    n = len(modulos)
    for i, m in enumerate(modulos):
        tem_teste = m["tem_teste_native"]
        eh_ultimo = (i == n - 1)
        prefixo_dir    = "│   └──" if eh_ultimo else "│   ├──"
        prefixo_filho  = "│       " if eh_ultimo else "│   │   "
        linhas.append(f"{prefixo_dir} {m['nome']}/")
        linhas.append(f"{prefixo_filho}├── {m['nome']}_config.h")
        linhas.append(f"{prefixo_filho}├── {m['nome']}.h")
        linhas.append(f"{prefixo_filho}└── {m['nome']}.cpp")
    linhas.append("└── test/")

    modulos_com_teste = [m["nome"] for m in modulos if m["tem_teste_native"]]
    n_t = len(modulos_com_teste)
    for i, nome in enumerate(modulos_com_teste):
        prefixo = "    └──" if i == n_t - 1 else "    ├──"
        linhas.append(f"{prefixo} test_{nome}/")

    linhas.append("```")
    return "\n".join(linhas)


def gerar_tabela_modulo(constantes_modulo):
    """Gera tabela markdown para as constantes de um modulo."""
    linhas = [
        "| Constante | Valor | Tipo C | Classe | Origem JSON |",
        "|---|---|---|---|---|"
    ]
    for c in constantes_modulo:
        calibrar = " [CALIBRAR]" if c.get("calibrar") else ""
        valor    = formatar_valor(c["valor"], c["tipo_c"])
        origem   = f"`{c['spec_arquivo']}#{c['spec_campo']}`"
        linhas.append(
            f"| `{c['constante']}` | {valor} | `{c['tipo_c']}` "
            f"| {c['classe']}{calibrar} | {origem} |"
        )
    return "\n".join(linhas)


def gerar_tabela_dm(decisoes):
    """Gera tabela de decisoes manuais inerentes."""
    linhas = [
        "| # | Constante | Modulo | Tipo C | Justificativa |",
        "|---|---|---|---|---|"
    ]
    for d in decisoes:
        linhas.append(
            f"| {d['id']} | `{d['constante']}` | {d['modulo']} "
            f"| `{d['tipo_c']}` | {d['justificativa']} |"
        )
    return "\n".join(linhas)


# ---------------------------------------------------------------------------
# Geracao de _config.h
# ---------------------------------------------------------------------------

def formatar_valor_c(valor, tipo_c):
    """
    Retorna o literal C++ adequado para o valor.
      - const char* : string entre aspas duplas
      - tipo nao-char* + valor string : simbolo sem aspas (ex: ADC_ATTEN_DB_11)
      - numerico : str(valor)
      - None : None (chamador decide o que fazer)
    """
    if valor is None:
        return None
    if tipo_c == "const char*":
        return f'"{valor}"'
    if isinstance(valor, str):
        return valor  # simbolo de plataforma/biblioteca
    return str(valor)


def gerar_config_h(modulo_meta, constantes_modulo, decisoes_modulo):
    """
    Gera o conteudo completo de <modulo>_config.h conforme
    CODING_STANDARD.md#estrutura-config-h.
    """
    nome = modulo_meta["nome"]
    linhas = [
        "#pragma once",
        "",
        f"// {nome}_config.h",
        f"// Fonte: spec/{nome}/{nome}.json (via spec/firmware_constants.json)",
        "// Nenhum valor neste arquivo e inventado.",
        "// Toda alteracao exige atualizacao de firmware_constants.json e regeneracao.",
        "",
        "#include <stdint.h>",
    ]

    for c in constantes_modulo:
        linhas.append("")
        linhas.append(f"// --- DERIVADO: {c['spec_arquivo']}#{c['spec_campo']} ---")
        val = formatar_valor_c(c["valor"], c["tipo_c"])
        calibrar = "  // [CALIBRAR] — confirmar apos prototipagem" if c.get("calibrar") else ""
        linhas.append(f"constexpr {c['tipo_c']} {c['constante']} = {val};{calibrar}")

    for d in decisoes_modulo:
        linhas.append("")
        linhas.append(f"// --- HARDCODED JUSTIFICADO: {d['id']} ---")
        linhas.append(f"// {d['justificativa']}")
        val = formatar_valor_c(d["valor"], d["tipo_c"])
        if d.get("tipo_plataforma"):
            linhas.append(
                f"// [PLATAFORMA] constexpr {d['tipo_c']} {d['constante']} = {val};"
            )
            linhas.append(
                f"// Declarar em {d['modulo']}.cpp apos includes de plataforma."
            )
        elif val is None:
            linhas.append(f"// [DETERMINAR] constexpr {d['tipo_c']} {d['constante']} = ???;")
        else:
            linhas.append(f"constexpr {d['tipo_c']} {d['constante']} = {val};")

    linhas.append("")
    return "\n".join(linhas)


# ---------------------------------------------------------------------------
# Manipulacao do documento
# ---------------------------------------------------------------------------

def substituir_secao(conteudo, key, novo_conteudo):
    """Substitui conteudo entre marcadores. Retorna (novo_conteudo, sucesso)."""
    begin = MARKER_BEGIN.format(key=key)
    end   = MARKER_END.format(key=key)
    i_begin = conteudo.find(begin)
    i_end   = conteudo.find(end)
    if i_begin == -1 or i_end == -1:
        return conteudo, False
    antes  = conteudo[:i_begin + len(begin)]
    depois = conteudo[i_end:]
    return antes + "\n" + novo_conteudo + "\n" + depois, True


def extrair_secao(conteudo, key):
    """Extrai conteudo entre marcadores."""
    begin = MARKER_BEGIN.format(key=key)
    end   = MARKER_END.format(key=key)
    i_begin = conteudo.find(begin)
    i_end   = conteudo.find(end)
    if i_begin == -1 or i_end == -1:
        return None
    return conteudo[i_begin + len(begin):i_end].strip()


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    modo_check = "--check" in sys.argv

    if not CONSTANTS_FILE.exists():
        print(f"AVISO: {CONSTANTS_FILE} nao encontrado. Geracao ignorada.")
        sys.exit(0)

    with open(CONSTANTS_FILE, encoding="utf-8") as f:
        dados = json.load(f)

    modulos    = dados["modulos"]
    constantes = dados["constantes"]
    decisoes   = dados["decisoes_manuais_inerentes"]

    # --- Validacao de cascata ---
    print("Validando cascata (firmware_constants.json vs spec JSONs)...")
    erros_cascata = validar_cascata(constantes)
    if erros_cascata:
        print("\nDivergencias de cascata detectadas:")
        for e in erros_cascata:
            print(e)
        print("\nCausa: um ou mais spec JSONs foram alterados sem atualizar")
        print("firmware_constants.json. Atualize o arquivo e regenere.")
        sys.exit(1)
    print(f"  OK: {len(constantes)} constantes validadas contra specs.")

    # --- Agrupamento por modulo (ordem preservada) ---
    por_modulo = defaultdict(list)
    modulos_vistos = []
    for c in constantes:
        m = c["modulo"]
        if m not in por_modulo:
            modulos_vistos.append(m)
        por_modulo[m].append(c)

    decisoes_por_modulo = defaultdict(list)
    for d in decisoes:
        decisoes_por_modulo[d["modulo"]].append(d)

    # --- Geracao dos conteudos ---
    arvore = gerar_arvore_diretorios(modulos)
    tabelas = {m["nome"]: gerar_tabela_modulo(por_modulo[m["nome"]]) for m in modulos}
    tabela_dm = gerar_tabela_dm(decisoes)
    config_hs = {
        m["nome"]: gerar_config_h(m, por_modulo[m["nome"]], decisoes_por_modulo[m["nome"]])
        for m in modulos
    }

    if not CODING_STANDARD.exists():
        print(f"ERRO: {CODING_STANDARD} nao encontrado.")
        sys.exit(1)

    conteudo_atual = CODING_STANDARD.read_text(encoding="utf-8")

    # --- Modo check: comparar sem escrever ---
    if modo_check:
        print("Verificando sync de CODING_STANDARD.md...")
        erros_sync = []
        secoes = {"arvore_diretorios": arvore, "decisoes_manuais": tabela_dm}
        for m in modulos:
            secoes[f"tabela_{m['nome']}"] = tabelas[m["nome"]]

        for key, esperado in secoes.items():
            atual = extrair_secao(conteudo_atual, key)
            if atual is None:
                erros_sync.append(f"  ERRO   marcador '{key}' ausente em CODING_STANDARD.md")
            elif atual.strip() != esperado.strip():
                erros_sync.append(
                    f"  ERRO   secao '{key}' desatualizada. "
                    f"Rodar: python scripts/generate_coding_standard.py"
                )

        if erros_sync:
            print("\nCODING_STANDARD.md fora de sync com firmware_constants.json:")
            for e in erros_sync:
                print(e)
            sys.exit(1)
        print(f"  OK: todas as {len(secoes)} secoes geradas estao em sync.")

        print("Verificando sync dos arquivos _config.h...")
        erros_config = []
        for m in modulos:
            config_path = REPO_ROOT / m["config_h"]
            esperado = config_hs[m["nome"]]
            if not config_path.exists():
                erros_config.append(
                    f"  ERRO   {m['config_h']} nao encontrado. "
                    f"Rodar: python scripts/generate_coding_standard.py"
                )
            elif config_path.read_text(encoding="utf-8") != esperado:
                erros_config.append(
                    f"  ERRO   {m['config_h']} desatualizado. "
                    f"Rodar: python scripts/generate_coding_standard.py"
                )

        if erros_config:
            print("\nArquivos _config.h fora de sync com firmware_constants.json:")
            for e in erros_config:
                print(e)
            sys.exit(1)
        print(f"  OK: {len(modulos)} arquivos _config.h em sync.")
        sys.exit(0)

    # --- Modo generate: escrever ---
    conteudo = conteudo_atual
    avisos = []

    conteudo, ok = substituir_secao(conteudo, "arvore_diretorios", arvore)
    if not ok:
        avisos.append("  AVISO: marcador 'arvore_diretorios' nao encontrado")

    for m in modulos:
        chave = f"tabela_{m['nome']}"
        conteudo, ok = substituir_secao(conteudo, chave, tabelas[m["nome"]])
        if not ok:
            avisos.append(f"  AVISO: marcador '{chave}' nao encontrado")

    conteudo, ok = substituir_secao(conteudo, "decisoes_manuais", tabela_dm)
    if not ok:
        avisos.append("  AVISO: marcador 'decisoes_manuais' nao encontrado")

    CODING_STANDARD.write_text(conteudo, encoding="utf-8")

    print(f"CODING_STANDARD.md atualizado.")
    print(f"  Modulos: {', '.join(m['nome'] for m in modulos)}")
    print(f"  Constantes DERIVADO: {len(constantes)}")
    print(f"  Decisoes manuais: {len(decisoes)}")
    if avisos:
        print("Avisos:")
        for a in avisos:
            print(a)

    # --- Gerar _config.h ---
    print("Gerando arquivos _config.h...")
    for m in modulos:
        config_path = REPO_ROOT / m["config_h"]
        config_path.parent.mkdir(parents=True, exist_ok=True)
        config_path.write_text(config_hs[m["nome"]], encoding="utf-8")
        print(f"  Gerado: {m['config_h']}")
    print(f"  {len(modulos)} arquivos _config.h gerados.")


if __name__ == "__main__":
    main()
