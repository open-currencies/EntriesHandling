#include "Type3Entry.h"

Type3Entry::Type3Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x22)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 1;
    // read applicant
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    applicant = CompleteID(dum);
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

    if (pos != l || applicant.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }

    byteSeq = str;
}

Type3Entry::Type3Entry(CompleteID &a, string &d) : applicant(a), description(d)
{
    if (applicant.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x22);
    byteSeq.append(applicant.to20Char());
    byteSeq.append(u.UllAsByteSeq(description.length()));
    byteSeq.append(description);
}

Type3Entry::~Type3Entry()
{
    //dtor
}

void Type3Entry::deleteData()
{
    applicant=CompleteID();
    description="";
    byteSeq="";
    referencedEntries.clear();
    currencies.clear();
}

int Type3Entry::getType()
{
    return 3;
}

CompleteID Type3Entry::getOwnerID()
{
    return applicant;
}

string* Type3Entry::getDescription()
{
    if (isGood()) return &description;
    return nullptr;
}

list<CompleteID>* Type3Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(applicant);
    }
    return &referencedEntries;
}

list<CompleteID>* Type3Entry::getCurrenciesAndObligations()
{
    return &currencies;
}

string* Type3Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

bool Type3Entry::isGood()
{
    return applicant.getNotary()>0;
}

Entry* Type3Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type3Entry* out = new Type3Entry(str);
    return out;
}
