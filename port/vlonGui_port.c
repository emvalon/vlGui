#include "vlonGui.h"
#include "vlonGui_port.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

struct vlonGui_driver_t vlonGui_driver;
uint8_t ms_per_tick;

void * vlonGui_malloc(uint32_t size)
{
    return malloc(size);
}

void vlonGui_free(void *addr)
{
    return free(addr);
}

uint32_t vlonGui_getTime(void)
{
    return times(NULL) * ms_per_tick;
}

// static void vlonGui_portDrawPixel(uint16_t x, uint16_t y, uint8_t color)
// {
//     ssd1306_DrawPixel(x, y, color);
// }

struct vlonGui_driver_t * vlonGui_portGetDriver(void)
{
    ms_per_tick = 1000 / sysconf(_SC_CLK_TCK);

    vlonGui_driver.pInit = vlonGui_portInit;
    vlonGui_driver.pDrawPoint = vlonGui_portDrawPixel;
    vlonGui_driver.pFresh = vlonGui_portRefresh;

    return &vlonGui_driver;
}