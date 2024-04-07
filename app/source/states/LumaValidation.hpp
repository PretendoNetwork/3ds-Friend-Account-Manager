#pragma once

#include "../common.hpp"

namespace LumaValidation
{
    bool checkIfLumaOptionsEnabled(MainStruct *mainStruct, C3D_RenderTarget* top_screen, C3D_RenderTarget* bottom_screen, u32 kDown, u32 kHeld, touchPosition touch);
}
