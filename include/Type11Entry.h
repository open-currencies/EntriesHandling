#ifndef TYPE11ENTRY_H
#define TYPE11ENTRY_H
#include "Entry.h"
#include "Util.h"
#include <string>

using namespace std;

class Type11Entry : public Entry
{
public:
    Type11Entry(string &str, unsigned long long v);
    Type11Entry(string &str);
    ~Type11Entry();
    string* getPublicKey();
    int getType();
    unsigned long long getValidityDate();

    CompleteID getOwnerID();
    list<CompleteID>* getReferencedEntries();
    list<CompleteID>* getCurrenciesAndObligations();
    string* getByteSeq(); // without the EOF byte
    bool isGood();
    Entry* createCopy();
protected:
private:
    string pubKeyStr;
    unsigned long long validUntil;
    string byteSeq;

    void deleteData();
};

#endif // TYPE11ENTRY_H
