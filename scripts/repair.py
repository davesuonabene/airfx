import os
import subprocess
import glob
import re

subprocess.run(["python3", "build_new_plugins.py"])

def fetch_and_fix_all():
    path = "source/airwindohhs/include/**/*.hpp"
    for file in glob.glob(path, recursive=True):
        with open(file, "r") as f:
            hpp_content = f.read()
            
        m = re.search(r'enum\s*\{[^\}]*?([a-zA-Z0-9_]+)\s*public\s*:', hpp_content)
        if not m:
            continue
            
        enum_target = m.group(1)
        
        class_m = re.search(r'class (\w+)(?: final)?\s*:\s*public Effect<T>', hpp_content)
        if not class_m:
            continue
        class_name = class_m.group(1)
        
        orig_h_path = f"source/airwindohhs/airwindows/{class_name}/source/{class_name}.h"
        if not os.path.exists(orig_h_path):
            orig_h_path = f"source/airwindohhs/airwindows/{class_name}/{class_name}.h"
            if not os.path.exists(orig_h_path):
                continue
                
        try:
            with open(orig_h_path, "r", encoding="mac_roman") as f:
                orig_h = f.read()
        except:
            with open(orig_h_path, "r", encoding="utf-8", errors="ignore") as f:
                orig_h = f.read()
                
        idx = orig_h.find(enum_target)
        if idx == -1:
            continue
            
        enum_end = orig_h.find("};", idx)
        if enum_end == -1:
            continue
            
        class_end = orig_h.rfind("};")
        if class_end < enum_end:
            continue
            
        variables = orig_h[enum_end + 2:class_end].strip()
        variables = variables.replace("private:", "")
        
        m2 = re.search(r'(' + enum_target + r')\s*public\s*:', hpp_content)
        if m2:
            new_content = hpp_content[:m2.start(1)] + enum_target + "\n    };\n    " + variables + "\n\n        public :" + hpp_content[m2.end():]
            with open(file, "w") as f:
                f.write(new_content)
            print(f"Patched {file}")

fetch_and_fix_all()
