#include "NotaryInfo.h"

NotaryInfo::NotaryInfo(string &str)
{
    const unsigned long long l = str.length();
    if (l < 42)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 0;
    // read pendingOrApprovedApplicationId
    if (l-pos < 20)
    {
        perror("bad format of str in NotaryInfo(string &str) 1");
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    pendingOrApprovedApplicationId = CompleteID(dum);
    pos+=20;
    // read initiatedThreads
    if (l-pos < 2)
    {
        perror("bad format of str in NotaryInfo(string &str) 1.5");
        deleteData();
        return;
    }
    string initiatedThreadsStr = str.substr(pos,2);
    initiatedThreads = u.byteSeqAsUs(initiatedThreadsStr);
    pos+=2;
    // read numOfOutgoingShares
    if (l-pos < 2)
    {
        perror("bad format of str in NotaryInfo(string &str) 2");
        deleteData();
        return;
    }
    string numOfOutgoingSharesStr = str.substr(pos,2);
    unsigned short numOfOutgoingShares = u.byteSeqAsUs(numOfOutgoingSharesStr);
    pos+=2;
    // read outgoing shares
    for (unsigned short i=0; i<numOfOutgoingShares; i++)
    {
        // notary num
        if (l-pos < 4)
        {
            perror("bad format of str in NotaryInfo(string &str) 3");
            deleteData();
            return;
        }
        dum = str.substr(pos,4);
        unsigned long notary = u.byteSeqAsUl(dum);
        pos+=4;
        // value
        if (l-pos < 8)
        {
            perror("bad format of str in NotaryInfo(string &str) 4");
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        double value = u.byteSeqAsDbl(dum);
        pos+=8;
        // add to map
        outgoingShares.insert(pair<unsigned long, double>(notary, value));
    }
    // read numOfIncomingShares
    if (l-pos < 2)
    {
        perror("bad format of str in NotaryInfo(string &str) 5");
        deleteData();
        return;
    }
    string numOfIncomingSharesStr = str.substr(pos,2);
    unsigned short numOfIncomingShares = u.byteSeqAsUs(numOfIncomingSharesStr);
    pos+=2;
    // read incoming shares
    for (unsigned short i=0; i<numOfIncomingShares; i++)
    {
        // notary num
        if (l-pos < 4)
        {
            perror("bad format of str in NotaryInfo(string &str) 6");
            deleteData();
            return;
        }
        dum = str.substr(pos,4);
        unsigned long notary = u.byteSeqAsUl(dum);
        pos+=4;
        // value
        if (l-pos < 8)
        {
            perror("bad format of str in NotaryInfo(string &str) 7");
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        double value = u.byteSeqAsDbl(dum);
        pos+=8;
        // add to map
        incomingShares.insert(pair<unsigned long, double>(notary, value));
    }
    // pubKeyId
    if (l-pos < 20)
    {
        perror("bad format of str in NotaryInfo(string &str) 7.5");
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    pubKeyId = CompleteID(dum);
    pos+=20;
    // liquidity of interest
    if (l-pos < 20)
    {
        perror("bad format of str in NotaryInfo(string &str) 8");
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    liquiOfInterest = CompleteID(dum);
    pos+=20;
    // collected fees
    if (l-pos < 8)
    {
        perror("bad format of str in NotaryInfo(string &str) 9");
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    collectedFees = u.byteSeqAsDbl(dum);
    pos+=8;
    // final test
    if (pos != l)
    {
        perror("bad format of str in NotaryInfo(string &str) 10");
        deleteData();
        return;
    }
    byteSeq = str;
}

NotaryInfo::NotaryInfo(CompleteID &p, unsigned short t, map<unsigned long, double> &o, map<unsigned long, double> &i, CompleteID &k, CompleteID &l, double c) :
    pendingOrApprovedApplicationId(p), initiatedThreads(t), outgoingShares(o), incomingShares(i), pubKeyId(k), liquiOfInterest(l), collectedFees(c)
{
    Util u;
    byteSeq.append(pendingOrApprovedApplicationId.to20Char());
    byteSeq.append(u.UsAsByteSeq(initiatedThreads));
    byteSeq.append(u.UsAsByteSeq(outgoingShares.size()));
    map<unsigned long, double>::iterator it;
    for (it=outgoingShares.begin(); it!=outgoingShares.end(); ++it)
    {
        byteSeq.append(u.UlAsByteSeq(it->first));
        byteSeq.append(u.DblAsByteSeq(it->second));
    }
    byteSeq.append(u.UsAsByteSeq(incomingShares.size()));
    for (it=incomingShares.begin(); it!=incomingShares.end(); ++it)
    {
        byteSeq.append(u.UlAsByteSeq(it->first));
        byteSeq.append(u.DblAsByteSeq(it->second));
    }
    byteSeq.append(pubKeyId.to20Char());
    byteSeq.append(liquiOfInterest.to20Char());
    byteSeq.append(u.DblAsByteSeq(collectedFees));
}

void NotaryInfo::deleteData()
{
    pendingOrApprovedApplicationId=CompleteID();
    initiatedThreads=0;
    outgoingShares.clear();
    incomingShares.clear();
    liquiOfInterest=CompleteID();
    pubKeyId=CompleteID();
    collectedFees=0;

    byteSeq="";
    Util u;
    byteSeq.append(pendingOrApprovedApplicationId.to20Char());
    byteSeq.append(u.UsAsByteSeq(0));
    byteSeq.append(u.UsAsByteSeq(0));
    byteSeq.append(pubKeyId.to20Char());
    byteSeq.append(liquiOfInterest.to20Char());
    byteSeq.append(u.DblAsByteSeq(collectedFees));
}

NotaryInfo::~NotaryInfo()
{
    //dtor
}

string* NotaryInfo::getByteSeq()
{
    return &byteSeq;
}

unsigned short NotaryInfo::getInitiatedThreadsNum()
{
    return initiatedThreads;
}

double NotaryInfo::getCollectedFees()
{
    return collectedFees;
}

CompleteID NotaryInfo::getLiquidityOfInterestId()
{
    return liquiOfInterest;
}

CompleteID NotaryInfo::getPubKeyId()
{
    return pubKeyId;
}

map<unsigned long, double>* NotaryInfo::getOutgoingShares()
{
    return &outgoingShares;
}

map<unsigned long, double>* NotaryInfo::getIncomingShares()
{
    return &incomingShares;
}

CompleteID NotaryInfo::getPendingOrApprovedApplicationId()
{
    return pendingOrApprovedApplicationId;
}
