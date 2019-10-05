#ifndef TYPE10ENTRY_H
#define TYPE10ENTRY_H
#include "Entry.h"
#include "CIDsSet.h"

class Type10Entry : public Entry
{
public:
    Type10Entry(string &str);
    Type10Entry(CompleteID &l, double a, CompleteID &s, CompleteID &t, double r);
    ~Type10Entry();
    int getType();
    CompleteID getOwnerID();
    list<CompleteID>* getReferencedEntries();
    list<CompleteID>* getCurrenciesAndObligations();
    CompleteID getCurrencyOrObl();
    double getTransferAmount();
    double getRequestedAmount();
    CompleteID getSource();
    CompleteID getTarget();
    string* getByteSeq();
    bool isGood();
    Entry* createCopy();
    bool satisfiesOfferOrRequest(CIDsSet &ids);
protected:
private:
    CompleteID liqui;
    double amount;
    CompleteID source;
    CompleteID target;
    double requested;

    string byteSeq;

    void deleteData();

    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;
};

#endif // TYPE10ENTRY_H
