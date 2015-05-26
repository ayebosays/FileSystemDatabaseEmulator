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
    return 0;
}

int Table::Build_Table
{
    return 0;
}

int Table::Search
{
    return 0;
}

int Table::IndexSearch(string value)
{
    return 0;
}


#endif /* defined(__CSE461Lab3__Table__) */
