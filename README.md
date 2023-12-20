# GCCG Restoration Project

![image](https://github.com/lesserfish/GCCG-Restoration/assets/73536889/2cb148ae-7a5a-4ad6-b809-54899ba2f4a1)

 Generic Collectible Card Game is a multiplayer multiplatform implementation of a card game engine. The card game engine is designed to be of general purpose core for several modules each defining the game specific behaviour. Players download a client for their platform and all modules of interest. After launching the client for one of the games, it connects to the central server of the game chosen. When connected, all aspects of the collecting, trading and playing are availble to the player.
Here is a brief summary of features:

- Deck editor including import and export from/to a text file.
- Fast game play: bureaucracy not implemented (on purpose).
- 1-8 players.
- You can spectate games without being a player yourself.
- Highly customizable: macros and scripting language included.
- Servers have knowledge of several game formats (for example Type 1, Type 1.5, etc. in Mtg).
- Custom game formats can be defined.
- Sell cards by determining a price and the number of cards for sale. Clients can see always the best offer.
- Easy trading: create your want list and the server helps you to find a trade.
- Sealed deck games.
- Export and import to/from existing deck formats. 

GCCG was created by Tommi Ronkainen in 2001. Unfortunately, it has not received any updated in several years, all of the forums are dead, and the installers no longer work.

GCCG Restoration project aims to fix existing bugs, and create easy and accessible ways to deploy servers, and install clients, allowing people to go back to this gem of time.

We have split this README into three sections:

- Client: How to install the client in Linux, Windows or Mac OS
- Server: How to easily deploy a server
- Build: How to build this project from scratch.

## Client

### Windows:

Download the latest Core file from the [releases](https://github.com/lesserfish/GCCG-Restoration/releases/tag/core) section of this Github page. Extract the GCCG directory somewhere nice.
Open the Install.bat file and follow the prompt and select the game you want to install.

### Linux:

Install the required libraries by running 

        sudo apt install libsdl1.2debian \
        libsdl-image1.2 \
        libsdl-ttf2.0-0 \
        libsdl-mixer1.2 \
        libsdl-net1.2

Download the latest Core file from the [releases](https://github.com/lesserfish/GCCG-Restoration/releases/tag/core) section of this Github page. Extract thte contents of the .tgz file to a directory of your choice.
Open a terminal, and cd' into the directory where you extracted everything.

Run the following command:

    ./gccg_package install client fonts linux-x86_64

Install your game of choice by running

    ./gccg_package game

Current supported games available are "mtg", "metw" and "lotr". You can list additional modules with ./gccg_package status

Install the game cards by running:

    ./gccg_package install game-cards*

### MacOS:

Download the latest Core file from the [releases](https://github.com/lesserfish/GCCG-Restoration/releases/tag/core) section of this Github page. Extract thte contents of the .tgz file to a directory of your choice.
Open a terminal, and cd' into the directory where you extracted everything.

Run the following command:

    ./gccg_package install client fonts darwin-x86_64

Install your game of choice by running

    ./gccg_package game

Current supported games available are "mtg", "metw" and "lotr". You can list additional modules with ./gccg_package status

Install the game cards by running:

    ./gccg_package install game-cards*

## Server

A dockerimage containing all of server modules can be found in lesserfish/gccg-base:latest.

If you are interested in deplying servers, there is docker-compose example file in Server/Docker-Compose/docker-compose.yaml in this directory.

Simply download this file and run 'docker-compose up'.

## Build

In Linux, download the required libraries:

    sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev libsdl-mixer1.2-dev libsdl-net1.2-dev

These libraries are also available in MacOS via homebrew.

Clone this repository, and run

    make

This will create a Build directory with all of the modules.

