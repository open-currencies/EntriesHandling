#ifndef TYPE6ENTRY_H
#define TYPE6ENTRY_H
#include "Entry.h"

class Type6Entry : public Entry
{
public:
    Type6Entry(string &str);
    Type6Entry(CompleteID &a, CompleteID &c, unsigned long long &t, string &d);
    ~Type6Entry();
    int getType();
    CompleteID getCurrency();
    unsigned long long getTenureStart();
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
    unsigned long long tenureStart;
    string description;

    string byteSeq;
    void deleteData();
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;
};

#endif // TYPE6ENTRY_H
