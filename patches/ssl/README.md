# 3DS SSL Patch
This is a patch for the 3DS SSL module to disable Root CA Verification originally from [here](https://github.com/SciresM/3DS-SSL-Patch), but slightly (no code differences) modified to fit the structure of nimbus better.

To use:

1. Get and decrypt the SSL module (0004013000002F02), and extract its [decompressed](https://github.com/TraceEntertains/nimbus/blob/main/DECOMPRESSING.md) code binary to the patch directory as "code.bin".
2. Run `make`.
3. Copy `out/code.ips` to `sdcard:/luma/titles/0004013000002F02/code.ips`.
4. Enable Game Patching and/or Allow Loading External FIRMs or System Modules under Luma3DS's configuration.

You may also need to send [ClCertA](https://github.com/SciresM/3DS-SSL-Patch/blob/master/ClCertA.p12?raw=true) as a client certificate. Unclear on that. ~~A copy is included as a .p12 resource, with the password `3ds`.~~ Certificate removed from this.

~~**NOTE**: As of May 01, 2017, Luma3DS [doesn't actually support patching system modules](https://github.com/AuroraWright/Luma3DS/blob/master/injector/source/patcher.c#L853). You'll need to comment out the check on that line (`if(CONFIG(PATCHGAMES))` will do fine), and rebuild luma manually to use.
I consider this to be a feature, not a bug, because having this installed will make it so that anyone can steal your 3ds's private information over the network because it makes all of your SSL requests insecure.~~ This is not true anymore, because to my knowledge, it only applies to non-ips patches.

That said, it's very useful for MitM debugging of how the 3DS communicates with its servers.
