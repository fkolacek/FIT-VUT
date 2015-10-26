#!/bin/bash

set -u

echo "###########################################################"
echo " Wavsep-stop.sh"
echo "###########################################################"

# Check config file
[ ! -f config ] && { echo "[!] There is no config file"; exit 1; }
source "config"

echo "[*] Checking if tomcat is running"
systemctl status tomcat >/dev/null 2>&1
if [ $? -eq 0 ]; then
  echo "[*] Service tomcat is running"
  systemctl stop tomcat 2>&1
  echo "[*] Stopping service tomcat.."
fi

systemctl status tomcat >/dev/null 2>&1

if [ $? -eq 0 ]; then
  echo "[!] Cannot stop tomcat, killing.."
  pkill tomcat 2>&1
fi

echo "[*] Service tomcat is stopped"

echo "[*] Checking if mariadb is running"
systemctl status mariadb >/dev/null 2>&1
if [ $? -eq 0 ]; then
  echo "[*] Service mariadb is running"
  systemctl stop mariadb 2>&1
  echo "[*] Stopping service mariadb.."
fi

systemctl status mariadb >/dev/null 2>&1

if [ $? -eq 0 ]; then
  echo "[!] Cannot stop mariadb, killing.."
  pkill mariadb 2>&1
fi

echo "[*] Service mariadb is stopped"

# Starting scan
export ROOT_PATH="$REVOK_REPO_PATH"

cd "$REVOK_REPO_PATH" 2>&1 || { echo "[!] Cannot enter Revok directory"; exit 1; }

echo "[*] Stopping Revokd.."
"$ROOT_PATH/revokd" stop >/dev/null 2>&1


cd "$LOGS_PATH" 2>&1 || { echo "[!] Cannot enter logs directory"; exit 1; }

vectors=(sql xss lfi rfi)

for vector in ${vectors[*]}; do

  [ ! -f "$WAVSEP_REPO_PATH/wavsep.${vector}.links" ] && { echo "[!] Cannot load wavsep.${vector}.links file"; exit 1; }
  
  attacksCount=$(cat "$WAVSEP_REPO_PATH/wavsep.${vector}.links" 2>/dev/null | wc -l)
  counter=0
  
  while read line; do
    while read att; do
		att=$(echo "$att" | sed -r 's/^.+(wavsep\/.+)$/\1/g')
		if grep "$att" "$WAVSEP_REPO_PATH/wavsep.${vector}.links" >/dev/null 2>&1; then
			let counter=counter+1
		fi
	done < <(sed -e '/"security_hardening_section/,$d' "$line" | tr " " "\n" | grep "$WAVSEP_HTTP_PATH" | grep -v "index-${vector}.jsp" | sed -r 's/^.*(http:.+jsp).*$/\1/g' | sort | uniq)
  done < <(find . -type f -name "*${vector}.html")
  
  passed=$(printf '%i %i' $counter $attacksCount | awk '{ pc=100*$1/$2; i=int(pc); print (pc-i<0.5)?i:i+1 }')
  echo "[*] Result for vector: ${vector} $counter/$attacksCount (${passed}%)"
 
done

#ls | grep .html | while read line; do
#  echo "[*] RESULTS $line"
#
#  vector=$(echo "$line" | sed -r 's/^.+\-([a-z]{3})\.html$/\1/g')
#
#  [ ! -f "$WAVSEP_REPO_PATH/wavsep.${vector}.links" ] && { echo "[!] Cannot load wavsep.${vector}.links file"; exit 1; }
#
#  attacksCount=$( cat "$WAVSEP_REPO_PATH/wavsep.${vector}.links" | wc -l)
#  counter=0
#
#  while read att
#  do
#    att=$(echo "$att" | sed -r 's/^.+(wavsep\/.+)$/\1/g')
#    if grep "$att" "$WAVSEP_REPO_PATH/wavsep.${vector}.links" >/dev/null 2>&1; then
#      let counter=counter+1
#    fi
#  done < <(sed -e '/"security_hardening_section/,$d' "$line" | tr " " "\n" | grep "$WAVSEP_HTTP_PATH" | grep -v "index-${vector}.jsp" | sed -r 's/^.*(http:.+jsp).*$/\1/g' | sort | uniq)
#
#  passed=$(printf '%i %i' $counter $attacksCount | awk '{ pc=100*$1/$2; i=int(pc); print (pc-i<0.5)?i:i+1 }')
#  echo "[*] Result for vector: ${vector} $counter/$attacksCount (${passed}%)"
#
#done
 
echo "[*] Done"
exit 0
