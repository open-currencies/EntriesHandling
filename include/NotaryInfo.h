#ifndef NOTARYINFO_H
#define NOTARYINFO_H
#include "CompleteID.h"

class NotaryInfo
{
public:
    NotaryInfo(string &str);
    NotaryInfo(CompleteID &p, unsigned short t, map<unsigned long, double> &o, map<unsigned long, double> &i, CompleteID &k, CompleteID &l, double c);
    ~NotaryInfo();
    CompleteID getPendingOrApprovedApplicationId();
    map<unsigned long, double>* getOutgoingShares();
    map<unsigned long, double>* getIncomingShares();
    CompleteID getPubKeyId();
    CompleteID getLiquidityOfInterestId();
    double getCollectedFees();
    string* getByteSeq();
    unsigned short getInitiatedThreadsNum();
protected:
private:
    CompleteID pendingOrApprovedApplicationId;
    unsigned short initiatedThreads;
    map<unsigned long, double> outgoingShares;
    map<unsigned long, double> incomingShares;
    CompleteID pubKeyId;
    CompleteID liquiOfInterest;
    double collectedFees;

    string byteSeq;

    void deleteData();
};

#endif // NOTARYINFO_H
