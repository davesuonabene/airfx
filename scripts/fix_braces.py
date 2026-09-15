"""
fix_braces.py
Fixes the brace structure at the end of generated .hpp files.

The correct structure should be:
    }         // end while/for loop
              
    }         // end process() function
              
};            // end class
} // namespace ...

Some generated files have an EXTRA } between process() and };  
Others are correct. This script normalizes all of them.
"""

import os
import re

include_dir = "source/airwindohhs/include"
fixed = 0
errors = 0

for root, dirs, files in os.walk(include_dir):
    for f in files:
        if not f.endswith('.hpp'):
            continue
        path = os.path.join(root, f)
        with open(path, 'r', encoding='utf-8', errors='replace') as fh:
            content = fh.read()
        
        # Find the namespace closing pattern at end of file
        # We need to match everything from the last few closing braces to the end
        # The correct pattern: "    }\n\n    }\n\n};\n} // namespace ..."
        # 
        # Possible broken patterns:
        # 1) Extra brace:  "    }\n\n    }\n}\n\n};\n} // namespace ..."
        # 2) Missing process brace: "    }\n};\n} // namespace ..."
        
        # Strategy: find the "} // namespace" line, work backwards to find };, 
        # and ensure exactly the right number of braces
        
        lines = content.split('\n')
        
        # Find the namespace closing line
        ns_idx = None
        for i in range(len(lines) - 1, -1, -1):
            if lines[i].startswith('} // namespace'):
                ns_idx = i
                break
        
        if ns_idx is None:
            continue
        
        # Find the }; line (end of class) - should be right before namespace close
        class_end_idx = None
        for i in range(ns_idx - 1, -1, -1):
            stripped = lines[i].strip()
            if stripped == '};':
                class_end_idx = i
                break
            elif stripped != '':
                break
        
        if class_end_idx is None:
            continue
            
        # Now count closing braces between class_end and the process body
        # Go backwards from class_end_idx to find closing braces
        brace_lines = []
        for i in range(class_end_idx - 1, -1, -1):
            stripped = lines[i].strip()
            if stripped == '}':
                brace_lines.append(i)
            elif stripped == '':
                continue  # skip blank lines
            else:
                break  # hit actual code
        
        # We need exactly 2 closing braces before };
        # One for the while/for loop, one for the process() function
        if len(brace_lines) == 2:
            continue  # Already correct
        elif len(brace_lines) == 3:
            # Extra brace - remove one
            # Remove the middle one (the extra)
            middle = brace_lines[1]
            lines.pop(middle)
            # Also remove any extra blank lines that result
            new_content = '\n'.join(lines)
            # Normalize: ensure pattern is }  \n\n    }\n\n};\n} // namespace
            new_content = re.sub(r'\}\s*\n\s*\n\s*\}\s*\n\s*\n\s*\}\s*\n\s*\n\};\n\} // namespace',
                                 lambda m: m.group(0),  # keep if already correct
                                 new_content)
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(new_content)
            fixed += 1
        elif len(brace_lines) == 1:
            # Missing process() closing brace - add one
            insert_after = brace_lines[0]
            # Insert a blank line and closing brace after the while loop close
            lines.insert(insert_after + 1, '')
            lines.insert(insert_after + 2, '    }')
            new_content = '\n'.join(lines)
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(new_content)
            fixed += 1
        else:
            print(f"  [WARN] {path}: unexpected {len(brace_lines)} closing braces")
            errors += 1

print(f"Fixed {fixed} files, {errors} warnings")
