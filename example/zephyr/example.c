#include "vlGui.h"
#include "vlGui_msgBox.h"
#include "vlGui_window.h"
#include "vlGui_input.h"
#include "vlGui_button.h"
#include "vlGui_selector.h"
#include "bitmap.h"

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include "devicetree.h"

struct vlGui_t oled;
struct vlGui_msgBox_t *msgbox;
struct vlGui_window_t *win, *topWin;
struct vlGui_button_t *btn;
struct vlGui_selector_t *sel;

static bool showIconName = true;

volatile int8_t pos = 0;

static void mainWindowDrawCb(struct vlGui_window_t *win, void *arg)
{
    int16_t x;

    vlGui_windowClear(win);
    x = 12;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_game);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_setting);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_connect);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_info);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_media);
}

static void topWindowDrawCb(struct vlGui_window_t *win, void *arg)
{
    uint8_t w,h;
    int16_t x;
    char *str;

    w = 10;
    h = 5;

    x = (128 - 5 * w) >> 1;

    for(uint8_t i = 0; i < 5; i++) {
        if(i == pos) {  
            vlGui_drawBlock(win, x + i * w, 64 - h, w, h, 1);
        } else {
            vlGui_drawRectangle(win, x + i * w, 64 - h, w, h, 1);
        }
    }

    vlGui_drawLine(win, 63, 4, 63, 44, 2, 1);

    if(!showIconName) {
        return;
    }

    vlGui_setFont(&vlGui_font11x18);
    switch (pos)
    {
    case 0:
        str = "Game";
        break;
    case 1:
        str = "Set";
        break;
    case 2:
        str = "Conn";
        break;
    case 3:
        str = "Info";
        break;
    case 4:
        str = "Media";
        break;
    default:
        break;
    }

    x = 96 - ((strlen(str) * vlGui_font11x18.fontWidth) >> 1);

    vlGui_drawString(win, x, 20, str, 1);
}

static void drawIconName(void)
{
   showIconName = true;
}

int mainWindowProcessKeyCb(struct vlGui_window_t *win, uint8_t key)
{
    switch (key)
    {
    case VLGUI_KEY_LETF:
        if(pos) {
            --pos;
            showIconName = false;
            vlGui_windowScrollAnimation(win, 128, 0, 500, drawIconName);
        }
        break;
    case VLGUI_KEY_RIGHT:
        if(pos < 4) {
            ++pos;
            showIconName = false;
            vlGui_windowScrollAnimation(win, -128, 0, 500, drawIconName);
        }
        break;
    case VLGUI_KEY_OK:
        switch (pos)
        {
        case 0: 
            msgbox = vlGui_msgBoxCreate(win);
            break;
        case 1:
            btn = vlGui_buttonCreate(win, 40, 20, 30, 16);
            break;
        case 2:
            sel = vlGui_selectorCreate(win);
            vlGui_selectorAddEntry(sel, "Shen Weilong");
            vlGui_selectorAddEntry(sel, "Valon Shen");
            vlGui_selectorAddEntry(sel, "Ma Suhong");
            vlGui_selectorAddEntry(sel, "Suhon Ma");
            vlGui_selectorAddEntry(sel, "VlonGui");
            break;
        default:
            break;
        }
    default:
        break;
    }

    return 0;
}

void vlGui_example(void)
{
    memset(&oled, 0, sizeof(oled));

    vlGui_screen_init(&oled, 128, 64);
    vlGui_register_driver(&oled, vlGui_portGetDriver());

    win = vlinGui_getMainWindow(&oled);
    vlGui_windowSetDrawCb(win, mainWindowDrawCb);
    vlGui_windowSetKeyCb(win, mainWindowProcessKeyCb);

    topWin = vlGui_windowCreate(win, 0, 0, win->win_width, win->win_height, 0);
    vlGui_windowSetDrawCb(topWin, topWindowDrawCb);

    while (1)
    {
        vlGui_refresh();
        k_msleep(10);
    }
    
}

#define LEFT_KEY        DT_NODELABEL(leftkey)
#define RIGHT_KEY       DT_NODELABEL(rightkey)
#define UP_KEY          DT_NODELABEL(upkey)
#define DOWN_KEY        DT_NODELABEL(downkey)
#define OK_KEY          DT_NODELABEL(okkey)
#define CANCEL_KEY      DT_NODELABEL(cancelkey)


struct KeyConfig_t {
    uint8_t key;
    gpio_pin_t pin;
    gpio_flags_t flags;
    char *gpio_name;
};

struct KeyState_t
{
    uint8_t state;
    uint8_t cnt;
    struct device *gpioDevice;
};


static const struct KeyConfig_t keyConfig[6] = {
    {
        VLGUI_KEY_LETF,
        DT_GPIO_PIN(LEFT_KEY, gpios),
        DT_GPIO_FLAGS(LEFT_KEY, gpios),
        DT_GPIO_LABEL(LEFT_KEY, gpios),
    },
    {
        VLGUI_KEY_RIGHT,
        DT_GPIO_PIN(RIGHT_KEY, gpios),
        DT_GPIO_FLAGS(RIGHT_KEY, gpios),
        DT_GPIO_LABEL(RIGHT_KEY, gpios),
    },
    {
        VLGUI_KEY_UP,
        DT_GPIO_PIN(UP_KEY, gpios),
        DT_GPIO_FLAGS(UP_KEY, gpios),
        DT_GPIO_LABEL(UP_KEY, gpios),
    },
    {
        VLGUI_KEY_DOWN,
        DT_GPIO_PIN(DOWN_KEY, gpios),
        DT_GPIO_FLAGS(DOWN_KEY, gpios),
        DT_GPIO_LABEL(DOWN_KEY, gpios),
    },
    {
        VLGUI_KEY_OK,
        DT_GPIO_PIN(OK_KEY, gpios),
        DT_GPIO_FLAGS(OK_KEY, gpios),
        DT_GPIO_LABEL(OK_KEY, gpios),
    },
    {
        VLGUI_KEY_CANCEL,
        DT_GPIO_PIN(CANCEL_KEY, gpios),
        DT_GPIO_FLAGS(CANCEL_KEY, gpios),
        DT_GPIO_LABEL(CANCEL_KEY, gpios),
    },
};

static struct KeyState_t keyState[sizeof(keyConfig) / sizeof(keyConfig[0])];

static void keyTask(void)
{
    uint8_t numKey;
    int ret;

    numKey = sizeof(keyConfig) / sizeof(keyConfig[0]);

    for(uint8_t i = 0; i < numKey; i++) {
        keyState[i].gpioDevice = device_get_binding(keyConfig[i].gpio_name);
        if (keyState[i].gpioDevice == NULL) {
            printk("Error: didn't find device,%s\n",keyConfig[i].gpio_name);
            return;
	    }

        ret = gpio_pin_configure(keyState[i].gpioDevice, keyConfig[i].pin, GPIO_INPUT | GPIO_PULL_UP);
        if (ret != 0) {
            printk("Error: failed to configure pin,%d\n", keyConfig[i].pin);
            return;
        }

        keyState[i].state = 1;
    }

    while(1) {
        for(uint8_t i = 0; i < numKey; i++) {
            ret = gpio_pin_get(keyState[i].gpioDevice, keyConfig[i].pin);
            if(ret == keyState[i].state) {
                keyState[i].cnt = 5;
            } else if(keyState[i].cnt) {
                --keyState[i].cnt;
            }

            if(keyState[i].cnt == 0) {
                keyState[i].state = ret;
                if(ret) {
                    vlGui_inputEnqueueKey(keyConfig[i].key);
                }
            }
            
        }
        k_msleep(10);
    }
}

K_THREAD_DEFINE(keyTask_id, 2000, keyTask, NULL, NULL, NULL,
		10, 0, 0);