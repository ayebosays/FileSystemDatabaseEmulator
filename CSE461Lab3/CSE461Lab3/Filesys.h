//
//  Filesys.h
//  CSE461Lab3
//
//  Created by Brandon Saunders on 5/1/15.
//  Copyright (c) 2015 Brandon Saunders. All rights reserved.
//

#ifndef CSE461Lab3_Filesys_h
#define CSE461Lab3_Filesys_h
#include "Sdisk.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;


// Filesys Class
class Filesys: public Sdisk
{
public:
    
    Filesys(string diskname);
    //fsys(string,int,int);
    int fsclose();
    int newfile(string file);
    int rmfile(string file);
    int getfirstblock(string file);
    int addblock(string file, string block);
    int delblock(string file, int blocknumber);
    int readblock(string file, int blocknumber, string& buffer);
    int writeblock(string file, int blocknumber, string buffer);
    int nextblock(string file, int blocknumber);
    vector<string> block(string buffer, int b);
    
    private :
    
    int fssync();                                                   //writes the Root and FAT to the disk.
    string buffer;
    int rootsize;                   // maximum number of entries in ROOT
    int fatsize;                    // number of blocks occupied by FAT
    vector<string> filename;        // filenames in ROOT
    vector<int> firstblock;         // firstblocks in ROOT parallel
    vector<int> fat;                // FAT # of blocks
};


Filesys::Filesys(string diskname): Sdisk(diskname)
{
    rootsize = getblocksize()/12;
    fatsize = (getnumberofblocks()*5/getblocksize())+1;
    cout << "rootside: " << rootsize << endl << "fatsize: " << fatsize << endl << "number of blocks: " <<  getnumberofblocks() << endl << getblocksize() << endl;
    
    
    /*
     string buffer;
     getblock(1,buffer);
     if (buffer[0] == "#")
     {
     //add filesystem
     // build root vectors. filename "XXXXXX", firstblock 0.
     // build FAT
     // run fssync - writes the FAT, and Root.
     }
     else
     {
     //read filesystem
     // 1. read block 1 containing root.
     // 2. use istringstream to input filename in first block.
     // 3. read blocks 2-k (k is Fatsize + 1)
     // 4. Use istringstream to extract FAT.
     }
     */
    
    for(int i=0; i<rootsize; i++)
    {
        filename.push_back("XXXXX");
        firstblock.push_back(0);
        
    }
    
    
    int k= getnumberofblocks();
    fat.push_back(fatsize);
    int j=0;
    
    while( j < fatsize - 1)
        
    {
        fat.push_back(0);
        j++;
    }
    
    for(int i = fatsize; i < k; i++)
        
    {
        fat.push_back(i+1);
    }
    fat[fat.size()-1] = 0;
    fssync();
}



// fsclose() - This module writes FAT and ROOT to the pdisk and sets dirty=0 (closing the pdisk).
int Filesys::fsclose()
{
    fssync();
    return 0;
}



// newfile - use filename. check if file exists. find first "XXXXXX" entry and change to filename.
// firstblock[i] = 0. fssync()
int Filesys::newfile(string file)
{
    for(unsigned int i = 0; i < filename.size(); i++)
    {
        if( filename[i] == "XXXXX")
        {
            filename[i] = file;
            fssync();
            return 1;
        }
    }
    
    return 0;
}



// removefile - use filename to find file. if firstblock[ ] != 0 - return an error message. Cannot remove a file if it's not empty.
// else - change filename[ ] to "XXXXXX".
// fssync(). rootsync and fatsync. have them seperate.
// This function removes the entry file from ROOT if the file is empty (first block is 0). It returns error codes of 1 if successful and 0 otherwise (not empty or file does not exist).
int Filesys::rmfile(string file)
{
    int x=getfirstblock(file);
    if(x != 0)
    {
        cout << "rmfile " << endl;
        return 0;
    }
    else
    {
        for(int i=0; i < rootsize; i++)
        {
            if(filename[i]==file)
            {
                cout << "filename rmfile" << endl;
                filename[i]="XXXXX";
            }
        }
    }
    fssync();
    return 1;
}







// get firstblock - This function returns the block number of the first block in file. It returns the error code of 0 if the file does not exist.


int Filesys::getfirstblock(string file)
{
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename[i] == file)
        {
            return firstblock[i];
        }
        
        cout << "file does not exist: " << file << endl;
        return -1;
    }
    return 0;
}





// addblock - This function adds a block of data stored in the string buffer to the end of file F and returns the block number. It returns error code of 1 if successful, 0 if the file does not exist, and returns -1 if there are no available blocks (file system is full!).


int Filesys::addblock(string file, string block)
{
    cout << "start of addblock" << endl;
    int id = getfirstblock(file);
    int allocate = fat[0];
    bool check = false;
    /*
     if (id == -1)
     {
     return -1;
     cout << "No such file: " << file;
     }
     */
    if (allocate == 0)
    {
        cout << "No space available";
        return -1;
    }
    if (id == 0)
    {
        for (int i = 0; i < filename.size(); i++)
        {
            if (filename[i] == file)
            {
                
                firstblock[i] = allocate;
                fat[0] = fat[allocate];
                fat[allocate] = 0;
                check = true;
                break;
            }
        }
        if (check == false)
        {
            cout << "file is empty";
            return -2;
        }
    }
    else
    {
        int nextblock = id;
        while (fat[nextblock] != 0)
        {
            nextblock = fat[nextblock];
            fat[nextblock] = allocate;
            fat[0] = fat[allocate];
            fat[allocate] = 0;
        }
        fat[nextblock] = allocate;
    }
    
    
    fat[0] = fat[fat[0]];
    fat[allocate] = 0;
    fssync(); //sync the root and fat.
    putblock(allocate, block);
    cout << "end of addblock" << endl;
    
    putblock(allocate, block);
    fssync();
    return allocate;
}





//
int Filesys::delblock(string file, int blocknumber){return 0;}


//
int Filesys::readblock(string file, int blocknumber, string& buffer){return 0;}


//
int Filesys::writeblock(string file, int blocknumber, string buffer){return 0;}



//
int Filesys::nextblock(string file, int blocknumber){return 0;}





vector<string> Filesys::block(string buffer, int b)
{
    // blocks the buffer into a list of blocks of size b
    vector<string> blocks;
    
    int numberofblocks = 0;
    
    if (buffer.length() % b == 0)
    {
        numberofblocks= buffer.length()/b;
    }
    else
    {
        numberofblocks= buffer.length()/b +1;
    }
    string tempblock;
    
    for (int i=0; i<numberofblocks; i++)
    {
        tempblock=buffer.substr(b*i,b);
        blocks.push_back(tempblock);
    }
    int lastblock=blocks.size()-1;
    
    for (int i=blocks[lastblock].length(); i<b; i++)
    {
        blocks[lastblock]+="#";
    }
    return blocks;
}


//writes the Root and FAT to the disk.
//This module writes FAT and ROOT to the sdisk. It should be used every time FAT and ROOT are modified.

int Filesys::fssync()
{
    // FAT
    // Uses ostringstream to write root vectors filename, firstblock -> buffer.
    // block the buffer -> write to block1 of Sdisk.
    
    ostringstream fatstream;
    string fatbuffer;
    
    for(int i=0; i < getnumberofblocks(); i++)
    {
        fatstream << fat[i]<<" ";
        fatbuffer = fatstream.str();
    }
    
    vector <string> blockbuff = block(fatbuffer, getblocksize());
    
    for (int i=0; i < blockbuff.size(); i++)
    {
        
        putblock(2+i, blockbuff[i]);
    }
    
    // Root
    // Use ostringstream to write FAT to buffer. block the buffer - >
    // Returns vector<string>
    // write the blocks -> 2 -> fatsize + 1
    
    ostringstream outstream;
    string buffer;
    
    for( int i = 0; i < rootsize; ++i )
    {
        outstream << filename[i] << " " << firstblock[i] << " ";
        buffer = outstream.str();
    }
    putblock(1, buffer); // from the first project
    return 1;
}








#endif
