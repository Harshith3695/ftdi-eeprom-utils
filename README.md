# ftdi-eeprom-utils
* Consists of programs that help read from and write to the EEPROM of FTDI USB chips, on th Linux platform. Programs are based on the D2xx drivers from FTDI.

NOTE: If ./read_eeprom.sh exits with this error:
```
FT_Open succeeded. Handle is 0x5632a20978f0
FT_GetDeviceInfo succeeded. Device is type 6.
FT_EE_Read failed
```
The FTDI EEPROM is blank and first has to be written to it before reading it.
