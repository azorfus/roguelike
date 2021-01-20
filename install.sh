#!/bin/bash
g++ -o arpg rogue.cpp map.cpp

if ! [ $(id -u) = 0 ]; then
	echo "Run this script as the super user.";
	exit
fi
cp arpg /usr/bin
echo "Done!, Type "arpg" to run the game!"
