#!/bin/bash
cmd=$1
echo $cmd
used1=`free | grep Mem | awk '{print $3}'`
time1=`date +%s%6N`;
bash -c "exec -a myProc $cmd > /dev/null 2>&1 &"
PID=`ps -ef | grep -P 'myProc\s\w+\.\w+\s\w+.\w' | awk '{print $2}'`
echo "PID: $PID"
mem=`awk 'BEGIN { used=0 }; /Rss/ { used += $2 } END { print used }' /proc/$PID/smaps`
time2=`date +%s%6N`
used2=`free | grep Mem | awk '{print $3'}`
echo "time: $[time2 - time1]us"
echo "Mem: $mem kB"
pkill -f myProc