/*
 
 Brandon Saunders
 April 25th, 2015
 
 C++ FAT File System Emulation
 Instantiated the Filesys constructor and sync() member function. Implemented a test in the main paired with the original
 test for Sdisk.
 
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
    int getnumberofblocks() { return numberofblocks; }              // Returns the number of blocks.
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
    //fsys(string,int,int);
    int fsclose();
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
    
    int fssync();                                                   //writes the Root and FAT to the disk.
    string buffer;
    int rootsize;                   // maximum number of entries in ROOT
    int fatsize;                    // number of blocks occupied by FAT
    vector<string> filename;        // filenames in ROOT
    vector<int> firstblock;         // firstblocks in ROOT parallel
    vector<int> fat;                // FAT # of blocks
};

Sdisk::Sdisk(string disk)
{
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
        spcfile << numberofblocks << " " << blocksize;
        cout << "The SPC file " << diskname.c_str() << " was created" << endl;
        cout << "The DAT file " << diskname.c_str() << " was created" << endl;
        
        for (int i=0; i<numberofblocks*blocksize; i++)
        {
            datfile.put('#');           // Fills the file with '#' character.
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
    checkfile.open((this->diskname).c_str(), ios::in|ios::out);
    if (checkfile.bad())
    {
        cout << "Cannot open the file" << endl;
    }
    else
    {
        fstream iofile;
        iofile.open((this->diskname).c_str());
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
    rootsize = getblocksize()/12;
    fatsize = (getnumberofblocks()*5/getblocksize())+1;
    cout << "rootside: " << rootsize << endl << "fatsize: " << fatsize << endl << "number of blocks: " <<  getnumberofblocks() << endl << getblocksize() << endl;
    
    
    /*
    string buffer;
    getblock(1,buffer);
    if (buffer[0] == "#")
    {
        //add filesystem
        // build root vectors. filename "XXXXXX", firstblock 0.
        // build FAT
        // run fssync - writes the FAT, and Root.
    }
    else
    {
        //read filesystem
        // 1. read block 1 containing root.
        // 2. use istringstream to input filename in first block.
        // 3. read blocks 2-k (k is Fatsize + 1)
        // 4. Use istringstream to extract FAT.
    }
    */
     
    for(int i=0; i<rootsize; i++)
    {
        filename.push_back("XXXXX");
        firstblock.push_back(0);

    }
    

    int k= getnumberofblocks();
    fat.push_back(fatsize);
    int j=0;
    
    while( j < fatsize - 1)
        
    {
        fat.push_back(0);
        j++;
    }
    
    for(int i = fatsize; i < k; i++)
        
    {
        fat.push_back(i+1);
    }
    fat[fat.size()-1] = 0;
    fssync();
}

/*
 Filesys::Filesys(string diskname): Sdisk(diskname)
 {
	rootsize=512;
	fatsize=512;
	for(int i=0; i<rootsize; i++)
	{
 filename.push_back("XXXXX");
	}
	for(int i=0; i<rootsize; i++)
	{
 firstblock.push_back(0);
	}
 
	fat.push_back(3);
	fat.push_back(1);
	fat.push_back(1);
	for(int i=3; i<fatsize; i++)
	{
 fat.push_back(i+1);
	}
	fssynch();
 }

 */


// fsclose() - This module writes FAT and ROOT to the pdisk and sets dirty=0 (closing the pdisk).
int Filesys::fsclose() { return 0;}



// newfile - use filename. check if file exists. find first "XXXXXX" entry and change to filename.
// firstblock[i] = 0. fssync()
int Filesys::newfile(string file)

{
    return 0;
}



// removefile - use filename to find file. if firstblock[ ] != 0 - return an error message. Cannot remove a file if it's not empty.
// else - change filename[ ] to "XXXXXX".
// fssync(). rootsync and fatsync. have them seperate.
// This function removes the entry file from ROOT if the file is empty (first block is 0). It returns error codes of 1 if successful and 0 otherwise (not empty or file does not exist).
int Filesys::rmfile(string file){return 0;}



// get firstblock - This function returns the block number of the first block in file. It returns the error code of 0 if the file does not exist.


int Filesys::getfirstblock(string file)
{
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename[i] == file)
        {
            return firstblock[i];
        }
        
        cout << "file does not exist: " << file << endl;
        return -1;
    }
    return 0;
}

// addblock - This function adds a block of data stored in the string buffer to the end of file F and returns the block number. It returns error code of 1 if successful, 0 if the file does not exist, and returns -1 if there are no available blocks (file system is full!).


int Filesys::addblock(string file, string block)
{
    int id = getfirstblock(file);
    if (id == -1)
    {
        return -1;
        cout << "No such file: " << file;
    }
    int allocate = fat[0];
    if (allocate == 0)
    {
        cout << "No space available";
        return -1;
    }
    if (id == 0)
    {
        cout << "file is empty";
        for (int i = 0; i < filename.size(); i++)
        {
            if (filename[i] == file)
            {
                firstblock[i] = allocate;
            } // file was empty
        }
    }
    else
    {
        int nextblock = id;
        while (fat[nextblock] != 0)
        {
            nextblock = fat[nextblock];
        }
        fat[nextblock] = allocate;
    }
    
    
    fat[0] = fat[fat[0]];
    fat[allocate] = 0;
    fssync(); //sync the root and fat.
    putblock(allocate, block);
    return 0;
}






int Filesys::delblock(string file, int blocknumber){return 0;}
int Filesys::readblock(string file, int blocknumber, string& buffer){return 0;}
int Filesys::writeblock(string file, int blocknumber, string buffer){return 0;}
int Filesys::nextblock(string file, int blocknumber){return 0;}

vector<string> Filesys::block(string buffer, int b)
{
 // blocks the buffer into a list of blocks of size b
     vector<string> blocks;
     
     int numberofblocks = 0;
     
     if (buffer.length() % b == 0)
     {
         numberofblocks= buffer.length()/b;
     }
     else
     {
         numberofblocks= buffer.length()/b +1;
     }
     string tempblock;
     
     for (int i=0; i<numberofblocks; i++)
     {
         tempblock=buffer.substr(b*i,b);
         blocks.push_back(tempblock);
     }
     int lastblock=blocks.size()-1;
 
     for (int i=blocks[lastblock].length(); i<b; i++)
     {
         blocks[lastblock]+="#";
     }
     return blocks;
     }


//writes the Root and FAT to the disk.
//This module writes FAT and ROOT to the sdisk. It should be used every time FAT and ROOT are modified.

int Filesys::fssync()
{
    // FAT
    // Uses ostringstream to write root vectors filename, firstblock -> buffer.
    // block the buffer -> write to block1 of Sdisk.
    
    ostringstream fatstream;
    string fatbuffer;
    
    for(int i=0; i < getnumberofblocks(); i++)
    {
        fatstream << fat[i]<<" ";
        fatbuffer = fatstream.str();
    }
    
    vector <string> blockbuff = block(fatbuffer, getblocksize());
    
    for (int i=0; i < blockbuff.size(); i++)
    {
        
        putblock(2+i, blockbuff[i]);
    }
    
    // Root
    // Use ostringstream to write FAT to buffer. block the buffer - >
    // Returns vector<string>
    // write the blocks -> 2 -> fatsize + 1
    
    ostringstream outstream;
    string buffer;
    
    for( int i = 0; i < rootsize; ++i )
    {
        outstream << filename[i] << " " << firstblock[i] << " ";
        buffer = outstream.str();
    }
    putblock(1, buffer); // from the first project
    return 1;
}
/*
 int getblocksize::fssynch()
 {
 ostringstream outstream;
 string buffer;
 
	for( int i = 0; i < rootsize; ++i )
	{
 outstream << filename[i] << " " << firstblock[i] << " ";
 buffer = outstream.str();
	}
	putblock(1, buffer); // from the first project
 
	istringstream instream;
	instream.str(buffer);
 
	string fn;
	int fb; // temp variable to record the data after going into the root directory
	for(int i = 1; i < rootsize; i++)
	{
 instream >> fn >> fb;
 filename.push_back(fn);
 firstblock.push_back(fb);
	}
 
	// Read fat
	string s;
	//string buffer;
	for (int i = 0; i < fatsize; i++)
 {
 getblock(i+2,s);
 buffer +=s;
 }
 //now we read from
 istringstream ibuffer;
 ibuffer.str(buffer);
 
 
	// now read the vector of integers
 int x;
 for(int i = 0; i<getnumberofblocks();i++)
 {
 ibuffer >> x;
 fat.push_back(x);
 }
	return 1;
 }
 */

// Main Instantiation.

int main()
{
    Sdisk disk1("disk1",256,128);
    Filesys fsys("disk1");
    fsys.newfile("file1");
    fsys.newfile("file2");
    string bfile;
    for (int i=1; i<=1024; i++)
    {
        bfile+="1";
    }
    vector<string> blocks=fsys.block(bfile,128);
                 
    int blocknumber=0;
    /*
    for (int i=0; i<=blocks.size(); i++)
    {
        blocknumber=fsys.addblock("file1",blocks[i]);
    }
    /*
     
    fsys.delblock("file1",fsys.getfirstblock("file1"));
     
    for (int i=1; i<=2048; i++)
    {
        bfile+="2";
    }
     
    for (int i=0; i<=blocks.size(); i++)
    {
        blocknumber=fsys.addblock("file2",blocks[i]);
    }
     
    fsys.delblock("file2",blocknumber);
    */
     
}


