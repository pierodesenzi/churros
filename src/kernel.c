#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "disk/disk.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"


uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

void terminal_set_marker(int x, int y){
    terminal_row = y;
    terminal_col = x;
}

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

void terminal_erase_height(int start, int size){
    video_mem = (uint16_t*)(0xb8000); //pointer to absolute address in memory
    terminal_row = 0;
    terminal_col = 0;
    for (int y = start; y < start + size; y++){
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

void printp(const char* str, char colour){
    size_t len = strlen(str);
    for (int i=0; i < len; i++){
        terminal_writechar(str[i], colour);
    }
}

void print(const char* str){
    printp(str, 15);
}

void sleep(int time){
    for(int i = 0; i<time; i++){
        
    }
}

void display_funky_intro(int times){
    terminal_initialize();
    terminal_set_marker(0, 13);
    printp("        ________________________________________________________________\n",6);
    printp("      .'                                                                '.\n",6);
    printp("     /      __________      ______________________________   ", 6); printp("      .--.   ",14);printp("\\ \n",6);
    printp("    /                                                        ", 6); printp("     /    \\ ",14);printp("  |\n",6);
    printp("   |     ___________________________________________________ ", 6); printp("    |      | ",14);printp(" |\n",6);
    printp("   |                                                         ", 6); printp("    |      | ",14);printp(" |\n",6);
    printp("    \\     ______________________                  __________", 6); printp("      \\    /",14);printp("  /\n",6);
    printp("     \\                                                      ", 6); printp("       '--'  ",14);printp("/\n",6);
    printp("      '.________________________________________________________________.\n", 6);
    printp("\n",6);
    printp("                                                            (c)P. Desenzi, 2023\n", 11);
    int c = 1;
    int colour = 1;
    while(c <= times){
        sleep(10000000);
        terminal_set_marker(0, 0);
        print("\n\n");
        printp("              |                                       #########   ##########\n",colour);
        printp("              |                                      ###########  ##########\n",colour);
        printp("              |                                      ##       ##  ##\n",colour);
        printp("      ____    |_____                                 ##       ##  #########\n",colour);
        printp("     /    \\   |     |   |      |   | ___    | ___    ##       ##  ##########\n",colour);
        printp("    |         |     |   |      |   |/   \\   |/   \\   ##       ##   #########\n",colour);
        printp("    |         |     |   |      |   |        |        ##       ##          ##\n",colour);
        printp("    |         |     |   |      |   |        |        ###########  ##########\n",colour);
        printp("     \\____/   |     |    \\____/    |        |         #########   ##########\n",colour);
        colour = (colour % 15) + 1;
        c++;
    }
    terminal_set_marker(0, 0);
}

static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main(){
    kheap_init(); // initialize heap
    terminal_initialize();
    disk_search_and_init();
    idt_init();
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    enable_paging();

    enable_interrupts();
    //display_funky_intro(10000);
    
}