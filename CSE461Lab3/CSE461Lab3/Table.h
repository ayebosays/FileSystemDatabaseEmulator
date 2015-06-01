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

class Table: public Filesys
{
    public :
        Table(string diskname,int numberofblocks,int blocksize, string flatfile, string indexfile);
        int Build_Table(string input_file);
        int Search(string value);

    private :
        string flatfile;
        string indexfile;
        int numberofrecords;
        int IndexSearch(string value);
};




//This constructor creates the table object. It creates the new (empty) files flatfile and indexfile in the file system on the Sdisk using diskname.

Table::Table(string diskname,int numberofblocks,int blocksize, string flatfile, string indexfile):Filesys(diskname,blocksize,numberofblocks)
{
    this->indexfile = indexfile;
    this->flatfile = flatfile;
    newfile(flatfile);
    newfile(indexfile);
}


//This module will read records from the input file (the raw data file described above), add the records to the flatfile and create index records consisting of the date and block number, and then add the index records to the index file. (Note that index records will have 10 bytes .. 5 bytes for the date and 5 bytes for the block number.)

int Table::Build_Table(string input_file)
{
    string block, record, date;
    unsigned long x;
    vector<string> blocks;
    stringstream ss;
    ifstream file(input_file.c_str());
    
    if(!file.good())
    {
        cout << "build_table(): Error could not open input file\n";
        return -1;
    }
    
    
    getline(file, record);
    while(!file.eof())
    {
        x = record.find("*");
        date = record.substr(0,x);
        //remove white spaces
        x = date.find(" ");
        if(x != string::npos)
            date = date.substr(0,x);
        
        ss << date << " " ;
        
        int flatfile_block_num = addblock(flatfile,record);
        ss << flatfile_block_num << " ";
        addblock(indexfile, ss.str());
        block.clear();
        ss.str("");
        ss.clear();
        
        getline(file, record);
    }
    return 1;
  
}
        
        
        
// This module accepts a key value, and searches the index file with a call to IndexSearch for the record where the date matches the specified value. IndexSearch returns the blocknumber of the block in the flat file where the target record is located. This block should then be read and the record displayed.

int Table::Search(string value)
{
    
    string buffer, date, end, place, type, ref, desc;
    vector <string> rec;
    
    
    signed long block_number = IndexSearch(value), lastPos, pos;
    
    if(block_number == -1)
    {
        cout << "The record could not be found. " << endl;
        return -1;
    }
    getblock(flatfile,block_number,buffer);
    
    lastPos = buffer.find_first_not_of("*",0);
    pos = buffer.find_first_of("*", lastPos);
    
    while (pos != string::npos || lastPos != string::npos)
    {
        rec.push_back( buffer.substr(lastPos, pos-lastPos));
        lastPos = buffer.find_first_not_of("*", pos);
        pos = buffer.find_first_of("*", lastPos);
    }
    
    cout << "Record found: " << endl;
    cout << "Date: " << rec.at(0) << endl;
    cout << "End: " << rec.at(1) << endl;
    cout << "Type: " << rec.at(2) << endl;
    cout << "Place: " << rec.at(3) << endl;
    cout << "Reference: " << rec.at(4) << endl;
    cout << "Description: " << rec.at(5) << endl;
    
    return 0;
}

//This module accepts a key value, and searches the index file indexfile for the record where the date matches the specified value. IndexSearch then returns the block number key of the index record where the match occurs.
        
int Table::IndexSearch(string value)
{
    //root, value;
    
    int current_block = getfirstblock(indexfile), blk_num;
    

    while( current_block != 0)
    {
        string block, date;
        stringstream ss;
        getblock(indexfile, current_block, block);
        ss.str(block);
        ss >> date >> blk_num;
        
        if(date == value)
        {
            cout << "blk_num " << blk_num << endl;
            return blk_num;
        }
        
        current_block = nextblock(indexfile, current_block);
    }
    
    return -1;
    //return current_block;
}


#endif /* defined(__CSE461Lab3__Table__) */
