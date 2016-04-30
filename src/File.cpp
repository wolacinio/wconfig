#include "File.h"
#include <iostream>

File::File() {}

string File::remTabs(string line)
{
    int tab = line.find("\t");
    if(tab == line.npos)
        return line;
    else
        return remTabs(line.replace(tab, 1, "    "));
}

void File::insertLine(string name, string path, int n)
{
    //name = remTabs(name);
    baseFile temp;
    strcpy(temp.name, name.c_str());
    strcpy(temp.path, path.c_str());
    
    //strcpy(temp.path, path.substr(1).c_str());

    //temp.name ="ds";

    fileBuff.insert(fileBuff.begin()+n, temp);
}

void File::appendLine(string line)
{
    line = remTabs(line);
    //lines.push_back(line);
}

void File::removeLine(int n)
{
    //lines.erase(lines.begin()+n);
}

