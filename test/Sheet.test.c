/*
	sgs_server
	File:/test/Sheet.test.c
	Date:2021.10.02
	By MIT License.
	Copyright (c) 2021 sgs developers.All rights reserved.
*/

#include<stdio.h>

#include<unistd.h>

#include"Sheet.h"

int main(void)
{
	sheet_init();

	static uint8_t buffer[4 * 64 * 64];
	int32_t win = sheet_alloc(buffer,64,64);
	sheet_show(win);

	sheet_refresh(0);
	sleep(3);

	sheet_move(win,120,120);
	sheet_refresh(0);
	sleep(3);

	sheet_destroy();
	return 0;
}
