OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
    # Linux-specific steps
    TARGET := linux_target
else ifeq ($(OS), Darwin)
    # macOS-specific steps
    TARGET := macos_target
else
    $(error Unsupported operating system: $(OS))
endif

# Makefile for GCCG project

# Define the directory structure
BUILD_DIR := Build
CLIENT_DIR := ./Client/latest
SERVER_DIR := ./Server/latest
CORE_NIX_DIR := ./Core/Unix
CORE_WIN_DIR := ./Core/Windows
MTG_DIR := ./Games/Mtg/latest
METW_DIR := ./Games/Metw/latest
FONT_DIR := ./Fonts/latest
LOTR_DIR := ./Games/Lotr/latest
MTG_SERVER_DIR := ./Server/Games/Mtg/latest
METW_SERVER_DIR := ./Server/Games/Metw/latest
LOTR_SERVER_DIR := ./Server/Games/Lotr/latest
SOURCE_DIR := ./Source/latest

# Define target files
CLIENT_TGZ := $(BUILD_DIR)/gccg-client-latest.tgz
SERVER_TGZ := $(BUILD_DIR)/gccg-server-latest.tgz
CORE_NIX_TGZ := $(BUILD_DIR)/gccg-core-unix-latest.tgz
CORE_WIN_TGZ := $(BUILD_DIR)/gccg-core-unix-latest.tgz
MTG_TGZ := $(BUILD_DIR)/gccg-mtg-latest.tgz
FONT_TGZ := $(BUILD_DIR)/gccg-fonts-latest.tgz
METW_TGZ := $(BUILD_DIR)/gccg-metw-latest.tgz
LOTR_TGZ := $(BUILD_DIR)/gccg-lotr-latest.tgz
MTG_SERVER_TGZ := $(BUILD_DIR)/gccg-mtg-server-latest.tgz
METW_SERVER_TGZ := $(BUILD_DIR)/gccg-metw-server-latest.tgz
LOTR_SERVER_TGZ := $(BUILD_DIR)/gccg-lotr-server-latest.tgz
MACOS_TAR_GZ := $(BUILD_DIR)/gccg-macos-latest.tgz

ifeq ($(ARCH), x86_64)
	LINUX_TAR_GZ := $(BUILD_DIR)/gccg-linux-x86_64-latest.tgz
else ifeq ($(ARCH), i386)
	LINUX_TAR_GZ := $(BUILD_DIR)/gccg-linux-i386-latest.tgz
else
    $(error Unsupported architecture: $(ARCH))
endif

# Define phony targets
.PHONY: all clean

all: $(TARGET)

linux_target: $(CLIENT_TGZ) $(SERVER_TGZ) $(CORE_NIX_TGZ) $(CORE_WIN_TGZ) $(MTG_TGZ) $(METW_TGZ) $(FONT_TGZ) $(LOTR_TGZ) $(MTG_SERVER_TGZ) $(METW_SERVER_TGZ) $(LOTR_SERVER_TGZ) $(LINUX_TAR_GZ)
macos_target: $(CLIENT_TGZ) $(SERVER_TGZ) $(CORE_NIX_TGZ) $(CORE_WIN_TGZ) $(MTG_TGZ) $(METW_TGZ) $(FONT_TGZ) $(LOTR_TGZ) $(MTG_SERVER_TGZ) $(METW_SERVER_TGZ) $(LOTR_SERVER_TGZ) $(MACOS_TAR_GZ)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CLIENT_TGZ): $(BUILD_DIR)
	tar -C $(CLIENT_DIR) -cvzf $@ .

$(SERVER_TGZ): $(BUILD_DIR)
	tar -C $(SERVER_DIR) -cvzf $@ .

$(CORE_NIX_TGZ): $(BUILD_DIR)
	tar -C $(CORE_NIX_DIR) -cvzf $@ .

$(CORE_WIN_TGZ): $(BUILD_DIR)
	tar -C $(CORE_WIN_DIR) -cvzf $@ .

$(MTG_TGZ): $(BUILD_DIR)
	tar -C $(MTG_DIR) -cvzf $@ .

$(METW_TGZ): $(BUILD_DIR)
	tar -C $(METW_DIR) -cvzf $@ .

$(FONT_TGZ): $(BUILD_DIR)
	tar -C $(FONT_DIR) -cvzf $@ .

$(LOTR_TGZ): $(BUILD_DIR)
	tar -C $(LOTR_DIR) -cvzf $@ .

$(MTG_SERVER_TGZ): $(BUILD_DIR)
	tar -C $(MTG_SERVER_DIR) -cvzf $@ .

$(METW_SERVER_TGZ): $(BUILD_DIR)
	tar -C $(METW_SERVER_DIR) -cvzf $@ .

$(LOTR_SERVER_TGZ): $(BUILD_DIR)
	tar -C $(LOTR_SERVER_DIR) -cvzf $@ .

$(LINUX_TAR_GZ): $(BUILD_DIR)
	make -C $(SOURCE_DIR) && tar -C $(SOURCE_DIR) -cvzf $@ ccg_client ccg_server
$(MACOS_TAR_GZ): $(BUILD_DIR)
	OS=mac make -C $(SOURCE_DIR)
	dylibbundler -od -b -x $(SOURCE_DIR)/ccg_client -p "@executable_path/.libs" -d $(SOURCE_DIR)/.libs
	cp -n /usr/local/lib/libSDL*.dylib $(SOURCE_DIR)/.libs
	tar -C $(SOURCE_DIR) -cvzf $@ ccg_client ccg_server .libs

clean:
	rm -rf $(BUILD_DIR)
	rm $(SOURCE_DIR)/ccg_client
	rm $(SOURCE_DIR)/ccg_server
	cd $(SOURCE_DIR) && make clean


