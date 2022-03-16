#include "vlonGui.h"
#include <zephyr.h>
#include "vlonGui_ssd1306.h"


struct vlonGui_driver_t vlonGui_driver;


void * vlonGui_malloc(uint32_t size)
{
    return k_malloc(size);
}

uint32_t vlonGui_getTime(void)
{
    return k_uptime_get_32();
}

static void vlonGui_portDrawPixel(uint16_t x, uint16_t y, uint8_t color)
{
    ssd1306_DrawPixel(x, y, color);
}


struct vlonGui_driver_t * vlonGui_portGetDriver(void)
{
    vlonGui_driver.pInit = ssd1306_Init;
    vlonGui_driver.pDrawPoint = vlonGui_portDrawPixel;
    vlonGui_driver.pFresh = ssd1306_UpdateScreen;

    return &vlonGui_driver;
}