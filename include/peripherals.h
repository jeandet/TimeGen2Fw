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

typedef struct device_id
{
	int VID;
	int PID;
}device_id;



const device_id OpenCoresSPW = {.VID=0x08, .PID=0x131};

typedef volatile struct  {
     union Control{
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

     union Status{
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

     union TransmissionClockScaler{
         struct   {
            uint32_t _:24;
            uint32_t TxClkDivFactor:8;
        }FIELDS;
        uint32_t Value;
    }TransmissionClockScaler;

     union Timecode{
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


#endif