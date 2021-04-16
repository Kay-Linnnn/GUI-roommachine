#ifndef _TEMP_MISC_H
#define _TEMP_MISC_H


#ifdef __cplusplus
extern "C" {
#endif

#include "gui_app.h"

	void ss20x_aodio_ao_setVolume(int vulume);

	int lthmedia_getlocalip(const char * eth_name, char *local_ip_addr);

	//image_module.h
	void image_dispaly(const char *img_path, int coo_x, int coo_y, int width, int height);
	void image_display_stop(void);

#ifdef __cplusplus
}
#endif


#endif