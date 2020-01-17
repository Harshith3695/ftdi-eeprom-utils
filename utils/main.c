#include <stdio.h>
#include "../examples/ftd2xx.h"

using namespace std;

int main(void)
{
    DWORD dwVID;
    DWORD dwPID;
    DWORD pdwVID;
    DWORD pdwPID;
    DWORD numDevs;

    FT_STATUS ftStatus;

    FT_DEVICE_LIST_INFO_NODE *devInfo;

    std::cout<<"Hello, x86_64!\n"<<std::endl;

    dwVID=403;
    dwPID=6010;
    ftStatus = FT_SetVIDPID(dwVID, dwPID);

    ftStatus = FT_GetVIDPID(&pdwVID, &pdwPID);
    if (ftStatus == FT_OK){    // Get the first devices VID & PID.
        printf("Device VID = 0x%d\n", pdwVID);
        printf("Device PID = 0x%d\n", pdwPID);
    }else{
        printf("Could not find any FTDI chips.\n");
        return 0;
    }

    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if (ftStatus == FT_OK){    // Get the first device serial number.
        printf("Number of FTDI devices connected is = %d\n", numDevs);
    }else{
        printf("Could not find any FTDI chips.\n");
        return 0;
    }

    if (numDevs > 0) {
    	// allocate storage for list based on numDevs.
    	devInfo=(FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
    	// get the device information list.
    	ftStatus=FT_GetDeviceInfoList(devInfo, &numDevs);
    	if (ftStatus == FT_OK){
    		for (int i=0; i<numDevs; i++){
    			printf("Device %d: \n", i);
    			printf("	Flags			= 0x%x\n", devInfo[i].Flags);
    			printf("	Type			= 0x%x\n", devInfo[i].Type);
    			printf("	ID				= 0x%x\n", devInfo[i].ID);
    			printf("	LocID			= 0x%x\n", devInfo[i].LocId);
    			printf("	Serial Number	= 0x%x\n", devInfo[i].SerialNumber);
    			printf("	Description		= 0x%x\n", devInfo[i].Description);
    			printf("	ftHandle		= 0x%x\n", devInfo[i].ftHandle);
    		}
    	}
    }

    return 0;
}
