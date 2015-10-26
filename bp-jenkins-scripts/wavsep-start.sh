#!/bin/bash

set -u

echo "###########################################################"
echo " Wavsep-start.sh"
echo "###########################################################"

# Check config file
[ ! -f config ] && { echo "[!] There is no config file"; exit 1; }
source "config"

if [ "$1" == "testing" ]; then
  REVOK_REPO_URL="$REVOK_TEST_REPO_URL"
fi

# Check & clear logs
[ ! -d "$LOGS_PATH" ] && { echo "[*] Creating logs folder: $LOGS_PATH"; mkdir -p "$LOGS_PATH"; }
rm -f "$LOGS_PATH/*.json"
rm -f "$LOGS_PATH/*.log"

packages=(tomcat jre mariadb-server curl)

for package in "${packages[@]}"; do
  echo "[*] Checking presence of $package package"

  if [[ $(which $package) == "" ]]; then
    echo "[*] Installing $package package"
    yum install -y "$package" >/dev/null

    if [ $? -ne 0 ]; then
      echo "[!] Cannot install $package package"
      exit 1
    fi
  fi
done

if [ -d "$TOMCAT_ROOT" ]; then
  echo "[*] There is already old wavsep instance present, cleaning.."
  rm -rf "$TOMCAT_ROOT"
  mkdir -p "$TOMCAT_ROOT"
fi

if [ ! -d "$WAVSEP_REPO_PATH" ]; then
  echo "[*] Cloning Wavsep repo to $WAVSEP_REPO_PATH"
  git clone "$WAVSEP_REPO_URL" "$WAVSEP_REPO_PATH" 2>&1 || { echo "[!] Unable to clone Wavsep repo to $WAVSEP_REPO_PATH"; exit 1; }
fi

# Entering Wavsep directory
cd "$WAVSEP_REPO_PATH" || { echo "[!] Cannot enter Wavsep directory"; exit 1; }

# Git dir check
[ ! git rev-parse --git-dir ] >/dev/null 2>&1 && { echo "[!] Wavsep directory is not valid git repository"; exit 1; }

echo "[*] Pulling updates to Wavsep repo"
git pull origin master 2>&1 || { echo "[!] Cannot pull update Wavsep repo"; exit 1; }

echo "[*] Cloning Wavsep to TomcatRoot"

git clone "file://$WAVSEP_REPO_PATH" "$TOMCAT_ROOT" 2>&1 || { echo "[!] Unable to clone Wavsep repo to TomcatRoot"; exit 1; }

if [ -d "$REVOK_REPO_PATH" ]; then
  rm -rf "$REVOK_REPO_PATH" >/dev/null 2>&1
fi

# Clone/update Revok
echo "[*] Cloning Revok repo to $REVOK_REPO_PATH"
git clone "$REVOK_REPO_URL" "$REVOK_REPO_PATH" 2>&1 || { echo "[!] Unable to clone Revok repo to $REVOK_REPO_PATH"; exit 1; }

cd "$REVOK_REPO_PATH" 2>&1 || { echo "[!] Cannot enter Revok directory"; exit 1; }

# Git dir check
[ ! git rev-parse --git-dir ] >/dev/null 2>&1 && { echo "[!] Revok directory is not valid git repository"; exit 1; }

echo "[*] Setting file permissions"
chown -R tomcat:tomcat "$TOMCAT_ROOT"
chmod -R g+rw "$TOMCAT_ROOT"

sed -i 's/8080/8082/g' /etc/tomcat/server.xml

systemctl status tomcat >/dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "[*] Starting tomcat service.."
  systemctl start tomcat 2>&1
fi

systemctl status tomcat >/dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "[!] Cannot start tomcat service"
  exit 1
fi

echo "[*] Service tomcat is running"

systemctl status mariadb >/dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "[*] Starting mariadb service.."
  systemctl start mariadb 2>&1
fi

systemctl status mariadb >/dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "[!] Cannot start mariadb service"
  exit 1
fi

echo "[*] Service mariadb is running"

echo "[*] Done"
exit 0

