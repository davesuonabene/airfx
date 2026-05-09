import os
import subprocess
from pathlib import Path

def verify_externals():
    script_dir = Path(__file__).parent
    externals_dir = script_dir.parent / "externals"
    if not externals_dir.exists():
        print(f"Externals directory not found at {externals_dir}")
        return

    all_so_files = list(externals_dir.glob("*.so"))
    total = len(all_so_files)
    successful = 0
    failures = []

    print(f"Verifying {total} externals using 'nm'...")

    for so_file in all_so_files:
        try:
            # Use 'nm' to check if 'ext_main' exists as a global text symbol
            result = subprocess.run(['nm', '-D', str(so_file)], capture_output=True, text=True)
            if ' T ext_main' in result.stdout:
                successful += 1
            else:
                failures.append((so_file.name, "Missing ext_main symbol"))
        except Exception as e:
            failures.append((so_file.name, str(e)))

    print(f"\nVerification Results:")
    print(f"Total: {total}")
    print(f"Successful: {successful}")
    print(f"Failures: {len(failures)}")

    if failures:
        print("\nTop 5 Failures:")
        for name, reason in failures[:5]:
            print(f"- {name}: {reason}")

if __name__ == "__main__":
    verify_externals()
