import os
import urllib.request
import re
import glob

def fetch_and_fix_console8():
    path = "source/airwindohhs/include/consoles/console8*.hpp"
    for file in glob.glob(path):
        basename = os.path.basename(file).replace(".hpp", "")
        class_name = ""
        with open(file, "r") as f:
            content = f.read()
            m = re.search(r'class (\w+)(?: final)?\s*:\s*public Effect<T>', content)
            if m:
                class_name = m.group(1)
        
        if not class_name:
            continue
            
        url = f"https://raw.githubusercontent.com/airwindows/airwindows/master/plugins/MacVST/{class_name}/source/{class_name}.h"
        
        try:
            req = urllib.request.Request(url)
            with urllib.request.urlopen(req) as response:
                orig_h = response.read().decode('utf-8')
        except Exception as e:
            continue
            
        # Extract variables from orig_h
        idx = orig_h.find("fix_total")
        if idx == -1:
            continue
            
        enum_end = orig_h.find("};", idx)
        if enum_end == -1:
            continue
            
        # The variables start after enum_end + 2
        # They end at the last "};" before #endif
        class_end = orig_h.rfind("};", enum_end + 2)
        if class_end == -1:
            continue
            
        variables = orig_h[enum_end + 2:class_end].strip()
        
        # Remove any access modifiers just in case, though usually none there
        
        # Now replace in our .hpp
        with open(file, "r") as f:
            hpp_content = f.read()
            
        # The current .hpp has:
        # fix_total
        #     };
        # 
        #         public :
        
        m = re.search(r'fix_total\s*};\s*public :', hpp_content)
        if m:
            new_content = hpp_content[:m.start()] + "fix_total\n    };\n    " + variables + "\n\n        public :" + hpp_content[m.end():]
            with open(file, "w") as f:
                f.write(new_content)
            print(f"Patched {file}")
        else:
            print(f"Could not find replacement target in {file}")

fetch_and_fix_console8()
