//
//  Shell.h
//  CSE461Lab3
//
//  Created by Brandon Saunders on 5/12/15.
//  Copyright (c) 2015 Brandon Saunders. All rights reserved.
//

#ifndef CSE461Lab3_Shell_h
#define CSE461Lab3_Shell_h
#include "Sdisk.h"
#include "Filesys.h"
#include <string>
#include <iostream>

class Shell: public Filesys
{
    public:
    Shell(string filename):Filesys(filename)
    {
        this->file_name = filename;
        Filesys(file_name);
    };
        int dir();              // call ls which lists all files
        int add(string file);   // add a new file using input from the keyboard.
        int del(string file);   // deletes the file
        int type(string file);  //lists the contents of file
        int copy(string file1, string file2);//copies file1 to file2
        string filename;
        string file_name;
        friend class Sdisk;
        friend class Filesys;
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
    
    newfile(file);
    int block = getfirstblock(file);
    //addblock(file, block);
    return 1;
     
}


// delete the blocks, delete the file.
int Shell::del(string file)    // deletes the file
{
    
    int block = getfirstblock(file);
    while (block > 0)
    {
        delblock(file, block);
        block = getfirstblock(file);
    }
    rmfile(file);
    
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
    
    int block = getfirstblock(file1);
    int block2 = getfirstblock(file2);
    string buffer;
    while(block != 0)
    {
        string tmp;
        buffer += tmp;
        block = nextblock(file1, block);
    }
    writeblock(file2, block2, buffer);
    
    return 1;
}





#endif
