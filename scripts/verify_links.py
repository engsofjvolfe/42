#!/usr/bin/env python3
"""
verify_links.py
Verifica se todos os [VER: arquivo.md#ancora] nos documentos .md resolvem.
Uso: python scripts/verify_links.py
"""

import re
import sys
from pathlib import Path

DOCS_DIR = Path(__file__).parent.parent
VER_PATTERN = re.compile(r'\[VER:\s*([^#\]\s]+\.md)#([^\]\s]+)\]')
ANCHOR_PATTERN = re.compile(r'<a\s+id=["\']([^"\']+)["\']')


def get_anchors(filepath: Path) -> set:
    """Extrai todos os <a id="..."> de um arquivo markdown."""
    try:
        content = filepath.read_text(encoding='utf-8')
    except FileNotFoundError:
        return set()
    return set(ANCHOR_PATTERN.findall(content))


def check_links(docs_dir: Path) -> list:
    errors = []
    warnings = []

    for md_file in sorted(docs_dir.glob('*.md')):
        content = md_file.read_text(encoding='utf-8')
        for match in VER_PATTERN.finditer(content):
            target_name = match.group(1).strip()
            anchor = match.group(2).strip()
            target_path = docs_dir / target_name

            if not target_path.exists():
                # Documento ainda não criado (forward reference planejada)
                warnings.append(
                    f"  AVISO  {md_file.name} → [{target_name}#{anchor}] "
                    f"arquivo ainda não existe"
                )
                continue

            anchors = get_anchors(target_path)
            if anchor not in anchors:
                errors.append(
                    f"  ERRO   {md_file.name} → [{target_name}#{anchor}] "
                    f"âncora não encontrada"
                )

    return errors, warnings


if __name__ == '__main__':
    errors, warnings = check_links(DOCS_DIR)

    if warnings:
        print("⚠️  Referências a documentos ainda não criados:")
        for w in warnings:
            print(w)

    if errors:
        print("\n❌ Links quebrados:")
        for e in errors:
            print(e)
        sys.exit(1)
    else:
        print("✅ Todos os [VER: file#anchor] resolvem corretamente.")
        if warnings:
            print(f"   ({len(warnings)} forward references aguardando criação)")
