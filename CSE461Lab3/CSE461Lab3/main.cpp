/*
 
 Brandon Saunders
 April 25th, 2015
 
 C++ FAT File System Emulation
 Instantiated the Filesys constructor and sync() member function. Implemented a test in the main paired with the original
 test for Sdisk.
 
 */
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


// Main Instantiation.

int main()
{
    Sdisk disk1("disk1",256,128);
    Filesys fsys("disk1");
    fsys.newfile("file1");
    fsys.newfile("file2");
    string bfile1;
    string bfile2;
    
    for (int i=1; i<=1024; i++)
    {
        bfile1 +="1";
    }
    vector<string> blocks= fsys.block(bfile1,128);
    
    int blocknumber = 0;
    
    cout << "Blocks.size() = " << blocks.size() << endl;
    
    for (int i=0; i < blocks.size(); i++)
    {
        blocknumber = fsys.addblock("file1",blocks[i]);
        cout << "i: " << i << endl;
    }
     
     /*
     fsys.delblock("file1",fsys.getfirstblock("file1"));
     
     for (int i=1; i<=2048; i++)
     {
     bfile2 += "2";
     }
     blocks=block(bfile2,128);

      
     for (int i=0; i < blocks.size(); i++)
     {
     blocknumber=fsys.addblock("file2",blocks[i]);
     }
     
     fsys.delblock("file2",blocknumber);
     */
                 
    
}
                 
    

