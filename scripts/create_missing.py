import os
import shutil
from pathlib import Path

def get_missing():
    headers_root = os.path.join("source", "airwindohhs", "include")
    objects_root = os.path.join("source", "objects")
    missing = []
    for subdir in os.listdir(headers_root):
        subdir_path = os.path.join(headers_root, subdir)
        if os.path.isdir(subdir_path):
            for header in os.listdir(subdir_path):
                if header.endswith(".hpp"):
                    base_name = os.path.splitext(header)[0]
                    # The name inside objects is airfx.{base_name}_tilde
                    output_name = f"airfx.{base_name.lower()}_tilde"
                    source_dir = os.path.join(objects_root, subdir, output_name)
                    if not os.path.exists(source_dir):
                        missing.append((subdir, base_name))
    return missing

def create_new(category, name):
    new_lines = []
    output_name = f"airfx.{name.lower()}_tilde"
    with open(os.path.join("scripts", "example.cpp"), "r", encoding="utf-8") as f:
        lines = f.readlines()
        for line in lines:
            new_line = line.replace("Example", name)
            new_line = new_line.replace("example", name.lower())
            new_line = new_line.replace("category", category)
            new_lines.append(new_line)
    
    new_dir = os.path.join("source", "objects", category, output_name)
    os.makedirs(new_dir, exist_ok=True)
    
    with open(os.path.join(new_dir, f"{output_name}.cpp"), "w", encoding="utf-8") as f:
        f.writelines(new_lines)
    
    # create CMakeLists.txt and ensure ${CMAKE_BUILD_TYPE} is quoted
    with open(os.path.join("scripts", "example.CMakeLists.txt"), "r", encoding="utf-8") as f:
        cmake_lines = f.read()
    
    cmake_lines = cmake_lines.replace('${CMAKE_BUILD_TYPE}', '\"${CMAKE_BUILD_TYPE}\"')
    
    with open(os.path.join(new_dir, "CMakeLists.txt"), "w", encoding="utf-8") as f:
        f.write(cmake_lines)
    
    with open("CMakeLists.txt", "a", encoding="utf-8") as f:
        f.write(f"add_airfx_object({category} {output_name})\n")

def main():
    missing = get_missing()
    for cat, name in missing:
        print(f"Creating wrapper for {cat} / {name}")
        create_new(cat, name)

if __name__ == "__main__":
    main()
