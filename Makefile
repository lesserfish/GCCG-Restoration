
# Makefile for GCCG project

# Define the directory structure
BUILD_DIR := Build
CLIENT_DIR := ./Client/latest
SERVER_DIR := ./Server/latest
CORE_DIR := ./Core/Unix
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
CORE_TGZ := $(BUILD_DIR)/gccg-core-unix-latest.tgz
MTG_TGZ := $(BUILD_DIR)/gccg-mtg-latest.tgz
FONT_TGZ := $(BUILD_DIR)/gccg-fonts-latest.tgz
METW_TGZ := $(BUILD_DIR)/gccg-metw-latest.tgz
LOTR_TGZ := $(BUILD_DIR)/gccg-lotr-latest.tgz
MTG_SERVER_TGZ := $(BUILD_DIR)/gccg-mtg-server-latest.tgz
METW_SERVER_TGZ := $(BUILD_DIR)/gccg-metw-server-latest.tgz
LOTR_SERVER_TGZ := $(BUILD_DIR)/gccg-lotr-server-latest.tgz
LINUX_TAR_GZ := $(BUILD_DIR)/gccg-linux-latest.tgz

# Define phony targets
.PHONY: all clean

all: $(CLIENT_TGZ) $(SERVER_TGZ) $(CORE_TGZ) $(MTG_TGZ) $(METW_TGZ) $(FONT_TGZ) $(LOTR_TGZ) $(MTG_SERVER_TGZ) $(METW_SERVER_TGZ) $(LOTR_SERVER_TGZ) $(LINUX_TAR_GZ)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CLIENT_TGZ): $(BUILD_DIR)
	tar -C $(CLIENT_DIR) -cvf $@ .

$(SERVER_TGZ): $(BUILD_DIR)
	tar -C $(SERVER_DIR) -cvf $@ .

$(CORE_TGZ): $(BUILD_DIR)
	tar -C $(CORE_DIR) -cvf $@ .

$(MTG_TGZ): $(BUILD_DIR)
	tar -C $(MTG_DIR) -cvf $@ .

$(METW_TGZ): $(BUILD_DIR)
	tar -C $(METW_DIR) -cvf $@ .

$(FONT_TGZ): $(BUILD_DIR)
	tar -C $(FONT_DIR) -cvf $@ .

$(LOTR_TGZ): $(BUILD_DIR)
	tar -C $(LOTR_DIR) -cvf $@ .

$(MTG_SERVER_TGZ): $(BUILD_DIR)
	tar -C $(MTG_SERVER_DIR) -cvf $@ .

$(METW_SERVER_TGZ): $(BUILD_DIR)
	tar -C $(METW_SERVER_DIR) -cvf $@ .

$(LOTR_SERVER_TGZ): $(BUILD_DIR)
	tar -C $(LOTR_SERVER_DIR) -cvf $@ .

$(LINUX_TAR_GZ): $(BUILD_DIR)
	cd $(SOURCE_DIR) && make all && tar cf ../../$@ ccg_client ccg_server

clean:
	rm -rf $(BUILD_DIR)
	cd $(SOURCE_DIR) && make clean


