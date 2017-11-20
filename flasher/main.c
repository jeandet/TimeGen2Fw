#include <stdio.h>
#include "peripherals.h"
#include "spim_driver.h"
#include "S25FL128L.h"
#include "bcc/ambapp.h"
#include <stdint.h>

extern int _binary_payload_bin_start;
extern int _binary_payload_bin_end;
extern int _binary_payload_bin_size;

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
    printf("Payload Start = %X\n", &_binary_payload_bin_start);
    printf("Payload End = %X\n", &_binary_payload_bin_end);
    printf("Payload Size = %d\n", &_binary_payload_bin_size);
    unsigned char* test=(unsigned char*) (&_binary_payload_bin_start);
    int i=0;
    erase_chip(spim, &SF25FL128);
    printf("[->");
    for(i=0;i < &_binary_payload_bin_size; )
    {
        write_page(spim, i, &(test[i]), &SF25FL128);
        if(i%1024==0)
            printf("\b->");
        i+=256;
    }
    printf("\b-]");
    printf("done\n");
	return 0;
}
