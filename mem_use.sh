#!/bin/bash
cmd=$1
<<<<<<< HEAD
user=`whoami`
<<<<<<< HEAD
echo "Command:  $cmd"
=======
echo $cmd
>>>>>>> parent of 301342e... Had to make a few fixes that were caused from making and testing the shell script "mem_use.sh"
=======
echo "Command: $cmd"
>>>>>>> parent of 4fe0ab0... Script now actually runs and terminates the command it is given, made another way of getting the memory of the running command, and tidied up the code structure so the "echo" commands are after everything has happened.
used1=`free | grep Mem | awk '{print $3}'`
time1=`date +%s%6N`;
bash -c "exec -a $cmd > /dev/null 2>&1 &"
temp=`ps -ef | grep -P "$cmd"` #| awk '{print $2}'`
IFS=' ' read -r -a array <<< "$temp"
PID="${array[1]}"
echo "PID: $PID"
mem=`awk 'BEGIN { used=0 }; /Rss/ { used += $2 } END { print used }' /proc/$PID/smaps`
time2=`date +%s%6N`
used2=`free | grep Mem | awk '{print $3}'`
echo "time: $[time2 - time1]us"
echo "Mem: $mem kB"
pkill -f cmd_proc