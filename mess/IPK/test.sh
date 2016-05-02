#!/bin/bash

rm -rf "./tests"

if [ ! -f "./urls.txt" ]; then
	echo "[!] No test data in urls.txt"
	exit 1
fi

if [ ! -d "./tests/webclient" ]; then
	mkdir -p "./tests/out"
fi

if [ ! -d "./tests/wget" ]; then
	mkdir -p "./tests/wget"
fi

clear
make clean 2>&1 >/dev/null
output=`make 2>&1`

if [ "$?" == "0" ]; then

	echo "[*] Successfully compiled"

	cp webclient ./tests
	cd ./tests

	while read line; do
		echo -n "[*] Test: webclient $line "
		cd out
		output=`../webclient "$line" 2>&1`
		cd ../wget
		output=`wget  "$line" 2>&1`
		cd ..

		outName=`find ./out -type f`
		wgetName=`find ./wget -type f`


        sumOut=`md5sum "$outName"`
        sumWget=`md5sum "$wgetName"`

        sumO=`echo $sumOut | awk '{printf $1}'`
        sumW=`echo $sumWget | awk '{printf $1}'`

        if [ "$sumO" == "$sumW" ]; then
            echo -e "[\033[32m""OK""\033[0m]"
        else
            echo -e "[\033[31m""FAILED""\033[0m]"
        fi

        echo " - $sumOut"
        echo " - $sumWget"

        diff "$outName" "$wgetName"

		rm "$outName"
		rm "$wgetName"

	done < <(cat ../urls.txt)
else
	echo "[!] Cannot compile program"
	echo "--------------------------"
	echo "$output"
fi
