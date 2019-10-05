#ifndef CIDSSET_H
#define CIDSSET_H
#include <set>
#include <list>
#include "CompleteID.h"

using namespace std;

class CIDsSet
{
public:
    CIDsSet();
    CIDsSet(list<CompleteID> &ids);
    ~CIDsSet();
    bool isEmpty();
    CompleteID first();
    CompleteID last();
    size_t size();
    bool contains(CompleteID id);
    void add(CompleteID &id);
    void add(list<CompleteID> &ids);
    void clear();
    set<CompleteID, CompleteID::CompareIDs>* getSetPointer();
    CIDsSet* createCopy();
    void deleteFirst();
protected:
private:
    set<CompleteID, CompleteID::CompareIDs> CIDs;
};

#endif // CIDSSET_H
