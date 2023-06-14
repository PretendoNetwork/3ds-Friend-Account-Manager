# Decompressing code.bin files
This is a tutorial on how to get, extract, and decompress code.bin files for patch compilation.

1. Have a hacked 3DS
2. Open GodMode9, then open "Title manager" -> "NAND / TWL"
3. Find all modules you need, then on each one, press A, then press "Manage Title...", then press "Build CIA (standard)".
4. Get all the CIAs (located in sd:/gm9/out) onto your computer in a dedicated folder using ftpd or an SD card reader.
5. Place a ctrtool executable in that folder.
6. Now run `ctrtool --contents=contents cianame.cia` (replace cianame with the cia's name), then run `ctrtool --exefs exefs.bin contentsname` (replace contentsname with the name of the file the previous command created), then last but not least, run `ctrtool --exefsdir=exefs --decompersscode exefs.bin`
7. Move the code.bin folder into its proper patches folder, then delete the exefs.bin, contents file, and the CIA for that module if you would like.
8. Repeat steps 6-7 for the rest of your CIAs (or just make a script to do it for you)
