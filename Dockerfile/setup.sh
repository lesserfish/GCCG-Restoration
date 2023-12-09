dpkg --add-architecture i386
apt update
apt install -y wget libsdl1.2debian:i386 libsdl-image1.2:i386 libsdl-ttf2.0-0:i386 libsdl-mixer1.2:i386 libsdl-net1.2:i386
wget http://gccg.sourceforge.net/modules/gccg-core-1.0.10.tgz
tar zxvf gccg-core-1.0.10.tgz
./gccg_package install client fonts linux-i386 mtg server mtg-server

# Instantiate Meta server
./Server factory-mtg
./Server meta-mtg

# Instantiate Table
./Server --server meta_server --bet 50 --players 2 --port PORT mtg
