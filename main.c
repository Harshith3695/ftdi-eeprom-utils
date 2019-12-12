#include <stdio.h>
#include "examples/ftd2xx.h"

int main(void)
{
    FT_STATUS ftStatus;
    DWORD numDevs;
    FT_DEVICE_LIST_INFO_NODE *devInfo;

    printf("Hello, x86_64!\n");

    ftStatus = FT_CreateDeviceInfoList(&numDevs);

    if (ftStatus == FT_OK){    // Get the first device serial number.
        printf("Number of FTDI devices connected is = %d\n", numDevs);
    }else{
        printf("Could not find any FTDI chips.\n");
        return 0;
    }

    return 0;
}
