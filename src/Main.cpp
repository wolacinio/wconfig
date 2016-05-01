#include <ncurses.h>
#include <string>

#include "Editor.h"

using namespace std;


void curses_init()
{
    initscr();
    noecho();
    cbreak();
    
    //curs_set(FALSE);
    //start_color();

    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    //init_color(COLOR_BLUE, 330, 500, 1000);

    attron(COLOR_PAIR(1));
    //bkgd(COLOR_PAIR(2));
}


int main(int argc, char* argv[])
{
    // Editor ed;
    // string fn = "";
    // if(argc > 1)
    // {
    //     fn = argv[1];
    //     ed = Editor(fn);
    // }
    // else
    // {
    //     ed = Editor();
    // }

    //  curses_init();

    // while(ed.getMode() != 'q')
    // {
    //     ed.updateStatus();
    //     ed.printStatusLine();
    //     ed.printBuff();
    //     int input = getch();
    //     ed.handleInput(input);
    //     refresh();
    // }

//////////////////////////////////////////////////////
    curses_init();
  
    int input;
    int parent_x, parent_y, new_x, new_y;

    WINDOW *field = newwin(LINES-3, 22, 0, 0);
    WINDOW *score = newwin(LINES-3, COLS-22, 0, 22);
    WINDOW *bottom = newwin(3, COLS, LINES-3, 0);

    keypad(score, true);
    //keypad(field, true);
    Editor ed = Editor(score, field);
    
   
    wtimeout(field, 20);
    wtimeout(score, 20);

    box(field, 0 , 0);
    box(score, 0 , 0);
    box(bottom, 0 , 0);

    parent_x = LINES;
    parent_y = COLS;
      int c;      
  while(ed.getMode() != 'q') {
    if(parent_y != COLS || parent_x != LINES){
        parent_x = LINES;
        parent_y = COLS;
        
        wresize(field, LINES-3, 22);
        wresize(score, LINES-3, COLS-22);
        wresize(bottom, 3, COLS);
        mvwin(bottom, LINES-3, 0);
        
        wclear(field);
        wclear(score);
        wclear(bottom);
        wclear(stdscr);
       
    }

    ed.updateStatus();
    ed.printStatusLine(bottom);

    parent_x = LINES;
    parent_y = COLS;
    ed.printMenu();
    ed.printBuff();
    LINES = parent_x;
    COLS = parent_y;

    box(field, 0 , 0);
    box(score, 0 , 0);
    box(bottom, 0 , 0);

    if(ed.getWindow()){
        keypad(field, false);
        input = wgetch(score);     
        ed.handleInput(input);
        curs_set(true);
    }else{
        keypad(field, true);
        curs_set(false);     
        input = wgetch(field);
        ed.handle(input);      
    }

    wrefresh(field);
    wrefresh(score);
    wrefresh(bottom);

    }


    delwin(field);
    delwin(score);
    delwin(bottom);
    
    endwin();
    return 0;
}