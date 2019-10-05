#ifndef TYPE15ENTRY_H
#define TYPE15ENTRY_H
#include <string>
#include <iostream>
#include "Type5Or15Entry.h"
#include "nlohmann/json.hpp"
#include <sys/stat.h>

using namespace std;
using json = nlohmann::json;

class Type15Entry : public Type5Or15Entry
{
public:
    Type15Entry(string &str);
    Type15Entry(const string& dirName, const string& fileName);
    ~Type15Entry();
    int getType();
    CompleteID getOwnerID();
    list<CompleteID>* getReferencedEntries(); // without currencies/obligations
    list<CompleteID>* getCurrenciesAndObligations();
    string* getByteSeq(); // without the EOF byte
    string* getDescription();
    bool isGood();
    double getRate();
    Entry* createCopy();
    double getMinTransferAmount();
protected:
private:
    Util u;
    CompleteID ownerId;
    list<CompleteID> ownerIdAsList;
    double rate;
    double minTransfer;
    string description; // in UTF-8
    string byteSeq;

    void createFromString(string &str);
    void deleteData();
};

#endif // TYPE15ENTRY_H
