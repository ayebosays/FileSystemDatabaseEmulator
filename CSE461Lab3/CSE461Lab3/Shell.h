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

class Shell
{
public:
    Shell(string filename, int blocksize, int numberofblocks)
    {
        // Sdisk disk(filename);
        this-> filesys = Filesys();
        filesys.start(Sdisk(filename));
    };
    int dir();              // call ls which lists all files
    int add(string file);   // add a new file using input from the keyboard.
    int del(string file);   // deletes the file
    int type(string file);  //lists the contents of file
    int copy(string file1, string file2);//copies file1 to file2
    string filename;
    string file_name;
    Sdisk disk;
    Filesys filesys;
};

int Shell::dir()                // lists all the files
{
    // dir lists files in the class Shell
    // Prototype: int Shell::dir()
    
    vector<string> flist;
    
    for (int i = 0; i < flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
    return 1;
    
}

// read in data from keyboard. getline...endl. create file.
// new file. getline. block it up with addblock.
int Shell::add(string file)     // add a new file using input from the keyboard
{
    
    filesys.newfile(file);
    int block = filesys.getfirstblock(file);
    //addblock(file, block);
    return 1;
    
}


// delete the blocks, delete the file.
int Shell::del(string file)    // deletes the file
{
    
    int block = filesys.getfirstblock(file);
    while (block > 0)
    {
        filesys.delblock(file, block);
        block = filesys.getfirstblock(file);
    }
    filesys.rmfile(file);
    
    return 1;
}


// cat function from before the midterm.
int Shell::type(string file)   //lists the contents of file
{
    
    return 1;
}


// was on the midterm....
int Shell::copy(string file1, string file2) //copies file1 to file2
{
    
    int block = filesys.getfirstblock(file1);
    int block2 = filesys.getfirstblock(file2);
    string buffer;
    while(block != 0)
    {
        string tmp;
        buffer += tmp;
        block = filesys.nextblock(file1, block);
    }
    filesys.writeblock(file2, block2, buffer);
    
    return 1;
}





#endif
