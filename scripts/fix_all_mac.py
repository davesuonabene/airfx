"""
fix_all_mac.py
Master fix script for macOS build of airfx.
Handles:
1. Missing constants (kshortA, predelay, delayA, etc.)
2. Extra closing braces in generated hpp files  
3. Missing process() closing braces
"""

import os
import re

include_dir = "source/airwindohhs/include"

# ============================================================
# 1. Fix constants for specific plugins
# ============================================================

def inject_constants(hpp_path, constants_block):
    """Insert constants right after the namespace opening { line."""
    with open(hpp_path, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    
    # Check if constants already present
    first_const = constants_block.strip().split('\n')[0].strip()
    if first_const in content:
        return False
    
    # Find the namespace line and insert after the blank line following it
    # Pattern: "namespace airwindohhs::xxx\n{\n\n"
    pattern = r'(namespace airwindohhs::\w+\s*\{\s*\n)'
    replacement = r'\1\n' + constants_block + '\n'
    new_content = re.sub(pattern, replacement, content, count=1)
    
    if new_content != content:
        with open(hpp_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        return True
    return False


# CreamCoat constants
creamcoat_path = os.path.join(include_dir, "reverb", "creamcoat.hpp")
if os.path.exists(creamcoat_path):
    if inject_constants(creamcoat_path, """const int predelay = 15000;
const int kshortA = 350;
const int kshortB = 1710;
const int kshortC = 1610;
const int kshortD = 835;
const int kshortE = 700;
const int kshortF = 1260;
const int kshortG = 1110;
const int kshortH = 1768;
const int kshortI = 280;
const int kshortJ = 2645;
const int kshortK = 1410;
const int kshortL = 1175;
const int kshortM = 12;
const int kshortN = 3110;
const int kshortO = 120;
const int kshortP = 2370;"""):
        print("  [const] creamcoat.hpp")

# CrunchCoat constants  
crunchcoat_path = os.path.join(include_dir, "reverb", "crunchcoat.hpp")
if os.path.exists(crunchcoat_path):
    if inject_constants(crunchcoat_path, """const int kshortA = 350;
const int kshortB = 1710;
const int kshortC = 1610;
const int kshortD = 835;
const int kshortE = 700;
const int kshortF = 1260;
const int kshortG = 1110;
const int kshortH = 1768;
const int kshortI = 280;
const int kshortJ = 2645;
const int kshortK = 1410;
const int kshortL = 1175;
const int kshortM = 12;
const int kshortN = 3110;
const int kshortO = 120;
const int kshortP = 2370;"""):
        print("  [const] crunchcoat.hpp")

# kCathedral3 constants
kcathedral3_path = os.path.join(include_dir, "reverb", "kcathedral3.hpp")
if os.path.exists(kcathedral3_path):
    if inject_constants(kcathedral3_path, """const int dscBuf = 90;
const int predelay = 15000;
const int vlfpredelay = 11000;
const int delayA = 824; const int delayB = 982; const int delayC = 753; const int delayD = 402;
const int delayE = 1206; const int delayF = 1235; const int delayG = 1396; const int delayH = 1330;
const int delayI = 248; const int delayJ = 62; const int delayK = 1407; const int delayL = 40;
const int delayM = 245; const int delayN = 1088; const int delayO = 775; const int delayP = 13;
const int delayQ = 1299; const int delayR = 127; const int delayS = 1497; const int delayT = 386;
const int delayU = 11; const int delayV = 971; const int delayW = 1403; const int delayX = 81;
const int delayY = 1480;"""):
        print("  [const] kcathedral3.hpp")

# ============================================================
# 2. Fix brace structure
# ============================================================

fixed_braces = 0
for root, dirs, files in os.walk(include_dir):
    for f in files:
        if not f.endswith('.hpp') or f == 'effect.hpp':
            continue
        path = os.path.join(root, f)
        with open(path, 'r', encoding='utf-8', errors='replace') as fh:
            content = fh.read()
        
        # Fix pattern: "}\n}\n\n};\n} // namespace" -> "}\n\n    }\n\n};\n} // namespace"
        # This is the "extra brace" pattern where there's } } }; } instead of } } };  }
        # 
        # More precisely: find cases where there are 3+ closing braces before };
        # The correct pattern at file end is:
        #     }       <- end of while/for loop (indented)
        #
        #     }       <- end of process() function (indented)  
        #
        # };          <- end of class
        # } // namespace ...
        
        # Pattern 1: Three } in a row before }; (extra brace from bad codegen)
        new_content = re.sub(
            r'(\n    \})\s*\n\}\s*\n\s*\n(\};\s*\n\} // namespace)',
            r'\1\n\n\2',
            content
        )
        
        if new_content != content:
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(new_content)
            fixed_braces += 1

print(f"  [braces] Fixed {fixed_braces} files with extra closing braces")

# ============================================================
# 3. Verify: check for remaining issues
# ============================================================
issues = 0
for root, dirs, files in os.walk(include_dir):
    for f in files:
        if not f.endswith('.hpp') or f == 'effect.hpp':
            continue
        path = os.path.join(root, f)
        with open(path, 'r', encoding='utf-8', errors='replace') as fh:
            content = fh.read()
        
        # Check that file ends properly
        if '} // namespace' not in content:
            print(f"  [WARN] {path}: missing namespace close")
            issues += 1
        
        # Count braces
        open_count = content.count('{')
        close_count = content.count('}')
        if open_count != close_count:
            print(f"  [WARN] {path}: brace mismatch (open={open_count}, close={close_count})")
            issues += 1

if issues == 0:
    print("  [OK] All files pass brace validation")
else:
    print(f"  [WARN] {issues} files have potential issues")
