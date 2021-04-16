#ifndef _FILE_SYSTEM_TEST_H
#define _FILE_SYSTEM_TEST_H


#ifdef __cplusplus
extern "C"{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif



int fs_test_open_file(void);

int fs_test_run_font(void);

int fs_test_flash_read(uint8_t *font_buf, int offset, int size);


//uint32_t LV_USER_FONT_ADDR = 0;



#ifdef __cplusplus
}
#endif



#endif
