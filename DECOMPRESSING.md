# Decompressing code.bin files
This is a tutorial on how to get, extract, and decompress code.bin files for patch compilation.

## Method 1
1. Have a hacked 3DS
2. Open GodMode9, then open "Title manager" -> "NAND / TWL"
3. Find all modules you need, then on each one, press A, then press "Manage Title...", then press "Build CIA (standard)".
4. Get all the CIAs (located in sd:/gm9/out) onto your computer in a dedicated folder using ftpd or an SD card reader.
5. Place a ctrtool executable in that folder.
6. Now run `ctrtool --contents=contents cianame.cia` (replace cianame with the cia's name), then run `ctrtool --exefs exefs.bin contentsname` (replace contentsname with the name of the file the previous command created), then last but not least, run `ctrtool --exefsdir=exefs --decompersscode exefs.bin`
7. Move the code.bin folder into its proper patches folder, then delete the exefs.bin, contents file, and the CIA for that module if you would like.
8. Repeat steps 6-7 for the rest of your CIAs (or just make a script to do it for you)
9. Done!

## Method 2
1. Have a hacked 3DS
2. Open GodMode9, then open "Title manager" -> "NAND / TWL"
3. Find a module you need, then press A, then press "Open title folder".
4. Then press A on the .app file in the folder, select "NCCH image options...".
5. Select "Extract .code"
6. 0:/gm9/out should now have a file called <Titleid>.dec.code, which is your code.bin for that app.
7. Repeat steps 3-6 for the remaining modules.
8. Done!

## Method 3 (Requires external programs)
1. Have a hacked 3DS
2. Download dump_modules.gm9 from [here](https://github.com/TraceEntertains/nimbus-module-extractor/releases/latest)
3. Put it in sd:/gm9/scripts
4. Run the script (Home Button -> Scripts -> dump_modules.gm9)
5. Turn off your 3DS and remove the SD card.
6. Put the SD card in your PC.
7. Extract the zip with the exe in it from [here](https://github.com/TraceEntertains/nimbus-module-extractor/releases/latest) (same link again)
8. Drag and drop your nimbus folder onto the executable while your 3DS SD card is in your PC.
9. Done!
