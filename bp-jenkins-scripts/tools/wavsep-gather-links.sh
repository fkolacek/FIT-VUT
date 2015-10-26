#!/bin/bash

# Check config file
[ ! -f ../config ] && { echo "[!] There is no config file"; exit 1; }
source "../config"
in="$1"
remoteHost="${WAVSEP_HTTP_PATH=}/"
remoteList="active/index-${in}.jsp"

curl -o list.html "${remoteHost}${remoteList}" >/dev/null 2>&1

path=$(echo  "${remoteHost}${remoteList}" | sed -r 's/^(.+)\/[^\/]+\.jsp/\1/g')

subIndex=$(grep "<a href" list.html | sed -r 's/^.*<a href=\"(.+)\">.*$/\1/g')

echo "$subIndex" | while read line; do	
	subPath=$(echo "$line" | sed -r 's/^(.+)\/[^\/]+\.jsp/\1/g')
	
	echo "${path}/${subPath}/index.jsp"
	
	curl -o index.html "${path}/${subPath}/index.jsp" >/dev/null 2>&1
	
	grep "<a href" index.html | sed -r 's/^.*<a href=\"(.+)\">.*$/\1/g' | while read subline; do
		echo "${path}/${subPath}/${subline}"
	
	done
done

rm -f index.html list.html
