import os
import shutil
from pathlib import Path
import re


def relative_path(*args) -> Path:
    project_root = Path(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    return Path(os.path.join(project_root, *args))


def get_k_name(category, filename):
    filepath = relative_path("source", "airwindohhs", "include", category, filename)
    with open(filepath, "r") as f:
        content = f.read()
        match = re.search(r'constexpr std::string_view k_name\{\s*"(.*?)"\s*\}', content)
        if match:
            return match.group(1)
    return filename.replace(".hpp", "").capitalize()


def create_new(category, name, filename_base):
    new_lines = []
    output_name = f"airfx.{filename_base}_tilde"
    with open(relative_path("scripts", "example.cpp"), "r") as f:
        lines = f.readlines()
        for line in lines:
            new_line = line.replace("Example", name)
            new_line = new_line.replace("example", filename_base)
            new_line = new_line.replace("category", category)
            new_lines.append(new_line)
    
    category_dir = relative_path("source", "objects", category)
    if not os.path.isdir(category_dir):
        os.mkdir(category_dir)
        
    new_dir = relative_path("source", "objects", category, output_name)
    if not os.path.isdir(new_dir):
        os.mkdir(new_dir)
    
    with open(relative_path("source", "objects", category, output_name, f"{output_name}.cpp"), "w") as f:
        f.writelines(new_lines)
    
    shutil.copy(relative_path("scripts", "example.CMakeLists.txt"), os.path.join(new_dir, "CMakeLists.txt"))
    
    cmakelists_path = relative_path("CMakeLists.txt")
    with open(cmakelists_path, "r") as f:
        cmake_content = f.read()
    
    add_line = f"add_airfx_object({category} {output_name})\n"
    if add_line not in cmake_content:
        with open(cmakelists_path, "a") as f:
            f.write(add_line)
    
    print(f"Created {output_name} in category {category}")


def main():
    include_root = relative_path("source", "airwindohhs", "include")
    objects_root = relative_path("source", "objects")

    for category in os.listdir(include_root):
        cat_path = os.path.join(include_root, category)
        if not os.path.isdir(cat_path):
            continue
            
        for filename in os.listdir(cat_path):
            if not filename.endswith(".hpp") or filename == "effect.hpp":
                continue
            
            filename_base = filename.replace(".hpp", "")
            output_name = f"airfx.{filename_base}_tilde"
            target_dir = os.path.join(objects_root, category, output_name)
            
            if not os.path.exists(target_dir):
                k_name = get_k_name(category, filename)
                create_new(category, k_name, filename_base)


if __name__ == "__main__":
    main()
