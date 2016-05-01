#include "File.h"
#include <iostream>

File::File() {}

void File::insertLine(string name, string path, int n)
{
    baseFile temp;
    strcpy(temp.name, name.c_str());
    strcpy(temp.path, path.c_str());
    fileBuff.insert(fileBuff.begin()+n, temp);
}

void File::insertLine(baseFile base, int n)
{
    fileBuff.insert(fileBuff.begin()+n, base);
}

void File::removeLine(int n)
{
    fileBuff.erase(fileBuff.begin()+n);
}
