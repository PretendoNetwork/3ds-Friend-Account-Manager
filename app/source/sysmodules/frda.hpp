#pragma once

#include "../common.hpp"

Result frdAInit();
void frdAExit();
Handle *frdAGetSessionHandle();
Result FRDA_CreateLocalAccount(u8 localAccountId, NascEnvironment nascEnvironment, u8 serverTypeField1, u8 serverTypeField2);
Result FRDA_GetLocalAccountId(u8 *localAccountId);
Result FRDA_SetLocalAccountId(u8 localAccountId);
Result FRDA_SetClientSdkVersion(u32 sdkVer);
Result FRDA_GetServerTypes(u32 *out);