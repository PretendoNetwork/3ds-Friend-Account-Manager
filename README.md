# Nimbus
## Pretendo account manager for the 3DS

## Usage

1. Grab the latest app and IPS patches from the [Releases](https://github.com/PretendoNetwork/nimbus/releases) page
2. Extract to the root of your 3DS SD card
3. Ensure Game Patching is enabled in Luma3DS settings (hold SELECT at boot to enter Luma settings)
4. Run the Nimbus homebrew and select the network you wish to use (Nintendo Network, or Pretendo Network)

## Building

1. Install devkitARM, libctru, [armips](https://github.com/Kingcom/armips), and [flips](https://github.com/Alcaro/Flips)
2. Copy decompressed `code.bin` files from the friends and act sysmodules in their respective `patches` directories
3. Run `make`

## Credits

Thanks to:

- [zaksabeast](https://github.com/zaksabeast) for the original 3ds-Friend-Account-Manager and all the research into the friends and act system titles
- [shutterbug2000](https://github.com/shutterbug2000) for the GUI
- [libctru](https://github.com/devkitPro/libctru) for the `frda.c` base, homebrew template, and all of the library functions
- All 3ds researchers
