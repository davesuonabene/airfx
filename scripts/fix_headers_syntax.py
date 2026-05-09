import os
import re
from pathlib import Path

def fix_headers():
    include_root = Path(__file__).parent.parent / "source" / "airwindohhs" / "include"
    
    for root, dirs, files in os.walk(include_root):
        for file in files:
            if file.endswith(".hpp"):
                filepath = Path(root) / file
                with open(filepath, "r") as f:
                    content = f.read()
                
                original = content
                
                # Fix 1: Ensure methods are public
                # If we find class ... { and the next word is not 'public:', add it.
                if "class" in content and "{" in content and "public:" not in content[:content.find("void process")]:
                    content = re.sub(r'(class \w+ final : public Effect<T>\s*\{)', r'\1\n  public:', content)

                # Fix 2: Misplaced public/private after enum/struct
                content = re.sub(r'(\w+)\s+(public|private)\s*:\s*(\w+)\s*\(\s*\)', r'\1;\n    \2:\n        \3()', content)
                
                # Fix 3: Console8 specific fix for members outside public block
                # Looking for: enum { ... fix_total public : Console8ChannelIn()
                # The previous regex might have missed some variations.
                if "fix_total" in content and "public" in content:
                     content = content.replace("fix_total\n\n        public :", "fix_total;\n\n        public :")

                # Fix 4: Closing braces issues
                content = re.sub(r'\}\s*\}\s*\}\s*;\s*\}', r'        }\n    }\n};\n}', content)
                content = re.sub(r'\}\s*\}\s*;\s*\}', r'    }\n};\n}', content)


                if content != original:
                    with open(filepath, "w") as f:
                        f.write(content)
                    print(f"Fixed syntax in {filepath.relative_to(include_root)}")

if __name__ == "__main__":
    fix_headers()
