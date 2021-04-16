/*
 * @incldue lvgl source file here
 * for some convenient inlcude ways
 *
 */

#ifndef _GUI_APP_H
#define _GUI_APP_H

#ifdef __cplusplus
  extern "C"{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#include <stdlib.h>
#include <stdio.h>

//comment this if run on board
#define _VS_SIMULATOR

//#define _SCREEN_SIZE_7_INCH
#define	SCREEN_BASE_RESOLUTION_HOR		800
#define SCREEN_BASE_RESOLUTION_VER		480

#define SCREEN_TENCELL_7inch_TITLE_HEIGHT	62

//#define		_SCREEN_SIZE_10_INCH
#define			_SCREEN_SIZE_7_INCH

#define _DEBUG_LVGL  1
#if _DEBUG_LVGL
//#define ui_printf(log_txt)   printf(log_txt)
#define ui_printf(...)  printf(__VA_ARGS__)

#ifdef	_VS_SIMULATOR
#define ui_printf_plus(fmt,...)		printf(fmt"\n",__VA_ARGS__)
#define	ui_printf_malloc_error(fmt, ...)	printf("<<<--UI MALLOC FAIL-->>>"fmt"\n", __VA_ARGS__)
#else
#define ui_printf_plus(fmt,arg...)		printf(fmt"\n",##arg)
#define	ui_printf_malloc_error(fmt, ...)	printf("<<<--UI MALLOC FAIL-->>>"fmt"\n", ##arg)
#endif

#else
#define ui_printf(...)
#endif

#define general_malloc(size_t)  LV_MEM_CUSTOM_ALLOC(size_t)
#define general_free(p)         LV_MEM_CUSTOM_FREE(p)

#define is_null_ptr(_ptr)    (_ptr == NULL)



/*Font Declare*/
#ifdef _SCREEN_SIZE_7_INCH
#define	THEME_CHINESE_FONT_NORMAL		SC_MicroSoft_YaHei_R16
#define THEME_CHINESE_FONT_LARGE		SC_MicroSoft_YaHei_R18
#define	THEME_CHINESE_FONT_SMALL		SC_MicroSoft_YaHei_R12
#define THEME_CHINESE_FONT_DYNAMIC_NORMAL	font_test_gb
#else
#define	THEME_CHINESE_FONT_NORMAL		SC_MicroSoft_YaHei_R18
#define THEME_CHINESE_FONT_LARGE		NULL
#define	THEME_CHINESE_FONT_SMALL		SC_MicroSoft_YaHei_R16
#endif // _SCREEN_SIZE_7_INCH
LV_FONT_DECLARE(THEME_CHINESE_FONT_NORMAL);
LV_FONT_DECLARE(THEME_CHINESE_FONT_LARGE);
LV_FONT_DECLARE(THEME_CHINESE_FONT_SMALL);
LV_FONT_DECLARE(THEME_CHINESE_FONT_DYNAMIC_NORMAL);


//include all the sub-directory file here so that each file include this H-file can access any other file
//or maybe we can just create every H-file for each sub-directory including all H-file under themself
//so that maybe no need to include every H-file needed in every C-file
#include "src/gui_sys_control/gui_scr_sys.h"

#include "src/gui_logic/GUI_self_define.h"
#include "src/gui_create/sigma_star_theme.h"

#include "universal_api_v7.h"

//Follow inlcude some misc exterior API simulator head files
//To avoid VS "undefined" error
#ifdef _VS_SIMULATOR
#include "ex_need/temp_misc.h"
#endif

#ifdef __cplusplus
}
#endif

#endif

