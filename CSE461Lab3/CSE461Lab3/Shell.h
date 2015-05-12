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
        Shell(string filename, int blocksize, int numberofblocks);
        int dir();// lists all files
        int add(string file);// add a new file using input from the keyboard
        int del(string file);// deletes the file
        int type(string file);//lists the contents of file
        int copy(string file1, string file2);//copies file1 to file2
    friend class Filesys;
};

/*
 Create function stubs (functions with no code) for the class functions.
 Work first on the constructor that creates the Shell.
 Work on the add function which creates a file.
 Work on the del function which deletes the file.
 Work on the type function which displays the content of the file.
*/

// Creates the shell
Shell::Shell(string filename, int blocksize, int numberofblocks): Filesys::Filesys(disk)
{
    

    
}





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


int Shell::add(string file)     // add a new file using input from the keyboard
{
    newfile(file);
    //addblock(file, block);
    return 1;
}

int Shell::del(string file)    // deletes the file
{
    rmfile(file);
    //delblock(file, block);
    return 1;
}


int Shell::type(string file)   //lists the contents of file
{
    return 1;
}


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
