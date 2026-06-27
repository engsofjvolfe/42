#!/usr/bin/env python3
"""
bump_version.py — Automatiza versionamento e data em documentos do projeto.

Uso:
    python bump_version.py <arquivo.md> <patch|minor|major> "<descricao da mudanca>"

Exemplo:
    python bump_version.py concept/00_conceito.md minor "Adiciona secao de restricoes fisicas"

O que faz:
    1. Le o frontmatter YAML do documento
    2. Incrementa a versao (PATCH, MINOR ou MAJOR)
    3. Atualiza o campo 'data' para hoje
    4. Adiciona entrada no Changelog do documento
    5. Exibe o novo numero de versao para uso em commits

Restricoes:
    - O documento deve ter frontmatter YAML delimitado por ---
    - O documento deve ter uma secao ## Changelog
    - Nao faz commit automaticamente — o operador decide quando commitar
"""

import sys
import re
from datetime import date
from pathlib import Path


def parse_version(v: str) -> tuple[int, int, int]:
    parts = v.strip().split('.')
    if len(parts) != 3:
        raise ValueError(f"Versao invalida: {v!r}. Esperado MAJOR.MINOR.PATCH")
    return int(parts[0]), int(parts[1]), int(parts[2])


def bump(major: int, minor: int, patch: int, level: str) -> tuple[int, int, int]:
    if level == 'major':
        return major + 1, 0, 0
    if level == 'minor':
        return major, minor + 1, 0
    if level == 'patch':
        return major, minor, patch + 1
    raise ValueError(f"Nivel invalido: {level!r}. Use patch, minor ou major")


def update_frontmatter(content: str, new_version: str, today: str) -> str:
    content = re.sub(
        r'^(versão:\s*)[\d.]+',
        lambda m: m.group(1) + new_version,
        content,
        flags=re.MULTILINE
    )
    content = re.sub(
        r'^(data:\s*)[\d-]+',
        lambda m: m.group(1) + today,
        content,
        flags=re.MULTILINE
    )
    return content


def update_id_table(content: str, new_version: str) -> str:
    """Atualiza a celula de versao na tabela de Identificacao do corpo, se existir."""
    content = re.sub(
        r'(\|\s*Vers[aã]o\s*\|\s*)[\d.]+(\s*\|)',
        lambda m: m.group(1) + new_version + m.group(2),
        content
    )
    return content


def add_changelog_entry(
    content: str,
    new_version: str,
    today: str,
    descricao: str,
    secao: str = '—'
) -> str:
    new_entry = (
        f'| {new_version} | {today} | {secao} | {descricao} | — |\n'
    )
    changelog_header = re.compile(
        r'^(## Changelog\s*\n\|[^\n]+\n\|[-|]+\n)',
        re.MULTILINE
    )
    match = changelog_header.search(content)
    if not match:
        print("AVISO: secao ## Changelog nao encontrada. Entrada nao adicionada.")
        return content
    insert_at = match.end()
    return content[:insert_at] + new_entry + content[insert_at:]


def main():
    if len(sys.argv) < 4:
        print(__doc__)
        sys.exit(1)

    filepath = Path(sys.argv[1])
    level = sys.argv[2].lower()
    descricao = sys.argv[3]
    secao = sys.argv[4] if len(sys.argv) > 4 else '—'

    if not filepath.exists():
        print(f"ERRO: arquivo nao encontrado: {filepath}")
        sys.exit(1)

    content = filepath.read_text(encoding='utf-8')

    # Extrair versao atual do frontmatter
    m = re.search(r'^versão:\s*([\d.]+)', content, re.MULTILINE)
    if not m:
        print(f"ERRO: campo 'versao:' nao encontrado em {filepath}")
        sys.exit(1)

    current = m.group(1)
    major, minor, patch = parse_version(current)
    new_major, new_minor, new_patch = bump(major, minor, patch, level)
    new_version = f"{new_major}.{new_minor}.{new_patch}"
    today = date.today().isoformat()

    content = update_frontmatter(content, new_version, today)
    content = update_id_table(content, new_version)
    content = add_changelog_entry(content, new_version, today, descricao, secao)

    filepath.write_text(content, encoding='utf-8')

    print(f"OK  {filepath}")
    print(f"    {current} -> {new_version}  ({today})")
    print(f"    Proximo passo:")
    print(f"    git add {filepath}")
    print(f"    git commit -m \"docs(<escopo>): <descricao no imperativo>\"")


if __name__ == '__main__':
    main()
