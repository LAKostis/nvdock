# nvdock
Fork of nvdock, a tray icon to easily launch the nvidia-settings control panel

## Differences from original version
- Using NVCtrlLib to query NVIDIA hardware via X11 protocol instead of grep/sed parsing
- Multi GPU support

## Current limitations comparing to original version
- Only X11 protocol, no Wayland (anyway nvidia-settings won't work with Wayland).
