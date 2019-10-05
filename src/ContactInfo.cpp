#include "ContactInfo.h"

ContactInfo::ContactInfo(string &str)
{
    if (str.length()<=20)
    {
        deleteData();
        return;
    }
    Util u;
    size_t pos = 0;
    string dum;
    // get lineage
    dum = str.substr(pos,2);
    unsigned short lineage = u.byteSeqAsUs(dum);
    pos+=2;
    // get notary nr
    dum = str.substr(pos,4);
    unsigned short notaryNr = u.byteSeqAsUl(dum);
    pos+=4;
    tNtrNr = TNtrNr(lineage, notaryNr);
    // get ip length
    dum = str.substr(pos,2);
    unsigned short ipLength = u.byteSeqAsUs(dum);
    pos+=2;
    // get ip
    if (str.length()-pos <= ipLength)
    {
        deleteData();
        return;
    }
    ip = str.substr(pos,ipLength);
    pos+=ipLength;
    // get port and validSince
    if (str.length()-pos <= 12)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    port = u.byteSeqAsUl(dum);
    pos+=4;
    dum = str.substr(pos,8);
    validSince = u.byteSeqAsUll(dum);
    pos+=8;
    // get signature length
    if (str.length()-pos <= 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    unsigned long long signatureLength = u.byteSeqAsUll(dum);
    pos+=8;
    // get signature
    signature = str.substr(pos, signatureLength);
    pos+=signatureLength;

    if (str.length()-pos == 2)
    {
        if (str.at(pos) == 0x04 || str.at(pos) == 0x0A || str.at(pos) == 0x0D) pos+=1;
    }
    if (str.length()-pos == 1)
    {
        if (str.at(pos) == 0x04 || str.at(pos) == 0x0A || str.at(pos) == 0x0D) pos+=1;
    }

    if (pos != str.length())
    {
        deleteData();
        return;
    }

    byteSeqence = str;
}

ContactInfo::ContactInfo(TNtrNr &t, string &i, unsigned long p, unsigned long long v, string &s)
    : tNtrNr(t), ip(i), port(p), validSince(v), signature(s)
{
    Util u;
    byteSeqence.append(u.UsAsByteSeq(tNtrNr.getLineage()));
    byteSeqence.append(u.UlAsByteSeq(tNtrNr.getNotaryNr()));
    byteSeqence.append(u.UsAsByteSeq(ip.length()));
    byteSeqence.append(ip);
    byteSeqence.append(u.UlAsByteSeq(port));
    byteSeqence.append(u.UllAsByteSeq(validSince));
    byteSeqence.append(u.UllAsByteSeq(signature.length()));
    byteSeqence.append(signature);
}

ContactInfo::~ContactInfo()
{
    //dtor
}

void ContactInfo::deleteData()
{
    tNtrNr=TNtrNr(0,0);
    ip="";
    port=0;
    validSince=0;
    signature="";
    byteSeqence="";
}

TNtrNr ContactInfo::getTotalNotaryNr()
{
    return tNtrNr;
}

string* ContactInfo::getSignature()
{
    return &signature;
}

string* ContactInfo::getByteSeq()
{
    return &byteSeqence;
}

string ContactInfo::getIP()
{
    return ip;
}

unsigned long ContactInfo::getPort()
{
    return port;
}

unsigned long long ContactInfo::getValidSince()
{
    return validSince;
}
