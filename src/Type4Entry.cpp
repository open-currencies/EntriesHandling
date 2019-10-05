#include "Type4Entry.h"

Type4Entry::Type4Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x23)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 1;
    // read notaryNr
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    notaryNr = u.byteSeqAsUl(dum);
    pos+=4;
    // read thread
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    thread = CompleteID(dum);
    pos+=20;
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
    if (l-pos < lenOfDescription || lenOfDescription < 1)
    {
        deleteData();
        return;
    }
    description = str.substr(pos,lenOfDescription);
    pos+=lenOfDescription;

    if (pos != l || notaryNr<=0 || thread.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }

    byteSeq = str;
}

Type4Entry::Type4Entry(unsigned long n, CompleteID &t, string &d) : notaryNr(n), thread(t), description(d)
{
    if (notaryNr<=0 || thread.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x23);
    byteSeq.append(u.UlAsByteSeq(notaryNr));
    byteSeq.append(thread.to20Char());
    byteSeq.append(u.UllAsByteSeq(description.length()));
    byteSeq.append(description);
}

Type4Entry::~Type4Entry()
{
    //dtor
}

void Type4Entry::deleteData()
{
    notaryNr=0;
    thread=CompleteID();
    byteSeq="";
    description="";
    referencedEntries.clear();
    currencies.clear();
}

unsigned long Type4Entry::getNotaryNr()
{
    return notaryNr;
}

int Type4Entry::getType()
{
    return 4;
}

CompleteID Type4Entry::getPreviousThreadEntry()
{
    return thread;
}

CompleteID Type4Entry::getOwnerID()
{
    return CompleteID();
}

list<CompleteID>* Type4Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(thread);
    }
    return &referencedEntries;
}

list<CompleteID>* Type4Entry::getCurrenciesAndObligations()
{
    return &currencies;
}

string* Type4Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

string* Type4Entry::getDescription()
{
    if (isGood()) return &description;
    return nullptr;
}

bool Type4Entry::isGood()
{
    return notaryNr>0;
}

Entry* Type4Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type4Entry* out = new Type4Entry(str);
    return out;
}
