udtproxy/udtproxy -Q 10 -D 10 -J 5 -a 10009 -A 20009 -b 30009 -B 40009 &
pid_proxy=$!

./rdtserver -s 30009 -d 40009 > misc/out.txt 2> misc/err.txt &
pid_server=$!

./rdtclient -s 10009 -d 20009 < misc/in2.txt &
pid_client=$!

sleep 60

kill -9 $pid_server
kill -9 $pid_client
kill -2 $pid_proxy

if diff -q misc/out.txt misc/in2.txt > /dev/null
then
	echo "Soubor se prenesl spravne"
else
	echo "Soubor se neprenesl spravne"
fi
