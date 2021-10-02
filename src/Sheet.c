/*
	sgs_server
	File:/src/Sheet.c
	Date:2021.10.02
	By MIT License.
	Copyright (c) 2021 sgs developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include<fbd/fbd.h>

#include"Sheet.h"
#include"Conf.h"

#pragma pack(push,0)
typedef struct {
	uint8_t a,r,g,b;
}Sheet_Pixel;
#pragma pack(pop)

typedef struct {
	Sheet_Pixel *buffer;
	uint32_t width,height;
	uint32_t x,y;			// The position of the left-up point
	unsigned int used:1;
	unsigned int hidden:1;
}Sheet;

struct Sheet_Ctl {
	Sheet sheets[SGS_CONF_SHEET_MAX_NUM];
	Sheet *sorted[SGS_CONF_SHEET_MAX_NUM];
	struct {
		uint32_t width,height;
		Fbd_Device *dev;
	}screen;
	uint32_t used;			// With no hidden sheet,this is equal
					// to the height of the top
	int32_t backgroundId;
};

static struct Sheet_Ctl *gSheetCtl;

static uint32_t get_height(int32_t id);

int sheet_init(void)
{
	/*	init the device	*/
	gSheetCtl = (struct Sheet_Ctl*)malloc(sizeof(struct Sheet_Ctl));
	if (!gSheetCtl)
		return -1;

	gSheetCtl->screen.dev = fbd_open("/dev/fb0");
	if (!gSheetCtl->screen.dev) {
		free(gSheetCtl);
		return -1;
	}

	gSheetCtl->screen.width		= fbd_width(gSheetCtl->screen.dev);
	gSheetCtl->screen.height	= fbd_height(gSheetCtl->screen.dev);

	void *buffer = malloc(sizeof(Sheet_Pixel) *
			      gSheetCtl->screen.width *
			      gSheetCtl->screen.width);
	if (!buffer)
		return -1;

	memset(gSheetCtl->sheets,0,sizeof(Sheet) * SGS_CONF_SHEET_MAX_NUM);

	int32_t background = sheet_alloc(buffer,gSheetCtl->screen.width,
					 gSheetCtl->screen.height);
	gSheetCtl->backgroundId = background;
	sheet_show(background);
	memset(buffer,255,sizeof(Sheet_Pixel) *
			gSheetCtl->screen.width *
			gSheetCtl->screen.height);

	sheet_refresh(0);

	return 0;
}

void sheet_destroy(void)
{
	fbd_close(gSheetCtl->screen.dev);
	free(gSheetCtl);
	return;
}

int32_t sheet_alloc(void *buffer,uint32_t width,uint32_t height)
{
	int32_t id = 0;
	for (;id < SGS_CONF_SHEET_MAX_NUM;id++) {
		if (!gSheetCtl->sheets[id].used) {
			Sheet *sheet = gSheetCtl->sheets + id;
			*sheet = (Sheet) {
						.used	= 1,
						.hidden = 1,
						.width	= width,
						.height	= height,
						.x	= 0,
						.y	= 0,
						.buffer	= buffer
					 };
			gSheetCtl->sorted[gSheetCtl->used] =
						gSheetCtl->sheets + id;
			gSheetCtl->used++;
			return id;
		}
	}
	return -1;
}

void sheet_show(int32_t id)
{
	gSheetCtl->sheets[id].hidden = 0;
	return;
}

void sheet_hide(int32_t id)
{
	gSheetCtl->sheets[id].hidden = 1;
	return;
}

void sheet_free(int32_t id)
{
	Sheet *sheet = gSheetCtl->sheets + id;
	sheet->used = 0;

	uint32_t height = get_height(id);
	for (height++;height < gSheetCtl->used;height++)
		gSheetCtl->sorted[height - 1] = gSheetCtl->sorted[height];
	gSheetCtl->used--;

	return;
}

void sheet_refresh(uint32_t start)
{
	for (uint32_t height = start;height < gSheetCtl->used;height++) {
		Sheet *sheet = gSheetCtl->sorted[height];
		if (sheet->hidden)
			continue;

		uint32_t xEnd = sheet->x + sheet->width;
		uint32_t yEnd = sheet->y + sheet->height;
		uint32_t sheetX = 0,sheetY = 0;
		for (uint32_t y = sheet->y;y < yEnd;y++) {
			for(uint32_t x = sheet->x;x < xEnd;x++) {
				Sheet_Pixel *pixel = sheet->buffer +
						     sheetY * sheet->width +
						     sheetX;
				fbd_point(gSheetCtl->screen.dev,
					  x,y,pixel->a,pixel->r,pixel->g,pixel->b);
				sheetX++;
			}
			sheetX = 0;
			sheetY++;
		}
	}
	return;
}

static uint32_t get_height(int32_t id)
{
	Sheet *target = gSheetCtl->sheets + id;
	for (uint32_t height = 0;height < gSheetCtl->used;height++) {
		if (gSheetCtl->sorted[height] == target)
			return height;
	}

	abort();
	return 0;
}

void sheet_move(int32_t id,uint32_t x,uint32_t y)
{
	Sheet *sheet = gSheetCtl->sheets + id;
	sheet->x = x;
	sheet->y = y;
}
