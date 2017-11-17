#include <stdio.h>
#include "peripherals.h"
#include "bcc/ambapp.h"
#include <stdint.h>

uint32_t match(void *info, uint32_t vendor, uint32_t device, uint32_t type, uint32_t depth, void *arg)
{
	struct amba_apb_info *apbi = info;
	if (type != AMBAPP_VISIT_APBSLAVE)
	{
		return 0;
	}
	return apbi->start;
}


OpenCoresSPW_t* find_opencores_spw()
{
	const uint32_t ioarea = 0xFFFFF000;
	const uint32_t depth = 4;
	return (OpenCoresSPW_t*) ambapp_visit(ioarea, OpenCoresSPW.VID, OpenCoresSPW.PID, AMBAPP_VISIT_APBSLAVE, depth, match, 0);
}

int main(void)
{
	printf("hello\n");
	OpenCoresSPW_t* spw = find_opencores_spw();
	if(spw == 0)
	{
		printf("No SpaceWire device found!\n");
		return 1;
	}
	printf("Found SpaceWire device @0x%X",spw);
	return 0;
}
