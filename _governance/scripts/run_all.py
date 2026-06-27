#!/usr/bin/env python3
"""
run_all.py
Executa todos os checks de documentação em sequência.
Uso: python scripts/run_all.py
Retorna exit code 0 se tudo passar, 1 se houver erros.
"""

import subprocess
import sys
from pathlib import Path

SCRIPTS = [
    ('verify_links.py',   'Links [VER: file#anchor]'),
    ('check_versions.py', 'Versões em depende_de'),
    ('check_impacta.py',  'Simetria impacta ↔ Rastreabilidade'),
]

scripts_dir = Path(__file__).parent
all_passed = True

print("=" * 60)
print("VERIFICAÇÃO DE DOCUMENTAÇÃO")
print("=" * 60)

for script_file, description in SCRIPTS:
    print(f"\n▶ {description}")
    print(f"  ({script_file})")
    result = subprocess.run(
        [sys.executable, scripts_dir / script_file],
        capture_output=False
    )
    if result.returncode != 0:
        all_passed = False

print("\n" + "=" * 60)
if all_passed:
    print("✅ TODOS OS CHECKS PASSARAM")
else:
    print("❌ CHECKS FALHARAM — corrigir erros acima antes de aprovar")
print("=" * 60)

sys.exit(0 if all_passed else 1)
