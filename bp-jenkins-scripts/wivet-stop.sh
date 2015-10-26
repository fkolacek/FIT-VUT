#!/bin/bash

set -u

echo "###########################################################"
echo " Wivet-stop.sh"
echo "###########################################################"

# Check config file
[ ! -f config ] && { echo "[!] There is no config file"; exit 1; }
source "config"

echo "[*] Checking wivet directory"
[ ! -d "$DOCUMENT_ROOT/wivet/offscanpages/statistics" ] && { echo "[!] Cannot find wivet directory"; exit 1; }

echo "[*] Checking if httpd is running"
systemctl status httpd >/dev/null 2>&1
if [ $? -eq 0 ]; then
  echo "[*] Service httpd is running"
  systemctl stop httpd 2>&1
  echo "[*] Stopping service httpd.."
fi

systemctl status httpd >/dev/null 2>&1 || { echo "[!] Cannot stop httpd, killing.."; pkill httpd 2>&1; }

echo "[*] Service httpd is stopped"

cd "$DOCUMENT_ROOT/wivet/offscanpages/statistics/" 2>&1 || { echo "[!] Cannot access statistics directory"; exit 1; }

ls | grep .dat | while read line; do
  echo "[*] RESULTS $line"

  if [ ! -f "$line" ]; then
    echo "[!] File does not exist"
  else
    number=$(cat "$line" | tr '}' '\n' | grep -c testcase)

    if [ "$number" == "0" ]; then
      echo "[!] Invalid format of result file"
    else
      passed=$(printf '%i %i' $number $WIVET_TEST_COUNT | awk '{ pc=100*$1/$2; i=int(pc); print (pc-i<0.5)?i:i+1 }')

      echo "[*] Result file: $line | Passed tests: ${number}/${WIVET_TEST_COUNT} (${passed}%)"
      cat "$line" | tr '{' '\n' | grep testcase | sed -r 's/.+s\:[0-9]+\:\"([0-9]+\_[0-9a-z]+)\"\;.+/\1/g' | tr '\n' ' '
      echo ""
    fi
  fi

  cp "$line" "$LOGS_PATH/${line}.json"
done

echo "[*] Cleaning WIVET statistics.."
rm -f "$DOCUMENT_ROOT"/wivet/offscanpages/statistics/*.dat

echo "[*] Done"
exit 0
