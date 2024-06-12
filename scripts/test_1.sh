# Basic testing for main.c

#start with a pre launch state
./pre_launch.sh


# timed event is immediate
# cancell at 5 secs
timeout 5 ../src/main
echo "state file:"
cat /opt/data/logs/StartUpData.log

timeout 6 ../src/main
echo "state file:"
cat /opt/data/logs/StartUpData.log


timeout 30 ../src/main
echo "state file:"
cat /opt/data/logs/StartUpData.log
