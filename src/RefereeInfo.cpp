#include "RefereeInfo.h"

RefereeInfo::RefereeInfo(CompleteID &a, unsigned long long t, double l, unsigned short r, CompleteID &p) :
    appointmentId(a), tenureEnd(t), liquidityCreated(l), refsAppointed(r), pendingOrApprovedApplicationId(p)
{
    Util u;
    byteSeq.append(appointmentId.to20Char());
    byteSeq.append(u.UllAsByteSeq(tenureEnd));
    byteSeq.append(u.DblAsByteSeq(liquidityCreated));
    byteSeq.append(u.UsAsByteSeq(refsAppointed));
    byteSeq.append(pendingOrApprovedApplicationId.to20Char());
}

void RefereeInfo::deleteData()
{
    appointmentId=CompleteID();
    tenureEnd=0;
    liquidityCreated=0;
    refsAppointed=0;
    pendingOrApprovedApplicationId=CompleteID();

    byteSeq = "";
    Util u;
    byteSeq.append(appointmentId.to20Char());
    byteSeq.append(u.UllAsByteSeq(tenureEnd));
    byteSeq.append(u.DblAsByteSeq(liquidityCreated));
    byteSeq.append(u.UsAsByteSeq(refsAppointed));
    byteSeq.append(pendingOrApprovedApplicationId.to20Char());
}

RefereeInfo::RefereeInfo(string &str)
{
    const unsigned long long l = str.length();
    if (l != 58)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 0;
    // read appointmentId
    if (l-pos < 20)
    {
        perror("bad format of str in RefereeInfo(string &str) 1");
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    appointmentId = CompleteID(dum);
    pos+=20;
    // read tenureEnd
    if (l-pos < 8)
    {
        perror("bad format of str in RefereeInfo(string &str) 2");
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    tenureEnd = u.byteSeqAsUll(dum);
    pos+=8;
    // read liquidityCreated
    if (l-pos < 8)
    {
        perror("bad format of str in RefereeInfo(string &str) 3");
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    liquidityCreated = u.byteSeqAsDbl(dum);
    pos+=8;
    // read refsAppointed
    if (l-pos < 2)
    {
        perror("bad format of str in RefereeInfo(string &str) 4");
        deleteData();
        return;
    }
    dum = str.substr(pos,2);
    refsAppointed = u.byteSeqAsUs(dum);
    pos+=2;
    // read pendingOrApprovedApplicationId
    if (l-pos < 20)
    {
        perror("bad format of str in RefereeInfo(string &str) 5");
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    pendingOrApprovedApplicationId = CompleteID(dum);
    pos+=20;
    // final test
    if (pos != l)
    {
        perror("bad format of str in RefereeInfo(string &str) 6");
        deleteData();
        return;
    }
    byteSeq = str;
}

CompleteID RefereeInfo::getAppointmentId()
{
    return appointmentId;
}

unsigned long long RefereeInfo::getTenureEnd()
{
    return tenureEnd;
}

double RefereeInfo::getLiquidityCreated()
{
    return liquidityCreated;
}

unsigned short RefereeInfo::getRefsAppointed()
{
    return refsAppointed;
}

CompleteID RefereeInfo::getPendingOrApprovedApplicationId()
{
    return pendingOrApprovedApplicationId;
}

RefereeInfo::~RefereeInfo()
{
    //dtor
}

string* RefereeInfo::getByteSeq()
{
    return &byteSeq;
}
