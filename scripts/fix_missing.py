import os
import re
from pathlib import Path

def fix():
    project_root = Path(__file__).parent.parent
    cmake_path = project_root / "CMakeLists.txt"
    objects_root = project_root / "source" / "objects"
    include_root = project_root / "source" / "airwindohhs" / "include"

    with open(cmake_path, "r") as f:
        lines = f.readlines()

    new_lines = []
    removed_count = 0
    
    # Pattern to match add_airfx_object(category object_name)
    pattern = re.compile(r"add_airfx_object\(\s*(\S+)\s+(\S+)\s*\)")

    for line in lines:
        match = pattern.search(line)
        if match:
            category = match.group(1)
            object_name = match.group(2)
            # Remove airfx. and _tilde
            header_name = object_name.replace("airfx.", "").replace("_tilde", "") + ".hpp"
            header_path = include_root / category / header_name
            
            if not header_path.exists():
                print(f"Removing missing plugin: {object_name} (header {header_name} not found in {category})")
                removed_count += 1
                # Also remove the object directory
                obj_dir = objects_root / category / object_name
                if obj_dir.exists():
                    import shutil
                    shutil.rmtree(obj_dir)
                continue
        
        new_lines.append(line)

    if removed_count > 0:
        with open(cmake_path, "w") as f:
            f.writelines(new_lines)
        print(f"Removed {removed_count} missing plugins from CMakeLists.txt and source/objects")
    else:
        print("No missing plugins found.")

if __name__ == "__main__":
    fix()
