#ifndef TYPE7ENTRY_H
#define TYPE7ENTRY_H
#include "Entry.h"

class Type7Entry : public Entry
{
public:
    Type7Entry(string &str);
    Type7Entry(CompleteID &a, CompleteID &c, double &am, double &f, double &fo, CompleteID &cl, unsigned long &p, string &d);
    ~Type7Entry();
    int getType();
    CompleteID getCurrency();
    double getAmount();
    unsigned long getProcessingTime();
    double getFee();
    double getOwnStake();
    CompleteID getLiquidityClaim();
    CompleteID getOwnerID();
    list<CompleteID>* getReferencedEntries();
    list<CompleteID>* getCurrenciesAndObligations();
    string* getByteSeq();
    string* getDescription();
    bool isGood();
    Entry* createCopy();
protected:
private:
    CompleteID applicant;
    CompleteID currency;
    double amount;
    double fee;
    double forfeit;
    CompleteID claim;
    unsigned long processingTime;
    string description;

    string byteSeq;
    void deleteData();
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;
};

#endif // TYPE7ENTRY_H
