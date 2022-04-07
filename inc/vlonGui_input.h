#ifndef _VLONGUI_INPUT_H_
#define _VLONGUI_INPUT_H_

#include <stdint.h>



#define VLGUI_KEY_ESC               (0xf0)

#define VLGUI_KEY_OK                (0xf9)
#define VLGUI_KEY_CANCEL            (0xfa)
#define VLGUI_KEY_LETF              (0xfb)
#define VLGUI_KEY_RIGHT             (0xfc)
#define VLGUI_KEY_UP                (0xfd)
#define VLGUI_KEY_DOWN              (0xfe)
#define VLGUI_KEY_NONE              (0xff)


void vlonGui_inputEnqueueKey(uint8_t key);

uint8_t vlonGui_inputGetKey(void);

void vlonGui_inputInit(void);

#endif