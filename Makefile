.PHONY: all patches app clean

all: patches app

FRIEND_TITLE_ID	:= 0004013000003202
ACT_TITLE_ID	:= 0004013000003802

OUT_FOLDER   := out
CIA_OUT_FOLDER := cia_out
3DSX_OUT_FOLDER := 3dsx_out
3DS_OUT	:= 3ds
CIA_OUT := cias
LUMA_OUT	:= luma/titles

patches:
	@mkdir -p $(OUT_FOLDER)/$(CIA_OUT_FOLDER)/$(LUMA_OUT)/$(FRIEND_TITLE_ID) $(OUT_FOLDER)/$(CIA_OUT_FOLDER)/$(LUMA_OUT)/$(ACT_TITLE_ID)
	@mkdir -p $(OUT_FOLDER)/$(3DSX_OUT_FOLDER)/$(LUMA_OUT)/$(FRIEND_TITLE_ID) $(OUT_FOLDER)/$(3DSX_OUT_FOLDER)/$(LUMA_OUT)/$(ACT_TITLE_ID)
	@$(MAKE) -C patches
	@cp -r patches/friends/out/* $(OUT_FOLDER)/$(3DSX_OUT_FOLDER)/$(LUMA_OUT)/$(FRIEND_TITLE_ID)
	@cp -r patches/act/out/* $(OUT_FOLDER)/$(3DSX_OUT_FOLDER)/$(LUMA_OUT)/$(ACT_TITLE_ID)
	@cp -r patches/friends/out/* $(OUT_FOLDER)/$(CIA_OUT_FOLDER)/$(LUMA_OUT)/$(FRIEND_TITLE_ID)
	@cp -r patches/act/out/* $(OUT_FOLDER)/$(CIA_OUT_FOLDER)/$(LUMA_OUT)/$(ACT_TITLE_ID)

app:
	@$(MAKE) -C app
	@mkdir -p $(OUT_FOLDER)/$(3DSX_OUT_FOLDER)/$(3DS_OUT)
	@cp app/*.3dsx $(OUT_FOLDER)/$(3DSX_OUT_FOLDER)/$(3DS_OUT)
	@mkdir -p $(OUT_FOLDER)/$(CIA_OUT_FOLDER)/$(CIA_OUT)
	@cp app/*.cia $(OUT_FOLDER)/$(CIA_OUT_FOLDER)/$(CIA_OUT)

clean:
	@$(MAKE) -C patches clean
	@$(MAKE) -C app clean
	@rm -rf out
