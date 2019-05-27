#define REQUEST_CNT 9
#define RESPONSE_CNT 9

#include "mhz19.h"
#include "uart.h"
#include <string.h>

char response[RESPONSE_CNT];

// serial command
char dataRequest[REQUEST_CNT]	= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
//char calibZero[REQUEST_CNT]	 	= {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
//char calibSpan[REQUEST_CNT]	 	= {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77};
//char calibAuto_on[REQUEST_CNT] 	= {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xe6};
//char calibAuto_off[REQUEST_CNT]	= {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};

int btoi(byte high, byte low)
{
	return (256 * (int)high) + (int)low;
}

byte calcCheckSum(char *data)
{
	byte checksum = 0;
	for(byte i = 1; i < 8; i++)
	{
		checksum += data[i];
	}
	checksum = 0xff-checksum;
	checksum++;
	return checksum;
}

int getPPM()
{
	if(result == MHZ19_RESULT_OK)
	{
		return btoi(response[2],response[3]);
	}
	return 0;
}

byte getTemperature()
{
	if(result == MHZ19_RESULT_OK)
	{
		return (byte)response[4] - 40;
	}
	return 0;
}

byte getStatus()
{
	if(result == MHZ19_RESULT_OK)
	{
		return response[5] > 0x0f ? 1 : 0;
	}
	return 0;
}

void write(char *command, byte length)
{
	for(byte i = 0; i < length; i++)
	{
		uart_putc(command[i]);
	}
}

byte getData()
{
	for(byte i = 0; i < RESPONSE_CNT;)
	{
		char c = uart_getc();
		response[i++] = c;
	}
	
	result = MHZ19_RESULT_OK;
	if (response[0] != 0xFF)
	result = MHZ19_RESULT_ERR_FB;
	if (response[1] != 0x86)
	result = MHZ19_RESULT_ERR_SB;
	
	byte checkSum = calcCheckSum(response);
	if (response[8] != checkSum)
	result = MHZ19_RESULT_ERR_CRC;
	
	return result;
}

byte retrieveData()
{
	memset(response, 0, RESPONSE_CNT);	
	write(dataRequest, REQUEST_CNT);	
	return getData();
}

byte waitData()
{	
	memset(response, 0, RESPONSE_CNT);
	return getData();
}