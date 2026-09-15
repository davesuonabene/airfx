import os
import subprocess

with open("CMakeLists.txt", "wb") as f:
    f.write(subprocess.check_output(["git", "show", "HEAD:CMakeLists.txt"]))

print("Restored CMakeLists.txt")

# Now we need to remove Console9 from it
with open("CMakeLists.txt", "r") as f:
    lines = f.readlines()
    
new_lines = []
for line in lines:
    if "console9" in line.lower() or "console9" in line.lower():
        continue
    new_lines.append(line)
    
with open("CMakeLists.txt", "w") as f:
    f.writelines(new_lines)

print("Removed Console9")
