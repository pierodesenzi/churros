#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define MAX_PATH 108

void kernel_main();
void print(const char* str);
void printp(const char* str, char colour);
void terminal_set_marker(int x, int y);

#endif
