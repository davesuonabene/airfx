import os
import glob

def fix_enums():
    path = "source/airwindohhs/include/consoles/*.hpp"
    for file in glob.glob(path):
        with open(file, "r") as f:
            content = f.read()
        
        if "fix_total\n\n        public :" in content:
            new_content = content.replace("fix_total\n\n        public :", "fix_total\n    };\n\n        public :")
            with open(file, "w") as f:
                f.write(new_content)
            print(f"Fixed enum in {file}")

        # Also remove Console9 from CMakeLists
        
fix_enums()

def fix_cmake():
    with open("CMakeLists.txt", "r") as f:
        lines = f.readlines()
    
    new_lines = []
    for line in lines:
        if "Console9" in line or "console9" in line:
            continue
        new_lines.append(line)
        
    with open("CMakeLists.txt", "w") as f:
        f.writelines(new_lines)
    print("Removed Console9 from CMakeLists.txt")

fix_cmake()
