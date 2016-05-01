#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include "Buffer.h"
#include "File.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdexcept>
#include <new>

class Editor
{
private:
    WINDOW *rightWindow;
    WINDOW *leftMenu;
    Buffer* buff;
    File* fileManager;
    string status;
    int x, y;
    int shiftDown;
    int shiftRight;
    bool info;
    char mode;
    bool win; 
    int positionMenu;
    int lengthMenu;
    string homePath;
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void deleteLine();
    void deleteLine(int);
    void saveFile();
    void openFile(char []);
    void checkVectorSize();
    void checkStringSize();
    static inline string & trim(string &);
public:
    int form;
    Editor(WINDOW *, WINDOW *);
    char getMode() {return mode;}
    void handleInput(int);
    void handle(int);
    void printMenu();
    void printBuff();
    void printStatusLine(WINDOW *);
    void updateStatus();
    void setWindow(bool);
    bool getWindow();
    void openFileSetting();
    void newFileSetting(bool = false);
    void saveFileSetting();
};

#endif
