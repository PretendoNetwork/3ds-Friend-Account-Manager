# Decompressing code.bin files
This is a tutorial on how to get, extract, and decompress code.bin files for patch compilation.

## Method 1
1. Have a hacked 3DS
2. Open GodMode9, press the HOME button, then open "Title manager" -> "NAND / TWL"
3. For each module you need (list below), then press A, then press "Open title folder".
4. Then press A on the .app file in the folder, select "NCCH image options...".
5. Select "Extract .code"
6. 0:/gm9/out should now have a file called <ModuleTitleID>.dec.code, which is your code.bin for that app.
7. Repeat steps 3-6 for the remaining modules.
8. For each module, move it to its proper folder named as code.bin (folder names are next to module names in module list below)
9. Done!

### Modules to dump
- Account (act): 0004013000003802
- Friends (friends): 0004013000003202
- HTTP (http): 0004013000002902
- Socket (socket): 0004013000002E02
- SSL (ssl): 0004013000002F02
- Miiverse (miiverse):
    - JPN: 000400300000BC02
    - USA: 000400300000BD02
    - EUR: 000400300000BE02

Any region miiverse module is fine and should generate a patch that is identical for all regions.

## Method 2 (Requires running a GM9 script)
1. Have a hacked 3DS
2. Download dump_modules.gm9 from [here](https://github.com/TraceEntertains/nimbus-module-extractor/releases/latest)
3. Put it in sd:/gm9/scripts
4. Run the script (Home Button -> Scripts -> dump_modules.gm9)
5. Turn off your 3DS and remove the SD card.
6. Copy the folder (named patches) from the nimbusmodules folder located on your SD card to your local clone of nimbus, and merge when prompted.
7. Done!
