/*
This file was originally taken from
https://github.com/devkitPro/libctru/blob/6360f4bdb1ca5f8131ffc92640c1dd16afb63083/libctru/source/services/frd.c
and modified to:
- Remove every command, except FRD_SetClientSdkVersion
- Add FRDA_CreateLocalAccount and FRDA_SetLocalAccountId
- Only allow frd:a to be used
*/

#include "frda.hpp"

static Handle frdHandle;
static int frdRefCount;

Result frdAInit() {
	Result ret = 0;

	if (AtomicPostIncrement(&frdRefCount))
		return 0;

	ret = srvGetServiceHandle(&frdHandle, "frd:a");
	if (R_FAILED(ret))
		AtomicDecrement(&frdRefCount);

	return ret;
}

void frdAExit() {
	if (AtomicDecrement(&frdRefCount))
		return;
	svcCloseHandle(frdHandle);
}

Handle *frdAGetSessionHandle(void) {
	return &frdHandle;
}

Result FRDA_CreateLocalAccount(Account localAccountId, NascEnvironment nascEnvironment, u8 serverTypeField1, u8 serverTypeField2) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x401, 4, 0);
	cmdbuf[1] = static_cast<u32>(localAccountId);
	cmdbuf[2] = static_cast<u32>(nascEnvironment);
	cmdbuf[3] = serverTypeField1;
	cmdbuf[4] = serverTypeField2;

	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result FRDA_GetLocalAccountId(Account *localAccountId) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0xB, 2, 0);

	if (R_FAILED(ret = svcSendSyncRequest(frdHandle))) return ret;

	*localAccountId = static_cast<Account>(cmdbuf[2]);

	return (Result)cmdbuf[1];
}

Result FRDA_SetLocalAccountId(Account localAccountId) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x403, 1, 0);
	cmdbuf[1] = static_cast<u32>(localAccountId);

	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result FRDA_SetClientSdkVersion(u32 sdkVer) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x32, 1, 2);
	cmdbuf[1] = sdkVer;
	cmdbuf[2] = IPC_Desc_CurProcessId();

	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))
		return ret;

	return (Result)cmdbuf[1];
}
