#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "./common.h"
#include <3ds/types.h>

typedef enum {
	NASC_ENV_Prod = 0,
	NASC_ENV_Test = 1,
	NASC_ENV_Dev = 2,
} NascEnvironment;

Result frdAInit();
void frdAExit();
Handle *frdAGetSessionHandle();
Result FRDA_CreateLocalAccount(u8 localAccountId, NascEnvironment nascEnvironment, u8 serverTypeField1, u8 serverTypeField2);
Result FRDA_GetLocalAccountId(u8 *localAccountId);
Result FRDA_SetLocalAccountId(u8 localAccountId);
Result FRDA_SetClientSdkVersion(u32 sdkVer);

#ifdef __cplusplus
}
#endif
