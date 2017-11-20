#ifndef S25FL128L_H
#define S25FL128L_H
#include "spim_driver.h"

const flash_desc SF25FL128 =
{
	.wren = 0x06,
	.page_program_cmd = 0x02,
	.sector_erase_cmd = 0x20,
	.chip_erase_cmd = 0x60,
	.address_bytes = 3,
	.read_cmd = 0x03,
	.read_id_cmd = 0x9F,
	.read_status_reg1_cmd = 0x05,
	.read_status_reg2_cmd = 0x07,
};

#endif
