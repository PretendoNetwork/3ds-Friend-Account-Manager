.PHONY: all clean

OUT_FOLDER      := out

PATCHES_OUT_FOLDER := $(OUT_FOLDER)/patches_out
CIA_OUT_FOLDER  := $(OUT_FOLDER)/cia_out
3DSX_OUT_FOLDER := $(OUT_FOLDER)/3dsx_out
COMBINED_OUT_FOLDER := $(OUT_FOLDER)/combined_out

3DS_OUT         := 3ds
CIA_OUT         := cias

LUMA_OUT            := luma/titles
LUMA_SYSMODULE_OUT  := luma/sysmodules

FRIENDS_TITLE_ID    := 0004013000003202
ACT_TITLE_ID        := 0004013000003802
HTTP_TITLE_ID       := 0004013000002902
SOCKET_TITLE_ID     := 0004013000002E02
SSL_TITLE_ID        := 0004013000002F02
MIIVERSE_ID_JPN     := 000400300000BC02
MIIVERSE_ID_USA     := 000400300000BD02
MIIVERSE_ID_EUR     := 000400300000BE02

ACT_OUT             := $(LUMA_SYSMODULE_OUT)/$(ACT_TITLE_ID).ips
FRIENDS_OUT         := $(LUMA_SYSMODULE_OUT)/$(FRIENDS_TITLE_ID).ips
HTTP_OUT            := $(LUMA_SYSMODULE_OUT)/$(HTTP_TITLE_ID).ips
SOCKET_OUT          := $(LUMA_SYSMODULE_OUT)/$(SOCKET_TITLE_ID).ips
SSL_OUT             := $(LUMA_SYSMODULE_OUT)/$(SSL_TITLE_ID).ips
MIIVERSE_OUT_JPN    := $(LUMA_OUT)/$(MIIVERSE_ID_JPN)
MIIVERSE_OUT_USA    := $(LUMA_OUT)/$(MIIVERSE_ID_USA)
MIIVERSE_OUT_EUR    := $(LUMA_OUT)/$(MIIVERSE_ID_EUR)

all:
	@rm -rf $(OUT_FOLDER)

# make patches + app folders
	@mkdir -p $(PATCHES_OUT_FOLDER)/$(LUMA_OUT) $(PATCHES_OUT_FOLDER)/$(LUMA_SYSMODULE_OUT) $(PATCHES_OUT_FOLDER)/$(3DS_OUT)
	@mkdir -p $(3DSX_OUT_FOLDER) $(CIA_OUT_FOLDER)/$(CIA_OUT) $(COMBINED_OUT_FOLDER)/$(CIA_OUT)
	@mkdir -p $(PATCHES_OUT_FOLDER)/$(MIIVERSE_OUT_USA) $(PATCHES_OUT_FOLDER)/$(MIIVERSE_OUT_EUR) $(PATCHES_OUT_FOLDER)/$(MIIVERSE_OUT_JPN)
	
# build patches
	@$(MAKE) -C patches
	
# copy patches to patches folders
	@cp -r patches/act/out/* $(PATCHES_OUT_FOLDER)/$(ACT_OUT)
	@cp -r patches/friends/out/* $(PATCHES_OUT_FOLDER)/$(FRIENDS_OUT)
	@cp -r patches/http/out/* $(PATCHES_OUT_FOLDER)/$(HTTP_OUT)
	@cp -r patches/socket/out/* $(PATCHES_OUT_FOLDER)/$(SOCKET_OUT)
	@cp -r patches/ssl/out/* $(PATCHES_OUT_FOLDER)/$(SSL_OUT)
	@cp -r patches/miiverse/out/* $(PATCHES_OUT_FOLDER)/$(MIIVERSE_OUT_JPN)
	@cp -r patches/miiverse/out/* $(PATCHES_OUT_FOLDER)/$(MIIVERSE_OUT_USA)
	@cp -r patches/miiverse/out/* $(PATCHES_OUT_FOLDER)/$(MIIVERSE_OUT_EUR)
	@cp -r patches/miiverse/*.pem $(PATCHES_OUT_FOLDER)/$(3DS_OUT)
	
# copy patches output to all 3 output folders
	@cp -r $(PATCHES_OUT_FOLDER)/* $(3DSX_OUT_FOLDER)
	@cp -r $(PATCHES_OUT_FOLDER)/* $(CIA_OUT_FOLDER)
	@cp -r $(PATCHES_OUT_FOLDER)/* $(COMBINED_OUT_FOLDER)

# remove patches folder
	@rm -rf $(PATCHES_OUT_FOLDER)

# build and copy the 3dsx version of the app
	@$(MAKE) -C app 3dsx
	@echo copied 3dsx to 3dsx/combined out folder...
	@cp app/*.3dsx $(3DSX_OUT_FOLDER)/$(3DS_OUT)
	@cp app/*.3dsx $(COMBINED_OUT_FOLDER)/$(3DS_OUT)
	
# build and copy the cia version of the app
	@$(MAKE) -C app cia
	@echo copied cia to cia/combined out folder...
	@cp app/*.cia $(CIA_OUT_FOLDER)/$(CIA_OUT)
	@cp app/*.cia $(COMBINED_OUT_FOLDER)/$(CIA_OUT)

clean:
	@$(MAKE) -C patches clean
	@$(MAKE) -C app clean
	@rm -rf $(OUT_FOLDER)
