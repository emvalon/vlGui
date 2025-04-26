/**
 * @file vlGui_menu.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-04-22
 *
 * Copyright © 2021 - 2023 Weilong Shen (valonshen@foxmail.com)
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
#include <string.h>
#include "vlGui.h"
#include "stdio.h"
#include "vlGui_menu.h"
#include "vlGui_base.h"
#include "fonts/vlGui_fonts.h"
#include "vlGui_input.h"

#define VLGUI_MENU_ENTRY_PADDING (1)
#define VLGUI_MENU_ENTRY_MARGIN (8)
#define VLGUI_MENU_LINE_SPACING (1)
#define VLGUI_MENU_ENTRY_RETRACE (8)
#define VLGUI_MENU_CHECK_BOX_SIZE (8)
#define VLGUI_MENU_CHECK_BOX_MARGIN (25)

#define VLGUI_MENU_FLAGS_MOVE_CHILD (0x0001)
#define VLGUI_MENU_FLAGS_FOLD_CHILDREN (0x0002)
#define VLGUI_MENU_FLAGS_MOVE_CHILD_AUX (0x0004)
#define VLGUI_MENU_FLAGS_MOVE_CHECK_BOX (0x0008)

static int
vlGui_menuEntryHasChildren(vlGui_menuEntry_t *entry)
{
    vlGui_menuEntry_t *next;

    next = entry->next;
    if (next && (entry->level < next->level)) {
        return 1;
    } else {
        return 0;
    }
}

static void
vlGui_menuDrawLeadingLine(vlGui_menu_t *menu, vlGui_menuEntry_t *entry, int16_t lineHight,
                          int16_t origin)
{
    uint8_t childLevel;
    int16_t bottom;
    int16_t x, dy;
    int16_t actualBottom;
    vlGui_menuEntry_t *next;
    vlGui_window_t *win;

    if (lineHight == 0) {
        return;
    }

    win = &menu->win;
    if (entry) {
        next = entry->next;
        childLevel = entry->level + 1;
        x = (VLGUI_MENU_ENTRY_RETRACE * entry->level) + VLGUI_MENU_ENTRY_MARGIN +
            VLGUI_MENU_ENTRY_PADDING + 2;
    } else {
        next = menu->entries;
        childLevel = 0;
        x = VLGUI_MENU_ENTRY_PADDING + 2;
    }
    actualBottom = origin;
    bottom = origin + (lineHight >> 1);
    dy = lineHight;
    while (next) {
        if (next->level == childLevel) {
            actualBottom = bottom;
            vlGui_drawLine(win, x, bottom, x + 3, bottom, 1, VLGUI_COLOR_WHITE);
            if (next->folded && vlGui_menuEntryHasChildren(next)) {
                vlGui_drawLine(win, x, bottom - 2, x + 3, bottom - 2, 1, VLGUI_COLOR_WHITE);
                vlGui_drawLine(win, x, bottom - 4, x + 3, bottom - 4, 1, VLGUI_COLOR_WHITE);
            }
        } else if (next->level < childLevel) {
            break;
        }
        if (next->show) {
            bottom += dy;
        }

        if (menu->flags & VLGUI_MENU_FLAGS_MOVE_CHILD) {
            if ((next == menu->selEntry) && menu->selEntry->childrenNum) {
                dy = (lineHight * menu->childOffset) >> 7;
            } else if (next->level <= menu->selEntry->level) {
                dy = lineHight;
            }
        }
        next = next->next;
    }
    if (actualBottom != origin) {
        vlGui_drawLine(win, x, origin, x, actualBottom, 1, VLGUI_COLOR_WHITE);
    }
}

static void
vlGui_menuDrawBox(vlGui_menu_t *menu, int16_t y, vlGui_menuEntry_t *entry)
{
    uint8_t color;
    uint16_t x, s, boxWidth, offset;
    vlGui_window_t *win;

    win = &menu->win;
    x = win->win_width - VLGUI_MENU_CHECK_BOX_MARGIN;
    s = menu->font->fontHeight - (2 * VLGUI_MENU_LINE_SPACING);
    color = VLGUI_COLOR_WHITE;

    vlGui_drawRectangle(win, x, y + VLGUI_MENU_LINE_SPACING, s, s, VLGUI_COLOR_WHITE);
    if ((menu->flags & VLGUI_MENU_FLAGS_MOVE_CHECK_BOX) && entry->boxAnimation) {
        boxWidth = menu->boxRatio * (s - 4) >> 7;
        if (!entry->isChecked) {
            boxWidth = s - boxWidth - 4;
            color = VLGUI_COLOR_BLACK;
        }
    } else {
        if (entry->isChecked) {
            boxWidth = s - 4;
        } else {
            color = VLGUI_COLOR_BLACK;
            boxWidth = s - 4;
        }
    }

    if (boxWidth) {
        offset = (s - boxWidth) >> 1;
        vlGui_drawBlock(win, x + offset, y + VLGUI_MENU_LINE_SPACING + offset, boxWidth, boxWidth,
                        color);
    }
}

static void
vlGui_menuDrawRadioButton(vlGui_menu_t *menu, int16_t y, vlGui_menuEntry_t *entry)
{
    uint16_t x, r;
    vlGui_window_t *win;

    VLGUI_UNUSED(entry);
    win = &menu->win;
    r = (menu->font->fontHeight - (2 * VLGUI_MENU_LINE_SPACING)) >> 1;
    x = win->win_width - VLGUI_MENU_CHECK_BOX_MARGIN + r;
    // color = VLGUI_COLOR_WHITE;

    vlGui_drawCircle(win, x, y + VLGUI_MENU_LINE_SPACING + r, r);
    r -= 3;
    vlGui_drawFilledCircle(win, x, y + VLGUI_MENU_LINE_SPACING + r + 2, r, VLGUI_COLOR_WHITE);
    // if ((menu->flags & VLGUI_MENU_FLAGS_MOVE_CHECK_BOX) && entry->boxAnimation) {
    //     boxWidth = menu->boxRatio * (s - 4) >> 7;
    //     if (!entry->isChecked) {
    //         boxWidth = s - boxWidth - 4;
    //         color = VLGUI_COLOR_BLACK;
    //     }
    // } else {
    //     if (entry->isChecked) {
    //         boxWidth = s-4;
    //     } else {
    //         color = VLGUI_COLOR_BLACK;
    //         boxWidth = s - 4;
    //     }
    // }

    // if (boxWidth) {
    //     offset = (s - boxWidth) >> 1;
    //     vlGui_drawBlock(win, x + offset, y + VLGUI_MENU_LINE_SPACING + offset,
    //                     boxWidth, boxWidth, color);
    // }
}

static void
vlGui_menuDraw(vlGui_window_t *win, uint8_t flag)
{
    int16_t x, y, dy, ySelected;
    int16_t lineHight;
    int16_t rectOffset;
    vlGui_menu_t *menu;
    vlGui_menuEntry_t *entry;
    vlGui_menuEntry_t *entrySelected;
    const struct vlGui_font_t *font;

    VLGUI_UNUSED(flag);
    ySelected = 0;
    menu = (vlGui_menu_t *)win;
    entrySelected = menu->selEntry;

    vlGui_windowClear(win);
    /* Set the text font */
    font = menu->font;
    vlGui_setFont(font);
    /* Calculate the hight for each line */
    lineHight = menu->lineHight;
    dy = lineHight;
    rectOffset = menu->selectedRectOffset;

    vlGui_menuDrawLeadingLine(menu, NULL, lineHight, menu->menuOffset);

    entry = menu->entries;
    y = menu->menuOffset;
    for (entry = menu->entries; entry; entry = entry->next) {
        if (!entry->show) {
            continue;
        }
        /* Check if this entry is in the screen */
        if (y >= win->win_height) {
            break;
        }
        /* If this entry has just been folded or unfolded, move the child
         * entries via the selected animation.
         * Lower level means this isn't the child of selected entry.
         */
        if (menu->flags & VLGUI_MENU_FLAGS_MOVE_CHILD) {
            if (entrySelected == entry) {
                dy = (lineHight * menu->childOffset) >> 7;
            } else if (entry->level <= entrySelected->level) {
                y += lineHight - dy;
                dy = lineHight;
            }
        }
        /* If this entry is unfolded and not at the bottom of window, draw its
         * leading line according to its child entries.
         */
        if (!entry->folded && ((y + lineHight) <= win->win_height)) {
            vlGui_menuDrawLeadingLine(menu, entry, dy, y + lineHight);
        }
        /* If this entry is above the window, check the next one. */
        if ((y + lineHight) <= 0) {
            y += dy;
            continue;
        }
        if (entry->checkbox) {
            vlGui_menuDrawBox(menu, y, entry);
        } else if (entry->radioButton) {
            vlGui_menuDrawRadioButton(menu, y, entry);
        }
        /* Draw the entry's text. The selected entry will be drawn later. */
        if (entry == entrySelected) {
            ySelected = y;
        } else {
            vlGui_drawString(win,
                             VLGUI_MENU_ENTRY_MARGIN + VLGUI_MENU_ENTRY_PADDING +
                                 (VLGUI_MENU_ENTRY_RETRACE * entry->level),
                             y + VLGUI_MENU_LINE_SPACING, entry->str, VLGUI_COLOR_WHITE);
        }
        y += dy;
    }

    /* The selected entry will be shown above its child entries. */
    if (menu->flags & VLGUI_MENU_FLAGS_MOVE_CHILD) {
        vlGui_drawBlock(win,
                        VLGUI_MENU_ENTRY_MARGIN + VLGUI_MENU_ENTRY_PADDING +
                            (VLGUI_MENU_ENTRY_RETRACE * entrySelected->level),
                        ySelected + VLGUI_MENU_LINE_SPACING, win->win_width - 2, font->fontHeight,
                        VLGUI_COLOR_BLACK);
    }
    vlGui_drawString(win,
                     VLGUI_MENU_ENTRY_MARGIN + VLGUI_MENU_ENTRY_PADDING +
                         (VLGUI_MENU_ENTRY_RETRACE * entrySelected->level),
                     ySelected + VLGUI_MENU_LINE_SPACING, entrySelected->str, VLGUI_COLOR_WHITE);
    /* Draw the selected bar. We may cover the child entries' text. */
    x = VLGUI_MENU_ENTRY_MARGIN + menu->selectedRectRetract;
    vlGui_drawBlock(win, x, rectOffset,
                    win->win_width - 2 - (2 * VLGUI_MENU_ENTRY_MARGIN) - menu->selectedRectRetract,
                    font->fontHeight, VLGUI_COLOR_CONVERT);
    vlGui_drawPoint(win, x, rectOffset, VLGUI_COLOR_BLACK);
    vlGui_drawPoint(win, x, rectOffset + font->fontHeight - 1, VLGUI_COLOR_BLACK);
    vlGui_drawPoint(win, win->win_width - VLGUI_MENU_ENTRY_MARGIN - 3, rectOffset,
                    VLGUI_COLOR_BLACK);
    vlGui_drawPoint(win, win->win_width - VLGUI_MENU_ENTRY_MARGIN - 3,
                    rectOffset + font->fontHeight - 1, VLGUI_COLOR_BLACK);

    // if (entrySelected->checkbox) {
    //     vlGui_drawBlock(win, win->win_width - 12 - 8,
    //                     ySelected + VLGUI_MENU_LINE_SPACING, 8, 8,
    //                     VLGUI_COLOR_BLACK);
    //     vlGui_drawRectangle(win, win->win_width - 12 - 9,
    //                         ySelected + VLGUI_MENU_LINE_SPACING -1, 10, 10,
    //                         VLGUI_COLOR_WHITE);
    // }

    float offset;
    float unit;

    if ((lineHight * menu->num) <= win->win_height) {
        return;
    }
    unit = (float)win->win_height / (lineHight * menu->num) * win->win_height;
    offset = (float)menu->menuOffset / (lineHight * menu->num) * win->win_height;

    // vlGui_drawLine(win, win->win_width - 2, 0,
    //                  win->win_width - 2, win->win_height,
    //                  1, VLGUI_COLOR_WHITE);

    vlGui_drawRectangle(win, win->win_width - 3, -offset, 2, unit, VLGUI_COLOR_WHITE);
}

static void
vlGui_menuEngineMoveChildCb(vlGui_menu_t *menu, int16_t delta)
{
    uint8_t childLevel;
    vlGui_menuEntry_t *next;

    if (!vlGui_engineIsRunning(&menu->mainEngine)) {
        /* If the selected entry has just been set as folded, we will fold all
         * of its child entries after the engine timed out.
         */
        if (menu->flags & VLGUI_MENU_FLAGS_FOLD_CHILDREN) {
            menu->selEntry->folded = 1;
            childLevel = menu->selEntry->level + 1;
            next = menu->selEntry->next;
            while (next && (next->level >= childLevel)) {
                next->folded = 1;
                if (next->show) {
                    --menu->num;
                }
                next->show = 0;
                next = next->next;
            }
        }
        menu->flags &= ~(VLGUI_MENU_FLAGS_FOLD_CHILDREN | VLGUI_MENU_FLAGS_MOVE_CHILD);
    } else {
        if (menu->flags & VLGUI_MENU_FLAGS_FOLD_CHILDREN) {
            menu->childOffset -= delta;
        } else {
            menu->childOffset += delta;
        }
    }
}

static void
vlGui_menuEngineMoveSelectCb(vlGui_menu_t *menu, int16_t delta)
{
    vlGui_window_t *win;
    int16_t lineHight;
    int16_t rectBottom;
    int16_t offset;

    offset = menu->selectedRectOffset + delta;
    if (offset < 0) {
        menu->menuOffset -= offset;
        if (menu->menuOffset > 0) {
            menu->selectedRectOffset = -menu->menuOffset;
            menu->menuOffset = 0;
        } else {
            menu->selectedRectOffset = 0;
        }
    } else {
        win = &menu->win;
        lineHight = menu->font->fontHeight + (2 * VLGUI_MENU_LINE_SPACING);
        rectBottom = win->win_height - lineHight;

        if (offset > rectBottom) {
            menu->menuOffset -= (offset - rectBottom);
            menu->selectedRectOffset = rectBottom;
        } else {
            menu->selectedRectOffset = offset;
        }
    }
}

static void
vlGui_menuEngineCheckBoxCb(vlGui_menu_t *menu, int16_t delta)
{
    menu->boxRatio += delta;
    if (!vlGui_engineIsRunning(&menu->mainEngine)) {
        menu->flags &= ~VLGUI_MENU_FLAGS_MOVE_CHECK_BOX;
        menu->selEntry->boxAnimation = 0;
    }
}

static void
vlGui_menuMainEngineCb(void *param, int16_t delta)
{
    vlGui_menu_t *menu;

    menu = (vlGui_menu_t *)param;
    if (menu->flags & VLGUI_MENU_FLAGS_MOVE_CHILD) {
        vlGui_menuEngineMoveChildCb(menu, delta);
    } else if (menu->flags & VLGUI_MENU_FLAGS_MOVE_CHECK_BOX) {
        vlGui_menuEngineCheckBoxCb(menu, delta);
    } else {
        vlGui_menuEngineMoveSelectCb(menu, delta);
    }
}

static void
vlGui_menuAuxEngineCb(void *param, int16_t delta)
{
    vlGui_menu_t *menu;

    menu = (vlGui_menu_t *)param;

    if (menu->flags & VLGUI_MENU_FLAGS_MOVE_CHILD_AUX) {
        menu->menuOffset += delta;
        menu->selectedRectOffset += delta;
        if (!vlGui_engineIsRunning(&menu->auxEngine)) {
            menu->flags &= ~VLGUI_MENU_FLAGS_MOVE_CHILD_AUX;
        }
    } else {
        menu->selectedRectRetract += delta;
    }
}

static int
vlGui_menuProcessKeyMoveSelect(vlGui_menu_t *menu, bool moveForward)
{
    uint8_t prevLevel;
    int8_t lineHeigh;
    int16_t selectBarDelta;
    vlGui_menuEntry_t *sel;

    sel = menu->selEntry;
    prevLevel = sel->level;
    while (1) {
        if (moveForward) {
            sel = sel->next;
        } else {
            sel = sel->prev;
        }
        if (!sel) {
            return -1;
        } else if (sel->show) {
            vlGui_engineFinish(&menu->mainEngine);
            vlGui_engineFinish(&menu->auxEngine);
            menu->selEntry = sel;
            lineHeigh = menu->font->fontHeight + (2 * VLGUI_MENU_LINE_SPACING);
            if (!moveForward) {
                lineHeigh = -lineHeigh;
            }
            vlGui_engineStart(&menu->mainEngine, lineHeigh, 600);
            selectBarDelta = sel->level - prevLevel;
            if (selectBarDelta) {
                selectBarDelta *= VLGUI_MENU_ENTRY_RETRACE;
                vlGui_engineStart(&menu->auxEngine, selectBarDelta, 600);
            }
            return 0;
        }
    }
}

static int
vlGui_menuProcessKeyFold(vlGui_menu_t *menu)
{
    bool fold;
    bool refresh;
    uint8_t childLevel;
    int16_t ref;
    int16_t adjust, winHight;
    vlGui_menuEntry_t *sel;
    vlGui_menuEntry_t *next;

    sel = menu->selEntry;
    childLevel = sel->level + 1;
    fold = false;
    refresh = false;
    next = sel->next;

    if (sel->folded) {
        sel->folded = 0;
        for (uint8_t i = 0; i < sel->childrenNum; next = next->next) {
            if (next->level == childLevel) {
                next->show = 1;
                ++menu->num;
                ++i;
                refresh = true;
            }
        }
    } else if (next && (next->level >= childLevel)) {
        /* The selected entry has child entries. Set the animation engine
         * to fold this entry. The engine callback will complete the last
         * operation. We don't modify its child entries here.
         */
        fold = true;
        refresh = true;
    }
    /* If the child entry needs to be shown, set a flag to indicate moving
     * the child entry, not the select bar.
     */
    if (refresh) {
        vlGui_engineFinish(&menu->mainEngine);
        vlGui_engineFinish(&menu->auxEngine);
        winHight = menu->win.win_height;
        if (fold) {
            menu->flags |= VLGUI_MENU_FLAGS_FOLD_CHILDREN | VLGUI_MENU_FLAGS_MOVE_CHILD;
            menu->childOffset = 1 << 7;
            /* We don't want to display blank at the bottom of window, unless
             * there are no enough entries to full it. Scroll the menu to
             * implement it. Similary, we shouldn't display blank at the top
             * of window.
             * 'ref' is the last entry's bottom. We will check if it is larger
             * than window's hight.
             */
            ref = menu->selectedRectOffset + menu->lineHight;
            while (next) {
                if (next->show && (next->level < childLevel)) {
                    ref += menu->lineHight;
                }
                next = next->next;
                if (ref > winHight) {
                    break;
                }
            }
            if (ref < winHight) {
                adjust = winHight - ref;
                if ((adjust + menu->menuOffset) > 0) {
                    adjust = -menu->menuOffset;
                }
                if (adjust) {
                    menu->flags |= VLGUI_MENU_FLAGS_MOVE_CHILD_AUX;
                    vlGui_engineStart(&menu->auxEngine, adjust, 300);
                }
            }

        } else {
            menu->flags |= VLGUI_MENU_FLAGS_MOVE_CHILD;
            menu->childOffset = 0;
            /* We will try to display all of children. Therefore, we need to
             * scroll the menu. If the selected entry is at the top of window,
             * will stop scrolling.
             */
            ref = winHight - ((sel->childrenNum + 1) * menu->lineHight);
            if (ref < 0) {
                ref = 0;
            }
            if (ref < menu->selectedRectOffset) {
                menu->flags |= VLGUI_MENU_FLAGS_MOVE_CHILD_AUX;
                adjust = ref - menu->selectedRectOffset;
                vlGui_engineStart(&menu->auxEngine, adjust, 300);
            }
        }
        vlGui_engineStart(&menu->mainEngine, 1 << 7, 300);
        return 0;
    } else {
        return -1;
    }
}

static int
vlGui_menuProcessKeyCheck(vlGui_menu_t *menu)
{
    bool isChecked;
    // uint8_t level;
    vlGui_menuEntry_t *sel;
    // vlGui_menuEntry_t *entry;

    sel = menu->selEntry;
    // level = sel->level;
    vlGui_engineFinish(&menu->mainEngine);
    vlGui_engineFinish(&menu->auxEngine);

    isChecked = !sel->isChecked;
    sel->isChecked = isChecked;
    sel->boxAnimation = 1;
    // entry = sel->next;
    // while (entry && (entry->level == level)) {
    //     /* Check if there are other checkboxes that has the same state. We need
    //      * to change it. Because
    //      */
    //     if (entry->checkbox && (entry->isChecked == isChecked)) {
    //         entry->isChecked = !isChecked;
    //         entry->boxAnimation = 1;
    //     }
    //     entry = entry->next;
    // }

    menu->boxRatio = 0;
    menu->flags |= VLGUI_MENU_FLAGS_MOVE_CHECK_BOX;
    vlGui_engineStart(&menu->mainEngine, (1 << 7), 300);
    return 0;
}

int
vlGui_menuProcessKey(vlGui_window_t *win, uint8_t key)
{
    int ret;
    vlGui_menu_t *menu;

    menu = (vlGui_menu_t *)win;
    switch (key) {
    case VLGUI_KEY_UP:
    case VLGUI_KEY_LEFT:
        ret = vlGui_menuProcessKeyMoveSelect(menu, false);
        break;
    case VLGUI_KEY_DOWN:
    case VLGUI_KEY_RIGHT:
        ret = vlGui_menuProcessKeyMoveSelect(menu, true);
        break;
    case VLGUI_KEY_OK:
        if (menu->selEntry->checkbox) {
            ret = vlGui_menuProcessKeyCheck(menu);
        } else {
            ret = vlGui_menuProcessKeyFold(menu);
        }
        break;
    default:
        return 0;
    }

    if (!ret) {
        vlGui_windowSetRefresh(win);
    }
    return 0;
}

vlGui_menu_t *
vlGui_menuCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width, uint16_t height)
{
    vlGui_menu_t *menu;
    VLGUI_ASSERT(parent);

    menu = (vlGui_menu_t *)vlGui_windowCreate(parent, x, y, width, height,
                                              sizeof(*menu) - sizeof(*parent));
    if (!menu) {
        return menu;
    }

    menu->entries = NULL;
    menu->selEntry = NULL;
    menu->num = 0;
    menu->font = &vlGui_font7x10;
    menu->win.type = VLGUI_WIN_TYPE_MENU;
    menu->win.pDrawWindow = vlGui_menuDraw;
    menu->win.pProcessKey = vlGui_menuProcessKey;
    menu->menuOffset = 0;
    menu->flags = 0;
    menu->selectedRectOffset = 0;
    menu->selectedRectRetract = 0;
    menu->lineHight = 10 + (2 * VLGUI_MENU_LINE_SPACING);

    vlGui_engineInit(&menu->mainEngine, vlGui_engineCurveOutQuint, vlGui_menuMainEngineCb, menu);
    vlGui_engineInit(&menu->auxEngine, vlGui_engineCurveOutQuint, vlGui_menuAuxEngineCb, menu);
    menu->win.engines = &menu->mainEngine;
    vlGui_engineAppend(&menu->mainEngine, &menu->auxEngine);

    return menu;
}

void
vlGui_menuAddEntry(vlGui_menu_t *menu, vlGui_menuEntry_t *parent, vlGui_menuEntry_t *new,
                   char *text)
{
    int8_t level;
    vlGui_menuEntry_t *prev;

    memset(new, 0, sizeof(vlGui_menuEntry_t));
    if (parent) {
        level = parent->level;
        prev = parent;
        ++parent->childrenNum;
        new->show = 0;
    } else {
        level = -1;
        prev = menu->entries;
        new->show = 1;
    }

    if (!prev) {
        menu->entries = new;
        menu->selEntry = new;
    } else {
        while (prev->next && (prev->next->level > level)) {
            prev = prev->next;
        }
    }

    new->prev = prev;
    if (prev) {
        new->next = prev->next;
        prev->next = new;
    }

    new->level = level + 1;
    new->childrenNum = 0;
    new->boxAnimation = 0;
    new->str = text;
    new->folded = 1;
}

void
vlGui_menuSetEntryAsCheckbox(vlGui_menuEntry_t *entry)
{
    entry->radioButton = 0;
    entry->checkbox = 1;
    entry->isChecked = 0;
}

void
vlGui_menuSetEntryAsRadioButton(vlGui_menuEntry_t *entry)
{
    entry->radioButton = 1;
    entry->checkbox = 0;
    entry->isChecked = 0;
}

void
vlGui_menuSetFont(vlGui_menu_t *menu, const struct vlGui_font_t *font)
{
    menu->font = font;
    /* Font is changed. Reset menu to initial state. */
    menu->selEntry = menu->entries;
    // menu->childOffset = 0;
    // menu->selectedRectOffset = 0;
    menu->lineHight = font->fontHeight + (2 * VLGUI_MENU_LINE_SPACING);
}

char *
vlGui_menuGetSelectedEntry(vlGui_menu_t *menu)
{
    vlGui_menuEntry_t *sel;

    sel = (vlGui_menuEntry_t *)menu->selEntry;
    if (sel) {
        return sel->str;
    }

    return NULL;
}