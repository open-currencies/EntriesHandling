#ifndef TYPE5ENTRY_H
#define TYPE5ENTRY_H
#include <string>
#include <iostream>
#include "Type5Or15Entry.h"
#include "nlohmann/json.hpp"
#include <sys/stat.h>

using namespace std;
using json = nlohmann::json;

class Type5Entry : public Type5Or15Entry
{
public:
    Type5Entry(string &str);
    Type5Entry(const string& dirName, const string& fileName);
    ~Type5Entry();
    int getType();
    unsigned long getRefereeTenure();
    bool isGoodOpPrProcessingTime(unsigned long t);
    CompleteID getOwnerID();
    list<CompleteID>* getReferencedEntries(); // without currencies/obligations
    list<CompleteID>* getCurrenciesAndObligations();
    string* getByteSeq(); // without the EOF byte
    string* getByteSeqTrunc(); // without the EOF byte + no description
    string* getDescription();
    bool isGood();
    Entry* createCopy();
    double getRate();
    bool isGoodFee(double amount, double fee);
    double getMinimalFee(double amount);
    double getMaximalFee(double amount);
    double getRefereeStake(double amount, double fee);
    double getLiquidityCreationLimit();
    unsigned short getRefAppointmentLimit();
    unsigned long minOpPrProcessingTime();
    unsigned long maxOpPrProcessingTime();
    double getMinTransferAmount();
    double getInitialLiquidity();
    unsigned long getRefApplProcessingTime();
    double getRefApplStake();
    double getParentShare();
protected:
private:
    void createFromString(string &str);
    void deleteData();

    Util u;
    CompleteID firstRefereeId;
    list<CompleteID> firstRefereeIdAsList;
    string description; // in UTF-8
    string byteSeq;
    string byteSeqTrunc;
    double rate;
    // referee parameters
    unsigned long refereeTenure;
    unsigned long refereeApplProcessingTime;
    double refereeApplStake;
    unsigned short maxAppointments;
    double initialLiquidity;
    double maxLiquidityCreation;
    double parentShare;
    // min transfer
    double minTransfer;
    // operation proposals data
    unsigned long minProcessingTime;
    unsigned long maxProcessingTime;
    vector<double> amounts; // must have at least 2 elements
    vector<double> minimalFees; // same size as amounts
    vector<double> maximalFees; // same size as amounts
    // referee stakes data
    vector<double> amountBasedRefereeStakes; // same size as amounts
    vector<double> fees; // must have at least 2 elements
    vector<double> feeBasedRefereeStakes; // same size as fees
};

#endif // TYPE5ENTRY_H
