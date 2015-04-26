/*
 
 Brandon Saunders
 
 This laboratory is designed to get you started with the the second part of the project.
 
 Open your source file and write or copy the code for the class template for the Sdisk.
 Now, copy the interface for the filesystem :
 
 Create function stubs (functions with no code) for the class functions.
 Work first on the Filesys constructor where you have to create the ROOT directory and the FAT.
 Work next on the fssync that writes the Root and FAT to the disk.
  
 
 #include <string>
 #include <vector>
 
 using namespace std;
 
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






#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

//Base Class
class Sdisk
{
    public :
    
    Sdisk(string diskname, int numberofblocks, int blocksize);
    int getblock(int blocknumber, string& buffer);
    int putblock(int blocknumber, string buffer);
    int getblocksize() {return blocksize;}
    int getblocknumber() { return numberofblocks; }
    string getfilename() { return diskname;}
    int numberofblocks;     // number of blocks on disk
    string diskname;        // file name of pseudo-disk
    int blocksize;          // block size in bytes
    
    private :
    
    
};

//Derived Class
class Filesys: public Sdisk
{
public:
    
    Filesys(string filename);
    int fsclose();
    int fssync();
    int newfile(string file);
    int rmfile(string file);
    int getfirstblock(string file);
    int addblock(string file, string block);
    int delblock(string file, int blocknumber);
    int readblock(string file, int blocknumber, string& buffer);
    int writeblock(string file, int blocknumber, string buffer);
    int nextblock(string file, int blocknumber);
    
    private :
    
    int rootsize;           // maximum number of entries in ROOT
    int fatsize;            // number of blocks occupied by FAT
    vector<string> filename;   // filenames in ROOT
    vector<int> firstblock; // firstblocks in ROOT parallel
    vector<int> fat;             // FAT # of blocks
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
        cout << "The disk: " << diskname.c_str() << "exists and can be written to succesfully" << endl;
        spcfile.open((this->diskname + ".spc").c_str(),ios::out);
        spcfile << numberofblocks << " " << blocksize << endl;

    }
    else
    {
        cout << "The disk: " << diskname.c_str() << "currently is unavailable." << endl;
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


// create the ROOT directory and the FAT.
Filesys::Filesys(string fname):Sdisk(fname,1,12)
{
    int number_of_bytes = 12;
    ostringstream ostream;
    cout << getblocknumber() << endl;
    rootsize = (getblocksize())/(number_of_bytes);
    fatsize = ((5*getblocknumber())/getblocksize() )+ 1;
    cout << "Rootsize " << rootsize << endl;
    cout << "FAT file size " << fatsize << endl;
    
    string buffer;
    getblock(1, buffer);
    if(buffer[0] == '&'){
        //build the initial root
        cout << "Building initial root" << endl;
        for (int i = 0; i < rootsize; i++){
            filename.push_back("XXXXXX");
            firstblock.push_back(0);
        }
        
        //build the FAT
        cout << "Building FAT file system" << endl;
        fat.push_back(2 + fatsize);
        for(int i = 1; i <= 1 + fatsize; i++){
            fat.push_back(1);
        }
        for(int i = 2 + fatsize; i < getblocknumber(); i++){
            fat[i] = i + 1;
            cout << fat[i] << endl;
        }
        fat[fat.size() - 1] = 0;
    }
    else{
        // This is being reached each time.
        //since file exists, read in root and fat
        cout << "The file exists, reading in ROOT and FAT" << endl;
        string buffer;
        istringstream readroot(buffer);
        readroot.str(buffer);
        
        //read the root directory
        for(int i = 0; i < rootsize; i++){
            readroot >> filename[i] >> firstblock[i];
        }
        
        
        //read the FAT
        string fatbuffer;
        for(int i = 0; i < fatsize; i++){
            string s;
            getblock(i + 2, s);
            fatbuffer += s;
        }
        
        istringstream fatstream;
        fatstream.str(fatbuffer); //load buffer into fatstream
        
        for(int i = 0; i < getblocknumber(); i++){
            int temp;
            fatstream >> temp;
            fat.push_back(temp);
        }
        
        //cout << fatbuffer << endl;
    }
    
    fssync();
};



//writes the Root and FAT to the disk.
int Filesys::fssync()
{
    //write updated root to disk
    ostringstream rootstream;
    
    for(int i = 0; i < rootsize; ++i){
        rootstream << filename[i] << " " << firstblock[i] << " " ;
    }
    string root = rootstream.str();
    for(long j = root.size(); j < getblocksize(); j++){
        root += "#";
    }
    putblock(1, root);
    
    //write the FAT onto the disk
    
    ostringstream fatstream;
    
    for(int i = 0; i < getblocknumber(); i++)
    {
        fatstream << fat[i] << " ";
    }
    string fatstring = fatstream.str();
    for(int i = 0; i < getblocksize(); i++)
    {
        fatstring += "#";
    }
    
    //writing the substring of the FAT
    for(int i = 0, blocks = 2; i < fatsize; i++){
        string temp = fatstring.substr(i * getblocksize(), getblocknumber());
        putblock(i + blocks, temp);
    }
    return 1; //successful!
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
    
    
    
    Filesys test1("test1");
    
    return 0;
}
