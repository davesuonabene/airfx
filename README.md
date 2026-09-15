# AirFX: Airwindows Port for Max/MSP

AirFX is a library of Max external objects generated from [Chris Johnson's Airwindows](https://www.airwindows.com/) VST source code. The project utilizes the [Cycling '74 Min-DevKit](https://github.com/Cycling74/min-devkit) to wrap the original bare-metal C++ DSP logic into objects compatible with the Max/MSP environment.

## Repositories
- **Original Airwindows Source**: [https://github.com/airwindows/airwindows](https://github.com/airwindows/airwindows)
- **Upstream AirFX Repository**: [https://github.com/isabelgk/airfx](https://github.com/isabelgk/airfx)

## Build Instructions

### Requirements
- **Windows**: Visual Studio 2019 or later, CMake 3.15+.
- **macOS**: Xcode Command Line Tools, CMake 3.15+ (install via `brew install cmake`).

### Compilation — Windows

```bash
cmake -S . -B build
cmake --build build --config Release -- /maxcpucount /p:CL_MPcount=16
```

Binaries (`.mxe64`) are output to the `externals/` directory.

### Compilation — macOS

The macOS build requires additional patching steps before compilation due to differences in how Clang handles the generated C++ headers compared to MSVC.

#### Step 1 — Generate headers and wrappers for new plugins

```bash
python3 scripts/build_new_plugins.py
```

#### Step 2 — Patch generated headers

The code generator (`grab.py`) produces headers with two classes of issues that MSVC tolerates but Clang rejects:

1. **Missing constant definitions**: Reverb plugins (`CreamCoat`, `CrunchCoat`, `kCathedral3`) use `const int` constants (`kshortA`, `predelay`, `delayA`, etc.) defined in the original VST2 headers but not carried over by the scraper. These must be injected into the namespace of the generated `.hpp` wrapper.

2. **Extra closing braces**: The brace-matching logic in the scraper occasionally emits a stray `}` between the `process()` function body and the class closing `};`, causing `expected ';' after class` errors.

3. **Broken string literals**: Some `k_long_description` strings contain embedded C++ source code with unescaped characters (e.g., `DitherFloat`), which breaks `std::string_view` initialization.

Run the automated fix scripts:

```bash
python3 scripts/repair.py
python3 scripts/fix_all_mac.py
```

> **Note:** `repair.py` patches missing member variables by cross-referencing the original VST2 headers in `source/airwindohhs/airwindows/`. `fix_all_mac.py` fixes constants, brace structure, and validates all `.hpp` files.

#### Step 3 — Configure and build

```bash
cmake -B build_mac -DCMAKE_BUILD_TYPE=Release
cmake --build build_mac --config Release --parallel 8
```

Universal binaries (`.mxo`, x86_64 + arm64) are output to the `externals/` directory. All 401 externals are ad-hoc code signed during the build.

## macOS-Specific Changes from the Windows Version

### Critical: `dspsetup` member naming fix (`airfx.hpp`)

The base class `airfx<>` in `source/airfx/airfx.hpp` declares a Min API `message<>` handler for the `"dspsetup"` message. This handler calls `setSampleRate()` on the wrapped Airwindows effect.

The Min API uses C++ SFINAE to detect whether a class has a member named `dspsetup` or `m_dspsetup`. If detected, the API calls `self->m_min_object.dspsetup(args)` during DSP chain compilation to notify the object of the current sample rate.

The original code named this member `m_dsp_setup` (with an extra underscore), which **does not match** either SFINAE pattern. As a result:

- `setSampleRate()` was **never called** on the wrapped `Effect<T>`.
- The effect's internal sample rate remained at `0.0`.
- Nearly every Airwindows plugin computes `overallscale *= getSampleRate()`, which produced `0.0`, leading to **division by zero**, `NaN`/`inf` propagation, and **complete audio silence**.

**Fix:** Renamed `m_dsp_setup` → `dspsetup` in `airfx.hpp`.

> This bug was latent on Windows as well, but MSVC's different template instantiation behavior may have masked it. The fix is correct for both platforms.

### Critical: `m_samplerate` Zero-Initialization Fix (`effect.hpp`)

In Max/MSP on macOS, the audio engine occasionally triggers an empty `process()` call *before* `dspsetup` can provide the true sample rate. Because `m_samplerate` in the base `Effect` class was uninitialized (defaulting to `0.0`), this caused a divide-by-zero when plugins calculated `sqrt(overallscale)`. This propagated `NaN` values into infinite impulse response (IIR) filters, permanently silencing the plugins (like `galactic~`).

**Fix:** Initialized `T m_samplerate{44100.0};` in `source/airwindohhs/include/effect.hpp`. This provides a safe fallback for premature `process()` calls, preventing NaN propagation across all DAWs and OS platforms.

### Generated header patching

The following files required manual or scripted intervention for Clang compatibility:

| Issue | Files Affected | Fix |
|-------|---------------|-----|
| Missing `const int` constants (`kshortA`–`kshortP`, `predelay`, `delayA`–`delayY`) | `creamcoat.hpp`, `crunchcoat.hpp`, `kcathedral3.hpp` | Injected constant definitions into namespace scope |
| Extra closing brace before `};` | ~275 generated `.hpp` files | Removed extra `}` to balance brace count |
| Broken `k_long_description` string literal | `ditherfloat.hpp` | Replaced with shortened description |

### Build system

- macOS uses Unix Makefiles (single-config generator), so `CMAKE_BUILD_TYPE=Release` must be set at **configure time**, not at build time. The `--config Release` flag to `cmake --build` is ignored by Makefiles.
- The `CMakeLists.txt` sets `CMAKE_OSX_ARCHITECTURES` to `x86_64;arm64` for universal binary output.

## Repository Structure

```text
airfx/
├── externals/              # Compiled binary targets (.mxe64 / .mxo)
├── init/                   # Max object mappings (mc.wrapper~ multichannel support)
├── source/
│   ├── airfx/              # Min-DevKit base class integration (airfx.hpp)
│   ├── airwindohhs/        # Airwindows-specific components
│   │   ├── airwindows/     # Upstream VST2 source subtrees
│   │   ├── include/        # Generated C++ DSP headers
│   │   └── scripts/        # Scraper and codegen pipeline
│   └── objects/            # Min-DevKit object wrappers (one per plugin)
├── scripts/                # Codegen templates (example.cpp, example.CMakeLists.txt)
├── build_new_plugins.py    # Plugin generation orchestration script
├── repair.py               # Cross-references VST2 headers to patch missing members
├── fix_all_mac.py          # macOS-specific header fixes (constants, braces, strings)
└── fix_all_headers.py      # Enum block repair script
```

## DSP Integration Pipeline

The porting process is automated to ensure consistency across the 400+ plugins.

### 1. DSP Scraper (`grab.py`)
The scraper performs lexical analysis on the original VST2 class definitions:
-   **Namespace Management**: Handles collisions with the standard library by adjusting slugs for reserved keywords (e.g., `floor`, `abs`).
-   **Static Analysis**: Extracts `AudioEffectX` member variables and converts `const int` constants into class-template-safe `static const` members for the HPP wrappers.
-   **Algorithm Extraction**: Isolates the DSP loop within `processDoubleReplacing`, handling non-standard character encodings and brace-matching edge cases (e.g., nested `enum` blocks in the private section).
-   **Metadata**: Aggregates descriptions and tags from the `airwindopedia.txt` database.

### 2. Header Generation
The extracted logic is injected into `source/airwindohhs/include/` via a template system. The resulting header defines an `Effect<T>` subclass that allows for compile-time floating-point precision selection.

### 3. Min-DevKit Wrapping
The orchestration script generates a C++ wrapper in `source/objects/` that inherits from `airfx<>`. This layer bridges the Min-DevKit message-passing system to the underlying `Effect<T>` instance.

## Adding New Models

To integrate additional Airwindows plugins:

1.  Sync the VST source directory to `source/airwindohhs/airwindows/`.
2.  Append the plugin metadata (title, category) to the `plugins_to_add` list in `build_new_plugins.py`.
3.  Execute the orchestration script:
    ```bash
    python3 scripts/build_new_plugins.py
    ```
4.  Run the header fix scripts (required for macOS):
    ```bash
    python3 scripts/repair.py
    python3 scripts/fix_all_mac.py
    ```
5.  Re-run the CMake build. The script automatically handles header generation, object wrapper creation, and root `CMakeLists.txt` registration.

## Implementation Details

### String Literal Handling
All plugin metadata is stored using C++ raw string literals `R"(...)"` to support unescaped quotes and special characters present in the original documentation.

### Processing Optimization
- **Windows**: The build system uses the `/MP` flag for multi-processor compilation.
- **macOS**: Use `--parallel N` with CMake to control parallelism.

## License
Both the AirFX wrapper and the original Airwindows source code are distributed under the MIT License.
