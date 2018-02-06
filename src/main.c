#include <stdio.h>
#include "peripherals.h"
#include "spim_driver.h"
#include "S25FL128L.h"
#include "bcc/ambapp.h"
#include <stdint.h>

int main(void)
{
	printf("hello\n");
	volatile OpenCoresSPW_t* spw = (OpenCoresSPW_t*)get_apb_base_addr(OpenCoresSPW, 0);
	if(spw == 0)
	{
		printf("No SpaceWire device found!\n");
		return 1;
	}
	printf("Found SpaceWire device @0x%X\n", (unsigned int)spw);
    	printf("hello from payload\n");
	spw->Control.FIELDS.LinkStart = 1;
	spw->Control.FIELDS.TimeCodeEnable = 1;
	return 0;
}
