#!/bin/bash

#set -x
#set -e

ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
ABSOLUTE_DIR=`dirname $ABSOLUTE_PATH`

EXEC_PATH="${ABSOLUTE_DIR}/examples/EEPROM/write"

echo "FTDI EEPROM Write Utility"
echo "________________________________________________________________"

if [ $# -eq 0 ] || [ $# -gt 2 ]; then
	echo "Usage: ./write_eeprom.sh <nodeRev> <nodeID>"
	exit 1
fi

if [ "$1" != "B" ] && [ "$1" != "C" ]; then
	echo "Error! Wrong node revision specified => $1"
	echo "Usage: ./write_eeprom.sh <nodeRev> <nodeID>"
	exit 1
fi

ARG=$(expr $2 + 1)
if [ "$2" == "" ] || [ ! $ARG ]; then
	echo "Error! Not a valid nodeID. => $2"
	echo "Usage: ./write_eeprom.sh <nodeRev> <nodeID>"
	exit 1
fi

CONNECTED=`${ABSOLUTE_DIR}/utils/devInfo/listDevs | grep FTDI | sed s/"Number of FTDI devices connected is = "//g`
if [ $CONNECTED -gt 0 ]; then
    echo "Found $CONNECTED FTDI devices."
else
    echo "No FTDI devices found. Exiting..."
    exit 1
fi

SF_WAS_ACTIVE=0
SF_STATUS=`sudo systemctl is-active serialforwarder@quadcomm.service`
if [[ "${SF_STATUS}" == "active" ]]; then
    echo "Stopping serialforwarder@quadcomm.service"
    sudo systemctl stop serialforwarder@quadcomm.service
    SF_WAS_ACTIVE=1
fi

FTDI_SIO_LOADED=0
FTDI_SIO=`lsmod | grep -o "ftdi_sio" | head -1`
USBSERIAL=`lsmod | grep -o "usbserial"`

if [[ "${FTDI_SIO}" == "ftdi_sio" ]]; then
    echo "Removing module: ${FTDI_SIO}"
    sudo rmmod ftdi_sio
    FTDI_SIO_LOADED=1
fi

if [[ "${USBSERIAL}" == "usbserial" ]]; then
        echo "Removing module: ${USBSERIAL}"
        sudo rmmod usbserial
fi

echo "________________________________________________________________"
echo "_______________________ Writing EEPROM _________________________"
sudo ${EXEC_PATH}/write_quadropus -r $1 -n DB000$2
sleep 1
echo "________________________________________________________________"

if [ $FTDI_SIO_LOADED ]; then
    echo "Reloading ftdi_sio..."
    sudo modprobe ftdi_sio
fi
sleep 3
if [ $SF_WAS_LOADED ]; then
    echo "Restarting serialforwarder@quadcomm.service ..."
    sudo systemctl restart serialforwarder@quadcomm.service
fi

echo "Completed $(/usr/bin/basename  "${BASH_SOURCE[0]}")"
