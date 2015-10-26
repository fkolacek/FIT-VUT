#!/bin/bash

set -u

echo "###########################################################"
echo " Wavsep-test.sh"
echo "###########################################################"

if [ ! -z "$1" ]; then
  VECTORS="$1"
else
  VECTORS="all"
fi

# Check config file
[ ! -f config ] && { echo "[!] There is no config file"; exit 1; }
source "config"

# Enter Revok directory
echo "[*] Entering Revok repo directory.."
cd "$REVOK_REPO_PATH" 2>&1 || { echo "[!] Cannot enter Revok directory"; exit 1; }

# Starting scan
export ROOT_PATH="$REVOK_REPO_PATH"

[ ! -f "$ROOT_PATH/revokd" ] && { echo "[!] There is no file revokd in Revok directory"; exit 1; }
[ ! -f "$ROOT_PATH/bin/setenv.sh" ] && { echo "[!] There is no file setenv.sh in Revok/bin directory"; exit 1; }

# Enter Revok directory
echo "[*] Entering Revok directory.."
cd "$ROOT_PATH" || { echo "[!] Cannot enter Revok directory"; exit 1; }

# Adjusting launcher
echo "[*] Hotfixing Revokd launcher.."
sed -i -r 's/^.+iptables.+$/true/g' revokd
sed -i -r 's/root/nobody/g' revokd

# Init Revokd
echo "[*] Initializing Revokd.."
"$ROOT_PATH/revokd" init 2>&1 || { echo "[!] Unable to initialize Revokd"; exit 1; }

# Start Revokd
echo "[*] Starting Revokd.."
"$ROOT_PATH/revokd" start 2>&1 || { echo "[!] Unable to start Revokd"; exit 1; }

echo "[*] Gathering PIDs.."

[ ! -f "$ROOT_PATH/var/run/activemq-master.pid" ] && { echo "[!] Cannot find PID for ActiveMQ"; "$ROOT_PATH/revokd" stop 2>&1; exit 1; }
[ ! -f "$ROOT_PATH/var/run/carolined.pid" ] && { echo "[!] Cannot find PID for Carolined"; "$ROOT_PATH/revokd" stop 2>&1; exit 1; }
[ ! -f "$ROOT_PATH/var/run/rackd.pid" ] && { echo "[!] Cannot find PID for Rackd"; "$ROOT_PATH/revokd" stop 2>&1; exit 1; }
[ ! -f "$ROOT_PATH/var/run/rest_served.pid" ] && { echo "[!] Cannot find PID for RestServed"; "$ROOT_PATH/revokd" stop 2>&1; exit 1; }

activemqPID=$(cat "$ROOT_PATH/var/run/activemq-master.pid")
carolinedPID=$(cat "$ROOT_PATH/var/run/carolined.pid")
rackdPID=$(cat "$ROOT_PATH/var/run/rackd.pid")
restPID=$(cat "$ROOT_PATH/var/run/rest_served.pid")

echo "[*] PIDs: ActiveMQ($activemqPID), Carolined($carolinedPID), Rackd($rackdPID), RestServed($restPID)"

# Check status of Revokd
"$ROOT_PATH/revokd" status 2>&1 >/dev/null || { echo "[!] Unable to check status of Revokd, stopping.."; "$ROOT_PATH/revokd" stop 2>&1; exit 1; }
echo "[*] All services are running.."

case "$VECTORS" in
"sql")
	paths=(active/index-sql.jsp)
	;;
"xss")
	paths=(active/index-xss.jsp)
	;;
"lfi")
	paths=(active/index-lfi.jsp)
	;;
"lfi-partially")
	paths=(active/LFI/LFI-Detection-Evaluation-GET-500Error/index.jsp active/LFI/LFI-Detection-Evaluation-GET-404Error/index.jsp active/LFI/LFI-Detection-Evaluation-GET-200Error/index.jsp active/LFI/LFI-Detection-Evaluation-GET-302Redirect/index.jsp active/LFI/LFI-Detection-Evaluation-GET-200Valid/index.jsp active/LFI/LFI-Detection-Evaluation-GET-200Identical/index.jsp active/LFI/LFI-Detection-Evaluation-POST-500Error/index.jsp active/LFI/LFI-Detection-Evaluation-POST-404Error/index.jsp active/LFI/LFI-Detection-Evaluation-POST-200Error/index.jsp active/LFI/LFI-Detection-Evaluation-POST-302Redirect/index.jsp)
	;;
"rfi")
	paths=(active/index-rfi.jsp)
	;;
*)
	paths=(active/index-sql.jsp active/index-xss.jsp active/index-lfi.jsp active/index-rfi.jsp)
	;;
esac

echo "[*] Starting scans.."
counter=0
for path in "${paths[@]}"; do
  echo "[*] Generating config for target: $WAVSEP_HTTP_PATH/$path"
  config='{"target":"'$WAVSEP_HTTP_PATH'/'$path'","login":"","login_button":true,"logtype":"none","positions":{"username":{"x":-206,"y":-206},"password":{"x":-206,"y":-206},"button":{"x":-206,"y":-206}},"username":"","password":"","email":"test@localhost","modules":["all"],"whitelist":["'$TARGET_IP'"]}'
  config64=$(echo "$config" | base64 -w 0)
  
  if [[ $path =~ LFI ]]; then
    vector="lfi"
  else
    vector=$(echo "$path" | sed -r 's/^.+\-([a-z]{3})\.jsp$/\1/g')
  fi
  
  id="12383d6c-c444-41bb-9bef-d225a06a496${counter}"
  scan='{"id":"'$id'","modules":["all"],"target_info":"'$config64'"}'
  
  echo "[*] Requesting scan $id (vector $vector).."
  curl -H 'Content-Type: application/json' -H 'Accept: application/json' -X PUT -d "$scan" -u revok:password http://$TARGET_IP:8443/scans
  
  echo "[*] Scan $id has been requested, sleeping for 10 secs.."
  sleep 10
  
  [ ! -f "$ROOT_PATH/report/00-STATUS" ] && { echo "[!] Cannot open 00-STATUS file, stopping.."; "$ROOT_PATH/revokd" stop 2>&1; exit 1; }
  #Scan 12383d6c-c444-41bb-9bef-d225a06a4960(20150426195428) is running...
  egrep "${id}.+is running" "$ROOT_PATH/report/00-STATUS" >/dev/null 2>&1 || { echo "[!] Job $id didn't start, stopping.."; "$ROOT_PATH/revokd" stop 2>&1; exit 1; }
  
  uid=$(sed -r 's/^Scan [a-f0-9\-]+\(([0-9]+)\).+$/\1/g' "$ROOT_PATH/report/00-STATUS")
  
  echo "[*] Scan $id ($uid) is running.."
  
  while ! egrep "${id}.+is FINISHED" "$ROOT_PATH/report/00-STATUS" >/dev/null 2>&1; do
    echo -n "."
    sleep 5
  done
  
  echo
  echo "[*] Scan $id ($uid) is finished"
  
  echo "[*] Storing logs & results for scan $uid to $LOGS_PATH"
  cp -fv "$ROOT_PATH/report/${uid}_report.html" "$LOGS_PATH/latest-${counter}-${vector}.html"
  cp -fv "$ROOT_PATH/report/${uid}_log.txt" "$LOGS_PATH/latest-${counter}-${vector}.txt"
  
  rm -f "$ROOT_PATH/report/*"
  
  let counter=counter+1
  
  echo "[*] Restarting tomcat.."
  systemctl restart tomcat >/dev/null 2>&1

  echo "[*] Restarting revokd.."
  "$ROOT_PATH/revokd" stop 2>&1
  "$ROOT_PATH/revokd" start 2>&1
done

echo "[*] Nothing more to do"

echo "[*] Stopping Revokd.."
"$ROOT_PATH/revokd" stop 2>&1

kill -0 $activemqPID >/dev/null 2>&1 && kill -9 $activemqPID
kill -0 $carolinedPID >/dev/null 2>&1 && kill -9 $carolinedPID
kill -0 $rackdPID >/dev/null 2>&1 && kill -9 $rackdPID
kill -0 $restPID >/dev/null 2>&1 && kill -9 $restPID

echo "[*] Done"

exit 0
