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
class Filesys
{
public:
    
    Filesys(Sdisk&);
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
    bool checkblock(string file, int blocknumber);
    vector<string> block(string buffer, int b);
    
    private :
    
    Sdisk disk;
    int fssync();                                                   //writes the Root and FAT to the disk.
    string buffer;
    int rootsize;                   // maximum number of entries in ROOT
    int fatsize;                    // number of blocks occupied by FAT
    vector<string> filename;        // filenames in ROOT
    vector<int> firstblock;         // firstblocks in ROOT parallel
    vector<int> fat;                // FAT # of blocks
};

// This constructor reads from the pdisk "disk" and either opens the existing file system on the disk or creates one for an empty disk. Recall the pdisk is a file of characters which we will manipulate as a raw hard disk drive. This file is logically divided up into number_of_blocks many blocks where each block has block_size many characters. Information is first read from block 1 to determine if an existing file system is on the disk. If a filesystem exists, it is opened and made available. Otherwise, the file system is created. The module creates a file system on the pdisk by creating an intial FAT and ROOT. A file system on the disk will have the following segments:
Filesys::Filesys(Sdisk& sdisk)
{
    this-> disk = sdisk;
    rootsize = disk.getblocksize()/12;
    fatsize = (disk.getnumberofblocks()*5) / (disk.getblocksize())+1;
    cout << "rootsize: " << rootsize << endl << "fatsize: " << fatsize << endl << "number of blocks: " <<  disk.getnumberofblocks() << endl << "getblocksize(): " << disk.getblocksize() << endl;
    
    
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
        filename.push_back("XXXXXX");
        firstblock.push_back(0);
    }
    
    
    int k= disk.getnumberofblocks();
    fat.push_back(fatsize + 2);

    
    for (int i = 0; i <= fatsize; i++)
    {
        fat.push_back(0);
        
    }
    
    for(int i = fatsize + 2; i < k; i++)
        
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
    bool empty = false;
    
    for (int i= 0; i < filename.size(); i++)
    {
        if (filename[i] == file)
        {
            cout << "File (" << file << ") already exists!" << endl;
            return 1;
        }
    }
    for (int i = 0; i < filename.size(); i++)
    {
        if( filename[i] == "XXXXXX")
        {
            
            empty = true;
            filename[i] = file;
            firstblock[i] = 0;
            break;
            
        }
    }
    
    if(!empty)
    {
        cout << "No empty slots in root directory.\n";
        return 0;
    }
    fssync();
    cout << "Successfully added file: " << file << endl;
    return 1;
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
                filename[i]="XXXXXX";
            }
        }
    }
    fssync();
    return 1;
}



// get firstblock - This function returns the block number of the first block in file. It returns the error code of 0 if the file does not exist.
int Filesys::getfirstblock(string file)
{
    bool found = false;
    int first_block = 0;
    
    
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename[i] == file)
        {
            found = true;
            first_block = firstblock[i];
            break;
        }
    }
    if(!found)
    {
        cout << "getFirstBlock(): File was not found!\n";
        return -1;
    }
    fssync();
    return first_block;
}





// addblock - This function adds a block of data stored in the string buffer to the end of file F and returns the block number. It returns error code of 1 if successful, 0 if the file does not exist, and returns -1 if there are no available blocks (file system is full!).
int Filesys::addblock(string file, string block)
{
    int id = getfirstblock(file);
    int allocate = fat[0];
    bool check = false;

     
    if (allocate == 0)
    {
        cout << "No space available" << endl;
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
                cout << "addblock" << endl;
                break;
            }
        }
        if (!check)
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
        }
        fat[nextblock] = allocate;
        fat[0] = fat[allocate];
        fat[allocate] = 0;
    }
    
    fssync(); //sync the root and fat.
    disk.putblock(allocate, block);
    
    return allocate;
}





// The function removes block numbered blocknumber from file and returns an error code of 1 if successful and 0 otherwise.
int Filesys::delblock(string file, int blocknumber)
{
    int first_block = getfirstblock(file);
    if(checkblock(file,blocknumber) == 0)
    {
        return 0;
    }
    for (int i = 0; i < filename.size(); i++)
    {
        if(firstblock[i] == blocknumber)
        {
            firstblock[i] = fat[blocknumber];
            fat[blocknumber] = fat[0];
            fat[0] = blocknumber;
            
            fssync();
            return 1;
        }
    }
    
    for(int i = 0; i < fat.size();i++)
    {
        if(fat[i] == blocknumber)
        {
            fat[i] = fat[blocknumber];
            fat[blocknumber] = fat[0];
            fat[0] = blocknumber;
            break;
        }
    }
    
    
    fssync();
    return 1;
}


//
int Filesys::readblock(string file, int blocknumber, string& buffer)
{
    disk.getblock(blocknumber,buffer);
    return 1;
}


//writes the buffer to the block numbered blocknumber in file. It returns an appropriate error code.

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
    disk.putblock(blocknumber,buffer);
    return 1;
    
}

// returns the number of the block that follows blocknumber in file. It will return 0 if blocknumber is the last block and -1 if some other error has occurred (such as file is not in the root directory, or blocknumber is not a block in file.)

int Filesys::nextblock(string file, int blocknumber)
{
    int blockid = getfirstblock(file);
    while (blockid != blocknumber)
    {
        blockid = fat[blockid];
    }
    return fat[blockid];
}





vector<string> Filesys::block(string buffer, int b)
{
    // blocks the buffer into a list of blocks of size b
    vector<string> blocks;
    
    int numberofblocks = 0;
    string tempblock;
    
    if (buffer.size() % b == 0)
    {
        numberofblocks = (int) (buffer.size()/b);
    }
    else
    {
        numberofblocks = (int) ((buffer.size()/b ) +1);
    }
    
    for (int i=0; i<numberofblocks; i++)
    {
        tempblock=buffer.substr((unsigned long) (b*i), (unsigned long) b);
        blocks.push_back(tempblock);
    }
    int lastblock= (int) (blocks.size()-1);
    
    for (int i= (int) blocks[lastblock].size(); i < b; i++)
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
    
    for(int i = 0; i < disk.getnumberofblocks(); i++)
    {
        fatstream << fat[i]<< " ";
        fatbuffer = fatstream.str();
    }
    
    vector <string> blockbuff = block(fatbuffer, disk.getblocksize());
    
    for (int i=0; i < blockbuff.size(); i++)
    {
        
        disk.putblock(2 + i, blockbuff[i]);
    }
    
    // Root
    // Use ostringstream to write FAT to buffer. block the buffer - >
    // Returns vector<string>
    // write the blocks -> 2 -> fatsize + 1
    
    ostringstream rootstream;
    string buffer;
    
    for( int i = 0; i < rootsize; ++i )
    {
        rootstream << filename[i] << " " << firstblock[i] << " ";
        buffer = rootstream.str();
    }
    disk.putblock(1, buffer);
    return 0;
}



bool Filesys::checkblock(string file, int blocknumber)
{
    int blockid = getfirstblock(file);
    if (blockid == -1)
    {
        cout << "File does not exist";
        return false;
    }
    else
    {
        while (blockid != 0)
        {
            if (blockid == blocknumber)
            {
                return true;
            }
            blockid = fat[blockid];
            return false;
        }
    }
    return true;
}





#endif
