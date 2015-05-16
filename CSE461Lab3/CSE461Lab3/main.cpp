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
    Shell("disk",256,128);
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
            cout << "dir" << endl;
            
            // use the ls function
            
        }
        if (command=="add")
        {
            // The variable op1 is the new file
            cout << "add" << endl;
            
        }
        if (command=="del")
        {
            cout << "del" << endl;
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



