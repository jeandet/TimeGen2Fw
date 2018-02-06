/*------------------------------------------------------------------------------
--  This file is a part of the LFR Offset analysis firmware
--  Copyright (C) 2016, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------*/
/*--                  Author : Alexis Jeandet
--                     Mail : alexis.jeandet@lpp.polytechnique.fr
----------------------------------------------------------------------------*/

#ifndef PERIPHERALS_H_INCLUDED
#define PERIPHERALS_H_INCLUDED

#include <stdint.h>

typedef enum {APB_IO=1,AHB_MEM=2,AHB_IO=3}AMBA_space_t;

typedef struct device_id
{
	int VID;
	int PID;
}device_id;

typedef union {
	 struct   {
	    uint32_t Address:12;
	    uint32_t _:2;
	    uint32_t Prefetchable:1;
	    uint32_t Cacheable:1;
	    uint32_t MASK:12;
	    uint32_t Type:4;
	}FIELDS;
	uint32_t Value;
}AHB_BAR_t;

typedef volatile struct  {
     union {
         struct   {
            uint32_t VendorID:8;
            uint32_t DeviceID:12;
	    uint32_t _:2;
            uint32_t Version:5;
            uint32_t IRQ:5;
        }FIELDS;
        uint32_t Value;
     }identification;

     uint32_t UserDefined[3];
     AHB_BAR_t BAR[4];

} AHB_PNP_entry_t;

typedef volatile struct  {
     union {
         struct   {
            uint32_t VendorID:8;
            uint32_t DeviceID:12;
	    uint32_t CT:2;
            uint32_t Version:5;
            uint32_t IRQ:5;
        }FIELDS;
        uint32_t Value;
     }identification;
     AHB_BAR_t BAR;
} APB_PNP_entry_t;


typedef volatile struct  {
     union {
         struct   {
            uint32_t _:4;
            uint32_t desctablesize:4;
            uint32_t __:10;
            uint32_t TxPacketIEN:1;
            uint32_t TxDescriptorIEN:1;
            uint32_t RxDescriptorIEN:1;
            uint32_t TimeCodeReceivedIEN:1;
            uint32_t LinkUpDownIEN:1;
            uint32_t CancelTxDMA:1;
            uint32_t StartTxDMA:1;
            uint32_t StartRxDMA:1;
            uint32_t TimeCodeEnable:1;
            uint32_t LinkDisable:1;
            uint32_t LinkAutoStart:1;
            uint32_t LinkStart:1;
            uint32_t ResetDMACore:1;
            uint32_t Reset:1;
        }FIELDS;
        uint32_t Value;
    }Control;

     union {
         struct   {
            uint32_t _:19;
            uint32_t RxBufferEmpty:1;
            uint32_t PackedReceived:1;
            uint32_t TxDescriptorFinished:1;
            uint32_t RxDescriptorFinished:1;
            uint32_t TimeCodeReceived:1;
            uint32_t Reserved:1;
            uint32_t AHBError:1;
            uint32_t TxDMAEnabled:1;
            uint32_t RxDMAEnabled:1;
            uint32_t CreditError:1;
            uint32_t EscapeError:1;
            uint32_t ParityError:1;
            uint32_t DisconnectError:1;
            uint32_t LinkStatus:2;
        }FIELDS;
        uint32_t Value;
    }Status;

     union {
         struct   {
            uint32_t _:24;
            uint32_t TxClkDivFactor:8;
        }FIELDS;
        uint32_t Value;
    }TransmissionClockScaler;

     union {
         struct   {
            uint32_t __:15;
            uint32_t SendTimecode:1;
            uint32_t _:2;
            uint32_t NextTimecode:6;
            uint32_t ControlBits:2;
            uint32_t LastReceivedTimecode:6;
        }FIELDS;
        uint32_t Value;
    }Timecode;

    uint32_t TxDescriptorPointer;
    uint32_t RxDescriptorPointer;

} OpenCoresSPW_t;

typedef volatile struct  {
     union {
         struct   {
            uint32_t _:24;
            uint32_t ReadCMD:8;
        }FIELDS;
        uint32_t Value;
    }Config;

     union {
         struct   {
            uint32_t _:27;
            uint32_t ResetCore:1;
            uint32_t ChipSelect:1;
            uint32_t EnableAltScaler:1;
            uint32_t InterruptEnable:1;
            uint32_t UserControl:1;
        }FIELDS;
        uint32_t Value;
    }Control;

     union {
         struct   {
	    uint32_t _:29;
	    uint32_t Initialized:1;
            uint32_t Busy:1;
            uint32_t Done:1;
        }FIELDS;
        uint32_t Value;
    }Status;

     union {
         struct   {
            uint32_t _:24;
            uint32_t Data:8;
        }FIELDS;
        uint32_t Value;
    }Receive;

    union {
         struct   {
            uint32_t _:24;
            uint32_t Data:8;
        }FIELDS;
        uint32_t Value;
    }Transmit;

} SPIMCTRL_t;


uint32_t get_apb_base_addr(device_id devid, int count);
uint32_t get_ahb_master_base_addr(device_id devid, int count, int BAR);
uint32_t get_ahb_slave_base_addr(device_id devid, int count, int BAR);

extern const device_id OpenCoresSPW;
extern const device_id SPIMCTRL;
extern const AHB_PNP_entry_t* AHB_Masters_PNP;






#endif
