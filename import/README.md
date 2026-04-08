# import/

This directory contains third-party source code adapted for integration into the RasPiK
kernel tree.

## Contents

| File | Origin | License |
|------|--------|---------|
| `src/emmc.c` | [John Cronin's EMMC driver](https://github.com/jncronin/rpi-boot) | MIT |
| `include/emmc.h` | same | MIT |

## Policy

Files here originate from third-party sources and are adapted for integration into RasPiK.
Adaptations may include API renames, allocator substitution, debug removal, and style
normalization. Files are **not** otherwise subject to the project's coding-style rules.
Upstream bug fixes should be ported manually rather than applied as-is.
