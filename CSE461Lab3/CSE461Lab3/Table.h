//
//  Table.h
//  CSE461Lab3
//
//  Created by Brandon Saunders on 5/24/15.
//  Copyright (c) 2015 Brandon Saunders. All rights reserved.
//

#ifndef __CSE461Lab3__Table__
#define __CSE461Lab3__Table__
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

class Table
{
    public :
        Table(string diskname,int blocksize,int numberofblocks, string flatfile, string indexfile);
        int Build_Table(string input_file);
        int Search(string value);
        Sdisk disk;
        Filesys filesystem;

    private :
        string flatfile;
        string indexfile;
        int numberofrecords;
        int IndexSearch(string value);
};




//This constructor creates the table object. It creates the new (empty) files flatfile and indexfile in the file system on the Sdisk using diskname.

Table::Table(string diskname,int blocksize,int numberofblocks, string flatfile, string indexfile)
{
    Sdisk disk(diskname, numberofblocks, blocksize);
    this->filesystem = Filesys();
    filesystem.start(Sdisk(diskname));
    
    this->indexfile = indexfile;
    this->flatfile = flatfile;
    filesystem.newfile(flatfile);
    filesystem.newfile(indexfile);
    
}


//This module will read records from the input file (the raw data file described above), add the records to the flatfile and create index records consisting of the date and block number, and then add the index records to the index file. (Note that index records will have 10 bytes .. 5 bytes for the date and 5 bytes for the block number.)

int Table::Build_Table(string input_file)
{
    vector<int> iblock;
    vector<string> ikey;
    ifstream infile;
    infile.open(input_file.c_str());
    string rec;
    getline(infile,rec);
    
    if (!infile.good())
    {
        cout << "Was not able to build the table" << endl;
        return -1;
    }
    
    while (infile.good())
    {
        string primkey = rec.substr(0,5); // Starting at position 0, 5 characters.
        vector<string> blocks = filesystem.block(rec, disk.getblocksize());
        int blockid = filesystem.addblock(flatfile, blocks[0]);
        ikey.push_back(primkey);
        iblock.push_back(blockid);
        getline(infile,rec);
        if(infile.bad() && ikey.size() > 0)
        {
            // write a block to the index file.
            ostringstream ibuffer;
            for(int i = 0; i < ikey.size(); i++)
            {
                ibuffer << ikey[i] << " " << iblock[i] << " ";
            }
            string buffer = ibuffer.str();
            vector<string> blocks2 = filesystem.block(buffer, disk.getblocksize());
            int error = filesystem.addblock(indexfile,blocks2[0]);
            ikey.clear();
            iblock.clear();
        }
    }
    
    
    
   
    
    return 1;
}
        
        
        
// This module accepts a key value, and searches the index file with a call to IndexSearch for the record where the date matches the specified value. IndexSearch returns the blocknumber of the block in the flat file where the target record is located. This block should then be read and the record displayed.

int Table::Search(string value)
{
    string buffer, date, end, place, type, ref, desc;
    vector <string> record;
    
    int block_number = IndexSearch(value), lastPos, pos;
    if(block_number == -1)
    {
        cout << "The record could not be found. " << endl;
        return -1;
    }
    Filesys fs;
    fs.getfirstblock(flatfile);
    
    
    cout << "Record found: " << endl;
    cout << "Date: " << record.at(0) << endl;
    cout << "End: " << record.at(1) << endl;
    cout << "Type: " << record.at(2) << endl;
    cout << "Place: " << record.at(3) << endl;
    cout << "Reference: " << record.at(4) << endl;
    cout << "Description: " << record.at(5) << endl;
    
    return 1;
}

//This module accepts a key value, and searches the index file indexfile for the record where the date matches the specified value. IndexSearch then returns the block number key of the index record where the match occurs.

        
int Table::IndexSearch(string value)
{
    //root, value;
    return 1;
}


#endif /* defined(__CSE461Lab3__Table__) */
