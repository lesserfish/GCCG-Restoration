#! /bin/bash
docker start gccg_backup && docker exec gccg_backup "/tmp/backup.sh"
