#!/bin/bash

set -u

echo "###########################################################"
echo " Wivet-start.sh"
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

packages=(httpd php git readlink tcl java-1.8.0-openjdk)

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

# Document root does not exist
[ ! -d "$DOCUMENT_ROOT" ] && { echo "[!] There is no DocumentRoot present in $DOCUMENT_ROOT"; exit 1; }

# Cleaning old Wivet instance
if [ -d "$DOCUMENT_ROOT/wivet" ]; then
  echo "[*] There is already old wivet instance present"
  rm -rf "$DOCUMENT_ROOT/wivet"
fi

# Cloning Wivet repo
if [ ! -d "$WIVET_REPO_PATH" ]; then
  echo "[*] Cloning Wivet repo to $WIVET_REPO_PATH"
  git clone "$WIVET_REPO_URL" "$WIVET_REPO_PATH" 2>&1 || { echo "[!] Unable to clone Wivet repo to $WIVET_REPO_PATH"; exit 1; }
fi

# Enter Wivet repo
cd "$WIVET_REPO_PATH" || { echo "[!] Cannot enter Wivet directory"; exit 1; }

# Git dir check
[ ! git rev-parse --git-dir ] >/dev/null 2>&1 && { echo "[!] Wivet directory is not valid git repository"; exit 1; }

echo "[*] Pulling updates to Wivet repo"
git pull origin master 2>&1 || { echo "[!] Cannot pull update Wivet repo"; exit 1; }

echo "[*] Cloning Wivet to DocumentRoot"
git clone "file://$WIVET_REPO_PATH" "$DOCUMENT_ROOT/wivet" 2>&1 || { echo "[!] Unable to clone Wivet repo to DocumentRoot"; exit 1; }

echo "[*] Setting file permissions"
chown -R apache:apache "$DOCUMENT_ROOT/wivet"
chmod -R g+rw "$DOCUMENT_ROOT/wivet"

sed -i 's/^;date\.timezone.+/date.timezone = Europe\/Prague/g' /etc/php.ini

if [ -d "$REVOK_REPO_PATH" ]; then
  rm -rf "$REVOK_REPO_PATH" >/dev/null 2>&1
fi

# Clone/update Revok
echo "[*] Cloning Revok repo to $REVOK_REPO_PATH"
git clone "$REVOK_REPO_URL" "$REVOK_REPO_PATH" 2>&1 || { echo "[!] Unable to clone Revok repo to $REVOK_REPO_PATH"; exit 1; }

cd "$REVOK_REPO_PATH" 2>&1 || { echo "[!] Cannot enter Revok directory"; exit 1; }

# Git dir check
[ ! git rev-parse --git-dir ] >/dev/null 2>&1 && { echo "[!] Revok directory is not valid git repository"; exit 1; }

systemctl status httpd >/dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "[*] Starting httpd service.."
  systemctl start httpd 2>&1
fi

systemctl status httpd >/dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "[!] Cannot start httpd service"
  exit 1
fi

echo "[*] Service httpd is running"

echo "[*] Done"

exit 0
