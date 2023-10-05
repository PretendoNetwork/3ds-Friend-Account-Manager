#include "acta.hpp"

static Handle actHandle;
static int actRefCount;

Result actAInit() {
	Result ret = 0;

	if (AtomicPostIncrement(&actRefCount))
		return 0;

	ret = srvGetServiceHandle(&actHandle, "act:a");
	if (R_FAILED(ret))
		AtomicDecrement(&actRefCount);

	return ret;
}

void actAExit() {
	if (AtomicDecrement(&actRefCount))
		return;
	svcCloseHandle(actHandle);
}

/**
 * Under the hood, this creates a local account by incrementing
 * the persistent id and using the current friend account.
 */
Result ACTA_CreateLocalAccount() {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x402, 0, 0);

	if (R_FAILED(ret = svcSendSyncRequest(actHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result ACTA_ResetAccount(Account account_index, bool format_nnid) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x404, 2, 0);
	cmdbuf[1] = static_cast<u32>(account_index);
	cmdbuf[2] = format_nnid;

	if (R_FAILED(ret = svcSendSyncRequest(actHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result ACTA_SetDefaultAccount(u8 account_index) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x409, 1, 0);
	cmdbuf[1] = account_index;

	if (R_FAILED(ret = svcSendSyncRequest(actHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

/**
 * Account index is 1-indexed and can be 1-8.  0 will fail.
 * Use 0xfe as account_index to get current account info.
 */
Result ACTA_GetAccountInfo(void *out, u32 out_size, u32 block_id, u8 account_index) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x6, 3, 2);
	cmdbuf[1] = account_index;
	cmdbuf[2] = out_size;
	cmdbuf[3] = block_id;
	cmdbuf[4] = (out_size << 4) | 12;
	cmdbuf[5] = (u32)out;

	if (R_FAILED(ret = svcSendSyncRequest(actHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result ACTA_GetFriendLocalAccountId(Account *out, u32 index) {
	return ACTA_GetAccountInfo(reinterpret_cast<u8*>(out), sizeof(u32), 0x2b, index);
}

Result ACTA_GetPersistentId(u32 *out, u32 index) {
	return ACTA_GetAccountInfo(out, sizeof(u32), 5, index);
}

Result ACTA_GetAccountManagerInfo(void *out, u32 out_size, u32 block_id) {
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x5, 2, 2);
	cmdbuf[1] = out_size;
	cmdbuf[2] = block_id;
	cmdbuf[3] = (out_size << 4) | 12;
	cmdbuf[4] = (u32)out;

	if (R_FAILED(ret = svcSendSyncRequest(actHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result ACTA_GetAccountCount(u32 *out) {
	return ACTA_GetAccountManagerInfo(out, sizeof(u32), 1);
}

#define ASSERT(action)				\
	if (R_FAILED(ret = action)) {	\
		return ret;					\
	}

Result ACTA_GetAccountIndexOfFriendAccountId(u32 *index, Account friend_account_id) {
	Result ret = 0;
	u32 account_count = 0;

	if (R_FAILED(ret = ACTA_GetAccountCount(&account_count))) {
		return ret;
	}

	for (u32 i = 0; i < account_count; i++) {
		u32 account_index = i + 1;
		Account found_friend_account_id = Account::Undefined;

		if (R_FAILED(ret = ACTA_GetFriendLocalAccountId(&found_friend_account_id, account_index))) {
			return ret;
		}

		if (friend_account_id == found_friend_account_id) {
			*index = static_cast<u32>(account_index);
			return 0;
		}
	}

	// Found nothing
	*index = 0;
	return 0;
}