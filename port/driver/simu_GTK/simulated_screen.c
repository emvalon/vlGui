/**
 * @file simulated_screen.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-03-16
 *
 * Copyright © 2021 - 2022 Weilong Shen (valonshen@foxmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <gtk/gtk.h>
#include "vlGui.h"
#include "vlGui_input.h"

#define SIMU_SCREEN_WIDTH 128
#define SIMU_SCREEN_HEIGHT 64
#define SIMU_SCREEN_MULTIPLE 2

uint8_t closed = 0;

/* Global variables declaration */
GtkWidget *window;
GtkWidget *draware;
vlGui_color pixMap[SIMU_SCREEN_WIDTH][SIMU_SCREEN_HEIGHT];

void
vlGui_portDrawPixel(uint16_t x, uint16_t y, vlGui_color color)
{
    if ((x >= SIMU_SCREEN_WIDTH) || (y >= SIMU_SCREEN_HEIGHT)) {
        VLGUI_ASSERT(0);
    }
    pixMap[x][y] = color;
}

vlGui_color
vlGui_portGetPixelColor(uint16_t x, uint16_t y)
{
    if (pixMap[x][y]) {
        return VLGUI_COLOR_WHITE;
    } else {
        return VLGUI_COLOR_BLACK;
    }
}

void
vlGui_portRefresh(void)
{

    if (closed) {
        return;
    }

    gdk_threads_enter();
    gtk_widget_queue_draw(draware);
    gdk_threads_leave();
}

static void
closeWin(GtkWidget *window, gpointer data)
{
    VLGUI_UNUSED(window);
    VLGUI_UNUSED(data);

    closed = 1;
    gtk_main_quit();
}

static gboolean
refreshCb(GtkWidget *da, GdkEventExpose *event, gpointer data)
{
    uint16_t x, y, ax, ay;
    GdkGC *gc;
    GdkColor color;
    GdkPoint ps[4];

    VLGUI_UNUSED(event);
    VLGUI_UNUSED(data);
    gc = gdk_gc_new(da->window);
    ax = 0;

    for (x = 0; x < SIMU_SCREEN_WIDTH; x++) {
        ay = 0;

        for (y = 0; y < SIMU_SCREEN_HEIGHT; y++) {
            ps[0].x = ax;
            ps[0].y = ay;
            ps[1].x = ax + SIMU_SCREEN_MULTIPLE;
            ps[1].y = ay;

            ay += SIMU_SCREEN_MULTIPLE;

            ps[2].x = ax + SIMU_SCREEN_MULTIPLE;
            ps[2].y = ay;
            ps[3].x = ax;
            ps[3].y = ay;

            if (pixMap[x][y]) {
                color.red = 50000;
                color.green = 50000;
                color.blue = 50000;
            } else {
                color.red = 0;
                color.green = 0;
                color.blue = 0;
            }
            gdk_gc_set_rgb_fg_color(gc, &color);

            gdk_draw_polygon(da->window, gc, 1, ps, 4);
        }
        ax += SIMU_SCREEN_MULTIPLE;
    }

    return TRUE;
}

static gboolean
keyPressCb(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    VLGUI_UNUSED(widget);
    VLGUI_UNUSED(data);
    guint keyvalue = event->keyval;

    switch (keyvalue) {
    case 0xff1b:
        vlGui_inputEnqueueKey(VLGUI_KEY_ESC);
        break;
    case 0xff51:
        vlGui_inputEnqueueKey(VLGUI_KEY_LEFT);
        break;
    case 0xff52:
        vlGui_inputEnqueueKey(VLGUI_KEY_UP);
        break;
    case 0xff53:
        vlGui_inputEnqueueKey(VLGUI_KEY_RIGHT);
        break;
    case 0xff54:
        vlGui_inputEnqueueKey(VLGUI_KEY_DOWN);
        break;
    case 0xff0d:
        vlGui_inputEnqueueKey(VLGUI_KEY_OK);
        break;

    default:
        break;
    }

    return FALSE;
}

static void
guiMain(void)
{
    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();
}

void
vlGui_portInit(uint8_t display)
{
    VLGUI_UNUSED(display);
    gtk_init(NULL, NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "vlGui Simulater");

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), SIMU_SCREEN_WIDTH * SIMU_SCREEN_MULTIPLE,
                                SIMU_SCREEN_HEIGHT * SIMU_SCREEN_MULTIPLE);

    g_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(closeWin), NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(keyPressCb), NULL);

    draware = gtk_drawing_area_new();
    gtk_widget_set_size_request(draware, SIMU_SCREEN_WIDTH * SIMU_SCREEN_MULTIPLE,
                                SIMU_SCREEN_HEIGHT * SIMU_SCREEN_MULTIPLE);
    gtk_container_add(GTK_CONTAINER(window), draware);

    g_signal_connect(draware, "expose_event", G_CALLBACK(refreshCb), NULL);

    gtk_widget_show_all(window);

    /*创建线程*/
    g_thread_new("reflash", (GThreadFunc)guiMain, NULL);
}