#include "Type9Entry.h"

Type9Entry::Type9Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x28)
    {
        deleteData();
        return;
    }
    Util u;
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
    pos+=20;
    // read threadId
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    threadId = CompleteID(dum);
    pos+=20;
    // check
    if (pos != l || ownerId.getNotary()<=0 || threadId.getNotary()<=0)
    {
        deleteData();
        return;
    }
    byteSeq = str;
}

Type9Entry::Type9Entry(CompleteID &o, CompleteID &t) : ownerId(o), threadId(t)
{
    if (ownerId.getNotary()<=0 || threadId.getNotary()<=0)
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x28);
    byteSeq.append(ownerId.to20Char());
    byteSeq.append(threadId.to20Char());
}

Type9Entry::~Type9Entry()
{
    //dtor
}

CompleteID Type9Entry::getOwnerID()
{
    return ownerId;
}

int Type9Entry::getType()
{
    return 9;
}

void Type9Entry::deleteData()
{
    ownerId = CompleteID();
    threadId = CompleteID();
    referencedEntries.clear();
    currencies.clear();
}

CompleteID Type9Entry::getPreviousThreadEntry()
{
    return threadId;
}

Entry* Type9Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type9Entry* out = new Type9Entry(str);
    return out;
}

bool Type9Entry::isGood()
{
    return ownerId.getNotary()>0;
}

string* Type9Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

list<CompleteID>* Type9Entry::getCurrenciesAndObligations()
{
    return &currencies;
}

list<CompleteID>* Type9Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(ownerId);
        referencedEntries.push_back(threadId);
    }
    return &referencedEntries;
}
