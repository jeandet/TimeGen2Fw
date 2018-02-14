#include <stdio.h>
#include "peripherals.h"
#include "spim_driver.h"
#include "S25FL128L.h"
#include "bcc/ambapp.h"
#include <stdint.h>

extern int _binary_payload_bin_start;
extern int _binary_payload_bin_end;
extern int _binary_payload_bin_size;
//
//int _binary_payload_bin_start = 0x40011304;
//int _binary_payload_bin_end =   0x40024104;
//int _binary_payload_bin_size =  0x00012e00;

int main(void)
{
	volatile SPIMCTRL_t* spim =(SPIMCTRL_t*)get_ahb_slave_base_addr(SPIMCTRL, 0, 0);
	if(0==spim)
	{
		printf("Error: no SPI Memory Controller found\n");
		return 1;
	}
	printf ("Found SPI Memory Controller @0x%X\n",(unsigned int)spim);
	spim->Control.FIELDS.ResetCore = 1;
	while(!spim->Status.FIELDS.Initialized);
	printf("SPIM initialized\n");
	printf("Payload Start = %X\n", &_binary_payload_bin_start);
	printf("Payload End = %X\n", &_binary_payload_bin_end);
	printf("Payload Size = %d\n", &_binary_payload_bin_size);
	unsigned char* payload=(unsigned char*) (&_binary_payload_bin_start);
	int i=0;
	erase_chip(spim, &SF25FL128);
	printf("[->");
	for(i=0;i < &_binary_payload_bin_size; )
	{
		write_page(spim, i, payload+i, &SF25FL128);
		if(i%1024==0)
			printf("\b->");
		i+=256;
	}
	printf("\b-]");
	printf("done\n");
	return 0;
}
