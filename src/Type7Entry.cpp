#include "Type7Entry.h"

Type7Entry::Type7Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x26)
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
    // read amount
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    amount = u.byteSeqAsDbl(dum);
    pos+=8;
    // read fee
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    fee = u.byteSeqAsDbl(dum);
    pos+=8;
    // read forfeit
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    forfeit = u.byteSeqAsDbl(dum);
    pos+=8;
    // read claim
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    claim = CompleteID(dum);
    pos+=20;
    // read processingTime
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    processingTime = u.byteSeqAsUl(dum);
    pos+=4;
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

    if (pos != l || amount<=0 || fee<=0 || processingTime<=0 || forfeit>=amount || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }

    byteSeq = str;
}

Type7Entry::Type7Entry(CompleteID &a, CompleteID &c, double &am, double &f, double &fo, CompleteID &cl, unsigned long &p, string &d) :
    applicant(a), currency(c), amount(am), fee(f), forfeit(fo), claim(cl), processingTime(p), description(d)
{
    if (amount<=0 || fee<=0 || processingTime<=0 || forfeit>=amount || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x26);
    byteSeq.append(applicant.to20Char());
    byteSeq.append(currency.to20Char());
    byteSeq.append(u.DblAsByteSeq(amount));
    byteSeq.append(u.DblAsByteSeq(fee));
    byteSeq.append(u.DblAsByteSeq(forfeit));
    byteSeq.append(claim.to20Char());
    byteSeq.append(u.UlAsByteSeq(processingTime));
    byteSeq.append(u.UllAsByteSeq(description.length()));
    byteSeq.append(description);
}

Type7Entry::~Type7Entry()
{
    deleteData();
}

void Type7Entry::deleteData()
{
    amount=0;
    fee=0;
    forfeit=0;
    processingTime=0;
    description="";
    applicant=CompleteID();
    currency=CompleteID();
    claim=CompleteID();
    byteSeq="";
    referencedEntries.clear();
    currencies.clear();
}

int Type7Entry::getType()
{
    return 7;
}

CompleteID Type7Entry::getCurrency()
{
    return currency;
}

unsigned long Type7Entry::getProcessingTime()
{
    return processingTime;
}

double Type7Entry::getAmount()
{
    return amount;
}

double Type7Entry::getFee()
{
    return fee;
}

double Type7Entry::getOwnStake()
{
    return forfeit;
}

CompleteID Type7Entry::getLiquidityClaim()
{
    return claim;
}

string* Type7Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

bool Type7Entry::isGood()
{
    return (amount>0);
}

CompleteID Type7Entry::getOwnerID()
{
    return applicant;
}

string* Type7Entry::getDescription()
{
    if (isGood()) return &description;
    return nullptr;
}

list<CompleteID>* Type7Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(applicant);
        referencedEntries.push_back(claim);
    }
    return &referencedEntries;
}

list<CompleteID>* Type7Entry::getCurrenciesAndObligations()
{
    if (currencies.size()==0 && isGood())
    {
        currencies.push_back(currency);
    }
    return &currencies;
}

Entry* Type7Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type7Entry* out = new Type7Entry(str);
    return out;
}
