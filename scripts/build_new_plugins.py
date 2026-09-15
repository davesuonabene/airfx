"""
build_new_plugins.py
Generates airfx wrapper objects for missing/newer Airwindows plugins,
then appends add_airfx_object() entries to the root CMakeLists.txt.
"""

import os
import sys
import re
from pathlib import Path

# Add grab.py's directory to path so we can import it
sys.path.insert(0, os.path.abspath(os.path.join("source", "airwindohhs", "scripts")))
from grab import Plugin, GrabError

OBJECTS_ROOT = os.path.join("source", "objects")
HEADERS_ROOT = os.path.join("source", "airwindohhs", "include")
PROJECT_ROOT  = Path(os.path.abspath(os.path.join("source", "airwindohhs")))
TEMPLATE_CPP  = os.path.join("scripts", "example.cpp")
TEMPLATE_CMAKE = os.path.join("scripts", "example.CMakeLists.txt")

CLASS_RE = re.compile(r"class\s+(\w+)\s+(?:final\s*)?:\s*public\s+Effect<T>")


def extract_class_name(hpp_path):
    with open(hpp_path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            m = CLASS_RE.search(line)
            if m:
                return m.group(1)
    return None


def generate_cpp(category: str, slug: str, class_name: str) -> str:
    with open(TEMPLATE_CPP, "r", encoding="utf-8") as f:
        src = f.read()
    src = src.replace("Example", class_name)
    src = src.replace("example", slug)
    src = src.replace("category", category)
    return src


def generate_cmake() -> str:
    with open(TEMPLATE_CMAKE, "r", encoding="utf-8") as f:
        src = f.read()
    # Fix unquoted ${CMAKE_BUILD_TYPE} that breaks multi-config generators
    src = src.replace('${CMAKE_BUILD_TYPE}', '"${CMAKE_BUILD_TYPE}"')
    return src


def process_plugin(plugin_name: str, category: str, cmake_root: str) -> tuple[bool, str]:
    """
    Returns (success, updated_cmake_root).
    """
    base_name = plugin_name.lower()
    output_name = f"airfx.{base_name}_tilde"
    obj_dir = os.path.join(OBJECTS_ROOT, category, output_name)

    # Skip if already exists
    if os.path.isdir(obj_dir):
        hpp_path = PROJECT_ROOT / "include" / category / (base_name + ".hpp")
        if hpp_path.exists():
            print(f"  [skip] {plugin_name} (already built)")
            return True, cmake_root

    # 1. Generate include wrapper via grab.py
    try:
        plug = Plugin(PROJECT_ROOT, plugin_name, category)
        plug.write(PROJECT_ROOT, PROJECT_ROOT / "scripts" / "res" / "template.hpp")
        print(f"  [hpp]  {plug.output_path}")
    except GrabError as e:
        print(f"  [FAIL] {plugin_name}: grab error — {e}")
        return False, cmake_root
    except Exception as e:
        print(f"  [FAIL] {plugin_name}: unexpected error — {e}")
        return False, cmake_root

    # 2. Extract actual class name from generated hpp
    class_name = extract_class_name(plug.output_path)
    if not class_name:
        print(f"  [WARN] {plugin_name}: could not extract class name, falling back to title-case")
        class_name = plugin_name  # likely wrong but better than crashing

    # 3. Create source/objects wrapper
    os.makedirs(obj_dir, exist_ok=True)

    cpp_src = generate_cpp(category, plug.slug, class_name)
    cpp_path = os.path.join(obj_dir, f"{output_name}.cpp")
    with open(cpp_path, "w", encoding="utf-8") as f:
        f.write(cpp_src)
    print(f"  [cpp]  {cpp_path}")

    cmake_src = generate_cmake()
    cmake_path = os.path.join(obj_dir, "CMakeLists.txt")
    with open(cmake_path, "w", encoding="utf-8") as f:
        f.write(cmake_src)

    # 4. Append to root CMakeLists.txt (if not already there)
    entry = f"add_airfx_object({category} {output_name})\n"
    if entry not in cmake_root:
        cmake_root += entry

    return True, cmake_root


def main():
    plugins_to_add = [
        # Newer brightness plugins
        ("Air4", "brightness"),
        # Newer tape plugins
        ("ToTape8", "tape"),
        ("ToTape7", "tape"),
        # All missing console variants
        ("Console9Buss",          "consoles"),
        ("Console9Channel",       "consoles"),
        ("ConsoleLABuss",         "consoles"),
        ("ConsoleLAChannel",      "consoles"),
        ("ConsoleMCBuss",         "consoles"),
        ("ConsoleMCChannel",      "consoles"),
        ("ConsoleMDBuss",         "consoles"),
        ("ConsoleMDChannel",      "consoles"),
        ("Console8LiteBuss",      "consoles"),
        ("Console8LiteChannel",   "consoles"),
        ("Console8BussHype",      "consoles"),
        ("Console8BussIn",        "consoles"),
        ("Console8BussOut",       "consoles"),
        ("Console8ChannelHype",   "consoles"),
        ("Console8ChannelIn",     "consoles"),
        ("Console8ChannelOut",    "consoles"),
        ("Console8SubHype",       "consoles"),
        ("Console8SubIn",         "consoles"),
        ("Console8SubOut",        "consoles"),
        ("Console0Buss",          "consoles"),
        ("Console0Channel",       "consoles"),
        ("PurestConsole3Buss",    "consoles"),
        ("PurestConsole3Channel", "consoles"),
        ("PurestConsole2Buss",    "consoles"),
        ("PurestConsole2Channel", "consoles"),
        ("Console7Buss",          "consoles"),
        ("Console7Channel",       "consoles"),
        ("Console7Cascade",       "consoles"),
        ("Console7Crunch",        "consoles"),
        ("PurestConsoleBuss",     "consoles"),
        ("PurestConsoleChannel",  "consoles"),
        ("Console6Buss",          "consoles"),
        ("Console6Channel",       "consoles"),
        ("PDBuss",                "consoles"),
        ("PDChannel",             "consoles"),
        ("Console5Buss",          "consoles"),
        ("Console5Channel",       "consoles"),
        ("Console5DarkCh",        "consoles"),
        ("C5RawBuss",             "consoles"),
        ("C5RawChannel",          "consoles"),
        ("AtmosphereBuss",        "consoles"),
        ("AtmosphereChannel",     "consoles"),
        ("EveryConsole",          "consoles"),
        # Other recently added plugins
        ("Dubly2",    "effects"),
        ("Dubly3",    "effects"),
        ("Pop3",      "dynamics"),
        ("StonefireComp", "dynamics"),
        ("DeRez3",    "lo-fi"),
        ("Flutter2",  "lo-fi"),
        ("TapeBias",  "lo-fi"),
        ("Isolator3", "filter"),
        ("Parametric","filter"),
        ("Galactic3", "reverb"),
        ("kCathedral3","reverb"),
        ("CreamCoat", "reverb"),
        ("CrunchCoat","reverb"),
        ("Hypersoft", "saturation"),
        ("Floor",      "bass"),
        ("Srsly3",    "stereo"),
        ("DitherFloat","dithers"),
    ]

    with open("CMakeLists.txt", "r", encoding="utf-8") as f:
        cmake_root = f.read()

    ok = 0
    fail = 0
    skip = 0

    for plugin_name, category in plugins_to_add:
        print(f"\nProcessing: {plugin_name} / {category}")
        success, cmake_root = process_plugin(plugin_name, category, cmake_root)
        if success:
            ok += 1
        else:
            fail += 1

    with open("CMakeLists.txt", "w", encoding="utf-8") as f:
        f.write(cmake_root)

    print(f"\nDone. OK={ok}, failed={fail}")

if __name__ == "__main__":
    main()
