#!/bin/bash

clear;make clean;make;echo "";valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./main input.fal

