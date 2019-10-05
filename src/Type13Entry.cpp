#include "Type13Entry.h"

Type13Entry::Type13Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x2C) return;
    Util u;
    string dum;
    size_t pos = 1;
    // read CompleteID
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    cID = CompleteID(dum);
    pos+=20;
    // read CompleteID of pred
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    cIDpred = CompleteID(dum);
    pos+=20;
    // read CompleteID of first
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    cIDfirst = CompleteID(dum);
    pos+=20;
    // check IDs and their relationships
    if ((cIDfirst>cID) || (cIDpred>cID) || (cID==cIDpred && cIDfirst!=cID)
            || cIDpred.getNotary()<=0 || cID.getNotary()<=0 || cIDfirst.getNotary()<=0
            || cIDpred.getTimeStamp()<=0 || cID.getTimeStamp()<=0 || cIDfirst.getTimeStamp()<=0)
    {
        deleteData();
        return;
    }
    // load lenOfSignature
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string lenOfSignatureStr = str.substr(pos,8);
    unsigned long long lenOfSignature = u.byteSeqAsUll(lenOfSignatureStr);
    pos+=8;
    // load signature
    if (l-pos < lenOfSignature)
    {
        deleteData();
        return;
    }
    signatureSeq = str.substr(pos,lenOfSignature);
    pos+=lenOfSignature;

    if (pos != l)
    {
        deleteData();
        return;
    }
    else byteSeq=str;

    // correct signatureSeq by extracting t12eSeq (if necessary)
    if (cID == cIDpred)
    {
        string lenOfT12eStr = signatureSeq.substr(0,8);
        unsigned long long lenOfT12e = u.byteSeqAsUll(lenOfT12eStr);
        t12eSeq = signatureSeq.substr(8, lenOfT12e);
        pos = lenOfT12e+8;
        signatureSeq = signatureSeq.substr(pos, signatureSeq.length()-pos);
    }
    else t12eSeq="";

    if (!isGood())
    {
        deleteData();
        return;
    }
}

Type13Entry::~Type13Entry()
{
    deleteData();
}

bool Type13Entry::isGood()
{
    return (signatureSeq.length()>0 && byteSeq.length()>0);
}

void Type13Entry::deleteData()
{
    byteSeq="";
    signatureSeq="";
    t12eSeq="";
}

CompleteID Type13Entry::getCompleteID()
{
    if (!isGood()) return CompleteID();
    return cID;
}

CompleteID Type13Entry::getPredecessorID()
{
    if (!isGood()) return CompleteID();
    return cIDpred;
}

CompleteID Type13Entry::getFirstID()
{
    if (!isGood()) return CompleteID();
    return cIDfirst;
}

string* Type13Entry::getSignature()
{
    if (!isGood()) return nullptr;
    return &signatureSeq;
}

string* Type13Entry::getByteSeq()
{
    if (!isGood()) return nullptr;
    return &byteSeq;
}

unsigned long Type13Entry::getNotary()
{
    if (!isGood()) return 0;
    return cID.getNotary();
}
