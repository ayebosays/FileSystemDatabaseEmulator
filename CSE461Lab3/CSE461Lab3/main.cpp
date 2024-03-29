/*
 
 Brandon Saunders
 April 25th, 2015
 
 C++ FAT File System Emulation
 Instantiated the Filesys constructor and sync() member function. Implemented a test in the main paired with the original
 test for Sdisk.
 
 */
#include "Filesys.h"
#include "Sdisk.h"
#include "Shell.h"
#include "Table.h"
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
    Table TBL("disk1", 256, 128,"flatfile","indexfile");
    TBL.Build_Table("data.txt");
    
    
    while(true)
    {
        cout << "Input the date you wish to search for: ";
        string x;
        cin >> x;
        TBL.Search(x);
        TBL.fsclose();
    }
     
    
    /*
    string filename = "disk";
    int blocksize = 128;
    int numberofblocks = 256;
    
    Shell shll(filename, blocksize, numberofblocks);
    string s;
    string command="go";
    string op1,op2;
    
    while (command != "quit")
    {
        command.clear();
        op1.clear();
        op2.clear();
        cout << "$";
        getline(cin,s);
        unsigned long firstblank = s.find(' ');
        if (firstblank < s.length()) s[firstblank]='#';
        unsigned long secondblank = s.find(' ');
        command=s.substr(0,firstblank);
        if (firstblank < s.length())
            op1=s.substr(firstblank+1,secondblank-firstblank-1);
        if (secondblank < s.length())
            op2=s.substr(secondblank+1);
        if (command=="dir")
        {
            shll.dir();
        }
        if (command=="add")
        {
            shll.add(op1);
        }
        if (command=="del")
        {
            shll.del(op1);
        }
        if (command=="type")
        {
            shll.type(op1);
        }
        if (command=="copy")
        {
            shll.copy(op1,op2);
        }
        if (command=="exit")
        {
            cout << "Exiting now..." << endl;
            return 0;
        }
        
    }
     
     */
    
    return 0;
}



