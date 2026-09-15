import os
import re

def get_missing():
    headers_root = os.path.join("source", "airwindohhs", "include")
    objects_root = os.path.join("source", "objects")
    # All directories in objects/ that don't have a compiled .mxe64
    missing = []
    # Actually, I can just check all objects
    for cat in os.listdir(objects_root):
        cat_dir = os.path.join(objects_root, cat)
        if os.path.isdir(cat_dir):
            for obj in os.listdir(cat_dir):
                obj_dir = os.path.join(cat_dir, obj)
                if os.path.isdir(obj_dir) and obj.startswith("airfx."):
                    base_name = obj[6:-6].lower()
                    hpp_path = os.path.join(headers_root, cat, f"{base_name}.hpp")
                    if os.path.exists(hpp_path):
                        missing.append((cat, base_name, hpp_path, obj_dir, obj))
    return missing

def fix_all():
    missing = get_missing()
    class_name_re = re.compile(r"class (\w+) (final )?: public Effect<T>")
    
    for cat, base_name, hpp_path, obj_dir, obj in missing:
        # Read hpp
        with open(hpp_path, "r", encoding="utf-8") as f:
            hpp_lines = f.readlines()
        
        # fix the extra braces at the end of the file in the process block
        # we know it ends with:
        #   }
        # }
        #   }
        # 
        # };
        # } // namespace ...
        
        # We find the last `};`
        class_end_idx = -1
        for i in range(len(hpp_lines)-1, -1, -1):
            if hpp_lines[i].startswith("};"):
                class_end_idx = i
                break
        
        if class_end_idx != -1:
            # Check lines just above it, and remove extra `}` that are at indentation 0 or 4
            # A typical grab.py error has an extra `}\n}`.
            # Let's cleanly fix it: we expect `    }\n\n};`
            changed = False
            i = class_end_idx - 1
            extra_braces_removed = 0
            while i >= 0 and extra_braces_removed < 2:
                line = hpp_lines[i].strip()
                if line == "}":
                    hpp_lines.pop(i)
                    changed = True
                    class_end_idx -= 1
                    extra_braces_removed += 1
                elif line == "":
                    pass
                else:
                    target_brace_idx = i
                    break
                i -= 1
            if changed:
                # put exactly one `    }` back
                hpp_lines.insert(class_end_idx, "    }\n")
                with open(hpp_path, "w", encoding="utf-8") as f:
                    f.writelines(hpp_lines)
            
        # extract class name
        class_name = None
        for line in hpp_lines:
            m = class_name_re.search(line)
            if m:
                class_name = m.group(1)
                break
        
        if class_name:
            cpp_path = os.path.join(obj_dir, f"{obj}.cpp")
            if os.path.exists(cpp_path):
                with open(cpp_path, "r", encoding="utf-8") as f:
                    cpp_text = f.read()
                
                # replace: using {base_name} = airwindohhs::{base_name}::{base_name}<double>;
                # with: using {base_name} = airwindohhs::{base_name}::{class_name}<double>;
                # OR if it was already wrongly replaced...
                # let's just do a regex replace
                # `airwindohhs::(.*)::(.*)<double>;`
                new_cpp_text = re.sub(
                    r"using (.+) = airwindohhs::(.+)::(.+)<double>;",
                    f"using \\1 = airwindohhs::\\2::{class_name}<double>;",
                    cpp_text
                )
                
                if new_cpp_text != cpp_text:
                    with open(cpp_path, "w", encoding="utf-8") as f:
                        f.write(new_cpp_text)

if __name__ == "__main__":
    fix_all()
