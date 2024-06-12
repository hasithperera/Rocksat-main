# log the ams status periodically with date time


while :
do
	date >> /opt/data/logs/stat.log
	monitor -ams | head -n 14 >> /opt/data/logs/stat.log
	sleep 300
	#echo "sample"
done
