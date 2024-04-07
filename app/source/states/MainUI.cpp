#include "MainUI.hpp"
#include "../sysmodules/frda.hpp"
#include "../sysmodules/acta.hpp"

Result MainUI::switchAccounts(u8 friend_account_id) {
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

Result MainUI::createAccount(u8 friend_account_id, NascEnvironment environmentId) {
	Result rc = 0;

	// (Re)Create the friend account
	handleResult(FRDA_CreateLocalAccount(friend_account_id, environmentId, 0, 1), "Create account");
	// Switch to the friend/act accounts
	handleResult(switchAccounts(friend_account_id), "Switch account");
	// Reset the act account
	handleResult(ACTA_ResetAccount(friend_account_id, true), "Reset account");

	return rc;
}

bool MainUI::drawUI(MainStruct *mainStruct, C3D_RenderTarget* top_screen, C3D_RenderTarget* bottom_screen, u32 kDown, u32 kHeld, touchPosition touch)
{
    // if start is pressed, exit to hbl/the home menu depending on if the app was launched from cia or 3dsx
    if (kDown & KEY_START) return true;
    
    C2D_SceneBegin(top_screen);
    DrawVersionString();
    C2D_DrawSprite(&mainStruct->top);
    
    C2D_SceneBegin(bottom_screen);
    
    if (mainStruct->buttonSelected == NascEnvironment::NASC_ENV_Prod) {
        if (mainStruct->currentAccount == NascEnvironment::NASC_ENV_Prod) {
            C2D_DrawSprite(&mainStruct->nintendo_loaded_selected);
            C2D_DrawSprite(&mainStruct->pretendo_unloaded_deselected);
        }
        else {
            C2D_DrawSprite(&mainStruct->nintendo_unloaded_selected);
            C2D_DrawSprite(&mainStruct->pretendo_loaded_deselected);
        }
    }
    else if (mainStruct->buttonSelected == NascEnvironment::NASC_ENV_Test) {
        if (mainStruct->currentAccount == NascEnvironment::NASC_ENV_Test) {
            C2D_DrawSprite(&mainStruct->nintendo_unloaded_deselected);
            C2D_DrawSprite(&mainStruct->pretendo_loaded_selected);
        }
        else {
            C2D_DrawSprite(&mainStruct->nintendo_loaded_deselected);
            C2D_DrawSprite(&mainStruct->pretendo_unloaded_selected);
        }
    }
    C2D_DrawSprite(&mainStruct->header);

    // handle touch input
    if (kDown & KEY_TOUCH) {
        if ((touch.px >= 165 && touch.px <= 165 + 104) && (touch.py >= 59 && touch.py <= 59 + 113)) {
            mainStruct->buttonSelected = NascEnvironment::NASC_ENV_Prod;
            mainStruct->buttonWasPressed = true;
        }
        else if ((touch.px >= 49 && touch.px <= 49 + 104) && (touch.py >= 59 && touch.py <= 59 + 113)) {
            mainStruct->buttonSelected = NascEnvironment::NASC_ENV_Test;
            mainStruct->buttonWasPressed = true;
        }
    }
    else if (kDown & KEY_LEFT || kDown & KEY_RIGHT) {
        mainStruct->buttonSelected = mainStruct->buttonSelected == NascEnvironment::NASC_ENV_Test ? NascEnvironment::NASC_ENV_Prod : NascEnvironment::NASC_ENV_Test;
    }
    
    if (kDown & KEY_A) {
        mainStruct->buttonWasPressed = true;
    }
    
    if (mainStruct->buttonWasPressed) {
        u8 accountId = (u8)mainStruct->buttonSelected + 1; // by default set accountId to nasc environment + 1

        if (switchAccounts(accountId) && accountId == 2) {
            createAccount(accountId, NascEnvironment::NASC_ENV_Test);
        }
        
        mainStruct->needsReboot = true;
        
        return true;
    }
    
    return false;
}