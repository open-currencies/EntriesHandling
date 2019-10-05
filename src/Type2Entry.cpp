#include "Type2Entry.h"

Type2Entry::Type2Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x21)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 1;
    // read predecessorID
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    predecessorID = CompleteID(dum);
    pos+=20;
    // read terminationID
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    terminationID = CompleteID(dum);
    pos+=20;
    // read key length
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    string lenOfKeyStr = str.substr(pos,4);
    unsigned long lenOfKey = u.byteSeqAsUl(lenOfKeyStr);
    pos+=4;
    // load key
    if (l-pos < lenOfKey || lenOfKey < 1)
    {
        deleteData();
        return;
    }
    publicKey = str.substr(pos,lenOfKey);
    pos+=lenOfKey;

    if (pos != l || terminationID.getNotary()<=0)
    {
        deleteData();
        return;
    }

    byteSeq = str;
}

Type2Entry::Type2Entry(CompleteID &p, CompleteID &t, string &k) : predecessorID(p), terminationID(t), publicKey(k)
{
    if (terminationID.getNotary()<=0 || (!predecessorID.isZero() && predecessorID.getNotary()<=0))
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x21);
    byteSeq.append(predecessorID.to20Char());
    byteSeq.append(terminationID.to20Char());
    byteSeq.append(u.UlAsByteSeq(publicKey.length()));
    byteSeq.append(publicKey);
}

Type2Entry::~Type2Entry()
{
    //dtor
}

CompleteID Type2Entry::getOwnerID()
{
    return CompleteID();
}

list<CompleteID>* Type2Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(terminationID);
        if (!predecessorID.isZero()) referencedEntries.push_back(predecessorID);
    }
    return &referencedEntries;
}

list<CompleteID>* Type2Entry::getCurrenciesAndObligations()
{
    return &currencies;
}

string* Type2Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

bool Type2Entry::isGood()
{
    return terminationID.getNotary()>0;
}

void Type2Entry::deleteData()
{
    terminationID=CompleteID();
    predecessorID=CompleteID();
    publicKey="";

    byteSeq="";
    referencedEntries.clear();
    currencies.clear();
}

Entry* Type2Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type2Entry* out = new Type2Entry(str);
    return out;
}

CompleteID Type2Entry::getPredecessorID()
{
    return predecessorID;
}

CompleteID Type2Entry::getTerminationID()
{
    return terminationID;
}

string* Type2Entry::getPublicKey()
{
    return &publicKey;
}

int Type2Entry::getType()
{
    return 2;
}
