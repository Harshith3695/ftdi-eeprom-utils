#!/bin/bash

ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
ABSOLUTE_DIR=`dirname $ABSOLUTE_PATH`

EXEC_PATH="${ABSOLUTE_DIR}/examples/EEPROM/read"

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

echo "${EXEC_PATH}/read_quadropus"
sudo ${EXEC_PATH}/read_quadropus
