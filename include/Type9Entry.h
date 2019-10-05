#ifndef TYPE9ENTRY_H
#define TYPE9ENTRY_H

#include "Entry.h"

class Type9Entry : public Entry
{
public:
    Type9Entry(string &str);
    Type9Entry(CompleteID &o, CompleteID &t);
    ~Type9Entry();
    CompleteID getOwnerID();
    int getType();
    CompleteID getPreviousThreadEntry();
    Entry* createCopy();
    bool isGood();
    string* getByteSeq();
    list<CompleteID>* getCurrenciesAndObligations();
    list<CompleteID>* getReferencedEntries();
protected:
private:
    CompleteID ownerId;
    CompleteID threadId;

    string byteSeq;
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;

    void deleteData();
};

#endif // TYPE9ENTRY_H
