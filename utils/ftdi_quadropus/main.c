/*
 * main.c
 *
 *  Created on: Dec 13, 2019
 *      Author: thk3695
 */


#include <stdio.h>
#include "includes/ftd2xx.h"

int main(void)
{
    DWORD dwVID;
    DWORD dwPID;
    DWORD pdwVID;
    DWORD pdwPID;
    DWORD numDevs;
    DWORD Flags;
    DWORD ID;
    DWORD Type;
    DWORD LocId;
    char SerialNumber[16];
    char Description[64];

    FT_STATUS ftStatus;

    FT_HANDLE ftHandleTemp;

    FT_DEVICE_LIST_INFO_NODE *devInfo;

    printf("Hello, x86_64!\n");

//    dwVID=403;
//    dwPID=6010;
//    ftStatus = FT_SetVIDPID(dwVID, dwPID);
//    if (ftStatus != FT_OK){    // Get the first devices VID & PID.
//        printf("Could not find any FTDI chips.\n");
//        return 0;
//    }

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
        printf("Number of FTDI devices connected is = %u\n", numDevs);
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
    		for (DWORD i=0; i<numDevs; i++){
    			printf("Device %d: \n", i);
    			printf("----------\n");
    			printf("	Flags            = 0x%u\n", devInfo[i].Flags);
    			printf("	Type             = 0x%u\n", devInfo[i].Type);
    			printf("	ID               = 0x%u\n", devInfo[i].ID);
    			printf("	LocID            = 0x%u\n", devInfo[i].LocId);
    			printf("	Serial Number    = 0x%s\n", devInfo[i].SerialNumber);
    			printf("	Description      = 0x%s\n", devInfo[i].Description);
    			printf("	ftHandle         = 0x%u\n", devInfo[i].ftHandle);
    		}
    	}
    }

    if (numDevs > 0) {
    	// get the device information list.
    	ftStatus=FT_GetDeviceInfoDetail(0, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandleTemp);
    	if (ftStatus == FT_OK){
    		for (DWORD i=0; i<numDevs; i++){
    			printf("Device %d: \n", i);
    			printf("----------\n");
    			printf("	Flags            = 0x%u\n", devInfo[i].Flags);
    			printf("	Type             = 0x%u\n", devInfo[i].Type);
    			printf("	ID               = 0x%u\n", devInfo[i].ID);
    			printf("	LocID            = 0x%u\n", devInfo[i].LocId);
    			printf("	Serial Number    = 0x%s\n", devInfo[i].SerialNumber);
    			printf("	Description      = 0x%s\n", devInfo[i].Description);
    			printf("	ftHandle         = 0x%u\n", devInfo[i].ftHandle);
    		}
    	}
    }

    return 0;
}
