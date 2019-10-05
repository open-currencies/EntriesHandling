#include "Type15Entry.h"

Type15Entry::Type15Entry(string &str)
{
    createFromString(str);
}

void Type15Entry::createFromString(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x2E) return;
    string dum;
    size_t pos = 1;
    // read ownerId
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    ownerId = CompleteID(dum);
    ownerIdAsList.push_back(ownerId);
    pos+=20;
    // read rate
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string rateStr = str.substr(pos,8);
    rate = u.byteSeqAsDbl(rateStr);
    pos+=8;
    // read minTransfer
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string minTransferStr = str.substr(pos,8);
    minTransfer = u.byteSeqAsDbl(minTransferStr);
    pos+=8;
    // read description length
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string lenOfDescriptionStr = str.substr(pos,8);
    unsigned long long lenOfDescription = u.byteSeqAsUll(lenOfDescriptionStr);
    pos+=8;
    // load description
    if (l-pos < lenOfDescription || lenOfDescription < 1 || lenOfDescription>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
    description = str.substr(pos,lenOfDescription);
    pos+=lenOfDescription;

    if (l-pos == 2)
    {
        if (str.at(pos) == 0x04 || str.at(pos) == 0x0A || str.at(pos) == 0x0D) pos+=1;
    }
    if (l-pos == 1)
    {
        if (str.at(pos) == 0x04 || str.at(pos) == 0x0A || str.at(pos) == 0x0D) pos+=1;
    }

    if (pos != l || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
}

// NOTE: for mingw: replace "stat" with "_stat" for struct and function

Type15Entry::Type15Entry(const string& dirName, const string& fileName)
{
    string t15eSuffix(".t15e");
    if (u.has_suffix(fileName, t15eSuffix))
    {
        string path(dirName);
        path.append(fileName);
        ifstream f(path.c_str(), ios::binary);
        if (f.good())
        {
            // load byte sequence
            ostringstream ostrm;
            ostrm << f.rdbuf();
            string str(ostrm.str());
            f.close();
            createFromString(str);
            return;
        }
        else
        {
            puts(u.c(path, " could not be read").c_str());
            f.close();
            deleteData();
            return;
        }
    }

    // try to load from json file
    string fileNameJ(dirName);
    fileNameJ.append(fileName);
    struct stat fJInfo;
    if (stat(fileNameJ.c_str(), &fJInfo) != 0)
    {
        puts(u.c(fileNameJ, " could not be found").c_str());
        return;
    }
    ifstream fJ(fileNameJ.c_str());
    if (!fJ.good())
    {
        puts(u.c(fileNameJ, " could not be read").c_str());
        fJ.close();
        return;
    }
    json j;
    fJ >> j;
    fJ.close();

    // load data
    string ownerIdStr = j["ownerId"];
    ownerId = CompleteID(ownerIdStr);
    if (ownerId.getNotary()<=0)
    {
        deleteData();
        return;
    }
    ownerIdAsList.push_back(ownerId);
    rate = j["rate"];
    minTransfer = j["minTransfer"];

    // read description from specified file
    string descriptionFileName(dirName);
    descriptionFileName.append(j["descriptionFile"]);
    struct stat fileInfo;
    if (stat(descriptionFileName.c_str(), &fileInfo) != 0)
    {
        puts(u.c(descriptionFileName, " could not be found").c_str());
        deleteData();
        return;
    }
    ifstream fstrm(descriptionFileName.c_str(), ios::binary);
    if (!fstrm.good())
    {
        puts(u.c(descriptionFileName, " could not be read").c_str());
        fstrm.close();
        deleteData();
        return;
    }
    ostringstream ostrm;
    ostrm << fstrm.rdbuf();
    description = ostrm.str();
    fstrm.close();

    if (description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }

    if (!isGood()) deleteData();
}

Type15Entry::~Type15Entry()
{
    deleteData();
}

CompleteID Type15Entry::getOwnerID()
{
    return ownerId;
}

list<CompleteID>* Type15Entry::getReferencedEntries()
{
    return &ownerIdAsList;
}

list<CompleteID>* Type15Entry::getCurrenciesAndObligations()
{
    return nullptr;
}

void Type15Entry::deleteData()
{
    ownerId=CompleteID();
    ownerIdAsList.clear();
    description="";
    rate=0;
    minTransfer=0;
    byteSeq="";
}

double Type15Entry::getMinTransferAmount()
{
    return minTransfer;
}

double Type15Entry::getRate()
{
    return rate;
}

bool Type15Entry::isGood()
{
    return (ownerId.getNotary()>0 && description.length()>0);
}

int Type15Entry::getType()
{
    return 15;
}

string* Type15Entry::getDescription()
{
    return &description;
}

string* Type15Entry::getByteSeq() // without the EOF byte
{
    if (!isGood()) return nullptr;
    if (byteSeq.length()<2)
    {
        byteSeq = "";
        byteSeq.push_back(0x2E);
        byteSeq.append(ownerId.to20Char());
        byteSeq.append(u.DblAsByteSeq(rate));
        byteSeq.append(u.DblAsByteSeq(minTransfer));
        byteSeq.append(u.UllAsByteSeq(description.length()));
        byteSeq.append(description);
    }
    return &byteSeq;
}

Entry* Type15Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type15Entry* out = new Type15Entry(str);
    return out;
}
