#!/usr/bin/env python3
"""
check_versions.py
Verifica se as versões declaradas em depende_de batem com as versões reais dos documentos.
Uso: python scripts/check_versions.py
"""

import re
import sys
from pathlib import Path

DOCS_DIR = Path(__file__).parent.parent
# Extrai: "- 00_conceito.md v0.1.0 [BLOQUEADOR]"
DEP_PATTERN = re.compile(r'-\s+([\w.]+\.md)\s+v([\d.]+)\s+\[')
# Extrai versão do YAML: "versão:       0.1.0"
VERSION_PATTERN = re.compile(r'^versão:\s+([\d.]+)', re.MULTILINE)


def get_declared_version(filepath: Path) -> str | None:
    """Extrai versão declarada no YAML de um documento."""
    try:
        content = filepath.read_text(encoding='utf-8')
    except FileNotFoundError:
        return None
    if not content.startswith('---'):
        return None
    end = content.find('---', 3)
    if end == -1:
        return None
    yaml_block = content[3:end]
    match = VERSION_PATTERN.search(yaml_block)
    return match.group(1) if match else None


def get_frontmatter(filepath: Path) -> str:
    """Retorna o bloco YAML de um documento."""
    try:
        content = filepath.read_text(encoding='utf-8')
    except FileNotFoundError:
        return ''
    if not content.startswith('---'):
        return ''
    end = content.find('---', 3)
    return content[3:end] if end != -1 else ''


def check_versions(docs_dir: Path) -> list:
    errors = []
    warnings = []

    for md_file in sorted(docs_dir.glob('*.md')):
        frontmatter = get_frontmatter(md_file)
        if not frontmatter:
            continue

        # Encontra bloco depende_de
        in_depends = False
        for line in frontmatter.splitlines():
            stripped = line.strip()
            if stripped.startswith('depende_de:'):
                in_depends = True
                continue
            if in_depends:
                if stripped and not line.startswith(' ') and not line.startswith('\t'):
                    in_depends = False
                    continue
                match = DEP_PATTERN.search(line)
                if not match:
                    continue
                dep_file = match.group(1)
                declared_ver = match.group(2)
                dep_path = docs_dir / dep_file

                if not dep_path.exists():
                    warnings.append(
                        f"  AVISO  {md_file.name}: depende de '{dep_file}' "
                        f"v{declared_ver} — arquivo ainda não existe"
                    )
                    continue

                actual_ver = get_declared_version(dep_path)
                if actual_ver is None:
                    warnings.append(
                        f"  AVISO  {md_file.name}: não foi possível ler "
                        f"versão de '{dep_file}'"
                    )
                elif declared_ver != actual_ver:
                    errors.append(
                        f"  ERRO   {md_file.name}: declara {dep_file} "
                        f"v{declared_ver} mas versão atual é v{actual_ver}"
                    )

    return errors, warnings


if __name__ == '__main__':
    errors, warnings = check_versions(DOCS_DIR)

    if warnings:
        print("⚠️  Avisos:")
        for w in warnings:
            print(w)

    if errors:
        print("\n❌ Versões desatualizadas:")
        for e in errors:
            print(e)
        sys.exit(1)
    else:
        print("✅ Todas as versões em depende_de estão corretas.")
