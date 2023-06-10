#ifndef _MAIN_H
#define _MAIN_H

#define MY_COLOR_1 CLITERAL(Color){ 0x16, 0x18, 0x1f, 0xFF }
#define MY_COLOR_2 CLITERAL(Color){ 0xF9, 0xB9, 0xAB, 0xFF }
#define MY_COLOR_3 CLITERAL(Color){ 0x4C, 0x7C, 0x94, 0xFF }
#define MY_COLOR_4 CLITERAL(Color){ 0xEC, 0x84, 0x94, 0xFF }
#define MY_COLOR_5 CLITERAL(Color){ 0xC4, 0xDC, 0xE6, 0xFF }
#define MY_COLOR_6 CLITERAL(Color){ 0x67, 0x3D, 0x2C, 0xFF }
//#define MY_COLOR_6 CLITERAL(Color){ 0x39, 0x27, 0x30, 0xFF }

typedef struct {
    unsigned int screen_width;
    unsigned int screen_height;
    unsigned int gui_scale;
} GlobalState;

#endif // !_MAIN_H
