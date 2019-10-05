#ifndef TYPE12ENTRY_H
#define TYPE12ENTRY_H
#include "CompleteID.h"
#include "Entry.h"
#include "Util.h"
#include "Type2Entry.h"
#include "Type3Entry.h"
#include "Type4Entry.h"
#include "Type5Entry.h"
#include "Type6Entry.h"
#include "Type7Entry.h"
#include "Type8Entry.h"
#include "Type9Entry.h"
#include "Type10Entry.h"
#include "Type11Entry.h"
#include "Type14Entry.h"
#include "Type15Entry.h"
#include <string>

using namespace std;

class Type12Entry
{
public:
    Type12Entry(string &str);
    ~Type12Entry();
    int underlyingType();
    Entry* underlyingEntry();
    CompleteID pubKeyID();
    string* getByteSeq(); // without EOF
    string* getSignedSequence();
    string* getSignature();
    unsigned long getFirstNotaryNr();
    unsigned long long getNotarizationTimeLimit();
    bool isGood();
protected:
private:
    string byteSeq;
    string underlyingEntryStr;
    unsigned long notaryNr;
    unsigned long long timeLimit;
    string signedSequence;
    string signatureSeq;
    CompleteID cID;
    Entry* entry;

    void deleteData();
};

#endif // TYPE12ENTRY_H
