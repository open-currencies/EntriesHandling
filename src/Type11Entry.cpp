#include "Type11Entry.h"

Type11Entry::Type11Entry(string &str, unsigned long long v) : pubKeyStr(str), validUntil(v)
{
    //ctor
}

Type11Entry::Type11Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x2A) return;
    Util u;
    string dum;
    size_t pos = 1;
    // read validUntil
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    validUntil = u.byteSeqAsUll(dum);
    pos+=8;
    // read key length
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string lenOfKeyStr = str.substr(pos,8);
    unsigned long long lenOfKey = u.byteSeqAsUll(lenOfKeyStr);
    pos+=8;
    // load key
    if (l-pos < lenOfKey)
    {
        deleteData();
        return;
    }
    pubKeyStr = str.substr(pos,lenOfKey);
    pos+=lenOfKey;

    if (pos != l || !isGood())
    {
        deleteData();
        return;
    }
}

void Type11Entry::deleteData()
{
    pubKeyStr = "";
    byteSeq = "";
    validUntil = 0;
}

bool Type11Entry::isGood()
{
    return (pubKeyStr.length() > 0 && validUntil>0);
}

Type11Entry::~Type11Entry()
{
    deleteData();
}

string* Type11Entry::getPublicKey()
{
    return &pubKeyStr;
}

unsigned long long Type11Entry::getValidityDate()
{
    return validUntil;
}

int Type11Entry::getType()
{
    return 11;
}

CompleteID Type11Entry::getOwnerID()
{
    return CompleteID();
}

list<CompleteID>* Type11Entry::getReferencedEntries()
{
    return nullptr;
}

list<CompleteID>* Type11Entry::getCurrenciesAndObligations()
{
    return nullptr;
}

string* Type11Entry::getByteSeq() // without the EOF byte
{
    if (!isGood()) return nullptr;
    if (byteSeq.length()<2)
    {
        Util u;
        byteSeq = "";
        byteSeq.push_back(0x2A);
        byteSeq.append(u.UllAsByteSeq(validUntil));
        byteSeq.append(u.UllAsByteSeq(pubKeyStr.length()));
        byteSeq.append(pubKeyStr);
    }
    return &byteSeq;
}

Entry* Type11Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type11Entry* out = new Type11Entry(str);
    return out;
}
