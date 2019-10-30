#include "CompleteID.h"

CompleteID::CompleteID(unsigned long n, unsigned long long i, unsigned long long t) : notary(n), id(i), timeStamp(t)
{
}

CompleteID::CompleteID() : notary(0), id(0), timeStamp(0)
{
}

void CompleteID::resetTo(CompleteID otherId)
{
    notary = otherId.notary;
    id = otherId.id;
    timeStamp = otherId.timeStamp;
    underlyingStr.clear();
}

bool CompleteID::loadFromHex(string &str)
{
    underlyingStr.clear();
    if (str.length() != 40) return false;
    string str2;
    if (!u.fromHex(str, str2)) return false;
    if (!loadFrom20Char(str2)) return false;
    return true;
}

string CompleteID::toHex()
{
    string str = to20Char();
    string str2;
    if (!u.toHex(str, str2)) return "";
    return str2;
}

CompleteID::CompleteID(string &str) : notary(0), id(0), timeStamp(0)
{
    if (str.length() == 20) loadFrom20Char(str);
    else if (str.length() == 27)
    {
        string str2 = u.b61hide(str);
        BigInt b61int(str2, 61);
        BigInt b256int(b61int, 256);
        string result = b256int.getString();
        unsigned char zero = 0;
        if (result.length() < 20) result.insert(0, 20-result.length(), (char) zero);
        loadFrom20Char(result);
    }
    else if (str.length() == 40) loadFromHex(str);
}

bool CompleteID::loadFrom20Char(string &str)
{
    underlyingStr.clear();
    if (str.length() != 20) return false;
    // get time stamp
    string dum = u.flip(str.substr(0,8));
    timeStamp = u.byteSeqAsUll(dum);
    // get notary
    dum = u.flip(str.substr(8,4));
    notary = u.byteSeqAsUl(dum);
    // get id
    dum = u.flip(str.substr(12,8));
    id = u.byteSeqAsUll(dum);
    to20Char();
    return true;
}

string CompleteID::to20Char()
{
    if (underlyingStr.length()!=20)
    {
        underlyingStr.clear();
        underlyingStr.append(u.flip(u.UllAsByteSeq(timeStamp)));
        underlyingStr.append(u.flip(u.UlAsByteSeq(notary)));
        underlyingStr.append(u.flip(u.UllAsByteSeq(id)));
    }
    return underlyingStr;
}

string CompleteID::to27Char()
{
    BigInt b256int(to20Char(), 256);
    BigInt b61int(b256int, 61);
    string result = b61int.getString();
    unsigned char zero = 0;
    if (result.length() < 27) result.insert(0, 27-result.length(), (char) zero);
    return u.b61show(result);
}

bool CompleteID::isZero()
{
    return (notary==0 && id==0 && timeStamp==0);
}

CompleteID::~CompleteID()
{
}

CompleteID CompleteID::maximum(CompleteID otherId)
{
    if (*this <= otherId) return otherId;
    return *this;
}

CompleteID CompleteID::minimum(CompleteID otherId)
{
    if (*this >= otherId) return otherId;
    return *this;
}

unsigned long CompleteID::getNotary()
{
    return notary;
}

unsigned long long CompleteID::getTimeStamp()
{
    return timeStamp;
}
