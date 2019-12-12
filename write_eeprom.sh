#!/bin/bash

ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
ABSOLUTE_DIR=`dirname $ABSOLUTE_PATH`

EXEC_PATH="${ABSOLUTE_DIR}/examples/EEPROM/write"

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

SF_STATUS=`sudo systemctl is-active serialforwarder@quadcomm.service`
if [[ "${SF_STATUS}" == "active" ]]; then
        echo "Stopping serialforwarder@quadcomm.service"
        sudo systemctl stop serialforwarder@quadcomm.service
fi

FTDI_SIO=`lsmod | grep -o "ftdi_sio" | head -1`
USBSERIAL=`lsmod | grep -o "usbserial"`

if [[ "${FTDI_SIO}" == "ftdi_sio" ]]; then
        echo "Removing module: ${FTDI_SIO}"
        sudo rmmod ftdi_sio
fi

if [[ "${USBSERIAL}" == "usbserial" ]]; then
        echo "Removing module: ${USBSERIAL}"
        sudo rmmod usbserial
fi

echo "_______________________ Writing EEPROM _________________________"
echo "${EXEC_PATH}/write_quadropus -n DB000$1"
sudo ${EXEC_PATH}/write_quadropus -n DB000$1
sleep 1
echo "________________________________________________________________"

sudo modprobe ftdi_sio
sleep 3
sudo systemctl restart serialforwarder@quadcomm.service

echo "Completed $(/usr/bin/basename  "${BASH_SOURCE[0]}")"
