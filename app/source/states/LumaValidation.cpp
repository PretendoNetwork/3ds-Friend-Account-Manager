#include "LumaValidation.hpp"
#include <format>

bool LumaValidation::checkIfLumaOptionsEnabled(MainStruct *mainStruct, C3D_RenderTarget* top_screen, C3D_RenderTarget* bottom_screen, u32 kDown, u32 kHeld, touchPosition touch) 
{	
    kDown |= kHeld; // make kDown have held keys aswell

    C2D_SceneBegin(top_screen);
    DrawVersionString();
    
    C2D_SceneBegin(bottom_screen);

    // if running on citra, skip all luma checks
    s64 isCitra = 0;
    svcGetSystemInfo(&isCitra, 0x20000, 0);
    if (isCitra) {
        mainStruct->state = 1;
		return false;
    }


    // if this is the first time the function has been run, get luma information
    if (mainStruct->firstRunOfState) {
        mainStruct->firmwareVersion = GetSystemInfoField(GetSystemInfoCFW, CFWSystemInfoField::FirmwareVersion);
        mainStruct->lumaVersion = UnpackLumaVersion(mainStruct->firmwareVersion);
        
        mainStruct->configVersion = GetSystemInfoField(GetSystemInfoCFW, CFWSystemInfoField::ConfigVersion); // this
        mainStruct->lumaConfigVersion = UnpackConfigVersion(mainStruct->configVersion);
        
        mainStruct->lumaOptions = GetSystemInfoField(GetSystemInfoCFW, CFWSystemInfoField::ConfigBits); // this
        mainStruct->externalFirmsAndModulesEnabled = GetLumaOptionByIndex(LumaConfigBitIndex::ExternalFirmsAndModules, mainStruct->lumaOptions); // this
        mainStruct->gamePatchingEnabled = GetLumaOptionByIndex(LumaConfigBitIndex::GamePatching, mainStruct->lumaOptions); // and this might need multiple updates due to the fact that they fluctuate occasionally, if need be i can make a function that handles multiple versions though
    }

    // if the major version of luma3ds is under the targetLumaVersion (defined earlier in the file), send an error
    if (std::get<0>(mainStruct->lumaVersion) < targetLumaVersion) {
        DrawString(0.5f, defaultColor, std::format("Your Luma3DS version is out of date, it should be Luma3DS {} or newer for {} to function. Press A to exit.", targetLumaVersion, APP_TITLE), 0);
        
        // if A is pressed, return true to exit
        if (kDown & KEY_A) return true;
    }
    
    // else if either external firms and modules or game patching is not enabled, send another error and draw luma info if b is pressed
    else if (!mainStruct->externalFirmsAndModulesEnabled || !mainStruct->gamePatchingEnabled) {
        if (kDown & KEY_B) {
            drawLumaInfo(mainStruct);
        }
        else {
            DrawString(0.5f, defaultColor, std::format("Enable external FIRMs and modules: {}\nEnable game patching: {}\n\nFor {} to work, both of these Luma3DS options should be ENABLED. To open Luma3DS settings, hold SELECT while booting your system.\n\n\
If you are sure both options are enabled and the options shown don't match your Luma3DS settings, please contact @traceentertains on Discord with an image of the more information screen attached.\nPress A to exit, or hold B for more information.", mainStruct->externalFirmsAndModulesEnabled, mainStruct->gamePatchingEnabled, APP_TITLE), 0);	
        }

        // if A is pressed, return true to exit, else if X and Y is pressed
        if (kDown & KEY_A) return true;
        else if (kDown & KEY_X && kDown & KEY_Y) mainStruct->state = 1; // bypass if I need some time to fix it and get it released
    }
    else {
        if (kDown & KEY_A) drawLumaInfo(mainStruct);
        else mainStruct->state = 1; // if A is held, show information, else go to the main menu
    }
    
    return false;
}