#include "peripherals.h"

const device_id OpenCoresSPW = {.VID=0x08, .PID=0x131};
const device_id SPIMCTRL = {.VID=0x01, .PID=0x45};

const AHB_PNP_entry_t* AHB_Masters_PNP = (AHB_PNP_entry_t*)0xFFFFF000;
const AHB_PNP_entry_t* AHB_Slaves_PNP = (AHB_PNP_entry_t*)0xFFFFF800;
const APB_PNP_entry_t* APB_Slaves_PNP = (APB_PNP_entry_t*)0x800FF000;

uint32_t get_apb_base_addr(device_id devid, int count)
{
	int i=0;
	for(i=0;i<512;i++)
	{
		if(devid.VID == APB_Slaves_PNP[i].identification.FIELDS.VendorID && devid.PID == APB_Slaves_PNP[i].identification.FIELDS.DeviceID)
		{
			if(count == 0)
				return 0x80000000 | (APB_Slaves_PNP[i].BAR.FIELDS.Address<<8);
			count -= 1;
		}
	}
	return 0;
}

uint32_t get_ahb_master_base_addr(device_id devid, int count, int BAR)
{
	int i=0;
	for(i=0;i<64;i++)
	{
		if(devid.VID == AHB_Masters_PNP[i].identification.FIELDS.VendorID && devid.PID == AHB_Masters_PNP[i].identification.FIELDS.DeviceID)
		{
			if(count == 0)
				return  AHB_Masters_PNP[i].BAR[BAR].FIELDS.Address<<20;
			count -= 1;
		}
	}
	return 0;
}

uint32_t get_ahb_slave_base_addr(device_id devid, int count, int BAR)
{
	int i=0;
	for(i=0;i<64;i++)
	{
		printf("Device %d VID=0x%X PID=0x%X\n",i,AHB_Slaves_PNP[i].identification.FIELDS.VendorID,AHB_Slaves_PNP[i].identification.FIELDS.DeviceID);
		if(devid.VID == AHB_Slaves_PNP[i].identification.FIELDS.VendorID && devid.PID == AHB_Slaves_PNP[i].identification.FIELDS.DeviceID)
		{
			if(count == 0)
			{
				if(AHB_Slaves_PNP[i].BAR[BAR].FIELDS.Type == AHB_IO)
					return  0xFFF00000 | AHB_Slaves_PNP[i].BAR[BAR].FIELDS.Address<<8;
				return AHB_Slaves_PNP[i].BAR[BAR].FIELDS.Address<<20;
			}
			count -= 1;
		}

	}
	return 0;
}

