#ifndef _VLONGUI_PORT_H_
#define _VLONGUI_PORT_H_

#include "vlonGui.h"

void vlonGui_portInit(void);

void vlonGui_portDrawPixel(uint16_t x, uint16_t y, vlonGui_color color);


void vlonGui_portRefresh(void);

#endif 