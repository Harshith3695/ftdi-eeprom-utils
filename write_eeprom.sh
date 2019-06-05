#!/bin/bash

ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
ABSOLUTE_DIR=`dirname $ABSOLUTE_PATH`

sudo systemctl stop serialforwarder@quadcomm.service

sudo rmmod ftdi_sio
sudo usbserial

EXEC_PATH="${ABSOLUTE_DIR}/examples/EEPROM/read"
sudo ${EXEC_PATH}/read
