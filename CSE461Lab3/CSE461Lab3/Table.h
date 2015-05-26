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

Table::Table(string diskname, string flatfile, string indexfile)
{
    return 1;
}

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

int Table::IndexSearch(string value)
{
    return 1;
}


#endif /* defined(__CSE461Lab3__Table__) */
