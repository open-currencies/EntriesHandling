#ifndef TYPE2ENTRY_H
#define TYPE2ENTRY_H
#include "CompleteID.h"
#include "Entry.h"
#include <string>

class Type2Entry : public Entry
{
public:
    Type2Entry(string &str);
    Type2Entry(CompleteID &p, CompleteID &t, string &k);
    ~Type2Entry();
    CompleteID getPredecessorID();
    CompleteID getTerminationID();
    string* getPublicKey();
    int getType();
    CompleteID getOwnerID();
    list<CompleteID>* getReferencedEntries();
    list<CompleteID>* getCurrenciesAndObligations();
    string* getByteSeq();
    bool isGood();
    Entry* createCopy();
protected:
private:
    CompleteID predecessorID;
    CompleteID terminationID;
    string publicKey;

    string byteSeq;
    void deleteData();
    list<CompleteID> referencedEntries;
    list<CompleteID> currencies;
};

#endif // TYPE2ENTRY_H
