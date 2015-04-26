/*
 
 Brandon Saunders
 April 25th, 2015
 
 C++ FAT File System Emulation
 
 */

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;


//Sdisk Class
class Sdisk
{
    public :
    Sdisk(string diskname);                                         // Default Constructor
    Sdisk(string diskname, int numberofblocks, int blocksize);
    int getblock(int blocknumber, string& buffer);
    int putblock(int blocknumber, string buffer);
    int getblocksize() {return blocksize; }                         // Returns the blocksize.
    int getnumberofblocks() { return numberofblocks; }                 // Returns the number of blocks.
    string getfilename() { return diskname; }                       // Returns the disk name.
    
    private :
    
    int numberofblocks;                                             // number of blocks on disk
    string diskname;                                                // file name of pseudo-disk
    int blocksize;                                                  // block size in bytes/the number of blocks.
    
    
};

// Filesys Class
class Filesys: public Sdisk
{
public:
    
    Filesys(string diskname);
    int fsclose();
    int fssync();                                                   //writes the Root and FAT to the disk.
    int newfile(string file);
    int rmfile(string file);
    int getfirstblock(string file);
    int addblock(string file, string block);
    int delblock(string file, int blocknumber);
    int readblock(string file, int blocknumber, string& buffer);
    int writeblock(string file, int blocknumber, string buffer);
    int nextblock(string file, int blocknumber);
    vector<string> block(string buffer, int b);
    
    
    private :
    
    int rootsize;                   // maximum number of entries in ROOT
    int fatsize;                    // number of blocks occupied by FAT
    vector<string> filename;        // filenames in ROOT
    vector<int> firstblock;         // firstblocks in ROOT parallel
    vector<int> fat;                // FAT # of blocks
};

Sdisk::Sdisk(string disk){

    diskname = disk + ".dat";

    string diskname1 = disk + ".spc";

    ifstream ifile;

    if(ifile.good()==true)

    {

        ifile.open(diskname1.c_str());

        ifile>>numberofblocks>>blocksize;

        ifile.close();

    }

    else

    {

        ifile.close();

        int n,b;

        cout<<"enter number of blocks: ";

        cin>>n;

        cout<<endl<<"enter blocksize: ";

        cin>>b;

        Sdisk::Sdisk(disk,n,b);

    }

    

}


// Sdisk default constructor
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
        cout << "The disk named: " << diskname.c_str() << " exists and is now ready to be written to." << endl;
    }
    else // .spc/.dat file creation.
    {
        cout << "The disk: " << diskname.c_str() << "could not be found. " << endl;
        cout << "Both the SPC and DAT file were not found. Creating both now. Please wait...." << endl;
        spcfile.open((this->diskname + ".spc").c_str(),ios::out);
        datfile.open((this->diskname + ".dat").c_str(),ios::out);
        spcfile << numberofblocks << " " << blocksize << endl;
        cout << "The SPC file " << diskname.c_str() << " was created" << endl;
        cout << "The DAT file " << diskname.c_str() << " was created" << endl;
        
        for (int i=0; i<numberofblocks*blocksize; i++) // Fills the file with '#' character.
        {
            datfile.put('#');
        }
    }
    spcfile.close();
    datfile.close();
    return;
}


// Get the Block Size. It returns

// an error code of 0 if unsuccessful and a 1 if it is successful.
int Sdisk::getblock(int blocknumber,string& buffer)
{
    bool good = 0;
    fstream checkfile;
    checkfile.open((this->diskname + ".dat").c_str(), ios::in | ios::out);
    checkfile.seekp(blocksize * blocknumber,ios::beg);
    
    if (checkfile.bad())
    {
        cout << "Cannot open the file" << endl;
    }
    else
    {
        for (int i = 0; i < blocksize;i++)
        {
            char y = checkfile.get();
            buffer = buffer + y;
        }
        good = 1;
        
    }
    checkfile.close();
    return good;
}


// writes the string buffer to block blocknumber.

// It returns an error code of 1 if successful and

// 1 if it's unsuccessful.
int Sdisk::putblock(int blocknumber, string buffer)
{
    bool good = 0;
    fstream checkfile;
    checkfile.open((this->diskname + ".dat").c_str(), ios::in|ios::out);
    if (checkfile.bad())
    {
        cout << "Cannot open the file" << endl;
    }
    else
    {
        fstream iofile;
        iofile.open((this->diskname + ".dat").c_str());
        iofile.seekp(blocksize * blocknumber,ios::beg);
        
        
        for (int i=0; i < blocksize;i++)
        {
            iofile.put(buffer[i]);
            good = 1;
        }
        
    }
    checkfile.close();
    return good;
}

Filesys::Filesys(string diskname): Sdisk(diskname)

{

    rootsize=getblocksize()/10;

    fatsize=getnumberofblocks()*5/getblocksize()+1;;

    cout << rootsize << endl << fatsize << endl << getnumberofblocks() << endl << getblocksize() << endl;

    for(int i=0; i<rootsize; i++)

    {

        filename.push_back("XXXXX");

    }

    for(int i=0; i < rootsize; i++)

    {

        firstblock.push_back(0);

    }

    int k= getnumberofblocks();

    fat.push_back(fatsize);

    int j=0;

    while( j < fatsize - 1)

    {

        fat.push_back(1);

        j++;

    }

    

    for(int i = fatsize; i < k; i++)

    {

        fat.push_back(i+1);

    }

    

    //fssync();

}



//Derived Class Declarations
int Filesys::fsclose() { return 0;}
int Filesys::newfile(string file){return 0;}
int Filesys::rmfile(string file){return 0;}
int Filesys::getfirstblock(string file){return 0;}
int Filesys::addblock(string file, string block){return 0;}
int Filesys::delblock(string file, int blocknumber){return 0;}
int Filesys::readblock(string file, int blocknumber, string& buffer){return 0;}
int Filesys::writeblock(string file, int blocknumber, string buffer){return 0;}
int Filesys::nextblock(string file, int blocknumber){return 0;}

#include <string>
#include <vector>

using namespace std;


/*
 // Blocking cpp.
 vector<string> block(string buffer, int b)
 {
 // blocks the buffer into a list of blocks of size b
 
 vector<string> blocks;
 int numberofblocks=0;
 
 if (buffer.length() % b == 0)
 { numberofblocks= buffer.length()/b;
 }
 else
 { numberofblocks= buffer.length()/b +1;
 }
 
 string tempblock;
 for (int i=0; i<numberofblocks; i++)
 { tempblock=buffer.substr(b*i,b);
 blocks.push_back(tempblock);
 }
 
 int lastblock=blocks.size()-1;
 
 for (int i=blocks[lastblock].length(); i<b; i++)
 { blocks[lastblock]+="#";
 }
 
 return blocks;
 
 }
 */



//writes the Root and FAT to the disk.
//This module writes FAT and ROOT to the sdisk. It should be used every time FAT and ROOT are modified.

int Filesys::fssync()

{

    

    ostringstream fatstream;

    string fatbuffer;

    for(int i=0; i<getnumberofblocks(); i++)

    {

        fatstream << fat[i]<<" ";

        fatbuffer = fatstream.str();

    }

    putblock(1, fatbuffer);

    

    ostringstream outstream;

    string buffer;

    

    for( int i = 0; i < rootsize; ++i )

    {

        outstream << filename[i] << " " << firstblock[i] << " ";

        buffer = outstream.str();

    }

    putblock(0, buffer); // from the first project

    return 1;

}






// Main Instantiation.

int main()
{
    Sdisk disk1("disk1",256,128);
    
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
    
    
    
    Filesys fsys("disk1");
    
    return 0;
}
