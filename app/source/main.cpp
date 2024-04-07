#include "sysmodules/acta.hpp"
#include "sysmodules/frda.hpp"
#include "sheet.h"
#include "sheet_t3x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.hpp"
#include "states/LumaValidation.hpp"
#include "states/MainUI.hpp"

MainStruct mainStruct = MainStruct();

static void sceneInit(void)
{
	C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoadFromMem(sheet_t3x, sheet_t3x_size);
	C2D_SpriteFromSheet(mainStruct.top, spriteSheet, sheet_top_idx);
	C2D_SpriteFromSheet(mainStruct.go_back, spriteSheet, sheet_go_back_idx);
	C2D_SpriteFromSheet(mainStruct.header, spriteSheet, sheet_header_idx);
	C2D_SpriteFromSheet(mainStruct.nintendo_unloaded_deselected, spriteSheet, sheet_nintendo_unloaded_deselected_idx);
	C2D_SpriteFromSheet(mainStruct.nintendo_unloaded_selected, spriteSheet, sheet_nintendo_unloaded_selected_idx);
	C2D_SpriteFromSheet(mainStruct.nintendo_loaded_selected, spriteSheet, sheet_nintendo_loaded_selected_idx);
	C2D_SpriteFromSheet(mainStruct.nintendo_loaded_deselected, spriteSheet, sheet_nintendo_loaded_deselected_idx);
	C2D_SpriteFromSheet(mainStruct.pretendo_unloaded_deselected, spriteSheet, sheet_pretendo_unloaded_deselected_idx);
	C2D_SpriteFromSheet(mainStruct.pretendo_unloaded_selected, spriteSheet, sheet_pretendo_unloaded_selected_idx);
	C2D_SpriteFromSheet(mainStruct.pretendo_loaded_selected, spriteSheet, sheet_pretendo_loaded_selected_idx);
	C2D_SpriteFromSheet(mainStruct.pretendo_loaded_deselected, spriteSheet, sheet_pretendo_loaded_deselected_idx);
	C2D_SpriteSetCenter(mainStruct.top, 0.49f, 0.49f);
	C2D_SpriteSetPos(mainStruct.top, 400/2, 240/2);
	C2D_SpriteSetPos(mainStruct.go_back, 0, 214);
	C2D_SpriteSetPos(mainStruct.header, 95, 14);
	C2D_SpriteSetPos(mainStruct.pretendo_loaded_selected, 49, 59);
	C2D_SpriteSetPos(mainStruct.pretendo_unloaded_selected, 49, 59);
	C2D_SpriteSetPos(mainStruct.pretendo_unloaded_deselected, 49, 59);
	C2D_SpriteSetPos(mainStruct.pretendo_loaded_deselected, 49, 59);
	C2D_SpriteSetPos(mainStruct.nintendo_loaded_selected, 165, 59);
	C2D_SpriteSetPos(mainStruct.nintendo_unloaded_selected, 165, 59);
	C2D_SpriteSetPos(mainStruct.nintendo_unloaded_deselected, 165, 59);
	C2D_SpriteSetPos(mainStruct.nintendo_loaded_deselected, 165, 59);
	
	textBuf = C2D_TextBufNew(4096); // initialize the text buffer with a max glyph count of 4096
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

	// set button selected and current account to nasc environment
	u32 serverTypes[3] = {};
	FRDU_GetServerTypes(serverTypes);
	
	mainStruct.buttonSelected = static_cast<NascEnvironment>(serverTypes[0]);
	mainStruct.currentAccount = mainStruct.buttonSelected;

    // if running on citra, skip all luma checks
    s64 isCitra = 0;
    svcGetSystemInfo(&isCitra, 0x20000, 0);
    if (isCitra) {
        mainStruct.state = 1;
        return false;
    }

	// Main loop
	while (aptMainLoop()) {
		u32 exit = 0;
		
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
		
		// TODO: change firstRunOfState into stateRunIndex, incrementing every time the state is run
		if (mainStruct.lastState != mainStruct.state) mainStruct.firstRunOfState = true;
		
		if (mainStruct.state == 0) {
			exit = LumaValidation::checkIfLumaOptionsEnabled(&mainStruct, top_screen, bottom_screen, kDown, kHeld, touch);
		} else {
			exit = MainUI::drawUI(&mainStruct, top_screen, bottom_screen, kDown, kHeld, touch);
		}
		
		mainStruct.lastState = mainStruct.state;
		mainStruct.firstRunOfState = false;
		
		C3D_FrameEnd(0);
		
		if (exit < 0) break;
	}

	// Deinitialize the libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	if (mainStruct.needsReboot) {
		NS_RebootSystem();
	}
	return 0;
}
