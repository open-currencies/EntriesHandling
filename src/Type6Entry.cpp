#include "Type6Entry.h"

Type6Entry::Type6Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x25)
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
    // read currency
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    currency = CompleteID(dum);
    pos+=20;
    // read tenureStart
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    tenureStart = u.byteSeqAsUll(dum);
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
    if (l-pos < lenOfDescription || lenOfDescription < 1)
    {
        deleteData();
        return;
    }
    description = str.substr(pos,lenOfDescription);
    pos+=lenOfDescription;

    if (pos != l || applicant.getNotary()<=0 || currency.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }

    byteSeq = str;
}

Type6Entry::Type6Entry(CompleteID &a, CompleteID &c, unsigned long long &t, string &d) :
    applicant(a), currency(c), tenureStart(t), description(d)
{
    if (applicant.getNotary()<=0 || currency.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x25);
    byteSeq.append(applicant.to20Char());
    byteSeq.append(currency.to20Char());
    byteSeq.append(u.UllAsByteSeq(tenureStart));
    byteSeq.append(u.UllAsByteSeq(description.length()));
    byteSeq.append(description);
}

Type6Entry::~Type6Entry()
{
    //dtor
}

void Type6Entry::deleteData()
{
    applicant=CompleteID();
    currency=CompleteID();
    tenureStart=0;
    description="";
    byteSeq="";
    referencedEntries.clear();
    currencies.clear();
}

int Type6Entry::getType()
{
    return 6;
}

CompleteID Type6Entry::getOwnerID()
{
    return applicant;
}

CompleteID Type6Entry::getCurrency()
{
    return currency;
}

unsigned long long Type6Entry::getTenureStart()
{
    return tenureStart;
}

string* Type6Entry::getDescription()
{
    if (isGood()) return &description;
    return nullptr;
}

list<CompleteID>* Type6Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(applicant);
    }
    return &referencedEntries;
}

list<CompleteID>* Type6Entry::getCurrenciesAndObligations()
{
    if (currencies.size()==0 && isGood())
    {
        currencies.push_back(currency);
    }
    return &currencies;
}

string* Type6Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

bool Type6Entry::isGood()
{
    return applicant.getNotary()>0;
}

Entry* Type6Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type6Entry* out = new Type6Entry(str);
    return out;
}
