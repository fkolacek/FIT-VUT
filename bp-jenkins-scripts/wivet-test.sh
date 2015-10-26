#!/bin/bash

#set -u
echo "###########################################################"
echo " Wivet-test.sh"
echo "###########################################################"

# Check config file
[ ! -f config ] && { echo "[!] There is no config file"; exit 1; }
source "config"

# Check httpd status
echo "[*] Checking httpd status.."
systemctl status httpd >/dev/null 2>&1 || { echo "[*] Starting httpd.."; systemctl start httpd; }
systemctl status httpd >/dev/null 2>&1 || { echo "[!] Cannot start httpd service"; exit 1; }
echo "[*] Service httpd is running"

# Enter Revok directory
echo "[*] Entering Revok directory.."
cd "$REVOK_REPO_PATH" 2>&1 || { echo "[!] Cannot enter Revok directory"; exit 1; }

# Starting scan
export ROOT_PATH="$REVOK_REPO_PATH"

[ ! -f "$ROOT_PATH/bin/setenv.sh" ] && { echo "[!] There is no file setenv.sh in Revok/bin directory"; exit 1; }
[ ! -f "$ROOT_PATH/python/bin/mitmdump" ] && { echo "[!] There is no file mitmdump in Revok/python/bin directory"; exit 1; }
[ ! -f "$ROOT_PATH/phantomjs/bin/phantomjs" ] && { echo "[!] There is no file phantomjs in Revok/phantomjs/bin directory"; exit 1; }
[ ! -f "$ROOT_PATH/caroline/modules/creep/creep.py" ] && { echo "[!] There is no file creep.py in Revok/caroline/modules/creep directory"; exit 1; }
[ ! -f "$ROOT_PATH/caroline/modules/creep/webcrawler.js" ] && { echo "[!] There is no file webcrawler.js in Revok/caroline/modules/creep directory"; exit 1; }

# Enter Revok directory
echo "[*] Entering Revok directory.."
cd "$ROOT_PATH" || { echo "[!] Cannot enter Revok directory"; exit 1; }

# Set env variables
echo "[*] Setting environment variables with ROOT_PATH=$ROOT_PATH .."
source "$ROOT_PATH/bin/setenv.sh"

echo "[*] Using $TARGET_IP as target IP"

# Starting mitmdump
echo "[*] Starting mitmdump.."
#python "$ROOT_PATH"/python/bin/mitmdump -s "$ROOT_PATH/caroline/modules/creep/creep.py" 192.168.1.15 -q >"$LOGS_PATH/mitmdump.log" 2>&1 &
python "$ROOT_PATH"/python/bin/mitmdump -s "$ROOT_PATH/caroline/modules/creep/creep.py" "$TARGET_IP" >"$LOGS_PATH/mitmdump.log" 2>&1 &
mpid=$!
echo "[*] mitmdump started, PID: $mpid, sleeping 5s.."
sleep 5

# Double check mitmdump
kill -0 $mpid >/dev/null 2>&1 || { echo "[*] mitmdump is not running"; exit 1; }

echo "[*] mitmdump running, so far, so good.."
ps aux | grep $mpid | grep -v grep

echo "[*] Starting phantomjs.."

echo '{"target":"http://'$TARGET_IP'/wivet/","login":"","login_button":true,"logtype":"none","positions":{"username":{"x":-206,"y":-206},"password":{"x":-206,"y":-206},"button":{"x":-206,"y":-206}},"username":"","password":"","email":"root@localhost","modules":["all"],"whitelist":["'$TARGET_IP'"],"width":1280,"height":800,"attempts":25,"delay":2000,"duration":240000,"depth":8}' | "$ROOT_PATH/phantomjs/bin/phantomjs" --proxy=localhost:8080 --ignore-ssl-errors=true --ssl-protocol=any "$ROOT_PATH/caroline/modules/creep/webcrawler.js" >"$LOGS_PATH/phantomjs.log" 2>&1 &

ppid=$!
echo "[*] phantomjs started, PID: $ppid"

echo -n "[*] Scanning.."
while kill -0 $ppid >/dev/null 2>&1; do
  echo -n "."
  sleep 5
done

echo
echo "[*] phantomjs died"

# Killing mitmdump
echo "[*] Killing mitmdump.."
kill -9 $mpid >/dev/null 2>&1

echo "[*] Done"
exit 0
