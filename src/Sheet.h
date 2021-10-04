/*
	sgs_server
	File:/src/Sheet.h
	Date:2021.10.03
	By MIT License.
	Copyright (c) 2021 sgs developers.All rights reserved.
*/

#ifndef __SGS_SHEET_H_INC__
#define __SGS_SHEET_H_INC__

#include<stdint.h>

int sheet_init(void);
void sheet_destroy(void);
void sheet_show(int32_t id);
void sheet_hide(int32_t id);
int32_t sheet_alloc(void *buffer,uint32_t width,uint32_t height);
void sheet_refresh(uint32_t start);
void sheet_move(int32_t id,uint32_t x,uint32_t y);
void sheet_updown(int32_t id,uint32_t height);

#endif
