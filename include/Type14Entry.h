#ifndef TYPE14ENTRY_H
#define TYPE14ENTRY_H
#include "Entry.h"
#include <list>
#include "CIDsSet.h"

class Type14Entry : public Entry
{
public:
    Type14Entry(string &str);
    Type14Entry(CompleteID &l, CompleteID &h, double &t, double &n, vector<double> &i, vector<CompleteID> &p, vector<unsigned char> &s);
    ~Type14Entry();
    int getType();
    bool isGood();
    string* getByteSeq();
    double getTransferableAmount();
    double getNonTransferAmount();
    double getTotalAmount();
    CompleteID getOwnerID();
    CompleteID getCurrencyOrObligation();
    CompleteID getPredecessorClaim();
    unsigned char getScenario(unsigned short i);
    double getImpact(unsigned short i);
    CompleteID getPredecessor(unsigned short i);
    bool hasPredecessorClaim();
    unsigned short getPredecessorsCount();
    list<CompleteID>* getOutflowEntries();
    list<CompleteID>* getReferencedEntries(); // without currencies/obligations
    list<CompleteID>* getCurrenciesAndObligations();
    bool outflowsDominate();
    Entry* createCopy();
    bool collectsFromTransaction(CIDsSet &ids);
protected:
private:
    CompleteID liqui;
    CompleteID holder;
    double totalAmount;
    double nonTransferAmount;
    int predecessorClaimIndex;

    vector<double> impacts;
    vector<CompleteID> predecessors;
    vector<unsigned char> scenarios;

    list<CompleteID> outflowEntries;
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;

    string byteSeq;

    void deleteData();
};

#endif // TYPE14ENTRY_H
