#!/usr/bin/env python3
"""
check_impacta.py
Verifica simetria bidirecional: se A declara B em impacta,
então B deve declarar A como Pai em sua Rastreabilidade.
Uso: python scripts/check_impacta.py
"""

import re
import sys
from pathlib import Path

DOCS_DIR = Path(__file__).parent.parent
# Extrai filename de linha de impacta: "  - 02_sensor_impacto.md [OBRIGATÓRIO]"
IMPACTA_FILE = re.compile(r'-\s+([\w.]+\.md)\s+\[')
# Extrai linha de Pai na Rastreabilidade
PAI_ROW = re.compile(r'^\|\s*Pai\s*\|\s*([\w.]+\.md)')


def get_frontmatter(filepath: Path) -> str:
    try:
        content = filepath.read_text(encoding='utf-8')
    except FileNotFoundError:
        return ''
    if not content.startswith('---'):
        return ''
    end = content.find('---', 3)
    return content[3:end] if end != -1 else ''


def get_impacta_files(md_file: Path) -> list:
    """Retorna lista de arquivos declarados em impacta."""
    frontmatter = get_frontmatter(md_file)
    if not frontmatter:
        return []

    files = []
    in_impacta = False
    for line in frontmatter.splitlines():
        stripped = line.strip()
        if stripped.startswith('impacta:'):
            in_impacta = True
            continue
        if in_impacta:
            if stripped and not line.startswith(' ') and not line.startswith('\t'):
                in_impacta = False
                continue
            match = IMPACTA_FILE.search(line)
            if match:
                files.append(match.group(1))
    return files


def get_pais(md_file: Path) -> list:
    """Retorna lista de documentos declarados como Pai na Rastreabilidade."""
    try:
        content = md_file.read_text(encoding='utf-8')
    except FileNotFoundError:
        return []

    pais = []
    in_rastreabilidade = False
    for line in content.splitlines():
        if '## Rastreabilidade' in line:
            in_rastreabilidade = True
            continue
        if in_rastreabilidade:
            if line.startswith('## ') and 'Rastreabilidade' not in line:
                break
            match = PAI_ROW.match(line)
            if match:
                pais.append(match.group(1))
    return pais


def check_impacta(docs_dir: Path) -> list:
    errors = []
    warnings = []

    for md_file in sorted(docs_dir.glob('*.md')):
        impacted = get_impacta_files(md_file)
        for dep_file in impacted:
            dep_path = docs_dir / dep_file
            if not dep_path.exists():
                warnings.append(
                    f"  AVISO  {md_file.name} impacta '{dep_file}' "
                    f"— arquivo ainda não existe"
                )
                continue

            pais = get_pais(dep_path)
            if md_file.name not in pais:
                errors.append(
                    f"  ERRO   {md_file.name} impacta {dep_file} "
                    f"mas {dep_file} não declara {md_file.name} como Pai"
                )

    return errors, warnings


if __name__ == '__main__':
    errors, warnings = check_impacta(DOCS_DIR)

    if warnings:
        print("⚠️  Avisos (forward references):")
        for w in warnings:
            print(w)

    if errors:
        print("\n❌ Vínculos impacta ↔ Rastreabilidade assimétricos:")
        for e in errors:
            print(e)
        sys.exit(1)
    else:
        print("✅ Todos os vínculos impacta ↔ Rastreabilidade estão simétricos.")
