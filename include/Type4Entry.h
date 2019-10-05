#ifndef TYPE4ENTRY_H
#define TYPE4ENTRY_H
#include "Entry.h"
#include "CompleteID.h"

class Type4Entry : public Entry
{
public:
    Type4Entry(string &str);
    Type4Entry(unsigned long n, CompleteID &t, string &d);
    ~Type4Entry();
    unsigned long getNotaryNr();
    int getType();
    CompleteID getPreviousThreadEntry();
    CompleteID getOwnerID();
    list<CompleteID>* getReferencedEntries();
    list<CompleteID>* getCurrenciesAndObligations();
    string* getByteSeq();
    string* getDescription();
    bool isGood();
    Entry* createCopy();
protected:
private:
    unsigned long long notaryNr;
    CompleteID thread;
    string description;

    string byteSeq;
    void deleteData();
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;
};

#endif // TYPE4ENTRY_H
