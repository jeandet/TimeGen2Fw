#ifndef SPIM_DRIVER_H
#define SPIM_DRIVER_H

#include <stdio.h>
#include "peripherals.h"

typedef struct flash_desc
{
	char wren;
	char page_program_cmd;
	char sector_erase_cmd;
	char chip_erase_cmd;
	char address_bytes;
	char read_cmd;
	char read_id_cmd;
	char read_status_reg1_cmd;
	char read_status_reg2_cmd;
}flash_desc;

int done(SPIMCTRL_t* dev)
{
	return dev->Status.FIELDS.Done;
}

void chip_select(SPIMCTRL_t* dev, char value)
{
	dev->Control.FIELDS.ChipSelect = !value;
}

void tx_byte(SPIMCTRL_t* dev, char byte)
{
	dev->Transmit.FIELDS.Data = byte;
	while(!done(dev));
}

uint32_t read_id(SPIMCTRL_t* dev, const flash_desc* flash)
{
	uint32_t id=0;
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, flash->read_id_cmd);
	int i=0;
	for(i=0;i<4;i++)
	{
		tx_byte(dev, 0xFF);
		id = (id<<8) + dev->Receive.FIELDS.Data;
	}
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
	return  id;
}

char read_reg(SPIMCTRL_t *dev, char cmd)
{
	char value=0;
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, cmd);
	tx_byte(dev, 0xFF);
	value = dev->Receive.FIELDS.Data;
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
	return  value;
}

char read_status_reg1(SPIMCTRL_t *dev, const flash_desc* flash)
{
	return read_reg(dev, flash->read_status_reg1_cmd);
}

char read_status_reg2(SPIMCTRL_t *dev, const flash_desc* flash)
{
	return read_reg(dev, flash->read_status_reg2_cmd);
}

char read_config_reg1(SPIMCTRL_t *dev)
{
	return read_reg (dev, 0x35);
}

char read_config_reg2(SPIMCTRL_t *dev)
{
	return read_reg (dev, 0x15);
}

char read_config_reg3(SPIMCTRL_t *dev)
{
	return read_reg (dev, 0x33);
}

char read_any_register(SPIMCTRL_t *dev, int reg)
{
	char value=0;
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, 0x65);
	tx_byte(dev, reg>>16);
	tx_byte(dev, reg>>8);
	tx_byte(dev, reg);
	tx_byte(dev, 0xFF);//dumy cycles
	tx_byte(dev, 0xFF);
	value = dev->Receive.FIELDS.Data;
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
	return  value;
}

void wren(SPIMCTRL_t* dev, const flash_desc* flash)
{
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, flash->wren);
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
	while(0x2!=(0x2 & read_status_reg1(dev, flash)));
}

void erase_sector(SPIMCTRL_t* dev, uint32_t address, const flash_desc* flash)
{
	wren(dev, flash);
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, flash->sector_erase_cmd);
	tx_byte(dev, address>>16);
	tx_byte(dev, address>>8);
	tx_byte(dev, address);
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
	while(1 == (1 & read_status_reg1 (dev, flash)));
}

void erase_chip(SPIMCTRL_t* dev, const flash_desc* flash)
{
	wren(dev, flash);
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, flash->chip_erase_cmd);
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
	while(1 == (1 & read_status_reg1 (dev, flash)));
}

void write_page(SPIMCTRL_t* dev, uint32_t address, unsigned char* data, const flash_desc* flash)
{
	wren(dev, flash);
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, flash->page_program_cmd);
	tx_byte(dev, address>>16);
	tx_byte(dev, address>>8);
	tx_byte(dev, address);
	int i=0;
	for(i=0; i<256; i++)
	{
		tx_byte(dev, data[i]);
	}
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
	while(1 == (1 & read_status_reg1 (dev, flash)));
}

void read_n(SPIMCTRL_t* dev, uint32_t address, unsigned char* data, int count, const flash_desc* flash)
{
	dev->Control.FIELDS.UserControl=1;
    	chip_select(dev,1);
	tx_byte(dev, flash->read_cmd);
	tx_byte(dev, address>>16);
	tx_byte(dev, address>>8);
	tx_byte(dev, address);
	int i=0;
	for(i=0; i<256;i++)
	{
		tx_byte(dev, 0xFF);
		data[i] = dev->Receive.FIELDS.Data;
	}
	chip_select(dev,0);
	dev->Control.FIELDS.UserControl=0;
}

#endif
