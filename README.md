# Nimbus
## Pretendo account manager for the 3DS

## Usage

1. Grab the latest app and IPS patches from the [Releases](https://github.com/PretendoNetwork/nimbus/releases) page
2. Extract to the root of your 3DS SD card
3. Install the Nimbus homebrew using FBI if using the CIA build
4. Run the Nimbus homebrew and select either to use a Pretendo or Nintendo account

## Building

1. Install devkitARM, libctru, [armips](https://github.com/Kingcom/armips), [makerom](https://github.com/3DSGuy/Project_CTR), [bannertool](https://github.com/Steveice10/bannertool/) and [flips](https://github.com/Alcaro/Flips)
2. Copy [decompressed `code.bin`](https://github.com/PretendoNetwork/nimbus/blob/main/DECOMPRESSING.md) files from the friends, ssl, miiverse, and act sysmodules in their respective `patches` directories (any Miiverse code.bin works)
3. Run `make`

## Credits

Thanks to:

- [pinklimes](https://github.com/gitlimes) for the banner
- [TraceEntertains](https://github.com/TraceEntertains) for making a CIA version of Nimbus and a few other things
- [SciresM](https://github.com/SciresM) for making the 3DS SSL patches
- [zaksabeast](https://github.com/zaksabeast) for the original 3ds-Friend-Account-Manager and all the research into the friends and act system titles
- [shutterbug2000](https://github.com/shutterbug2000) for the GUI
- [libctru](https://github.com/devkitPro/libctru) for the `frda.c` base, homebrew template, and all of the library functions
- All 3ds researchers
