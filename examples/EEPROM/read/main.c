/*
	To build use the following gcc statement
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o read main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../ftd2xx.h"

int main(int argc, char *argv[])
{
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle0;
	int iport;
	static FT_PROGRAM_DATA Data;
	static FT_DEVICE ftDevice;
	DWORD libraryVersion = 0;
	int retCode = 0;

	ftStatus = FT_GetLibraryVersion(&libraryVersion);
	if (ftStatus == FT_OK)
	{
		printf("* Program meta-data:\n");
		printf("--------------------\n");
		printf("Library version\t= 0x%x\n", (unsigned int)libraryVersion);
	}
	else
	{
		printf("Error reading library version.\n");
		return 1;
	}

	if(argc > 1) {
		sscanf(argv[1], "%d", &iport);
	}
	else {
		iport = 0;
	}
	printf("Opening port %d\n", iport);

	ftStatus = FT_Open(iport, &ftHandle0);
	if(ftStatus != FT_OK) {
		/*
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open(%d) failed\n", iport);
		return 1;
	}

	printf("FT_Open succeeded. Handle is %p\n", ftHandle0);

	ftStatus = FT_GetDeviceInfo(ftHandle0,
	                            &ftDevice,
	                            NULL,
	                            NULL,
	                            NULL,
	                            NULL);
	if (ftStatus != FT_OK)
	{
		printf("FT_GetDeviceType FAILED!\n");
		retCode = 1;
		goto exit;
	}

	printf("FT_GetDeviceInfo succeeded. Device is type %d.\n",
	       (int)ftDevice);

	/* MUST set Signature1 and 2 before calling FT_EE_Read */
	Data.Signature1 = 0x00000000;
	Data.Signature2 = 0xffffffff;
	Data.Manufacturer = (char *)malloc(256); /* E.g "FTDI" */
	Data.ManufacturerId = (char *)malloc(256); /* E.g. "FT" */
	Data.Description = (char *)malloc(256); /* E.g. "USB HS Serial Converter" */
	Data.SerialNumber = (char *)malloc(256); /* E.g. "FT000001" if fixed, or NULL */

	if (Data.Manufacturer == NULL ||
	    Data.ManufacturerId == NULL ||
	    Data.Description == NULL ||
	    Data.SerialNumber == NULL)
	{
		printf("Failed to allocate memory.\n");
		retCode = 1;
		goto exit;
	}

	ftStatus = FT_EE_Read(ftHandle0, &Data);
	if(ftStatus != FT_OK) {
		printf("FT_EE_Read failed\n");
		retCode = 1;
		goto exit;
	}

	printf("FT_EE_Read succeeded.\n\n");

	printf("* Common device parameters:\n");
	printf("---------------------------\n");
	printf("Signature1\t= \"%d\"\n", (int)Data.Signature1);
	printf("Signature2\t= \"%d\"\n", (int)Data.Signature2);
	printf("Version\t\t= \"%d\"\n", (int)Data.Version);
	printf("VendorId\t= \"0x%04X\"\n", Data.VendorId);
	printf("ProductId\t= \"0x%04X\"\n", Data.ProductId);
	printf("Manufacturer\t= \"%s\"\n", Data.Manufacturer);
	printf("ManufacturerId\t= \"%s\"\n", Data.ManufacturerId);
	printf("Description\t= \"%s\"\n", Data.Description);
	printf("SerialNumber\t= \"%s\"\n", Data.SerialNumber);
	printf("MaxPower\t= \"%d\"\n", Data.MaxPower);
	printf("PnP\t\t= \"%d\"\n", Data.PnP) ;
	printf("SelfPowered\t= \"%d\"\n", Data.SelfPowered);
	printf("RemoteWakeup\t= \"%d\"\n", Data.RemoteWakeup);

	printf("\n* Device Specific Info.:\n");
	printf("------------------------\n");
	printf("Device type = %d.\n", (int)ftDevice);

	if (ftDevice == FT_DEVICE_BM)
	{
		/* Rev4 (FT232B) extensions */
		printf("BM:\n");
		printf("---\n");
		printf("\tRev4\t\t= \"0x%X\"\n", Data.Rev4);
		printf("\tIsoIn\t\t= \"0x%X\"\n", Data.IsoIn);
		printf("\tIsoOut\t\t= \"0x%X\"\n", Data.IsoOut);
		printf("\tPullDownEnable\t= \"0x%X\"\n", Data.PullDownEnable);
		printf("\tSerNumEnable\t= \"0x%X\"\n", Data.SerNumEnable);
		printf("\tUSBVersionEnable= \"0x%X\"\n", Data.USBVersionEnable);
		printf("\tUSBVersion\t= \"0x%X\"\n", Data.USBVersion);
	}

	if (ftDevice == FT_DEVICE_2232C)
	{
		/* Rev 5 (FT2232C) extensions */
		printf("2232RC:\n");
		printf("-------\n");
		printf("\tRev5\t\t= \"0x%X\"\n", Data.Rev5);
		printf("\tIsoInA\t\t= \"0x%X\"\n", Data.IsoInA);
		printf("\tIsoInB\t\t= \"0x%X\"\n", Data.IsoInB);
		printf("\tIsoOutA\t\t= \"0x%X\"\n", Data.IsoOutA);
		printf("\tIsoOutB\t\t= \"0x%X\"\n", Data.IsoOutB);
		printf("\tPullDownEnable\t= \"0x%X\"\n", Data.PullDownEnable5);
		printf("\tSerNumEnable\t= \"0x%X\"\n", Data.SerNumEnable5);
		printf("\tUSBVersionEnable= \"0x%X\"\n", Data.USBVersionEnable5);
		printf("\tUSBVersion\t= \"0x%X\"\n", Data.USBVersion5);
		printf("\tAIsHighCurrent\t= \"0x%X\"\n", Data.AIsHighCurrent);
		printf("\tBIsHighCurrent\t= \"0x%X\"\n", Data.BIsHighCurrent);
		printf("\tIFAIsFifo\t= \"0x%X\"\n", Data.IFAIsFifo);
		printf("\tIFAIsFifoTar\t= \"0x%X\"\n", Data.IFAIsFifoTar);
		printf("\tIFAIsFastSer\t= \"0x%X\"\n", Data.IFAIsFastSer);
		printf("\tAIsVCP\t\t= \"0x%X\"\n", Data.AIsVCP);
		printf("\tIFBIsFifo\t= \"0x%X\"\n", Data.IFBIsFifo);
		printf("\tIFBIsFifoTar\t= \"0x%X\"\n", Data.IFBIsFifoTar);
		printf("\tIFBIsFastSer\t= \"0x%X\"\n", Data.IFBIsFastSer);
		printf("\tBIsVCP\t\t= \"0x%X\"\n", Data.BIsVCP);
	}

	if (ftDevice == FT_DEVICE_232R)
	{
		/* Rev 6 (FT232R) extensions */
		printf("232R:\n");
		printf("-----\n");
		printf("\tUseExtOsc\t= \"0x%X\"\n", Data.UseExtOsc);				// Use External Oscillator
		printf("\tHighDriveIOs\t= \"0x%X\"\n", Data.HighDriveIOs);			// High Drive I/Os
		printf("\tEndpointSize\t= \"0x%X\"\n", Data.EndpointSize);			// Endpoint size

		printf("\tPullDownEnableR\t= \"0x%X\"\n", Data.PullDownEnableR);		// non-zero if pull down enabled
		printf("\tSerNumEnableR\t= \"0x%X\"\n", Data.SerNumEnableR);			// non-zero if serial number to be used

		printf("\tInvertTXD\t= \"0x%X\"\n", Data.InvertTXD);				// non-zero if invert TXD
		printf("\tInvertRXD\t= \"0x%X\"\n", Data.InvertRXD);				// non-zero if invert RXD
		printf("\tInvertRTS\t= \"0x%X\"\n", Data.InvertRTS);				// non-zero if invert RTS
		printf("\tInvertCTS\t= \"0x%X\"\n", Data.InvertCTS);				// non-zero if invert CTS
		printf("\tInvertDTR\t= \"0x%X\"\n", Data.InvertDTR);				// non-zero if invert DTR
		printf("\tInvertDSR\t= \"0x%X\"\n", Data.InvertDSR);				// non-zero if invert DSR
		printf("\tInvertDCD\t= \"0x%X\"\n", Data.InvertDCD);				// non-zero if invert DCD
		printf("\tInvertRI\t= \"0x%X\"\n", Data.InvertRI);				// non-zero if invert RI

		printf("\tCbus0\t\t= \"0x%X\"\n", Data.Cbus0);					// Cbus Mux control
		printf("\tCbus1\t\t= \"0x%X\"\n", Data.Cbus1);					// Cbus Mux control
		printf("\tCbus2\t\t= \"0x%X\"\n", Data.Cbus2);					// Cbus Mux control
		printf("\tCbus3\t\t= \"0x%X\"\n", Data.Cbus3);					// Cbus Mux control
		printf("\tCbus4\t\t= \"0x%X\"\n", Data.Cbus4);					// Cbus Mux control

		printf("\tRIsD2XX\t\t= \"0x%X\"\n", Data.RIsD2XX);				// non-zero if using D2XX
	}

	if (ftDevice == FT_DEVICE_2232H)
	{
		/* Rev 7 (FT2232H) extensions */
		printf("FT2232H:\n");
		printf("--------\n");
		printf("\tPullDownEnable\t= \"0x%X\"\n", Data.PullDownEnable7);
		printf("\tSerNumEnable\t= \"0x%X\"\n", Data.SerNumEnable7);

		printf("\tALSlowSlew\t= \"0x%X\"\n", Data.ALSlowSlew);
		printf("\tALSchmittInput\t= \"0x%X\"\n", Data.ALSchmittInput);
		printf("\tALDriveCurrent\t= \"0x%X\"\n", Data.ALDriveCurrent);
		printf("\tAHSlowSlew\t= \"0x%X\"\n", Data.AHSlowSlew);
		printf("\tAHSchmittInput\t= \"0x%X\"\n", Data.AHSchmittInput);
		printf("\tAHDriveCurrent\t= \"0x%X\"\n", Data.AHDriveCurrent);

		printf("\tBLSlowSlew\t= \"0x%X\"\n", Data.BLSlowSlew);
		printf("\tBLSchmittInput\t= \"0x%X\"\n", Data.BLSchmittInput);
		printf("\tBLDriveCurrent\t= \"0x%X\"\n", Data.BLDriveCurrent);
		printf("\tBHSlowSlew\t= \"0x%X\"\n", Data.BHSlowSlew);
		printf("\tBHSchmittInput\t= \"0x%X\"\n", Data.BHSchmittInput);
		printf("\tBHDriveCurrent\t= \"0x%X\"\n", Data.BHDriveCurrent);

		printf("\tIFAIsFifo\t= \"0x%X\"\n", Data.IFAIsFifo7);
		printf("\tIFAIsFifoTar\t= \"0x%X\"\n", Data.IFAIsFifoTar7);
		printf("\tIFAIsFastSer\t= \"0x%X\"\n", Data.IFAIsFastSer7);
		printf("\tAIsVCP\t\t= \"0x%X\"\n", Data.AIsVCP7);
		printf("\tIFBIsFifo\t= \"0x%X\"\n", Data.IFBIsFifo7);
		printf("\tIFBIsFifoTar\t= \"0x%X\"\n", Data.IFBIsFifoTar7);
		printf("\tIFBIsFastSer\t= \"0x%X\"\n", Data.IFBIsFastSer7);
		printf("\tBIsVCP\t\t= \"0x%X\"\n", Data.BIsVCP7);

		printf("\tPowerSaveEnable\t= \"0x%X\"\n", Data.PowerSaveEnable);
	}

	if (ftDevice == FT_DEVICE_4232H)
	{
		/* Rev 8 (FT4232H) extensions */
		printf("FT4232H:\n");
		printf("--------\n");
		printf("\tPullDownEnable\t= \"0x%X\"\n", Data.PullDownEnable8);
		printf("\tSerNumEnable\t= \"0x%X\"\n", Data.SerNumEnable8);

		printf("\tASlowSlew\t= \"0x%X\"\n", Data.ALSlowSlew);
		printf("\tASchmittInput\t= \"0x%X\"\n", Data.ALSchmittInput);
		printf("\tADriveCurrent\t= \"0x%X\"\n", Data.ALDriveCurrent);
		printf("\tBSlowSlew\t= \"0x%X\"\n", Data.AHSlowSlew);
		printf("\tBSchmittInput\t= \"0x%X\"\n", Data.AHSchmittInput);
		printf("\tBDriveCurrent\t= \"0x%X\"\n", Data.AHDriveCurrent);

		printf("\tCSlowSlew\t= \"0x%X\"\n", Data.BLSlowSlew);
		printf("\tCSchmittInput\t= \"0x%X\"\n", Data.BLSchmittInput);
		printf("\tCDriveCurrent\t= \"0x%X\"\n", Data.BLDriveCurrent);
		printf("\tDSlowSlew\t= \"0x%X\"\n", Data.BHSlowSlew);
		printf("\tDSchmittInput\t= \"0x%X\"\n", Data.BHSchmittInput);
		printf("\tDDriveCurrent\t= \"0x%X\"\n", Data.BHDriveCurrent);

		printf("\tARIIsTXDEN\t= \"0x%X\"\n", Data.ARIIsTXDEN);
		printf("\tBRIIsTXDEN\t= \"0x%X\"\n", Data.BRIIsTXDEN);
		printf("\tCRIIsTXDEN\t= \"0x%X\"\n", Data.CRIIsTXDEN);
		printf("\tDRIIsTXDEN\t= \"0x%X\"\n", Data.DRIIsTXDEN);

		printf("\tAIsVCP\t\t= \"0x%X\"\n", Data.AIsVCP8);
		printf("\tBIsVCP\t\t= \"0x%X\"\n", Data.BIsVCP8);
		printf("\tCIsVCP\t\t= \"0x%X\"\n", Data.CIsVCP8);
		printf("\tDIsVCP\t\t= \"0x%X\"\n", Data.DIsVCP8);
	}

	if (ftDevice == FT_DEVICE_232H)
	{
		/* Rev 9 (FT232H) extensions */
		printf("FT232H:\n");
		printf("-------\n");
		printf("\tPullDownEnableH\t= \"0x%X\"\n", Data.PullDownEnableH);
		printf("\tSerNumEnableH\t= \"0x%X\"\n", Data.SerNumEnableH);

		printf("\tACSlowSlewH\t= \"0x%X\"\n", Data.ACSlowSlewH);
		printf("\tACSchmittInputH\t= \"0x%X\"\n", Data.ACSchmittInputH);
		printf("\tACDriveCurrentH\t= \"0x%X\"\n", Data.ACDriveCurrentH);
		printf("\tADSlowSlew\t= \"0x%X\"\n", Data.ADSlowSlewH);
		printf("\tADSchmittInput\t= \"0x%X\"\n", Data.ADSchmittInputH);
		printf("\tADDriveCurrent\t= \"0x%X\"\n", Data.ADDriveCurrentH);

		printf("\tCbus0H\t\t= \"0x%X\"\n", Data.Cbus0H);
		printf("\tCbus1H\t\t= \"0x%X\"\n", Data.Cbus1H);
		printf("\tCbus2H\t\t= \"0x%X\"\n", Data.Cbus2H);
		printf("\tCbus3H\t\t= \"0x%X\"\n", Data.Cbus3H);
		printf("\tCbus4H\t\t= \"0x%X\"\n", Data.Cbus4H);
		printf("\tCbus5H\t\t= \"0x%X\"\n", Data.Cbus5H);
		printf("\tCbus6H\t\t= \"0x%X\"\n", Data.Cbus6H);
		printf("\tCbus7H\t\t= \"0x%X\"\n", Data.Cbus7H);
		printf("\tCbus8H\t\t= \"0x%X\"\n", Data.Cbus8H);
		printf("\tCbus9H\t\t= \"0x%X\"\n", Data.Cbus9H);

		printf("\tIsFifoH\t\t= \"0x%X\"\n", Data.IsFifoH);
		printf("\tIsFifoTarH\t= \"0x%X\"\n", Data.IsFifoTarH);
		printf("\tIsFastSerH\t= \"0x%X\"\n", Data.IsFastSerH);
		printf("\tIsFT1248H\t= \"0x%X\"\n", Data.IsFT1248H);
		printf("\tFT1248LsbH\t= \"0x%X\"\n", Data.FT1248LsbH);
		printf("\tFT1248FlwCntrl\t= \"0x%X\"\n", Data.FT1248FlowControlH);
		printf("\tIsVCPH\t\t= \"0x%X\"\n", Data.IsVCPH);
		printf("\tPowerSaveEnableH= \"0x%X\"\n", Data.PowerSaveEnableH);
	}

exit:
	free(Data.Manufacturer);
	free(Data.ManufacturerId);
	free(Data.Description);
	free(Data.SerialNumber);
	FT_Close(ftHandle0);
	if (!retCode){
		printf("------------------------------------\n");
		printf("Device read successful. Returning %d.\n", retCode);
	}
	return retCode;
}
