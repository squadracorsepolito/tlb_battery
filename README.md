# TSAL Latch Board Firmware
---
![3d_pcb_view](./doc/media/TLB_PCB_3D_view.png)

## Dependencies
- `CMake`
- `arm-none-eabi-gcc`
- `openocd`
- `STM32CubeMx`

## Compile
```
cmake 
make clean
```
## Flash On Linux
1) the stlink programmer is correctly connected.
2) the board is powerd on (24V power with DC/DC or 5V power without)
3) Linux Udev rules are correctly set for `openocd` (check `/etc/udev/rules.d` folder or the [openocd documentation](https://openocd.org/doc-release/README)). The result of not having correctly set `udev` rules is that `openocd` will not run because of missing permissions, while it will run with `sudo` (** DON'T RUN OPENOCD or any other binary WITH SUDO **)
4) Run from the repo root directory
```
make flash
```
Or
```
openocd -f openocd.cfg -c "program build/tlb_battery.elf verify reset exit"
```

