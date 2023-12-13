#!/bin/bash

source /tmp/.env
apt update 
apt install -y rclone ca-certificates 
rclone config create $NAME s3 provider Other env_auth false access_key_id $ACCESS_KEY secret_access_key $SECRET_KEY endpoint $ENDPOINT
rclone sync /mnt/data/ $NAME:gccg-backup

