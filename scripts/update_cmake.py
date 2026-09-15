import os

def update_cmake():
    # Read all objects
    objects_dir = "source/objects"
    categories = [d for d in os.listdir(objects_dir) if os.path.isdir(os.path.join(objects_dir, d))]
    
    with open("CMakeLists.txt", "r") as f:
        cmake_content = f.read()
        
    lines = cmake_content.splitlines()
    
    # We will find the add_airfx_object lines
    # and if any object is missing, we append it at the end of the file.
    
    missing = []
    for cat in categories:
        cat_dir = os.path.join(objects_dir, cat)
        for obj in os.listdir(cat_dir):
            if os.path.isdir(os.path.join(cat_dir, obj)) and obj.startswith("airfx."):
                # obj is like "airfx.console8litebuss_tilde"
                search_str = f"add_airfx_object({cat} {obj})"
                if search_str not in cmake_content and "console9" not in obj.lower():
                    missing.append(search_str)
                    
    if missing:
        lines.append("")
        lines.append("# Newly added by script")
        for m in missing:
            lines.append(m)
            
        with open("CMakeLists.txt", "w") as f:
            f.write("\n".join(lines) + "\n")
            
        print(f"Added {len(missing)} missing plugins to CMakeLists.txt")
    else:
        print("No missing plugins found.")

update_cmake()
