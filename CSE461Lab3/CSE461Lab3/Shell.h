//
//  Shell.h
//  CSE461Lab3
//
//  Created by Brandon Saunders on 5/12/15.
//  Copyright (c) 2015 Brandon Saunders. All rights reserved.
//

#ifndef CSE461Lab3_Shell_h
#define CSE461Lab3_Shell_h

#include "Filesys.h"
#include "Sdisk.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

class Shell: public Filesys
{
public:
    Shell(string filename, int blocksize, int numberofblocks);
    int dir();              // call ls which lists all files
    int add(string file);   // add a new file using input from the keyboard.
    int del(string file);   // deletes the file
    int type(string file);  //lists the contents of file
    int copy(string file1, string file2);//copies file1 to file2
    string filename;
    string file_name;
};

int Shell::dir()
{
    vector<string> flist = ls();
    for (int i = 0; i < flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
    return 1;
}

Shell::Shell(string filename, int blocksize, int numberofblocks):Filesys(filename,blocksize,numberofblocks)
{
}

// read in data from keyboard. getline...endl. create file.
// new file. getline. block it up with addblock.
int Shell::add(string file)     // add a new file using input from the keyboard
{
    int err = newfile(file);
    
    if (err == -1)
    {
        return -1;
    }
    
    string contains;
    int blocknumber =0;
    cout << "Input file contents: " << endl;
    
    getline(cin,contains);
    
    vector<string> blocks = block(contains, 128);
    
    for(int i =0; i < blocks.size(); i++)
    {
        blocknumber = addblock(file, blocks[i]);
    }
    
    return 1;
    
}

// delete the blocks, delete the file.
int Shell::del(string file)    // deletes the file
{
    
    int currentblock = getfirstblock(file);
    while (currentblock > 0)
    {
        delblock(file, currentblock);
        currentblock = getfirstblock(file);
    }
    rmfile(file);
    
    return 1;
}

// cat function from before the midterm.
int Shell::type(string file)   //lists the contents of file
{
    int currentblock = getfirstblock(file);
    while(currentblock > 0)
    {
        string buffer;
        readblock(file, currentblock, buffer);
        currentblock = nextblock(file, currentblock);
        
        if(currentblock == 0)
        {
            while(buffer.back() == '#')
            {
                buffer = buffer.substr(0,buffer.length() - 1);
            }
            cout << buffer;
        }
        cout << endl;
    }
    
    return 1;
}

// was on the midterm....
int Shell::copy(string file1, string file2) //copies file1 to file2
{
    del(file2); // clears out file2.
    newfile(file2);
    int currentblock = getfirstblock(file1);
    if (currentblock == -1)
    {
        return 0;
    }
    
    while(currentblock > 0)
    {
        string buffer;
        readblock(file1, currentblock, buffer);
        addblock(file2, buffer);
        currentblock = nextblock(file1, currentblock);
    }
    return 1;
}





#endif
