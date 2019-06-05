/*
	Modified:	Harshith Kumar Thirumala.
	Notes:		To build use the following gcc statement
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o write main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib

	TODO: Adding a conf file, to pass other parameters using it.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../ftd2xx.h"

void print_usage(void);
void gen_rand_id(char* node);

void gen_rand_id(char* node){
	char prefix[3];
	char postfix[7];
	srand(time(0));
	for (int k=0; k<2; k++){
		int j = ((rand()%25)+65);
		char tmp = (char)j;
		prefix[k] = tmp;
	}
	int i = ((rand()%900000)+99999);
	sprintf(postfix, "%d", i);
	strncpy(node,prefix,2);
	strncat(node,postfix,6);
}

void print_usage(){
	printf("/nUsage: sudo ./write (For writing a random <nodeID | serialNo>)\n");
	printf("       sudo ./write -n <nodeID | serialNo>\n");
	printf("       sudo ./write -i <interface port>\n\n");
}

int main(int argc, char *argv[])
{
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle0;
	FT_DEVICE	ftDevice;
	FT_PROGRAM_DATA Data;
	int retCode = 0;
	int iport = 0;
	char node[9] = "FT";
	char man_id[3] = "";
	int opt;
	int validOpt = 0;

	if (argc > 1){
		while((opt = getopt(argc, argv, "n:i:h")) != -1){
			validOpt = 1;
			switch(opt){
				case 'n':
					strcpy(node, optarg);
					break;
				case 'i':
					iport = atoi(optarg);
					break;
				case 'h':
					print_usage();
					return 1;
				default:
					printf("Error! <nodeID | serialNo> or <interface port> not found.\nSee Usage for more info.\n");
					print_usage();
					return 1;
			}
		}
		if (!validOpt){
			printf("Error! <nodeID | serialNo> or <interface port> not found.\nSee Usage for more info.\n");
			print_usage();
			return 1;
		}
	}
//	else{
//		print_usage();
//		return 1;
//	}

	printf("* Program meta-data:\n");
	printf("--------------------\n");

	if (!strcmp(node,"FT")){
		printf("Generating random <nodeID | serialNo>.\n");
		gen_rand_id(node);
	}

	strncpy(man_id, node, 2);

	printf("Node ID (Ser. no.)\t= %s\n", node);
	printf("Interface port\t\t= %d\n", iport);
	printf("Manufacturer ID\t\t= %s\n", man_id);


	printf("Opening port\t\t= %d\n", iport);
	FT_SetVIDPID(0x0403, 0x6011);
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

	printf("ftHandle0\t\t= %p\n", ftHandle0);

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

	printf("FT_GetDeviceInfo succeeded. Device type = %d.\n\n", (int)ftDevice);

	printf("* Writing common device parameters:\n");
	printf("-----------------------------------\n");
	Data.Signature1		= 0x00000000;
	Data.Signature2		= 0xffffffff;
	Data.VendorId		= 0x0403;
	switch((int)ftDevice){	// Product ID
		case 0:		// FT_DEVICE_BM
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x6001;
		break;
		case 1:		// FT_DEVICE_AM
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x6001;
		break;
		case 2:		// FT_DEVICE_100AX
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x6001;
		break;
		case 3:		// FT_DEVICE_UNKNOWN
			printf("Error, Unknown Device or Device Type!");
		break;
		case 4:		// FT_DEVICE_2232C
			Data.Version		= 0x00000001;
			Data.ProductId		= 0x6010;
		break;
		case 5:		// FT_DEVICE_232R
			Data.Version		= 0x00000002;
			Data.ProductId		= 0x6001;
		break;
		case 6:		// FT_DEVICE_2232H
			Data.Version		= 0x00000003;
			Data.ProductId		= 0x6010;
		break;
		case 7:		// FT_DEVICE_4232H
			Data.Version		= 0x00000004;
			Data.ProductId		= 0x6011;
		break;
		case 8:		// FT_DEVICE_232H
			Data.Version		= 0x00000005;
			Data.ProductId		= 0x6014;
		break;
		case 9:		// FT_DEVICE_X_SERIES
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x6015;
		break;
		case 10:	// FT_DEVICE_4222H_0
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x601C;
		break;
		case 11:	// FT_DEVICE_4222H_1_2
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x601C;
		break;
		case 12:	// FT_DEVICE_4222H_3
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x601C;
		break;
		case 13:	// FT_DEVICE_4222_PROG
			Data.Version		= 0x00000000;
			Data.ProductId		= 0x601C;
		break;
		default:
			printf("Error, Unknown Device or Device Type!");
	}
	Data.Manufacturer 	= "Databuoy Corporation.";
	Data.ManufacturerId	= man_id;
	Data.Description	= "Quadropus RevC.";
	Data.SerialNumber	= node;			// if fixed, or NULL

	Data.MaxPower		= 200;
	Data.PnP		= 1;
	Data.SelfPowered	= 0;
	Data.RemoteWakeup	= 0;

	printf("* Writing device specific parameters:\n");
	printf("-------------------------------------\n");

	if (ftDevice == FT_DEVICE_BM)
	{
	/* FT232B (Rev4) extensions. */
		Data.Rev4		= 1;
		Data.IsoIn		= 0;
		Data.IsoOut		= 0;
		Data.PullDownEnable	= 1;
		Data.SerNumEnable	= 1;
		Data.USBVersionEnable	= 0;
		Data.USBVersion		= 0x110;
	}

	if (ftDevice == FT_DEVICE_2232C)
	{
	/* FT2232 (Rev5) extensions. */
		Data.Rev5		= 1;				// non-zero if Rev5 chip, zero otherwise
		Data.IsoInA		= 0;				// non-zero if in endpoint is isochronous
		Data.IsoInB		= 0;				// non-zero if in endpoint is isochronous
		Data.IsoOutA		= 0;				// non-zero if out endpoint is isochronous
		Data.IsoOutB		= 0;				// non-zero if out endpoint is isochronous
		Data.PullDownEnable5	= 0;				// non-zero if pull down enabled
		Data.SerNumEnable5	= 1;				// non-zero if serial number to be used
		Data.USBVersionEnable5	= 0;				// non-zero if chip uses USBVersion
		Data.USBVersion5	= 0x0200;			// BCD (0x0200 => USB2)
		Data.AIsHighCurrent	= 0;				// non-zero if interface is high current
		Data.BIsHighCurrent	= 0;				// non-zero if interface is high current
		Data.IFAIsFifo		= 0;				// non-zero if interface is 245 FIFO
		Data.IFAIsFifoTar	= 0;				// non-zero if interface is 245 FIFO CPU target
		Data.IFAIsFastSer	= 0;				// non-zero if interface is Fast serial
		Data.AIsVCP		= 1;				// non-zero if interface is to use VCP drivers
		Data.IFBIsFifo		= 1;				// non-zero if interface is 245 FIFO
		Data.IFBIsFifoTar	= 0;				// non-zero if interface is 245 FIFO CPU target
		Data.IFBIsFastSer	= 0;				// non-zero if interface is Fast serial
		Data.BIsVCP		= 1;				// non-zero if interface is to use VCP drivers
	}

	if (ftDevice == FT_DEVICE_232R)
	{
	/* FT232R (Rev6) extensions. */
		Data.UseExtOsc		= 0;				// Use External Oscillator
		Data.HighDriveIOs	= 0;				// High Drive I/Os
		Data.EndpointSize	= 0;				// Endpoint size
		Data.PullDownEnableR	= 0;				// non-zero if pull down enabled
		Data.SerNumEnableR	= 0;				// non-zero if serial number to be used
		Data.InvertTXD		= 0;				// non-zero if invert TXD
		Data.InvertRXD		= 0;				// non-zero if invert RXD
		Data.InvertRTS		= 0;				// non-zero if invert RTS
		Data.InvertCTS		= 0;				// non-zero if invert CTS
		Data.InvertDTR		= 0;				// non-zero if invert DTR
		Data.InvertDSR		= 0;				// non-zero if invert DSR
		Data.InvertDCD		= 0;				// non-zero if invert DCD
		Data.InvertRI		= 0;				// non-zero if invert RI
		Data.Cbus0		= 0;				// Cbus Mux control
		Data.Cbus1		= 0;				// Cbus Mux control
		Data.Cbus2		= 0;				// Cbus Mux control
		Data.Cbus3		= 0;				// Cbus Mux control
		Data.Cbus4		= 0;				// Cbus Mux control
		Data.RIsD2XX		= 0;				// non-zero if using D2XX driver
	}

	if (ftDevice == FT_DEVICE_2232H)
	{
	/* FT2232H (Rev7) extensions. */
		Data.PullDownEnable7	= 0;				// non-zero if pull down enabled
		Data.SerNumEnable7	= 1;				// non-zero if serial number to be used
		Data.ALSlowSlew		= 0;				// non-zero if AL pins have slow slew
		Data.ALSchmittInput	= 0;				// non-zero if AL pins are Schmitt input
		Data.ALDriveCurrent	= 4;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.AHSlowSlew		= 0;				// non-zero if AH pins have slow slew
		Data.AHSchmittInput	= 0;				// non-zero if AH pins are Schmitt input
		Data.AHDriveCurrent	= 4;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.BLSlowSlew		= 0;				// non-zero if BL pins have slow slew
		Data.BLSchmittInput	= 0;				// non-zero if BL pins are Schmitt input
		Data.BLDriveCurrent	= 4;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.BHSlowSlew		= 0;				// non-zero if BH pins have slow slew
		Data.BHSchmittInput	= 0;				// non-zero if BH pins are Schmitt input
		Data.BHDriveCurrent	= 4;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.IFAIsFifo7		= 0;				// non-zero if interface is 245 FIFO
		Data.IFAIsFifoTar7	= 0;				// non-zero if interface is 245 FIFO CPU target
		Data.IFAIsFastSer7	= 0;				// non-zero if interface is Fast serial
		Data.AIsVCP7		= 1;				// non-zero if interface is to use VCP drivers
		Data.IFBIsFifo7		= 1;				// non-zero if interface is 245 FIFO
		Data.IFBIsFifoTar7	= 0;				// non-zero if interface is 245 FIFO CPU target
		Data.IFBIsFastSer7	= 0;				// non-zero if interface is Fast serial
		Data.BIsVCP7		= 1;				// non-zero if interface is to use VCP drivers
	}

	if (ftDevice == FT_DEVICE_4232H)
	{
	/* FT4232H (Rev8) extensions. */
		Data.PullDownEnable8	= 0;				// non-zero if pull down enabled
		Data.SerNumEnable8	= 0;				// non-zero if serial number to be used
		Data.ASlowSlew		= 0;				// non-zero if A pins have slow slew
		Data.ASchmittInput	= 0;				// non-zero if A pins are Schmitt input
		Data.ADriveCurrent	= 0;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.BSlowSlew		= 0;				// non-zero if B pins have slow slew
		Data.BSchmittInput	= 0;				// non-zero if B pins are Schmitt input
		Data.BDriveCurrent	= 0;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.CSlowSlew		= 0;				// non-zero if C pins have slow slew
		Data.CSchmittInput	= 0;				// non-zero if C pins are Schmitt input
		Data.CDriveCurrent	= 0;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.DSlowSlew		= 0;				// non-zero if D pins have slow slew
		Data.DSchmittInput	= 0;				// non-zero if D pins are Schmitt input
		Data.DDriveCurrent	= 0;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.ARIIsTXDEN		= 0;				// non-zero if port A uses RI as RS485TXDEN
		Data.BRIIsTXDEN		= 0;				// non-zero if port Buses RI as RS485TXDEN
		Data.CRIIsTXDEN		= 0;				// non-zero if port C uses RI as RS485TXDEN
		Data.DRIIsTXDEN		= 0;				// non-zero if port D uses RI as RS485TXDEN
		Data.AIsVCP8		= 0;				// non-zero if interface is to use VCP drivers
		Data.BIsVCP8		= 0;				// non-zero if interface is to use VCP drivers
		Data.CIsVCP8		= 0;				// non-zero if interface is to use VCP drivers
		Data.DIsVCP8		= 0;				// non-zero if interface is to use VCP drivers
	}

	if (ftDevice == FT_DEVICE_232H)
	{
	/* FT232H (Rev9) extensions. */
		Data.PullDownEnableH	= 0;				// non-zero if pull down enabled
		Data.SerNumEnableH	= 0;				// non-zero if serial number to be used
		Data.ACSlowSlewH	= 0;				// non-zero if AC pins have slowslew
		Data.ACSchmittInputH	= 0;				// non-zero if AC pins are Schmitt input
		Data.ACDriveCurrentH	= 0;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.ADSlowSlewH	= 0;				// non-zero if AD pins have slow slew
		Data.ADSchmittInputH	= 0;				// non-zero if AD pins are Schmitt input
		Data.ADDriveCurrentH	= 0;				// valid values are 4mA, 8mA, 12mA, 16mA
		Data.Cbus0H		= 0;				// Cbus Mux control
		Data.Cbus1H		= 0;				// Cbus Mux control
		Data.Cbus2H		= 0;				// Cbus Mux control
		Data.Cbus3H		= 0;				// Cbus Mux control
		Data.Cbus4H		= 0;				// Cbus Mux control
		Data.Cbus5H		= 0;				// Cbus Mux control
		Data.Cbus6H		= 0;				// Cbus Mux control
		Data.Cbus7H		= 0;				// Cbus Mux control
		Data.Cbus8H		= 0;				// Cbus Mux control
		Data.Cbus9H		= 0;				// Cbus Mux control
		Data.IsFifoH		= 0;				// non-zero if interface is 245 FIFO
		Data.IsFifoTarH		= 0;				// non-zero if interface is 245 FIFO CPU target
		Data.IsFastSerH		= 0;				// non-zero if interface is Fastserial
		Data.IsFT1248H		= 0;				// non-zero if interface is FT1248
		Data.FT1248CpolH	= 0;				// FT1248 clock polarity-clockidlehigh(1) or clockidlelow(0)
		Data.FT1248LsbH		= 0;				// FT1248 data is LSB(1) or MSB(0)
		Data.FT1248FlowControlH	= 0;				// FT1248 flow control enable
		Data.IsVCPH		= 0;				// non-zero if interface is to use VCP drivers
		Data.PowerSaveEnableH	= 0;				// non-zero if using ACBUS7 to save power for self-powered designs
	}

exit:
	ftStatus = FT_EE_Program(ftHandle0, &Data);
	if(ftStatus != FT_OK) {
		printf("FT_EE_Program failed (%d)\n", (int)ftStatus);
		FT_Close(ftHandle0);
	}
	FT_Close(ftHandle0);
//	printf("------------------------------------------\n");
	printf("Writing to device successful. Returning %d\n", retCode);
	return 0;
}
