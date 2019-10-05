#ifndef TYPE8ENTRY_H
#define TYPE8ENTRY_H
#include "CompleteID.h"
#include "Entry.h"

class Type8Entry : public Entry
{
public:
    Type8Entry(string &str);
    Type8Entry(CompleteID &a, CompleteID &t, CompleteID &c, string &d);
    ~Type8Entry();
    CompleteID getTerminationID();
    int getType();
    CompleteID getPreviousThreadEntry();
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
    CompleteID appointment;
    CompleteID thread;
    CompleteID claim;
    string description;

    string byteSeq;
    void deleteData();
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;
};

#endif // TYPE8ENTRY_H
