#include "CIDsSet.h"

CIDsSet::CIDsSet()
{
    //ctor
}

CIDsSet::~CIDsSet()
{
    clear();
}

CIDsSet::CIDsSet(list<CompleteID> &ids)
{
    add(ids);
}

size_t CIDsSet::size()
{
    return CIDs.size();
}

bool CIDsSet::contains(CompleteID id)
{
    return CIDs.count(id)>0;
}

void CIDsSet::add(CompleteID &id)
{
    if (contains(id)) return;
    CIDs.insert(id);
}

void CIDsSet::add(list<CompleteID> &ids)
{
    list<CompleteID>::iterator it;
    for(it=ids.begin(); it!=ids.end(); ++it) add(*it);
}

void CIDsSet::clear()
{
    CIDs.clear();
}

CIDsSet* CIDsSet::createCopy()
{
    CIDsSet* out = new CIDsSet();
    set<CompleteID, CompleteID::CompareIDs>::iterator it;
    for(it=CIDs.begin(); it!=CIDs.end(); ++it)
    {
        CompleteID id = *it;
        out->add(id);
    }
    return out;
}

void CIDsSet::deleteFirst()
{
    CIDs.erase(CIDs.begin());
}

bool CIDsSet::isEmpty()
{
    return size()==0;
}

CompleteID CIDsSet::first()
{
    return *CIDs.begin();
}

CompleteID CIDsSet::last()
{
    return *CIDs.rbegin();
}

set<CompleteID, CompleteID::CompareIDs>* CIDsSet::getSetPointer()
{
    return &CIDs;
}
