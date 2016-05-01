#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>
#include <cstring>

using namespace std;

class File
{
private:


public:
    File();
    struct baseFile{
        char name[20];
        char path[60];
    };
    
    vector <baseFile> fileBuff;

    void insertLine(string, string, int);
    void insertLine(baseFile, int);
    void removeLine(int);

};

#endif
