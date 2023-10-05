# Nimbus
## Pretendo account manager for the 3DS

## Usage

1. Grab the latest app and IPS patches from the [Releases](https://github.com/PretendoNetwork/nimbus/releases) page
2. Extract to the root of your 3DS SD card
3. Install the Nimbus homebrew using FBI (or FBI Reloaded) if using the CIA build
4. Run the Nimbus homebrew and select either to use a Pretendo or Nintendo account
     - If it doesn't work, reboot your 3DS while holding SELECT and ensure that "Enable loading external FIRMs and modules" and "Enable game patching" are both turned on, as well as ensuring that your Luma3DS version is 13.0 or higher.

## Building

1. Clone the repository recursively using `git clone https://github.com/PretendoNetwork/nimbus --recursive`
    - If you have cloned the repository previously, please run `git pull` and `make clean` while in the nimbus folder to avoid errors and broken files
    - On top of that, if you cloned it before 1.0.2 released, you might also need to run `git submodule update --init --recursive` while in the nimbus folder
2. Install devkitARM, libctru, [armips](https://github.com/Kingcom/armips), [makerom](https://github.com/3DSGuy/Project_CTR), [bannertool](https://github.com/Steveice10/bannertool) and [flips](https://github.com/Alcaro/Flips)
3. Copy [decompressed `code.bin`](https://github.com/PretendoNetwork/nimbus/blob/main/DECOMPRESSING.md) files from the friends, ssl, miiverse, and act sysmodules in their respective `patches` directories (any Miiverse code.bin works for the miiverse module)
4. Run `make`

## Credits

Thanks to:

- [pinklimes](https://github.com/gitlimes) for the CIA version banner
- [TraceEntertains](https://github.com/TraceEntertains) for making a CIA version of Nimbus and maintaining the project
- [SciresM](https://github.com/SciresM) for making the 3DS SSL patches
- [zaksabeast](https://github.com/zaksabeast) for the original 3ds-Friend-Account-Manager and all the research into the friends and act system titles
- [shutterbug2000](https://github.com/shutterbug2000) for the GUI
- [libctru](https://github.com/devkitPro/libctru) for the `frda.c` base, homebrew template, and other library functions (and thanks to citro2d for part of a system font function)
- [Universal-Core](https://github.com/Universal-Team/Universal-Core) for the string drawing functions
- All other 3DS researchers
