//
//  Sdisk.h
//  CSE461Lab3
//
//  Created by Brandon Saunders on 5/1/15.
//  Copyright (c) 2015 Brandon Saunders. All rights reserved.
//

#ifndef CSE461Lab3_Sdisk_h
#define CSE461Lab3_Sdisk_h
#include "Sdisk.h"

#include<iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

//Sdisk Class
class Sdisk
{
    public :
    Sdisk() { }
    
    Sdisk(string diskname);                                         // Default Constructor
    Sdisk(string diskname, int numberofblocks, int blocksize);
    int getblock(int blocknumber, string& buffer);
    int putblock(int blocknumber, string buffer);
    int getblocksize() {return blocksize; }                         // Returns the blocksize.
    int getnumberofblocks() { return numberofblocks; }              // Returns the number of blocks.
    string getfilename() { return diskname; }                       // Returns the disk name.
    friend class Shell;
    friend class Filesys;
    
    private :
    
    int numberofblocks;                                             // number of blocks on disk
    string diskname;                                                // file name of pseudo-disk
    string diskname1;
    int blocksize;                                                  // block size in bytes/the number of blocks.
};

Sdisk::Sdisk(string disk)
{
    diskname = disk + ".dat";
    diskname1 = disk + ".spc";
    ifstream ifile(diskname1.c_str());
    
    if(ifile.is_open())
    {
        ifile >> numberofblocks >> blocksize;
        ifile.close();
    }
    
    else
    {
        cout << "Was unable to open the file" << endl;
    }
}

// Sdisk default constructor
Sdisk::Sdisk(string disk, int numberofblocks, int blocksize)
{
    this->diskname = disk + ".dat";
    this->diskname1 = disk + ".spc";
    this->numberofblocks = numberofblocks;
    this->blocksize = blocksize;
    fstream spcfile;
    fstream datfile;
    spcfile.open((this->diskname1).c_str(),ios::in | ios::out);
    datfile.open((this->diskname).c_str(),ios::in | ios::out);
    
    if (spcfile.good() && datfile.good())
    {
        cout << "The disk named: " << diskname.c_str() << " exists and is now ready to be written to." << endl;
    }
    else // .spc/.dat file creation.
    {
        cout << "The disk: " << diskname.c_str() << "could not be found. " << endl;
        cout << "Both the SPC and DAT file were not found. Creating both now. Please wait...." << endl;
        spcfile.open((this->diskname1).c_str(),ios::out);
        datfile.open((this->diskname).c_str(),ios::out);
        spcfile << numberofblocks << " " << blocksize;
        cout << "The SPC file " << diskname.c_str() << " was created" << endl;
        cout << "The DAT file " << diskname.c_str() << " was created" << endl;
        
        for (int i=0; i<numberofblocks*blocksize; i++)
        {
            datfile.put('#');           // Fills the file with '#' character.
        }
    }
    spcfile.close();
    datfile.close();
    return;
}

// Get the Block Size. It returns
// an error code of 0 if unsuccessful and a 1 if it is successful.
int Sdisk::getblock(int blocknumber,string& buffer)
{
    bool good = 0;
    fstream checkfile;
    checkfile.open((this->diskname).c_str(), ios::in | ios::out);
    checkfile.seekp(blocksize * blocknumber,ios::beg);
    if (checkfile.bad())
    {
        cout << "Cannot open the file" << endl;
    }
    else
    {
        for (int i = 0; i < blocksize;i++)
        {
            char y = (char) checkfile.get();
            buffer = buffer + y;
        }
        good = 1;
    }
    checkfile.close();
    return good;
}


// writes the string buffer to block blocknumber.
// It returns an error code of 1 if successful and
// 1 if it's unsuccessful.
int Sdisk::putblock(int blocknumber, string buffer)
{
    bool good = 0;
    fstream checkfile;
    checkfile.open((this->diskname).c_str(), ios::in|ios::out);
    if (checkfile.bad())
    {
        cout << "Cannot open the file" << endl;
    }
    else
    {
        //fstream iofile;
        //iofile.open((this->diskname).c_str());
        checkfile.seekp(blocksize * blocknumber,ios::beg);
        for (int i=0; i < buffer.size() ;i++)
        {
            checkfile.put(buffer[i]);
            good = 1;
        }
    }
    checkfile.close();
    return good;
}




#endif
