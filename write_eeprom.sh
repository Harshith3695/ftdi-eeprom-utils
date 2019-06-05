#!/bin/bash

ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
ABSOLUTE_DIR=`dirname $ABSOLUTE_PATH`

if [ $# -eq 0 ] || [ $# -gt 1 ]; then
	echo "Usage: ./write_eeprom.sh <nodeID>"
	exit 1
fi

ARG=$(expr $1 + 1)
if [ ! $ARG ]; then
	echo "Error! Not a valid nodeID."
	echo "Usage: ./write_eeprom.sh <nodeID>"
	exit 1
fi

sudo systemctl stop serialforwarder@quadcomm.service

sudo rmmod ftdi_sio
sudo usbserial

EXEC_PATH="${ABSOLUTE_DIR}/examples/EEPROM/write"
sudo ${EXEC_PATH}/write -n DB000$1
echo ${EXEC_PATH}/write -n DB000$1
