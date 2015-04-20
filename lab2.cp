
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


class Sdisk
{
    public :
    
    Sdisk(string diskname, int numberofblocks, int blocksize);
    int getblock(int blocknumber, string& buffer);
    int putblock(int blocknumber, string buffer);
    
    private :
    
    string diskname;        // file name of pseudo-disk
    int numberofblocks;     // number of blocks on disk
    int blocksize;          // block size in bytes
};

Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)
{
    this->diskname = diskname;
    this->numberofblocks = numberofblocks;
    this->blocksize = blocksize;
    
    fstream spcfile;
    fstream datfile;
    
    spcfile.open((this->diskname + ".spc").c_str(),ios::in | ios::out);
    datfile.open((this->diskname + ".dat").c_str(),ios::in | ios::out);

    if (spcfile.good() && datfile.good())
    {
        cout << "The disk: " << diskname.c_str() << "exists and can be written to succesfully";
    }
    else
    {
        cout << "The disk: " << diskname.c_str() << "currently is unavailable.";
        spcfile.open((this->diskname + ".spc").c_str(),ios::out);
        datfile.open((this->diskname + ".dat").c_str(),ios::out);
        spcfile << numberofblocks << " " << blocksize << endl;
        
        for (int i=0; i<numberofblocks*blocksize; i++)
        {
            datfile.put('&');
        }
    }
    


    spcfile.close();
    datfile.close();
    return;
}


int Sdisk::getblock(int blocknumber,string& buffer)
{
    bool good = 0;
    fstream checkfile;
    checkfile.open((this->diskname + ".dat").c_str(), ios::in | ios::out);
    checkfile.seekp(blocksize * blocknumber,ios::beg);

    if (checkfile.bad())
    {
        cout << "Cannot open the file";
        
    }

    else
    {
        for (int i = 0; i<blocksize;i++)
        {
            char y = checkfile.get();
            buffer = buffer + y;
        }
        good = 1;
        
    }
    checkfile.close();
    return good;
}



int Sdisk::putblock(int blocknumber, string buffer)
{
    bool good = 0;
    fstream checkfile;
    checkfile.open((this->diskname + ".dat").c_str(), ios::in|ios::out);
    if (checkfile.bad())
    {
        cout << "Cannot open the file";
    }
    else
    {
        fstream iofile;
        iofile.open((this->diskname + ".dat").c_str());
        iofile.seekp(blocksize*blocknumber,ios::beg);
        
        for (int i=0;i<blocksize;i++)
        {
            iofile.put(buffer[i]);
        }
        good = 1;
        }
    checkfile.close();
    return good;
}

// You can use this to test your Sdisk class

int main()
{
    Sdisk disk1("test1",16,32);
    string block1, block2, block3, block4;
    for (int i=1; i<=32; i++) block1=block1+"1";
    for (int i=1; i<=32; i++) block2=block2+"2";
    disk1.putblock(4,block1);
    disk1.getblock(4,block3);
    cout << "Should be 32 1s : ";
    cout << block3 << endl;
    disk1.putblock(8,block2);
    disk1.getblock(8,block4);
    cout << "Should be 32 2s : ";
    cout << block4 << endl;
    return 0;
}
