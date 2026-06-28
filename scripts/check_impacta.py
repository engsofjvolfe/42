#!/usr/bin/env python3
"""
check_impacta.py
Verifica simetria bidirecional: se A declara B em impacta,
entao B deve declarar A como Pai em sua Rastreabilidade.
Uso: python scripts/check_impacta.py
"""

import re
import sys
from pathlib import Path

DOCS_DIR = Path(__file__).parent.parent
IMPACTA_FILE = re.compile(r'-\s+([\w.]+\.md)\s+\[')
PAI_ROW = re.compile(r'^\|\s*Pai\s*\|\s*([\w.]+\.md)')


def build_registry(docs_dir: Path) -> dict:
    """Constrói registro {nome_arquivo: path} para todos os .md fora de dirs ocultos."""
    registry = {}
    for md_file in docs_dir.rglob('*.md'):
        rel = md_file.relative_to(docs_dir)
        if any(part.startswith('.') for part in rel.parts):
            continue
        name = md_file.name
        if name not in registry:
            registry[name] = md_file
    return registry


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
    """Retorna lista de arquivos .md declarados em impacta."""
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


def check_impacta(docs_dir: Path) -> tuple:
    errors = []
    warnings = []

    registry = build_registry(docs_dir)

    for md_file in sorted(registry.values()):
        impacted = get_impacta_files(md_file)
        for dep_file in impacted:
            dep_path = registry.get(dep_file)
            if dep_path is None:
                warnings.append(
                    f"  AVISO  {md_file.name} impacta '{dep_file}' "
                    f"-- arquivo ainda nao existe"
                )
                continue

            pais = get_pais(dep_path)
            if md_file.name not in pais:
                errors.append(
                    f"  ERRO   {md_file.name} impacta {dep_file} "
                    f"mas {dep_file} nao declara {md_file.name} como Pai"
                )

    return errors, warnings


if __name__ == '__main__':
    errors, warnings = check_impacta(DOCS_DIR)

    if warnings:
        print("Avisos (forward references):")
        for w in warnings:
            print(w)

    if errors:
        print("\nVinculos impacta <-> Rastreabilidade assimetricos:")
        for e in errors:
            print(e)
        sys.exit(1)
    else:
        print("Todos os vinculos impacta <-> Rastreabilidade estao simetricos.")
