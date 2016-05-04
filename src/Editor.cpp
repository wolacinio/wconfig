#include "Editor.h"

using namespace std;

Editor::Editor(WINDOW * p, WINDOW * p1)
{
    form = false;
    rightWindow = p;
    leftMenu = p1;
    x=1;
    y=1;
    mode='n';
    homePath = getenv("HOME");
    homePath += "/wconfig/menu.conf";
    positionMenu = 1;
    lengthMenu = 0;

    info = false;
    win = false;

    shiftDown = 0;
    shiftRight = 0;
    status = "Tryb normalny";

    buff = new Buffer();
    buff->appendLine("");
    fileManager = new File();

    openFileSetting();
}

void Editor::updateStatus()
{
    getmaxyx(rightWindow, LINES, COLS);
    switch(mode)
    {
    case 'n':
        status = "Tryb normalny";
        break;
    case 'e':
        status = "Tryb edycji";
        break;
    case 'q':
        status = "Koniec";
        break;
    }

    char buffer[30];
    if(!info)
        sprintf(buffer, "   Kolumna: %d Wiersz: %d",(x+shiftRight),(y+shiftDown));
    status += buffer;
}

void Editor::handleInput(int c)
{
    switch(mode){
        case 'x':
            if((c >= 32 && c <= 126) || c == KEY_LEFT || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN){
                mode = 'e';
                info = false;
            }
        break;
    case 'n':
        switch(c){
        case 'q':
            mode = 'q';
            break;
        case 'e':
            mode = 'e';
            break;
        case 's':
            saveFile();
            break;
        case 9:
            win = !win;
        break;
        }
        break;
    case 'e':
        switch(c){
            case KEY_LEFT:
                moveLeft();
                return;
            case KEY_RIGHT:
                moveRight();
                return;
            case KEY_UP:
                moveUp();
                return;
            case KEY_DOWN:
                moveDown();
                return;
            case 27:
                // Escape
                mode = 'n';
                break;
            case KEY_BACKSPACE:
                if((x+shiftRight) == 1 && (y+shiftDown) > 1){
                    int t1 = buff->lines[y-2+shiftDown].length() + buff->lines[y+shiftDown-1].length();
                    int t2 = buff->lines[y-2+shiftDown].length();
                    buff->lines[y-2+shiftDown] += buff->lines[y+shiftDown-1];
                    moveUp();
                    while(x+shiftRight-1 < t1)
                        moveRight();    
                    if(shiftRight>0)
                        x = COLS-2;
                    else
                        x = t2+1;
                    deleteLine(y+shiftDown);
                }
                else if((x+shiftRight) == 1 && (y+shiftDown) == 1)
                    break;
                else if((x+shiftRight) >= 1 && (y+shiftDown) >= 1){
                    if(x == 1){
                        buff->lines[y+shiftDown-1].erase(x+(--shiftRight)-1, 1);
                    }
                    else{
                        buff->lines[y+shiftDown-1].erase((--x)+shiftRight-1, 1);
                        if(shiftRight > 0){
                            moveLeft();
                            moveRight();
                        }
                    }
            }
            break;
        case KEY_DC:
            // Delete
            if((x+shiftRight-1) == buff->lines[y+shiftDown-1].length() && (y+shiftDown-1) != buff->lines.size() - 1){
                buff->lines[y+shiftDown-1] += buff->lines[y+shiftDown];
                deleteLine(y+shiftDown);
            }
            else{
                buff->lines[y+shiftDown-1].erase(x+shiftRight-1, 1);
            }
            break;
        case KEY_ENTER:
            // Enter
            if(x+shiftRight-1 < buff->lines[y+shiftDown-1].length()){
                buff->insertLine(buff->lines[y+shiftDown-1].substr(x+shiftRight-1, buff->lines[y+shiftDown-1].length() - x+shiftRight+1), y+shiftDown);
                buff->lines[y+shiftDown-1].erase(x+shiftRight-1, buff->lines[y+shiftDown-1].length() - x+shiftRight+1);
            }
            else
                buff->insertLine("", y + shiftDown);
            x = 1;
            moveDown();
            break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
            //TAB
            buff->lines[y+shiftDown-1].insert(x+shiftRight-1, 4, ' ');
            if(shiftRight == 0 && (x+4) > COLS-1){
                shiftRight = x+4-COLS-2;
                x = COLS-3;
            }
            else if(shiftRight > 0){
                shiftRight += 0;
            }
            else{
                x += 4;
            }
            break;

        default:
            if(c >= 32 && c <= 126){
                buff->lines[y+shiftDown-1].insert(x+shiftRight-1, 1, char(c));
                if(x >= COLS-2)
                    shiftRight++;
                else
                    x++;
            }
            else if(c > 32){
                mode = 'x';
                status = "Nie poprawny znak!";
                info = true;
            }
            break;
        }
        break;
    }
}

void Editor::moveLeft()
{
    if(x == 1 && shiftRight > 0) --shiftRight;
    if(x-1 > 0){
        x--;
        wmove(rightWindow, y, x);
    }else if(buff->lines[y+shiftDown-1].length() == 0){
        int t = buff->lines[y+shiftDown-2].length();
        if(t < COLS-2){
            x = t;
        }
        else{
            x = COLS-2;
            shiftRight = t - x;
        }
        ++x;
        --y;
    }
}

void Editor::moveRight()
{
    if(x == COLS-2 && x+shiftRight <= buff->lines[y+shiftDown-1].length()) shiftRight++;
    if(x < COLS-2 && x <= buff->lines[y+shiftDown-1].length()){
        x++;
        wmove(rightWindow, y, x);
    }else if(buff->lines[y+shiftDown-1].length() == x - 1){
        int t = buff->lines[y+shiftDown].length();
        if(t < COLS-2){
            x = t;
        }
        else{
            x = COLS-2;
            shiftRight = t - x;
        }
        ++x;
        ++y;
    }
}

void Editor::moveUp()
{
    if(y == 1 && shiftDown > 0) shiftDown--;
    if(y-1 > 0)
        y--;

    x = 1;
    shiftRight = 0;

    wmove(rightWindow, y, x);
}

void Editor::moveDown()
{
    if(y == LINES-2 && y+shiftDown < buff->lines.size())
        shiftDown++;

    if(y < buff->lines.size() && y < LINES-2)
        y++;

    x = 1;
    shiftRight = 0;

    wmove(rightWindow, y, x);
}

void Editor::printBuff()
{
    for(int i=0; i<LINES-2; i++){
        if(i >= buff->lines.size()){
            wmove(rightWindow, i+1, 1);
            wclrtoeol(rightWindow);
        }
        else{
            string temp;
            if(buff->lines[i+shiftDown].length() > shiftRight)
                temp.insert(0, buff->lines[i+shiftDown], shiftRight, COLS-2);
            else
                temp.insert(0, "");
            mvwprintw(rightWindow, i+1, 1, temp.c_str());

            char buf[20];
            sprintf(buf, "%d", LINES);
            
        }
        wclrtoeol(rightWindow);
    }
    wmove(rightWindow, y, x);
}

void Editor::printStatusLine(WINDOW * a)
{
    int i = status.length();
    wattron(a, A_REVERSE);
    mvwprintw(a, 1, 2, status.c_str());
    while(i < COLS)
        mvwprintw(a, LINES-2, i++, " "); 
    wclrtoeol(a);
    wattroff(a, A_REVERSE);
}

void Editor::deleteLine()
{
    buff->removeLine(y+shiftDown);
}

void Editor::deleteLine(int i)
{
    buff->removeLine(i);
}

void Editor::checkVectorSize()
{
    //something
}
void Editor::checkStringSize()
{
    //something
}

void Editor::setWindow(bool var)
{
    win = var;
}
bool Editor::getWindow()
{
    return win;
}

void Editor::printMenu()
{
    for(int i = 0; i < lengthMenu; i++){
        if(positionMenu-1 == i){
            wattron(leftMenu, A_REVERSE);
            mvwprintw(leftMenu, i+1, 1, fileManager->fileBuff[i].name);
            wclrtoeol(leftMenu);
            wattroff(leftMenu, A_REVERSE);
        }else{
            mvwprintw(leftMenu, i+1, 1, fileManager->fileBuff[i].name);
        }
        
    }
}

void Editor::handle(int c)
{
    switch(c){
        case 9:
            win = !win;
            break;
        case 10:
            openFile(fileManager->fileBuff[positionMenu-1].path);
            win = true;
            break;
        case 'n':
            newFileSetting();
            saveFileSetting();
            break;
        case 27:
        case 'q':
            mode = 'q';
            break;
        case 'e':
            newFileSetting(true);
            saveFileSetting();
            break;
        case 'r':
            fileManager->removeLine(positionMenu-1);
            if(positionMenu != 1)
                positionMenu--;
            lengthMenu--;
            wclear(leftMenu);      
            printMenu();
            wrefresh(leftMenu);
            break;          
        case KEY_UP:
            if(positionMenu > 1)
                --positionMenu;
            break;
        case KEY_DOWN:
            if(positionMenu < lengthMenu)
                ++positionMenu;
            break;
    }

}

void Editor::openFile(char path[])
{
    free(buff);
    buff = new Buffer();

    ifstream file;
    file.open(path, fstream::in);
    
    if(file.good())
    {
        while(!file.eof())
        {
            string temp;
            getline(file, temp);
            try{
                buff->appendLine(temp);
            }
            catch(bad_alloc){
                win = false;
                mode = 'x';
                status = "Problem z alokowaniem pamieci!";
                info = true;
                break;
            }
        }
    }else{
        win = false;
        mode = 'x';
        status = "Problem z otwarciem pliku!";
        info = true;
        buff = new Buffer();
        buff->appendLine("");
    }
    file.close();
}

void Editor::saveFile()
{
    ofstream file;
    file.open(fileManager->fileBuff[positionMenu-1].path, fstream::trunc | fstream::out);
    if(file.is_open()){
        for(int i=0; i<buff->lines.size(); i++){
            file << buff->lines[i] << endl;
            file.flush();
        }
        status = "Plik zapisany!";
        mode = 'x';
        info = true;
    }else{
        mode = 'x';
        status = "Problem z zapisem pliku!";
        info = true;
    }
    file.close();
}

void Editor::openFileSetting()
{
    ifstream file;
    file.open(homePath.c_str(), fstream::in);
    int i=1;
    if(file.good())
    {
        while(!file.eof())
        {
            string temp, name, path;
            getline(file, temp);
            int positionName = temp.find(";");
            if(positionName <= 0) continue;
            try{
              name = trim(temp).substr(0, positionName);
              path = temp.substr(positionName+1);
              if(name.length() <= 0 || path.length() <= 0){
                win = false;
                mode = 'x';
                status = "Plik ustawien jest nieprawidłowo sformatowany!";
                info = true;
                break;
              }
              else
                fileManager->insertLine(name, trim(path), lengthMenu++);
            }
            catch(out_of_range& range){
                win = false;
                mode = 'x';
                status = "Plik ustawien jest nieprawidłowo sformatowany!";
                info = true;
                break;
            }
        }
    }else{
        win = false;
        mode = 'x';
        status = "Problem z otwarciem pliku ustawien!";
        info = true;
    }
    file.close();
}

void Editor::newFileSetting(bool update)
{
    WINDOW *okno = newwin(0,0,0,0);
    int ch;
    File::baseFile temp;
    string txt;
    int state = 1;

    keypad(okno, true);
    curs_set(true);
    wtimeout(okno, 20);

    box(okno, 0, 0);
    wrefresh(okno);
    if(update) txt = fileManager->fileBuff[positionMenu-1].name;
    while(ch != 27)
    {       
        ch = wgetch(okno); 
        switch(ch)
        {   
            case 10://Enter
                if(txt.length() > 0){
                    if(state == 1){
                        if(update){
                            memset(fileManager->fileBuff[positionMenu-1].name, 0, 20);
                            strcpy(fileManager->fileBuff[positionMenu-1].name, txt.c_str());
                            txt = fileManager->fileBuff[positionMenu-1].path;
                    
                        }else{
                            strcpy(temp.name, txt.c_str());   
                            txt.clear();
                        }
                    state++;
                    }else if(state == 2){
                        if(update){
                            memset(fileManager->fileBuff[positionMenu-1].path, 0, 40);
                            strcpy(fileManager->fileBuff[positionMenu-1].path, txt.c_str());
                        }else{
                            strcpy(temp.path, txt.c_str());
                            fileManager->insertLine(temp, lengthMenu++);
                        }
                        ch = 27;
                        break;
                    }
                }            
                break;
            case KEY_BACKSPACE:
                if(txt.length() > 0)
                    txt.erase(txt.length()-1, 1);
                break;
            default:
                if(ch >= 32 && ch <= 126){ 
                    txt += ch;
                }
                break;
        }

        werase(okno);
        mvwprintw(okno, (int)(LINES/4), (int)(COLS/2)-30, "F O R M U L A R Z   T W O R Z E N I A / E D Y C J I  M E N U");
        if(state == 1)
            mvwprintw(okno, (int)(LINES/2), (int)(COLS/2), "Nazwa: ");
        else
            mvwprintw(okno, (int)(LINES/2), (int)(COLS/2), "Sciezka: ");    
        mvwprintw(okno, (int)(LINES/2), (int)(COLS/2)+10, txt.c_str());
        box(okno, 0, 0);
        wrefresh(okno); 

    }
    delwin(okno);

}

void Editor::saveFileSetting()
{
    ofstream file;
    file.open(homePath.c_str(), fstream::trunc | fstream::out);
    if(file.is_open()){
        for(int i = 0; i < fileManager->fileBuff.size(); i++){
            file << fileManager->fileBuff[i].name << ";" << fileManager->fileBuff[i].path << endl;
            file.flush();
        }

        status = "Plik ustawien zapisany!";
        mode = 'x';
        info = true;
    }else{
        mode = 'x';
        status = "Problem z zapisem pliku ustawien!";
        info = true;
    }
    file.close();
}

string & Editor::trim(string & str){
    while(str[0] == ' ')
        str.erase(str.begin()+str.find_first_of(" "));
    while(str[str.length()-1] == ' ') 
        str.erase(str.begin()+str.find_last_of(" "));
    return str;
}