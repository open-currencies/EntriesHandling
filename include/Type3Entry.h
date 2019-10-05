#ifndef TYPE3ENTRY_H
#define TYPE3ENTRY_H
#include "Entry.h"

class Type3Entry : public Entry
{
public:
    Type3Entry(string &str);
    Type3Entry(CompleteID &a, string &d);
    ~Type3Entry();
    int getType();
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
    string description;

    string byteSeq;
    void deleteData();
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;
};

#endif // TYPE3ENTRY_H
