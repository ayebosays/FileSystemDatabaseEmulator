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
{   // (string diskname, int numberofblocks, int blocksize)
    //
    //This main program inputs commands to the shell.
    //It inputs commands as : command op1 op2
    //You should modify it to work for your implementation.
    //
    string filename = "disk";
    int blocksize = 128;
    int numberofblocks = 256;
    
    Shell(filename, blocksize, numberofblocks);
    string s;
    string command="go";
    string op1,op2;
    Shell shll;
    
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
            cout << "dir" << endl;
            shll.dir();
            // use the ls function
            
        }
        if (command=="add")
        {
            // The variable op1 is the new file
            cout << "What is the file you wish to add?:";
            shll.add(op1);
            
        }
        if (command=="del")
        {
            cout << "del" << endl;
            shll.del(op1);
            // The variable op1 is the file
        }
        if (command=="type")
        {
            cout << "type" << endl;
            // The variable op1 is the file
        }
        if (command=="copy")
        {
            cout << "copy" << endl;
            shll.copy(op1,op2);
            // The variable op1 is the source file and the variable op2 is the destination file.
        }
        if (command=="exit")
        {
            cout << "Exiting now..." << endl;
            return 0;
        }
        
    }
    
    return 0;
}



