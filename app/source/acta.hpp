#pragma once

#include "common.hpp"

#define ACT_CURRENT_ACCOUNT 0xfe

Result actAInit();
void actAExit();
Result ACTA_CreateLocalAccount();
Result ACTA_GetAccountInfo(void *out, u32 out_size, u32 block_id, u8 account_index);
Result ACTA_GetAccountCount(u32 *out);
Result ACTA_GetFriendLocalAccountId(Account *out, u32 index);
Result ACTA_GetPersistentId(u32 *out, u32 index);
Result ACTA_GetAccountIndexOfFriendAccountId(u32 *index, Account friend_account_id);
Result ACTA_ResetAccount(u8 account_index, bool format_nnid);
Result ACTA_SetDefaultAccount(u8 account_index);