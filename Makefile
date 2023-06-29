.PHONY: all clean

OUT_FOLDER      := out

CIA_OUT_FOLDER  := $(OUT_FOLDER)/cia_out
3DSX_OUT_FOLDER := $(OUT_FOLDER)/3dsx_out

3DS_OUT	        := 3ds
CIA_OUT         := cias

LUMA_OUT	    := luma/titles

FRIEND_TITLE_ID	:= 0004013000003202
ACT_TITLE_ID	:= 0004013000003802
SSL_TITLE_ID    := 0004013000002F02
JUXT_ID_JPN     := 000400300000BC02
JUXT_ID_USA     := 000400300000BD02
JUXT_ID_EUR     := 000400300000BE02

FRIENDS_OUT     := $(LUMA_OUT)/$(FRIEND_TITLE_ID)
ACT_OUT         := $(LUMA_OUT)/$(ACT_TITLE_ID)
SSL_OUT         := $(LUMA_OUT)/$(SSL_TITLE_ID)
JUXT_OUT_JPN    := $(LUMA_OUT)/$(JUXT_ID_JPN)
JUXT_OUT_USA    := $(LUMA_OUT)/$(JUXT_ID_USA)
JUXT_OUT_EUR    := $(LUMA_OUT)/$(JUXT_ID_EUR)

all:
	@rm -rf $(OUT_FOLDER)

	@mkdir -p $(3DSX_OUT_FOLDER)/$(FRIENDS_OUT) $(3DSX_OUT_FOLDER)/$(ACT_OUT)
	@mkdir -p $(3DSX_OUT_FOLDER)/$(SSL_OUT) $(3DSX_OUT_FOLDER)/$(JUXT_OUT_JPN)
	@mkdir -p $(3DSX_OUT_FOLDER)/$(JUXT_OUT_USA) $(3DSX_OUT_FOLDER)/$(JUXT_OUT_EUR)
	@mkdir -p $(3DSX_OUT_FOLDER)/$(3DS_OUT) $(CIA_OUT_FOLDER)
	
	@$(MAKE) -C patches
	
	@cp -r patches/friends/out/* $(3DSX_OUT_FOLDER)/$(FRIENDS_OUT)
	@cp -r patches/act/out/* $(3DSX_OUT_FOLDER)/$(ACT_OUT)
	@cp -r patches/ssl/out/* $(3DSX_OUT_FOLDER)/$(SSL_OUT)
	@cp -r patches/juxt/out/* $(3DSX_OUT_FOLDER)/$(JUXT_OUT_JPN)
	@cp -r patches/juxt/out/* $(3DSX_OUT_FOLDER)/$(JUXT_OUT_USA)
	@cp -r patches/juxt/out/* $(3DSX_OUT_FOLDER)/$(JUXT_OUT_EUR)
	@cp -r patches/juxt/juxt-prod.pem $(3DSX_OUT_FOLDER)/$(3DS_OUT)
	
	@cp -r $(3DSX_OUT_FOLDER)/* $(CIA_OUT_FOLDER)


	@$(MAKE) -C app
	
	
	@cp app/*.3dsx $(3DSX_OUT_FOLDER)/$(3DS_OUT)
	
	@mkdir -p $(CIA_OUT_FOLDER)/$(CIA_OUT)
	@cp app/*.cia $(CIA_OUT_FOLDER)/$(CIA_OUT)

clean:
	@$(MAKE) -C patches clean
	@$(MAKE) -C app clean
	@rm -rf $(OUT_FOLDER)
