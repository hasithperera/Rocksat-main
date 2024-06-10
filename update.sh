#scp root@rp:/opt/data/Rocksat-main/src/* ./src/
#rsync root@rp:/opt/data/Rocksat-main/src/* ./src/
#scp root@rp:/opt/data/greg/spi-api.c ./src/
#scp root@rp:/opt/data/greg/spi.c ./src/
#git add ./src/


# file updates made on the RP
rsync -av root@rp3:/opt/data/Rocksat-main/ ./
rsync -av root@rp3:/opt/data/logs/ ./logs/
