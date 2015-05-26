//
//  Table.h
//  CSE461Lab3
//
//  Created by Brandon Saunders on 5/24/15.
//  Copyright (c) 2015 Brandon Saunders. All rights reserved.
//

#ifndef __CSE461Lab3__Table__
#define __CSE461Lab3__Table__
#include "Filesys"
#include <stdio.h>
#include <string>
using namespace std;

class Table: public Filesys
{
    public :
        Table(string diskname, string flatfile, string indexfile);
        int Build_Table(string input_file);
        int Search(string value);
    private :
        string flatfile;
        string indexfile;
        int numberofrecords;
        Filesys filesystem;
        int IndexSearch(string value);
};




//This constructor creates the table object. It creates the new (empty) files flatfile and indexfile in the file system on the Sdisk using diskname.

Table::Table(string diskname, string flatfile, string indexfile)
{
    fs = diskname;
    flat_file = flatfile;
    index_file = indexfile;
    fs -> newfile(flat_file);
    fs -> newfile(index_file);
    return 1;
}


//This module will read records from the input file (the raw data file described above), add the records to the flatfile and create index records consisting of the date and block number, and then add the index records to the index file. (Note that index records will have 10 bytes .. 5 bytes for the date and 5 bytes for the block number.)

int Table::Build_Table
{
    string block, record, date;
    int blk_num;
    vector<string> blocks, keys;
    vector<int> links;
    stringstream ss;
    ifstream file(in_file.c_str());
    
    if (!file.good())
    {
        cout << "Was not able to build the table" << endl;
        return -1;
    }
        
    getline(file, record);
    blk_num = fs -> addblock(flatfile, record);
    
        getline(file, record);
        blk_num = fs -> addblock(flatfile, record);
        
        
    
    return 1;
}
        
        
        
// This module accepts a key value, and searches the index file with a call to IndexSearch for the record where the date matches the specified value. IndexSearch returns the blocknumber of the block in the flat file where the target record is located. This block should then be read and the record displayed.

int Table::Search
{
    string buffer, date, end, place, type, ref, desc;
    vector <string> record;
    
    int block_number = IndexSearch(value), lastPos, pos;
    if(block_number == -1)
    {
        cout << "The record could not be found. " << endl;
        return -1;
    }
    Filesys -> getblock(flatfile,block_number,buffer);
    
    
    cout << "Record found: " << endl;
    cout << "Date: " << record.at(0) << endl;
    cout << "End: " << record.at(1) << endl;
    cout << "Type: " << record.at(2) << endl;
    cout << "Place: " << record.at(3) << endl;
    cout << "Reference: " << record.at(4) << endl;
    cout << "Description: " << record.at(5) <<< endl;
    
    return 1;
}

//This module accepts a key value, and searches the index file indexfile for the record where the date matches the specified value. IndexSearch then returns the block number key of the index record where the match occurs.

        
int Table::IndexSearch(string value)
{
    //root, value;
    return 1;
}


#endif /* defined(__CSE461Lab3__Table__) */
