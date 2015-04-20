//
//  main.cpp
//  CSE461Lab2
//
//  Created by Brandon Saunders on 4/14/15.
//  Copyright (c) 2015 Brandon Saunders. All rights reserved.
//
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//This constructor incorporates the creation of the disk with the "formatting" of the device. It accepts the integer values numberofblocks, blocksize, a string diskname and creates a Sdisk (pseudo-disk). The Sdisk is a file of characters which we will manipulate as a raw hard disk drive. The function will check if the file diskname exists. If the file exists, it is opened and treated as a Sdisk with numberofblocks many blocks of size blocksize. If the file does not exist, the function will create a file called diskname which contains numberofblocks*blocksize many characters. This file is logically divided up into numberofblocks many blocks where each block has blocksize many characters.

class Sdisk
{
    public :
    
    Sdisk(string diskname, int numberofblocks, int blocksize);
    int getblock(int blocknumber, string& buffer);
    int putblock(int blocknumber, string buffer);
    
    private :
    
    string diskname;        // file name of pseudo-disk
    int numberofblocks;     // number of blocks on disk
    int blocksize;          // block size in bytes
};

// retrieves block blocknumber from the disk and stores the data in the string buffer. It returns an error code of 1 if successful and 0 otherwise.
// getblock to open the file and get a block

int Sdisk::getblock(int blocknumber,string& buffer)
{
    bool good = 0;
    ifstream checkfile;
    checkfile.open((diskname + ".spc").c_str());
    if (checkfile.bad())
    {
        cout << "Cannot open the file";
        
    }
    else
    {
        for (int i = 0; i<blocksize;i++)
        {
            char y = checkfile.get();
            buffer = buffer + y;
        }
        good = 1;
        
    }
    checkfile.close();
    return good;
}

// writes the string buffer to block blocknumber. It returns an error code of 1 if successful and 0 otherwise.
// put block to open the file and write a block.

int Sdisk::putblock(int blocknumber, string buffer)
{
    bool good = 0;
    ifstream checkfile;
    checkfile.open((diskname + ".dat").c_str());
    if (checkfile.bad())
    {
        cout << "Cannot open the file";
        
    }
    else
    {
        fstream iofile;
        iofile.open((diskname + ".dat").c_str());
        iofile.seekp(blocksize*blocknumber,ios::beg);
        
        for (int i=0;i<blocksize;i++)
        {
            iofile.put(buffer[i]);
        }
        good = 1;
        }
    checkfile.close();
    return good;
}

// You can use this to test your Sdisk class

int main()
{
    Sdisk disk1("test1",16,32);
    /*string block1, block2, block3, block4;
     for (int i=1; i<=32; i++) block1=block1+"1";
     for (int i=1; i<=32; i++) block2=block2+"2";
     disk1.putblock(4,block1);
     disk1.getblock(4,block3);
     cout << "Should be 32 1s : ";
     cout << block3 << endl;
     disk1.putblock(8,block2);
     disk1.getblock(8,block4);
     cout << "Should be 32 2s : ";
     cout << block4 << endl;
     return 0;*/
    cout << "Finished main";
    
}
