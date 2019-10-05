#ifndef REFEREEINFO_H
#define REFEREEINFO_H
#include "CompleteID.h"

class RefereeInfo
{
public:
    RefereeInfo(string &str);
    RefereeInfo(CompleteID &a, unsigned long long t, double l, unsigned short r, CompleteID &p);
    ~RefereeInfo();
    string* getByteSeq();
    CompleteID getAppointmentId();
    unsigned long long getTenureEnd();
    double getLiquidityCreated();
    unsigned short getRefsAppointed();
    CompleteID getPendingOrApprovedApplicationId();
protected:
private:
    CompleteID appointmentId;
    unsigned long long tenureEnd;
    double liquidityCreated;
    unsigned short refsAppointed;
    CompleteID pendingOrApprovedApplicationId;

    string byteSeq;

    void deleteData();
};

#endif // REFEREEINFO_H
