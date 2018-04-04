#!/bin/bash
cmd=$1
user=`whoami`
echo "Command:  $cmd"
used1=`free | grep Mem | awk '{print $3}'`
time1=`date +%s%6N`;
bash -c "exec $cmd > /dev/null 2>&1 &"
#echo `ps -ef | grep -P "$cmd"`
temp=`ps -ef | grep -P "$cmd"` #| awk '{print $2}'`
IFS=' ' read -r -a array <<< "$temp"
PID="${array[1]}"
mem1=`awk 'BEGIN { used=0 }; /Rss/ { used += $2 } END { print used }' /proc/$PID/smaps`
mem2=`pmap -x $PID | grep -i total | awk '{print $4}'`
time2=`date +%s%6N`
used2=`free | grep Mem | awk '{print $3}'`
echo "PID: $PID"
echo "time: $[time2 - time1]us"
echo "Mem: $mem1 kB"
echo "Mem: $mem2 kB"
#pkill -f cmd_proc
pkill -f "$cmd"