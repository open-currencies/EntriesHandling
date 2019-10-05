#ifndef TYPE13ENTRY_H
#define TYPE13ENTRY_H
#include "CompleteID.h"
#include "Type12Entry.h"

class Type13Entry
{
public:
    Type13Entry(string &str);
    ~Type13Entry();
    CompleteID getCompleteID();
    CompleteID getPredecessorID();
    CompleteID getFirstID();
    unsigned long getNotary();
    string* getByteSeq();
    string* getSignature();
    bool isGood();
protected:
private:
    string byteSeq;
    string signatureSeq;
    string t12eSeq;
    CompleteID cID;
    CompleteID cIDpred;
    CompleteID cIDfirst;
    void deleteData();
};

#endif // TYPE13ENTRY_H
