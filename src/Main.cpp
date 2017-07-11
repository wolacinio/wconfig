#include <ncurses.h>
#include <string>
#include "Editor.h"

using namespace std;

void curses_init()
{
    initscr();
    noecho();
    cbreak();
    //start_color();
    //init_pair(1, COLOR_BLUE,COLOR_BLACK);
    //init_pair(2, COLOR_WHITE,COLOR_BLACK);
    //init_pair(3, COLOR_WHITE,COLOR_BLACK);
}

int main(int argc, char* argv[])
{
    curses_init();
  
    int input;
    int parent_x, parent_y, c;

    WINDOW *leftWindow = newwin(LINES-3, 22, 0, 0);
    WINDOW *rightWindow = newwin(LINES-3, COLS-22, 0, 22);
    WINDOW *bottomWindow = newwin(3, COLS, LINES-3, 0);

    wattron(leftWindow, A_BOLD|COLOR_PAIR(1));
    wattron(rightWindow, COLOR_PAIR(2));
    wattron(bottomWindow, COLOR_PAIR(2));

    keypad(rightWindow, true);

    Editor ed = Editor(rightWindow, leftWindow);
   
    wtimeout(leftWindow, 20);
    wtimeout(rightWindow, 20);

    box(leftWindow, 0 , 0);
    box(rightWindow, 0 , 0);
    box(bottomWindow, 0 , 0);

    parent_x = LINES;
    parent_y = COLS;

    while(ed.getMode() != 'q') {
        if(parent_y != COLS || parent_x != LINES){
            parent_x = LINES;
            parent_y = COLS;
            
            wresize(leftWindow, LINES-3, 22);
            wresize(rightWindow, LINES-3, COLS-22);
            wresize(bottomWindow, 3, COLS);
            mvwin(bottomWindow, LINES-3, 0);
            
            wclear(leftWindow);
            wclear(rightWindow);
            wclear(bottomWindow);
            wclear(stdscr);          
        }

        ed.updateStatus();
        ed.printStatusLine(bottomWindow);

        parent_x = LINES;
        parent_y = COLS;
        ed.printMenu();
        ed.printBuff();
        LINES = parent_x;
        COLS = parent_y;

        box(leftWindow, 0 , 0);
        box(rightWindow, 0 , 0);
        box(bottomWindow, 0 , 0);

        // mvwprintw(leftWindow, 0, 8, "Pliki:");
        // wmove(leftWindow, ed.getY(), ed.getX());
        // mvwprintw(rightWindow, 0, (COLS-5)/2, "Edytor:");
        // wmove(rightWindow, ed.getY(), ed.getX());
        // mvwprintw(bottomWindow, 0, COLS/2+5, "Komunikaty:");
        // wmove(bottomWindow, ed.getY(), ed.getX());

        if(ed.getWindow()){
            wattron(rightWindow, A_BOLD|COLOR_PAIR(1));
            wattron(leftWindow, COLOR_PAIR(2));
            keypad(leftWindow, false);
            input = wgetch(rightWindow);     
            ed.handleInput(input);
            curs_set(true);
        }else{
            wattron(leftWindow, A_BOLD|COLOR_PAIR(1));
            wattron(rightWindow, COLOR_PAIR(2));
            keypad(leftWindow, true);
            curs_set(false);     
            input = wgetch(leftWindow);
            ed.handle(input);      
        }
        
        wrefresh(leftWindow);
        wrefresh(rightWindow);
        wrefresh(bottomWindow);

    }

    delwin(leftWindow);
    delwin(rightWindow);
    delwin(bottomWindow);

    endwin();
    return 0;
}