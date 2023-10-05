#include "acta.hpp"
#include "frda.hpp"
#include "sheet.h"
#include "sheet_t3x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


C2D_Sprite debug_button;
C2D_Sprite debug_header;
C2D_Sprite go_back;
C2D_Sprite header;
C2D_Sprite nintendo_unloaded_deselected;
C2D_Sprite nintendo_unloaded_selected;
C2D_Sprite nintendo_loaded_selected;
C2D_Sprite nintendo_loaded_deselected;
C2D_Sprite pretendo_unloaded_deselected;
C2D_Sprite pretendo_unloaded_selected;
C2D_Sprite pretendo_loaded_selected;
C2D_Sprite pretendo_loaded_deselected;
C2D_Sprite top;

u32 screen = 0;
u32 state = 0;
u32 lastState = 0;

Account currentEnv = Account::Undefined;
Account buttonSelected = Account::Undefined;
NascEnvironment environment;

u32 defaultColor = C2D_Color32(255, 255, 255, 0xFF);

bool firstStateRun = true;

bool buttonWasPressed = false;
bool needsReboot = false;

// startup checking variables
s64 firmwareVersion;
std::tuple<u8, u8, u8> lumaVersion;

s64 configVersion;
std::tuple<u8, u8> lumaConfigVersion;

s64 lumaOptions;
bool gamePatchingEnabled;
bool externalFirmsAndModulesEnabled;

static void sceneInit(void)
{
	C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoadFromMem(sheet_t3x, sheet_t3x_size);
	C2D_SpriteFromSheet(&top, spriteSheet, sheet_top_idx);
	C2D_SpriteFromSheet(&go_back, spriteSheet, sheet_go_back_idx);
	C2D_SpriteFromSheet(&header, spriteSheet, sheet_header_idx);
	C2D_SpriteFromSheet(&nintendo_unloaded_deselected, spriteSheet, sheet_nintendo_unloaded_deselected_idx);
	C2D_SpriteFromSheet(&nintendo_unloaded_selected, spriteSheet, sheet_nintendo_unloaded_selected_idx);
	C2D_SpriteFromSheet(&nintendo_loaded_selected, spriteSheet, sheet_nintendo_loaded_selected_idx);
	C2D_SpriteFromSheet(&nintendo_loaded_deselected, spriteSheet, sheet_nintendo_loaded_deselected_idx);
	C2D_SpriteFromSheet(&pretendo_unloaded_deselected, spriteSheet, sheet_pretendo_unloaded_deselected_idx);
	C2D_SpriteFromSheet(&pretendo_unloaded_selected, spriteSheet, sheet_pretendo_unloaded_selected_idx);
	C2D_SpriteFromSheet(&pretendo_loaded_selected, spriteSheet, sheet_pretendo_loaded_selected_idx);
	C2D_SpriteFromSheet(&pretendo_loaded_deselected, spriteSheet, sheet_pretendo_loaded_deselected_idx);
	C2D_SpriteSetCenter(&top, 0.49f, 0.49f);
	C2D_SpriteSetPos(&top, 400/2, 240/2);
	C2D_SpriteSetPos(&go_back, 0, 214);
	C2D_SpriteSetPos(&header, 95, 14);
	C2D_SpriteSetPos(&pretendo_loaded_selected, 49, 59);
	C2D_SpriteSetPos(&pretendo_unloaded_selected, 49, 59);
	C2D_SpriteSetPos(&pretendo_unloaded_deselected, 49, 59);
	C2D_SpriteSetPos(&pretendo_loaded_deselected, 49, 59);
	C2D_SpriteSetPos(&nintendo_loaded_selected, 165, 59);
	C2D_SpriteSetPos(&nintendo_unloaded_selected, 165, 59);
	C2D_SpriteSetPos(&nintendo_unloaded_deselected, 165, 59);
	C2D_SpriteSetPos(&nintendo_loaded_deselected, 165, 59);
	
	textBuf = C2D_TextBufNew(4096); // initialize the text buffer with a max glyph count of 4096
}

/**
 * Switches the friends and act accounts.
 *
 * This will also create the act account if it's missing, just to be safe.
 * This will fail if the friend account hasn't been created yet.
 */
Result switchAccounts(Account friend_account_id) {
	Result rc = 0;

	handleResult(FRDA_SetLocalAccountId(friend_account_id), "Switch account");
	if (rc) {
		return rc;
	}

	u32 act_account_index = 0;
	handleResult(ACTA_GetAccountIndexOfFriendAccountId(&act_account_index, friend_account_id), "Get persistent id for creation");

	if (act_account_index == 0) {
		handleResult(ACTA_CreateLocalAccount(), "Create act account");
		handleResult(ACTA_GetAccountIndexOfFriendAccountId(&act_account_index, friend_account_id),
								 "Get persistent id after creation");
	}

	handleResult(ACTA_SetDefaultAccount(act_account_index), "Set default account");

	return rc;
}

Result createAccount(Account friend_account_id) {
	Result rc = 0;

	// (Re)Create the friend account
	handleResult(FRDA_CreateLocalAccount(friend_account_id, environment, 0, 1), "Create account");
	// Switch to the friend/act accounts
	handleResult(switchAccounts(friend_account_id), "Switch account");
	// Reset the act account
	handleResult(ACTA_ResetAccount(static_cast<u8>(friend_account_id), true), "Reset account");

	return rc;
}

void drawLumaInfo() {
	DrawString(0.5f, defaultColor, std::format("Luma version is {}.{}.{}\nLuma config version is {}.{}\n\nLuma3DS config bits are:\n{:016b}\n{:016b}\n{:016b}\n{:016b}",
	std::get<0>(lumaVersion), std::get<1>(lumaVersion), std::get<2>(lumaVersion), std::get<0>(lumaConfigVersion), std::get<1>(lumaConfigVersion), lumaOptions >> 48, (lumaOptions >> 32) & 0xFFFF, (lumaOptions >> 16) & 0xFFFF, lumaOptions & 0xFFFF), 0);
}

bool checkIfLumaOptionsEnabled(C3D_RenderTarget* top_screen, C3D_RenderTarget* bottom_screen, u32 kDown, u32 kHeld, touchPosition touch) 
{	
	kDown |= kHeld; // make kDown have held keys aswell

	C2D_SceneBegin(top_screen);
	DrawVersionString();
	
	C2D_SceneBegin(bottom_screen);
	
	// if this is the first time the function has been run, get luma information
	if (firstStateRun) {
		firmwareVersion = GetSystemInfoField(GetSystemInfoCFW, CFWSystemInfoField::FirmwareVersion);
		lumaVersion = UnpackLumaVersion(GetSystemInfoCFW);
		
		configVersion = GetSystemInfoField(GetSystemInfoCFW, CFWSystemInfoField::ConfigVersion); // this
		lumaConfigVersion = UnpackConfigVersion(configVersion);
		
		lumaOptions = GetSystemInfoField(GetSystemInfoCFW, CFWSystemInfoField::ConfigBits); // this
		externalFirmsAndModulesEnabled = GetLumaOptionByIndex(LumaConfigBitIndex::ExternalFirmsAndModules, lumaOptions); // this
		gamePatchingEnabled = GetLumaOptionByIndex(LumaConfigBitIndex::GamePatching, lumaOptions); // and this might need multiple updates due to the fact that they fluctuate occasionally, if need be i can make a function that handles multiple versions though
	}
	
	// if the major version of luma3ds is under the targetLumaVersion (defined earlier in the file), send an error
	if (std::get<0>(lumaVersion) < targetLumaVersion) {
		DrawString(0.5f, defaultColor, std::format("Your Luma3DS version is out of date, it should be Luma3DS {} or newer for {} to function. Press A to exit.", targetLumaVersion, APP_TITLE), 0);
		
		// if A is pressed, return true to exit
		if (kDown & KEY_A) return true;
	}
	
	// else if either external firms and modules or game patching is not enabled, send another error and draw luma info if b is pressed
	else if (!externalFirmsAndModulesEnabled || !gamePatchingEnabled) {
		if (kDown & KEY_B) {
			drawLumaInfo();
		}
		else {
			DrawString(0.5f, defaultColor, std::format("Enable external FIRMs and modules: {}\nEnable game patching: {}\n\nFor {} to work, both of these Luma3DS options should be ENABLED. To open Luma3DS settings, hold SELECT while booting your system.\n\n\
If you are sure both options are enabled and the options shown don't match your Luma3DS settings, please contact @traceentertains on Discord with an image of the more information screen attached.\nPress A to exit, or hold B for more information.", externalFirmsAndModulesEnabled, gamePatchingEnabled, APP_TITLE), 0);	
		}

		// if A is pressed, return true to exit, else if X and Y is pressed
		if (kDown & KEY_A) return true;
		else if (kDown & KEY_X && kDown & KEY_Y) state = 2; // bypass if I need some time to fix it and get it released
	}
	else {
		if (kDown & KEY_A) drawLumaInfo();
		else state = 2; // if A is held, show information, else go to the main menu
	}
	
	return false;
}

bool drawUI(C3D_RenderTarget* top_screen, C3D_RenderTarget* bottom_screen, u32 kDown, u32 kHeld, touchPosition touch)
{
	// if start is pressed, exit to hbl/the home menu depending on if the app was launched from cia or 3dsx
	if (kDown & KEY_START) return true;
	
	C2D_SceneBegin(top_screen);
	DrawVersionString();
	C2D_DrawSprite(&top);
	
	C2D_SceneBegin(bottom_screen);
	
	if (buttonSelected == Account::Nintendo) {
		if (currentEnv == Account::Nintendo) {
			C2D_DrawSprite(&nintendo_loaded_selected);
			C2D_DrawSprite(&pretendo_unloaded_deselected);
		}
		else {
			C2D_DrawSprite(&nintendo_unloaded_selected);
			C2D_DrawSprite(&pretendo_loaded_deselected);
		}
	}
	else if (buttonSelected == Account::Pretendo) {
		if (currentEnv == Account::Pretendo) {
			C2D_DrawSprite(&nintendo_unloaded_deselected);
			C2D_DrawSprite(&pretendo_loaded_selected);
		}
		else {
			C2D_DrawSprite(&nintendo_loaded_deselected);
			C2D_DrawSprite(&pretendo_unloaded_selected);
		}
	}
	C2D_DrawSprite(&header);

	if (kDown & KEY_TOUCH) {
		if ((touch.px >= 165 && touch.px <= 165 + 104) && (touch.py >= 59 && touch.py <= 59 + 113)) {
			buttonSelected = Account::Nintendo;
			buttonWasPressed = true;
		}
		else if ((touch.px >= 49 && touch.px <= 49 + 104) && (touch.py >= 59 && touch.py <= 59 + 113)) {
			buttonSelected = Account::Pretendo;
			buttonWasPressed = true;
		}
	}
	else if (kDown & KEY_LEFT || kDown & KEY_RIGHT) {
		buttonSelected = buttonSelected == Account::Pretendo ? Account::Nintendo : Account::Pretendo;
	}
	
	environment = AccountToNascEnvironment(buttonSelected);
	
	if (kDown & KEY_A) {
		buttonWasPressed = true;
	}
	
	if (buttonWasPressed) {
		if (switchAccounts(buttonSelected) && buttonSelected == Account::Pretendo) {
			createAccount(buttonSelected);
		}
		
		needsReboot = true;
		
		return true;
	}
	
	return false;
}

int main()
{
	// Initialize the libs
	nsInit();
	frdAInit();
	actAInit();
	gfxInitDefault();
	
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// This version or higher is required creating/swapping friend accounts
	FRDA_SetClientSdkVersion(0x70000c8);

	// Create screen
	C3D_RenderTarget* top_screen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom_screen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// Initialize the scene
	sceneInit();

	// get the local account id for the current enviroment and set the selected button to it
	FRDA_GetLocalAccountId(&currentEnv);
	environment = AccountToNascEnvironment(currentEnv);
	
	if (currentEnv == Account::Undefined) currentEnv = Account::Nintendo;
	
	buttonSelected = currentEnv;

	// Main loop
	while (aptMainLoop()) {
		bool exit = false;
		
		// get any input, and if applicable the location where the screen was touched
		hidScanInput();
		touchPosition touch;
		hidTouchRead(&touch);
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		
		C2D_TargetClear(top_screen, C2D_Color32(21, 22, 28, 0xFF));
		C2D_TargetClear(bottom_screen, C2D_Color32(21, 22, 28, 0xFF));
		
		if (lastState != state) firstStateRun = true;
		
		if (state == 0) {
			exit = checkIfLumaOptionsEnabled(top_screen, bottom_screen, kDown, kHeld, touch);
		}
		else {
			exit = drawUI(top_screen, bottom_screen, kDown, kHeld, touch);
		}
		
		lastState = state;
		firstStateRun = false;
		
		C3D_FrameEnd(0);
		
		if (exit) break;
	}

	// Deinitialize the libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	if(needsReboot){
		NS_RebootSystem();
	}
	return 0;
}
