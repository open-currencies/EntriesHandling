#include "Type8Entry.h"

Type8Entry::Type8Entry(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x27)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 1;
    // read appointment
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    appointment = CompleteID(dum);
    pos+=20;
    // read thread
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    thread = CompleteID(dum);
    pos+=20;
    // read claim
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    claim = CompleteID(dum);
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

    if (pos != l || appointment.getNotary()<=0 || thread.getNotary()<=0
            || claim.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }

    byteSeq = str;
}

Type8Entry::Type8Entry(CompleteID &a, CompleteID &t, CompleteID &c, string &d) :
    appointment(a), thread(t), claim(c), description(d)
{
    if (appointment.getNotary()<=0 || thread.getNotary()<=0
            || claim.getNotary()<=0 || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    byteSeq.push_back(0x27);
    byteSeq.append(appointment.to20Char());
    byteSeq.append(thread.to20Char());
    byteSeq.append(claim.to20Char());
    byteSeq.append(u.UllAsByteSeq(description.length()));
    byteSeq.append(description);
}

Type8Entry::~Type8Entry()
{
    //dtor
}

void Type8Entry::deleteData()
{
    appointment=CompleteID();
    thread=CompleteID();
    claim=CompleteID();
    byteSeq="";
    description="";
    referencedEntries.clear();
    currencies.clear();
}

CompleteID Type8Entry::getTerminationID()
{
    return appointment;
}

int Type8Entry::getType()
{
    return 8;
}

CompleteID Type8Entry::getPreviousThreadEntry()
{
    return thread;
}

CompleteID Type8Entry::getLiquidityClaim()
{
    return claim;
}

CompleteID Type8Entry::getOwnerID()
{
    return CompleteID();
}

list<CompleteID>* Type8Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(appointment);
        referencedEntries.push_back(thread);
        referencedEntries.push_back(claim);
    }
    return &referencedEntries;
}

list<CompleteID>* Type8Entry::getCurrenciesAndObligations()
{
    return &currencies;
}

string* Type8Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

string* Type8Entry::getDescription()
{
    if (isGood()) return &description;
    return nullptr;
}

bool Type8Entry::isGood()
{
    return appointment.getNotary()>0;
}

Entry* Type8Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type8Entry* out = new Type8Entry(str);
    return out;
}
