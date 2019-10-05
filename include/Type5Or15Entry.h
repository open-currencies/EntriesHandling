#ifndef TYPE5OR15ENTRY_H
#define TYPE5OR15ENTRY_H
#include "CompleteID.h"
#include "Entry.h"
#include <list>

using namespace std;

class Type5Or15Entry : public Entry
{
public:
    Type5Or15Entry();
    virtual ~Type5Or15Entry();
    virtual CompleteID getOwnerID() =0;
    virtual list<CompleteID>* getReferencedEntries() =0;
    virtual list<CompleteID>* getCurrenciesAndObligations() =0;
    virtual int getType() =0;
    virtual string* getByteSeq() =0; // without the EOF byte
    virtual bool isGood() =0;
    virtual double getMinTransferAmount() =0;
    virtual double getRate() =0;
protected:
private:
};

#endif // TYPE5OR15ENTRY_H
