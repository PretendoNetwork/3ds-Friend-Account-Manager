#pragma once

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <stdint.h>
#include <citro2d.h>
#include <3ds.h>
#include <algorithm>
#include <string>
#include <format>
#include "../build/version.hpp"

enum class Account : u8 {
	Undefined = 0,
	Nintendo = 1, // prod
	Pretendo = 2 // test
};

enum class NascEnvironment : u8 {
	NASC_ENV_Prod = 0, // nintendo
	NASC_ENV_Test = 1, // pretendo
	NASC_ENV_Dev = 2,
};

#define handleResult(action, name)           \
	rc = action;                               \
	if (R_FAILED(rc)) {                        \
		printf("%s error: %08lx\n\n", name, rc); \
	}

extern NascEnvironment AccountToNascEnvironment(Account accountId);

// credit to the universal-team for most/all of the code past here
extern C2D_Font font;
extern C2D_TextBuf textBuf;
extern CFG_Region loadedSystemFont;

void GetStringSize(float size, float *width, float *height, const char *text);

/*
	Get String or Text Height.

	float size: The Textsize.
	std::string Text: The Text.
	C2D_font fnt: (Optional) The wanted C2D_font. Is nullptr by default.
*/

float GetStringHeight(float size, const char *text);
void DrawString(float size, u32 color, std::string text, int flags);

// this is kinda from citro2d
CFG_Region GetSystemRegion();

// code thats mostly by me again
void DrawVersionString();

bool GetLumaOptionByIndex(int index, s64 options);
s64 GetSystemInfoField(s32 category, s32 accessor);
std::tuple<u8, u8, u8> UnpackLumaVersion(s64 packed_version);
std::tuple<u8, u8> UnpackConfigVersion(s64 packed_config_version);