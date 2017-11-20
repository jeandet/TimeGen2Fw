#include <stdio.h>
#include "peripherals.h"
#include "spim_driver.h"
#include "S25FL128L.h"
#include "bcc/ambapp.h"
#include <stdint.h>

uint32_t match(void *info, uint32_t vendor, uint32_t device, uint32_t type, uint32_t depth, void *arg)
{
	if (type == AMBAPP_VISIT_APBSLAVE )
	{
        printf("Found APB slave\n");
        struct amba_apb_info *apbi = info;
        return apbi->start;
	}
	if (type == AMBAPP_VISIT_AHBSLAVE )
	{
        printf("Found AHB slave\n");
        struct amba_ahb_info *ahbi = info;
        int i=0;
        for(i=0;i<4;i++)
        {
            if(ahbi->bar[i].type == 3)
               return ahbi->bar[0].start;
        }
	}
    return 0;
}

void* find_device(device_id id)
{
	const uint32_t ioarea = 0xFFFFF000;
	const uint32_t depth = 16;
	return (void*) ambapp_visit(ioarea, id.VID, id.PID, AMBAPP_VISIT_AHBMASTER | AMBAPP_VISIT_AHBSLAVE | AMBAPP_VISIT_APBSLAVE, depth, match, 0);
}

int main(void)
{
	printf("hello\n");
	volatile OpenCoresSPW_t* spw = find_device(OpenCoresSPW);
    volatile SPIMCTRL_t* spim =(SPIMCTRL_t*)0xFFF20000;
	if(spw == 0)
	{
		printf("No SpaceWire device found!\n");
		return 1;
	}
	printf("Found SpaceWire device @0x%X\n", spw);
    spim->Control.FIELDS.ResetCore = 1;
    while(!spim->Status.FIELDS.Initialized);
    printf("SPIM initialized\n");
    printf("FLASH ID = 0x%X\n", read_id(spim, &SF25FL128));
    printf("FLASH CR2V reg = 0x%X\n", read_config_reg2(spim));
    printf("FLASH CR1V reg = 0x%X\n", read_config_reg1(spim));
    unsigned char test[256];
    int i=0;
    for(i=0;i<256;i++)
    {
        test[i]=i;
    }
    erase_sector(spim, 0, &SF25FL128);
    write_page(spim, 0, test, &SF25FL128);
    printf("Status Reg1 = 0x%X\n", read_status_reg1(spim, &SF25FL128));
    printf("Status Reg2 = 0x%X\n", read_status_reg2(spim, &SF25FL128));
    read_n(spim, 0, test, 256, &SF25FL128);
    for(i=0;i<256;i++)
    {
        printf("Data @%d = 0x%X\n",i, test[i]);
    }
	return 0;
}
