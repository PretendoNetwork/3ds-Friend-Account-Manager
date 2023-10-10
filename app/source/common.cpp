#include "common.hpp"
#include <3ds/services/cfgu.h>

NascEnvironment AccountToNascEnvironment(Account accountId) {
	return static_cast<NascEnvironment>(static_cast<u8>(accountId) - 1);
}

// credit to the universal-team for most/all of the code past here
C2D_Font font;
C2D_TextBuf textBuf;
CFG_Region loadedSystemFont = GetSystemRegion();

void GetStringSize(float size, float *width, float *height, const char *text) {
	C2D_Text c2d_text;

	C2D_TextFontParse(&c2d_text, font, textBuf, text);

	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}

float GetStringHeight(float size, const char *text) {
	float height = 0;

	GetStringSize(size, NULL, &height, text);

	return height;
}

void DrawString(float size, u32 color, std::string text, int flags) {
	C2D_Text c2d_text;
	
	C2D_TextBufClear(textBuf);

	C2D_TextFontParse(&c2d_text, font, textBuf, text.c_str());
	C2D_TextOptimize(&c2d_text);
	
	float x = 8;
	float y = 8;
	
	float screenWidthBottom = 320;
	float screenHeight = 240;
	
	switch (loadedSystemFont) {
		case CFG_REGION_CHN:
		case CFG_REGION_KOR:
		case CFG_REGION_TWN:
			y += 3.0f * size;
			break;
		default:
			break;
	}

	float heightScale = std::min(size, size*((screenHeight - y * 2)/GetStringHeight(size, text.c_str())));

	C2D_DrawText(&c2d_text, C2D_WithColor | C2D_WordWrap | flags, x, y, 0.5f, size, heightScale, color, screenWidthBottom - x * 2);
}

// this is kinda from citro2d
CFG_Region GetSystemRegion() {
	u8 systemRegion = 0;
	Result rc = CFGU_SecureInfoGetRegion(&systemRegion);
	if (R_FAILED(rc))
	{
		return CFG_REGION_USA;
	}
	
	return static_cast<CFG_Region>(systemRegion);
}

// code thats mostly by me again 
void DrawVersionString() {
	C2D_Text c2d_text;
	std::string text = std::format("{} {}.{}.{}", APP_TITLE, VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
	float size = 0.5f;
	int offset = 6;
	int bottomOffset = (240 + 2)- offset; // 3ds vertical resolution - offset, but add 2 to the vertical resolution to account for the vertical padding?
	int horizontalOffset = 400 - offset;
	
	C2D_TextBufClear(textBuf);
	
	C2D_TextFontParse(&c2d_text, font, textBuf, text.c_str());
	C2D_TextOptimize(&c2d_text);

	C2D_DrawText(&c2d_text, C2D_WithColor | C2D_AlignRight, horizontalOffset, bottomOffset - GetStringHeight(size, text.c_str()), 0.5f, size, size, C2D_Color32(255, 255, 255, 0xFF));
	
}

bool GetLumaOptionByIndex(LumaConfigBitIndex index, s64 options) {
    return ((options >> (static_cast<s32>(index))) & 0x1) == 1; // if the last bit of options shifted by the right index amount of bits is 1, return true
}

s64 GetSystemInfoField(s32 category, CFWSystemInfoField accessor) {
	s64 out = 0;
	svcGetSystemInfo(&out, category, static_cast<s32>(accessor));
	
	return out;
}

std::tuple<u8, u8, u8> UnpackLumaVersion(s64 packed_version) {
	return { (packed_version >> 24) & 0xFF, (packed_version >> 16) & 0xFF, (packed_version >> 8) & 0xFF };
}

std::tuple<u8, u8> UnpackConfigVersion(s64 packed_config_version) {
	return { (packed_config_version >> 16) & 0xFF, packed_config_version & 0xFF };
}

