#ifndef ENTRY_H
#define ENTRY_H
#include "CompleteID.h"
#include "Util.h"
#include <list>
#include <string>

#define maxDescriptionLengthInK 1500

using namespace std;

class Entry
{
public:
    Entry();
    virtual ~Entry();
    virtual CompleteID getOwnerID() =0;
    virtual list<CompleteID>* getReferencedEntries() =0; // without currencies/obligations
    virtual list<CompleteID>* getCurrenciesAndObligations() =0;
    virtual int getType() =0;
    virtual string* getByteSeq() =0; // without the EOF byte
    virtual bool isGood() =0;
    virtual Entry* createCopy() =0;
protected:
private:
};

#endif // ENTRY_H
