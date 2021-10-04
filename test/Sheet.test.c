/*
	sgs_server
	File:/test/Sheet.test.c
	Date:2021.10.03
	By MIT License.
	Copyright (c) 2021 sgs developers.All rights reserved.
*/

#include<stdio.h>
#include<string.h>

#include<unistd.h>

#include"Sheet.h"

int main(void)
{
	sheet_init();

	static uint8_t buffer[4 * 64 * 64];
	static uint8_t buffer2[4 * 64 * 64];
	for (uint32_t i = 0;i < 64 * 64 * 4;i+=4) {
		buffer[i] = 0;
		buffer[i + 1] = 255;
		buffer[i + 2] = 0;
		buffer[i + 3] = 0;
	}
	int32_t win = sheet_alloc(buffer,64,64);
	int32_t win2 = sheet_alloc(buffer2,64,64);
	sheet_show(win);
	sheet_show(win2);

	sheet_refresh(0);
	sleep(3);

	sheet_updown(win,2);
	sheet_refresh(0);
	sleep(3);

	sheet_move(win,120,120);
	sheet_refresh(0);
	sleep(3);

	sheet_destroy();
	return 0;
}
