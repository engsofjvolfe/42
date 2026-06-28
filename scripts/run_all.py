#!/usr/bin/env python3
"""
run_all.py
Executa todos os checks de documentação em sequência.
Uso: python scripts/run_all.py
Retorna exit code 0 se tudo passar, 1 se houver erros.
"""

import os
import subprocess
import sys
from pathlib import Path

sys.stdout.reconfigure(encoding='utf-8')

SCRIPTS = [
    ('verify_links.py',              'Links [VER: file#anchor]'),
    ('check_versions.py',            'Versoes em depende_de'),
    ('check_impacta.py',             'Simetria impacta <-> Rastreabilidade'),
    ('generate_coding_standard.py',  'Cascata specs -> CODING_STANDARD.md'),
]

# Argumentos extras por script (script_file -> lista de args adicionais)
SCRIPT_ARGS = {
    'generate_coding_standard.py': ['--check'],
}

scripts_dir = Path(__file__).parent
all_passed = True

print("=" * 60)
print("VERIFICAÇÃO DE DOCUMENTAÇÃO")
print("=" * 60)

for script_file, description in SCRIPTS:
    print(f"\n▶ {description}")
    print(f"  ({script_file})")
    args_extras = SCRIPT_ARGS.get(script_file, [])
    result = subprocess.run(
        [sys.executable, scripts_dir / script_file] + args_extras,
        capture_output=False,
        env={**os.environ, 'PYTHONIOENCODING': 'utf-8'},
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
