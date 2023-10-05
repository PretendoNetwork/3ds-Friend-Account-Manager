#pragma once

#include "common.hpp"

Result frdAInit();
void frdAExit();
Handle *frdAGetSessionHandle();
Result FRDA_CreateLocalAccount(Account localAccountId, NascEnvironment nascEnvironment, u8 serverTypeField1, u8 serverTypeField2);
Result FRDA_GetLocalAccountId(Account *localAccountId);
Result FRDA_SetLocalAccountId(Account localAccountId);
Result FRDA_SetClientSdkVersion(u32 sdkVer);
