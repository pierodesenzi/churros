#include "kernel.h"
#include <stddef.h>
#include <stdint.h>


uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t resolve_character(char c, char colour){
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour){
    video_mem[(y * VGA_WIDTH) + x] = resolve_character(c, colour);
}

void terminal_writechar(char c, char colour){
    if (c == '\n'){
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH){
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_initialize(){
    video_mem = (uint16_t*)(0xb8000); //pointer to absolute address in memory
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++){
        for (int x = 0; x < VGA_WIDTH; x++){
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

size_t strlen(const char* str){
    size_t len = 0;
    while (str[len]){
        len++;
    }
    return len;
    
}

void print(const char* str){
    size_t len = strlen(str);
    for (int i=0; i < len; i++){
        terminal_writechar(str[i], 15);
    }
}

void printp(const char* str, char colour){
    size_t len = strlen(str);
    for (int i=0; i < len; i++){
        terminal_writechar(str[i], colour);
    }
}

void kernel_main(){
    terminal_initialize();
    //printp("hello", 6);
    print("\n\n");
    printp("              |                                       #########   ##########\n",13);
    printp("              |                                      ###########  ##########\n",13);
    printp("              |                                      ##       ##  ##\n",13);
    printp("      ____    |_____                                 ##       ##  #########\n",13);
    printp("     /    \\   |     |   |      |   | ___    | ___    ##       ##  ##########\n",13);
    printp("    /         |     |   |      |   |/   \\   |/   \\   ##       ##   #########\n",13);
    printp("    |         |     |   |      |   |        |        ##       ##          ##\n",13);
    printp("    \\         |     |   |      |   |        |        ###########  ##########\n",13);
    printp("     \\____/   |     |    \\____/    |        |         #########   ##########\n",13);
    printp("\n",6);
    printp("          ______________________________________________________________\n",6);
    printp("      .'                                                                '.\n",6);
    printp("     /    ____________      ______________________________   ", 6); printp("      .--.   ",14);printp("\\ \n",6);
    printp("    /                                                        ", 6); printp("     /    \\ ",14);printp("  |\n",6);
    printp("   |        ________________________________________________ ", 6); printp("    |      | ",14);printp(" |\n",6);
    printp("   |                                                         ", 6); printp("    |      | ",14);printp(" |\n",6);
    printp("    \\    _______________________                  __________", 6); printp("      \\    /",14);printp("  /\n",6);
    printp("     \\                                                      ", 6); printp("       '--'  ",14);printp("/\n",6);
    printp("      '.________________________________________________________________.\n", 6);
    printp("\n",6);
    printp("                                                            (c)P. Desenzi, 2023\n", 11);
}