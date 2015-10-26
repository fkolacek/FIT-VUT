#!/bin/bash

request="Transfer-mode: 63
Transfer-order: 1
Get: root
Get: xkolac12
Get: pulse";

for i in {1..100}; do
#echo $i
	echo "$request" | nc -q -1 -i $((RANDOM%5+1)) localhost 9999 &
	sleep $((RANDOM%2+0))
	echo "PROCES: $i"
done

#while true; do
#	ps aux | grep "server"
#	sleep 1
#	clear
#done
