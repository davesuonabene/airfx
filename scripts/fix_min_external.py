import os
import re

obj_dir = "source/objects"

for root, dirs, files in os.walk(obj_dir):
    for f in files:
        if f.endswith(".cpp"):
            path = os.path.join(root, f)
            with open(path, "r", encoding="utf-8") as file:
                content = file.read()
            
            # Match MIN_EXTERNAL(slug_tilde);
            # and replace it with MIN_EXTERNAL_CUSTOM(slug_tilde, "airfx.slug~");
            
            def repl(m):
                slug = m.group(1).replace("_tilde", "")
                return f'MIN_EXTERNAL_CUSTOM({m.group(1)}, "airfx.{slug}~");'
            
            new_content = re.sub(r'MIN_EXTERNAL\(([^)]+)\);', repl, content)
            
            if new_content != content:
                with open(path, "w", encoding="utf-8") as file:
                    file.write(new_content)
                print(f"Patched {path}")

# Also update example.cpp template
example_path = "scripts/example.cpp"
if os.path.exists(example_path):
    with open(example_path, "r", encoding="utf-8") as file:
        content = file.read()
    
    def repl_example(m):
        # example_tilde -> "airfx.example~"
        return 'MIN_EXTERNAL_CUSTOM(example_tilde, "airfx.example~");'
        
    new_content = re.sub(r'MIN_EXTERNAL\(example_tilde\);', repl_example, content)
    with open(example_path, "w", encoding="utf-8") as file:
        file.write(new_content)
    print(f"Patched {example_path}")
