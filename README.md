# MTGServer

This is a docker-compose file to setup a Magic the Gathering GCCG Server.
Simply clone this directory, cd to Docker-Compose, and run

        docker-compose up

To connect to this server, go to where you installed GCCH and run

        ./Mtg --server IPADDR --port 29100 --user USERNAME

Dockerfile is just used to create the image lesserfish/gccg-base:latest, which is used by the docker-compose, so pay no mind to it.

