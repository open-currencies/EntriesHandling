#include "Type10Entry.h"

Type10Entry::Type10Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x29)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 1;
    // read liqui
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    liqui = CompleteID(dum);
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
    // read source
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    source = CompleteID(dum);
    pos+=20;
    // read target
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    target = CompleteID(dum);
    pos+=20;
    // read requested
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    requested = u.byteSeqAsDbl(dum);
    pos+=8;

    if (pos != l || amount<0 || requested<0 || amount+requested<=0 || liqui.getNotary()<=0
            || source.getNotary()<=0 // basic tests
            || (amount<=0 && target != liqui) // transfer request
            || (amount>0 && target == liqui) // actual transfer (or exchange request)
            || (target.getNotary()<=0 && !(target.isZero() && requested==0)) // deletion
       )
    {
        deleteData();
        return;
    }

    byteSeq = str;
}

Type10Entry::Type10Entry(CompleteID &l, double a, CompleteID &s, CompleteID &t, double r) :
    liqui(l), amount(a), source(s), target(t), requested(r)
{
    byteSeq="";
    if (amount<0 || requested<0 || amount+requested<=0 || liqui.getNotary()<=0
            || source.getNotary()<=0 // basic tests
            || (amount<=0 && target != liqui) // transfer request
            || (amount>0 && target == liqui) // actual transfer (or exchange request)
            || (target.getNotary()<=0 && !(target.isZero() && requested==0)) // deletion
       )
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x29);
    byteSeq.append(liqui.to20Char());
    byteSeq.append(u.DblAsByteSeq(amount));
    byteSeq.append(source.to20Char());
    byteSeq.append(target.to20Char());
    byteSeq.append(u.DblAsByteSeq(requested));
}

string* Type10Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

Type10Entry::~Type10Entry()
{
    deleteData();
}

void Type10Entry::deleteData()
{
    amount=-1;
    requested=0;
    liqui=CompleteID();
    source=CompleteID();
    target=CompleteID();
    byteSeq="";
    referencedEntries.clear();
    currencies.clear();
}

bool Type10Entry::satisfiesOfferOrRequest(CIDsSet &ids)
{
    if (!isGood()) return false;
    return ids.contains(target);
}

bool Type10Entry::isGood()
{
    return (amount>=0 && requested>=0 && amount+requested>0);
}

int Type10Entry::getType()
{
    return 10;
}

CompleteID Type10Entry::getCurrencyOrObl()
{
    return liqui;
}

double Type10Entry::getTransferAmount()
{
    return amount;
}

double Type10Entry::getRequestedAmount()
{
    return requested;
}

CompleteID Type10Entry::getSource()
{
    return source;
}

CompleteID Type10Entry::getTarget()
{
    return target;
}

CompleteID Type10Entry::getOwnerID()
{
    return CompleteID();
}

list<CompleteID>* Type10Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(source);
        if (!target.isZero()) referencedEntries.push_back(target);
    }
    return &referencedEntries;
}

list<CompleteID>* Type10Entry::getCurrenciesAndObligations()
{
    if (currencies.size()==0 && isGood())
    {
        currencies.push_back(liqui);
    }
    return &currencies;
}

Entry* Type10Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type10Entry* out = new Type10Entry(str);
    return out;
}
