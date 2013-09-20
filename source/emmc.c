#include "emmc.h"
#include "mailbox.h"
#include "timer.h"
#include "stringutil.h"

volatile Emmc* gEmmc;

unsigned int EmmcInitialise(void)
{
	printf("ssed - Initializing external mass media controller.\n");
	
	gEmmc = (Emmc*)EMMC_BASE;

	// Power cycle to ensure initial state
	EmmcPowerCycle();

	printf("ssed - Version: %d Vendor: %d SdVersion: %d Slot status: %d\n",
		gEmmc->SlotisrVer.raw, 
		gEmmc->SlotisrVer.bits.vendor, 
		gEmmc->SlotisrVer.bits.sdversion, 
		gEmmc->SlotisrVer.bits.slot_status);
	
	printf("ssed - Resetting circuit... ");

	// Reset the entire host circuit
	gEmmc->Control1.bits.srst_hc = 1;

	// Disable internal clock
	gEmmc->Control1.bits.clk_intlen = 0;

	// Disable SD clock
	gEmmc->Control1.bits.clk_en = 0;

	// Wait for controller to reset 
	//WARNING: If reset fails we will hang forever, but it's K (for now) because we're showing a message
	while(gEmmc->Control1.bits.clk_en != 0 || gEmmc->Control1.bits.srst_data != 0 || gEmmc->Control1.bits.srst_hc != 0) continue;
	
	printf("Done!\n");

	printf("ssed - Control0: %d.\n", gEmmc->Control0);
	printf("ssed - Control1: %d.\n", gEmmc->Control1);
	printf("ssed - Control2: %d.\n", gEmmc->Control2);
	
	printf("ssed - Capabilities 0: %d.\n", gEmmc->reserved);
	printf("ssed - Capabilities 1: %d.\n", gEmmc->reserved2);

	wait(10);
	
	// gEmmc->Arg1 = [11:8] Voltage
	// gEmmc->CmdTm = SendIfCond;
	
	// if(returned response )
	// {
	    // // It's a Ver2.00 or later SD memory card 
		// if(is NOT valid response)
		// {
			// // Unusable card
			// return;
		// }
		
		// // Card agreed with our voltage etc
		
		// do
		// {
			// gEmmc->CmdTm = Acmd41;
		// }while(card returns busy);
		
		// if(Not card is ready)
		// {
			// // Unsable card
			// return;
		// }
		
		// if(ccs in response == 1)
		// {
			// // SD is a High capacity or extended capacity card
		// }
		// else if(ccs in response == 0)
		// {
			// // sd is a standard capacity sd card
		// }
	// }
	// else
	// {
		// // Ver2.00 or later SD memory card (voltage mismatch)
		// // or Ver1.X SD Memory Card or not SD Memory Card at all			
		// while(card is busy)
		// {
			// gEmmc->CmdTm = Acmd41; 
			
			// if(no response to last command )
			// {
				// // Not a SD memory card
			// }
		// }
		
		// if(Not card is ready)
		// {
			// // Useless card
		// }	
	// }
	
	// // Send CMD11 or CMD12 based on response from Acmd41
	// if(acmd41 response S18R and S18A is == 1)
	// {
		// // Switch over to 1.8Volt for no apparent reason
		// gEmmc->CmdTm = VoltageSwitch
	// }
	
	// // Get the CID from the card
	// gEmmc->CmdTm = AllSendCid;
	
	// // Store CID
	
	// // Get the address of the card so we can address it
	// gEmmc->CmdTm = SendRelativeAddr;
	
	
	
	return 0; // Code below not finished
	
	// TODO: Flesh out pseudo code
	// Send CMD0
	
	// send CMD8
	//		response: Version 2 > SD
	//	  		valid response
	//				Non-compatible voltage range or check pattern is not correct UNUSABLE CARD
	//				
	//			Card with compatible voltage range
	// 				ACMD41 with HCS=0 or 1 (1 if we support high capacity)
	//					Card ready?
	//						Yes - is CCS in response?
	//							CCS 0: Ver 2.0 > SD card
	//							CCS 1: Ver 2.0 > SDHC or SDXC
	//						No - Send ACM41 again
	//						Timed out - Unusable card
	// 		no response : return
	//		
	//		Set voltage to High 
	// 		Request card id
	// 		Request card address
	// 		Place card in transfer state
	
	return 0;
}

unsigned int EmmcSendCommand(EmmcCommand cmd, unsigned int argument, unsigned int timeout, cmd_rspns_type response)
{
	gEmmc->Arg1 = argument;
	gEmmc->Cmdtm.bits.CmdRspnsType = response;	
	gEmmc->Cmdtm.bits.CmdIndex = (int)cmd;

	if(response > 0)
	{
		// Do stuff...
		return gEmmc->Resp0;
	}
	return 0;	
}
unsigned int EmmcPowerOn(void)
{
	return Mailbox_SetDevicePowerState(0x0, 1);
}

unsigned int EmmcPowerOff(void)
{
	return Mailbox_SetDevicePowerState(0x0, 0);
}

unsigned int EmmcPowerCycle(void)
{
	printf("ssed - Power cycling: ");
	
	unsigned int res = 0;
	if((res = EmmcPowerOff()) < 0)
	{
		printf("Failed!\n");
		return -1;
	}	
	
	wait(50);
	
	if((res = EmmcPowerOn()) < 0)
		printf("Failed!\n");
	else
		printf("Success!\n");
	
	return EmmcPowerOn();
}
