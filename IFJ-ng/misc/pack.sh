#!/bin/bash
cd ..
make clean
find . -maxdepth 1 -type f \( -iname "*.h~" -or -iname "*.c~" \) -exec rm  {} \;
rm xkolac12.tgz
tar cvzf xkolac12.tgz *.c *.h Makefile rozdeleni dokumentace.pdf
mv xkolac12.tgz misc/
cd misc/
./is_it_ok.sh xkolac12.tgz test
rm -rf test
