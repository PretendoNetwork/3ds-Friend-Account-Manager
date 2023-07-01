# Decompressing code.bin files
This is a tutorial on how to get, extract, and decompress code.bin files for patch compilation.

## Method 1
1. Have a hacked 3DS
2. Open GodMode9, then open "Title manager" -> "NAND / TWL"
3. For each module you need (list below), then press A, then press "Open title folder".
4. Then press A on the .app file in the folder, select "NCCH image options...".
5. Select "Extract .code"
6. 0:/gm9/out should now have a file called <ModuleTitleID>.dec.code, which is your code.bin for that app.
7. Repeat steps 3-6 for the remaining modules.
8. Done!

### Modules to dump
- Account (act): 0004013000003802
- Friends (friends): 0004013000003202
- SSL (ssl): 0004013000002F02
- Miiverse (miiverse):
    - USA: 000400300000BD02
    - EUR: 000400300000BE02
    - JPN: 000400300000BC02

Any miiverse module is fine and should generate a patch that works and + is identical for all regions.

## Method 2 (Requires running a GM9 script)
1. Have a hacked 3DS
2. Download dump_modules.gm9 from [here](https://github.com/TraceEntertains/nimbus-module-extractor/releases/latest)
3. Put it in sd:/gm9/scripts
4. Run the script (Home Button -> Scripts -> dump_modules.gm9)
5. Turn off your 3DS and remove the SD card.
6. Put the SD card in your PC.
7. Extract the zip with the exe in it from [here](https://github.com/TraceEntertains/nimbus-module-extractor/releases/latest) (same link again)
8. Drag and drop your nimbus folder onto the executable while your 3DS SD card is in your PC.
9. Done!