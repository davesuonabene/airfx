import urllib.request
import zipfile
import io
import os
import shutil

print("Downloading zip...")
req = urllib.request.Request("https://github.com/airwindows/airwindows/archive/refs/heads/master.zip")
with urllib.request.urlopen(req) as resp:
    data = resp.read()

print("Extracting...")
with zipfile.ZipFile(io.BytesIO(data)) as z:
    z.extractall("/tmp/airwindows_repo")

# Copy the MacVST plugins directory to source/airwindohhs/airwindows
src = "/tmp/airwindows_repo/airwindows-master/plugins/MacVST"
dst = "source/airwindohhs/airwindows"
os.makedirs(dst, exist_ok=True)

for item in os.listdir(src):
    s = os.path.join(src, item)
    d = os.path.join(dst, item)
    if os.path.isdir(s):
        shutil.copytree(s, d, dirs_exist_ok=True)

print("Done downloading airwindows plugins!")
