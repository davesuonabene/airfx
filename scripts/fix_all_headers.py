import os
import re
import glob

def fetch_and_fix_all():
    path = "source/airwindohhs/include/**/*.hpp"
    for file in glob.glob(path, recursive=True):
        with open(file, "r") as f:
            hpp_content = f.read()
            
        m = re.search(r'([a-zA-Z0-9_]+)\s*public\s*:', hpp_content)
        if not m:
            continue
            
        enum_target = m.group(1)
        # Verify it's an enum item (usually ends in _total, or is just a variable name)
        
        class_m = re.search(r'class (\w+)(?: final)?\s*:\s*public Effect<T>', hpp_content)
        if not class_m:
            continue
        class_name = class_m.group(1)
        
        orig_h_path = f"source/airwindohhs/airwindows/{class_name}/source/{class_name}.h"
        if not os.path.exists(orig_h_path):
            orig_h_path = f"source/airwindohhs/airwindows/{class_name}/{class_name}.h"
            if not os.path.exists(orig_h_path):
                print(f"Could not find orig header for {class_name}")
                continue
                
        try:
            with open(orig_h_path, "r", encoding="mac_roman") as f:
                orig_h = f.read()
        except:
            with open(orig_h_path, "r", encoding="utf-8", errors="ignore") as f:
                orig_h = f.read()
                
        idx = orig_h.find(enum_target)
        if idx == -1:
            print(f"Could not find {enum_target} in {orig_h_path}")
            continue
            
        enum_end = orig_h.find("};", idx)
        if enum_end == -1:
            continue
            
        class_end = orig_h.rfind("};")
        if class_end < enum_end:
            continue
            
        variables = orig_h[enum_end + 2:class_end].strip()
        variables = variables.replace("private:", "")
        
        new_content = hpp_content[:m.start(1)] + enum_target + "\n    };\n    " + variables + "\n\n        public :" + hpp_content[m.end():]
        with open(file, "w") as f:
            f.write(new_content)
        print(f"Patched {file}")

fetch_and_fix_all()
