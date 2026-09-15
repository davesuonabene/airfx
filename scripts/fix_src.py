import os
import shutil
import glob

base = "source/airwindohhs/airwindows"
for d in os.listdir(base):
    plugin_dir = os.path.join(base, d)
    source_dir = os.path.join(plugin_dir, "source")
    if os.path.isdir(source_dir):
        for f in os.listdir(source_dir):
            shutil.copy(os.path.join(source_dir, f), plugin_dir)

print("Moved source files up one level!")
