#include "Type12Entry.h"

Type12Entry::~Type12Entry()
{
    deleteData();
}

int Type12Entry::underlyingType()
{
    // return the type of the underling entry
    if (underlyingEntryStr.length() < 2) return 0;
    else if (underlyingEntryStr.at(0)==0x2A) return 11;
    else if (underlyingEntryStr.at(0)==0x21) return 2;
    else if (underlyingEntryStr.at(0)==0x22) return 3;
    else if (underlyingEntryStr.at(0)==0x23) return 4;
    else if (underlyingEntryStr.at(0)==0x24) return 5;
    else if (underlyingEntryStr.at(0)==0x25) return 6;
    else if (underlyingEntryStr.at(0)==0x26) return 7;
    else if (underlyingEntryStr.at(0)==0x27) return 8;
    else if (underlyingEntryStr.at(0)==0x28) return 9;
    else if (underlyingEntryStr.at(0)==0x29) return 10;
    else if (underlyingEntryStr.at(0)==0x2D) return 14;
    else if (underlyingEntryStr.at(0)==0x2E) return 15;
    else return 0;
}

CompleteID Type12Entry::pubKeyID()
{
    if (!isGood() || underlyingType() == 11) return CompleteID();
    // return the complete ID of the signatory
    return cID;
}

string* Type12Entry::getSignedSequence()
{
    if (!isGood()) return nullptr;
    return &signedSequence;
}

string* Type12Entry::getSignature()
{
    if (!isGood()) return nullptr;
    return &signatureSeq;
}

Entry* Type12Entry::underlyingEntry()
{
    if (!isGood()) return nullptr;
    return entry;
}

unsigned long Type12Entry::getFirstNotaryNr()
{
    if (!isGood()) return 0;
    return notaryNr;
}

bool Type12Entry::isGood()
{
    return (underlyingEntryStr.length()>0 && signatureSeq.length()>0
            && byteSeq.length()>0 && entry!=nullptr && entry->isGood());
}

string* Type12Entry::getByteSeq()
{
    if (!isGood()) return nullptr;
    return &byteSeq;
}

unsigned long long Type12Entry::getNotarizationTimeLimit()
{
    if (!isGood()) return 0;
    return timeLimit;
}

Type12Entry::Type12Entry(string &str) : entry(nullptr)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x2B)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 1;
    // load lenOfEntry
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string lenOfEntryStr = str.substr(pos,8);
    unsigned long long lenOfEntry = u.byteSeqAsUll(lenOfEntryStr);
    pos+=8;
    size_t signedSequenceStart = pos;
    // load entry
    if (l-pos < lenOfEntry)
    {
        deleteData();
        return;
    }
    underlyingEntryStr = str.substr(pos,lenOfEntry);
    pos+=lenOfEntry;
    // read CompleteID of signatory
    if (underlyingType() != 11)
    {
        if (l-pos < 20)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,20);
        cID = CompleteID(dum);
        pos+=20;
    }
    // read first notary nr
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    notaryNr = u.byteSeqAsUl(dum);
    pos+=4;
    // read time limit
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    timeLimit = u.byteSeqAsUll(dum);
    pos+=8;
    // read signed sequence
    signedSequence = str.substr(signedSequenceStart, pos-signedSequenceStart);
    // read signature length
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string lenOfSigStr = str.substr(pos,8);
    unsigned long long lenOfSig = u.byteSeqAsUll(lenOfSigStr);
    pos+=8;
    // read signature
    if (l-pos < lenOfSig)
    {
        deleteData();
        return;
    }
    signatureSeq = str.substr(pos,lenOfSig);
    pos+=lenOfSig;

    if (pos != l)
    {
        deleteData();
        return;
    }
    else byteSeq=str;

    if (underlyingType() == 15)
    {
        Type15Entry *type15entry = new Type15Entry(underlyingEntryStr);
        entry = type15entry;
    }
    else if (underlyingType() == 14)
    {
        Type14Entry *type14entry = new Type14Entry(underlyingEntryStr);
        entry = type14entry;
    }
    else if (underlyingType() == 11)
    {
        Type11Entry *type11entry = new Type11Entry(underlyingEntryStr);
        entry = type11entry;
    }
    else if (underlyingType() == 10)
    {
        Type10Entry *type10entry = new Type10Entry(underlyingEntryStr);
        entry = type10entry;
    }
    else if (underlyingType() == 9)
    {
        Type9Entry *type9entry = new Type9Entry(underlyingEntryStr);
        entry = type9entry;
    }
    else if (underlyingType() == 8)
    {
        Type8Entry *type8entry = new Type8Entry(underlyingEntryStr);
        entry = type8entry;
    }
    else if (underlyingType() == 7)
    {
        Type7Entry *type7entry = new Type7Entry(underlyingEntryStr);
        entry = type7entry;
    }
    else if (underlyingType() == 6)
    {
        Type6Entry *type6entry = new Type6Entry(underlyingEntryStr);
        entry = type6entry;
    }
    else if (underlyingType() == 5)
    {
        Type5Entry *type5entry = new Type5Entry(underlyingEntryStr);
        entry = type5entry;
    }
    else if (underlyingType() == 4)
    {
        Type4Entry *type4entry = new Type4Entry(underlyingEntryStr);
        entry = type4entry;
    }
    else if (underlyingType() == 3)
    {
        Type3Entry *type3entry = new Type3Entry(underlyingEntryStr);
        entry = type3entry;
    }
    else if (underlyingType() == 2)
    {
        Type2Entry *type2entry = new Type2Entry(underlyingEntryStr);
        entry = type2entry;
    }

    if (entry == nullptr || !entry->isGood())
    {
        deleteData();
        return;
    }

    if (!isGood())
    {
        deleteData();
        return;
    }
}

void Type12Entry::deleteData()
{
    underlyingEntryStr = "";
    signedSequence = "";
    signatureSeq = "";
    byteSeq = "";
    if (entry!=nullptr)
    {
        delete entry;
        entry=nullptr;
    }
}
