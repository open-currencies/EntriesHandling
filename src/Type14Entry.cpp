#include "Type14Entry.h"

Type14Entry::Type14Entry(string &str) : totalAmount(-1), nonTransferAmount(0)
{
    double localTotalAmount=0;
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x2D)
    {
        deleteData();
        return;
    }
    Util u;
    string dum;
    size_t pos = 1;
    // read liqui
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    liqui = CompleteID(dum);
    pos+=20;
    // read holder
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    holder = CompleteID(dum);
    pos+=20;
    // read total amount
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    totalAmount = u.byteSeqAsDbl(dum);
    pos+=8;
    // read non-transfer amount
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    nonTransferAmount = u.byteSeqAsDbl(dum);
    pos+=8;
    // read predesessorsNr
    if (l-pos < 2)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,2);
    unsigned short predesessorsNr = u.byteSeqAsUs(dum);
    pos+=2;
    // read impacts
    double maxAbsImpact = 0;
    for (unsigned short i=0; i<predesessorsNr; i++)
    {
        if (l-pos < 8)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        double impact = u.byteSeqAsDbl(dum);
        impacts.push_back(impact);
        pos+=8;
        localTotalAmount+=impact;
        maxAbsImpact = max(maxAbsImpact, abs(impact));
    }
    // read predecessors
    for (unsigned short i=0; i<predesessorsNr; i++)
    {
        if (l-pos < 20)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,20);
        CompleteID id(dum);
        predecessors.push_back(id);
        pos+=20;
    }
    // read scenarios
    for (unsigned short i=0; i<predesessorsNr; i++)
    {
        if (l-pos < 1)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,1);
        scenarios.push_back(u.byteSeqAsUc(dum));
        pos+=1;
    }

    // final length check
    if (pos != l)
    {
        deleteData();
        return;
    }
    // some more consistency checks
    if (liqui.getNotary()<=0 || holder.getNotary()<=0
            || nonTransferAmount>totalAmount || !u.roughlyTheSameS(totalAmount, localTotalAmount, maxAbsImpact))
    {
        deleteData();
        return;
    }
    unsigned short n = impacts.size();
    if (n<0 || n!=impacts.size() || n!=predecessors.size() || n!=scenarios.size())
    {
        deleteData();
        return;
    }
    // check impacts and create outflowEntries
    predecessorClaimIndex=-1;
    unsigned short c = 0;
    vector<double>::iterator it1;
    vector<CompleteID>::iterator it2 = predecessors.begin();
    vector<unsigned char>::iterator it3 = scenarios.begin();
    for (it1 = impacts.begin(); it1 != impacts.end(); ++it1)
    {
        unsigned char sc = *it3;
        if (it2->getNotary()<=0 || sc>9)
        {
            deleteData();
            return;
        }
        if (sc == 2 || sc == 5 || sc == 6)
        {
            if (*it1 > 0 || outflowEntries.size()>0)
            {
                deleteData();
                return;
            }
            outflowEntries.push_back(*it2);
        }
        else
        {
            if (*it1 < 0)
            {
                deleteData();
                return;
            }
        }
        if (sc==0) // type 0 scenario
        {
            if (predecessorClaimIndex<0) predecessorClaimIndex=c;
            else
            {
                deleteData();
                return;
            }
        }
        ++it3;
        ++it2;
        c++;
    }

    byteSeq = str;
}

Type14Entry::Type14Entry(CompleteID &l, CompleteID &h, double &t, double &n, vector<double> &i, vector<CompleteID> &p, vector<unsigned char> &s) :
    liqui(l), holder(h), totalAmount(t), nonTransferAmount(n), impacts(i), predecessors(p), scenarios(s)
{
    if (liqui.getNotary()<=0 || holder.getNotary()<=0 || nonTransferAmount>totalAmount)
    {
        deleteData();
        return;
    }
    unsigned short predsNr = impacts.size();
    if (predsNr<0 || predsNr!=impacts.size() || predsNr!=predecessors.size() || predsNr!=scenarios.size())
    {
        deleteData();
        return;
    }
    // build byteSeq
    Util u;
    double localTotalAmount=0;
    byteSeq = "";
    byteSeq.push_back(0x2D);
    byteSeq.append(liqui.to20Char());
    byteSeq.append(holder.to20Char());
    byteSeq.append(u.DblAsByteSeq(totalAmount));
    byteSeq.append(u.DblAsByteSeq(nonTransferAmount));
    byteSeq.append(u.UsAsByteSeq(predsNr));
    string impactsAsString;
    string predsAsString;
    string scenariosAsString;
    predecessorClaimIndex=-1;
    unsigned short c = 0;
    vector<double>::iterator it1;
    vector<CompleteID>::iterator it2 = predecessors.begin();
    vector<unsigned char>::iterator it3 = scenarios.begin();
    double maxAbsImpact = 0;
    for (it1 = impacts.begin(); it1 != impacts.end(); ++it1)
    {
        unsigned char sc = *it3;
        if (it2->getNotary()<=0 || sc>9)
        {
            deleteData();
            return;
        }
        if (sc == 2 || sc == 5 || sc == 6)
        {
            if (*it1 > 0 || outflowEntries.size()>0)
            {
                deleteData();
                return;
            }
            outflowEntries.push_back(*it2);
        }
        else
        {
            if (*it1 < 0)
            {
                deleteData();
                return;
            }
        }
        if (sc==0) // type 0 scenario
        {
            if (predecessorClaimIndex<0) predecessorClaimIndex=c;
            else
            {
                deleteData();
                return;
            }
        }
        localTotalAmount += (*it1);
        impactsAsString.append(u.DblAsByteSeq(*it1));
        predsAsString.append(it2->to20Char());
        scenariosAsString.append(u.UcAsByteSeq(*it3));
        ++it3;
        ++it2;
        c++;
        maxAbsImpact = max(maxAbsImpact, abs(*it1));
    }
    if (!u.roughlyTheSameS(totalAmount, localTotalAmount, maxAbsImpact))
    {
        deleteData();
        return;
    }
    byteSeq.append(impactsAsString);
    byteSeq.append(predsAsString);
    byteSeq.append(scenariosAsString);
}

Type14Entry::~Type14Entry()
{
    deleteData();
}

list<CompleteID>* Type14Entry::getReferencedEntries()
{
    if (referencedEntries.size()==0 && isGood())
    {
        referencedEntries.push_back(holder);
        vector<CompleteID>::iterator it = predecessors.begin();
        for (it = predecessors.begin(); it != predecessors.end(); ++it)
        {
            referencedEntries.push_back(*it);
        }
    }
    return &referencedEntries;
}

list<CompleteID>* Type14Entry::getCurrenciesAndObligations()
{
    if (currencies.size()==0 && isGood())
    {
        currencies.push_back(liqui);
    }
    return &currencies;
}

void Type14Entry::deleteData()
{
    liqui=CompleteID();
    holder=CompleteID();
    totalAmount=-1;
    nonTransferAmount=0;
    predecessorClaimIndex = -1;
    impacts.clear();
    predecessors.clear();
    scenarios.clear();
    outflowEntries.clear();
    byteSeq="";
    referencedEntries.clear();
    currencies.clear();
}

bool Type14Entry::collectsFromTransaction(CIDsSet &ids)
{
    if (!isGood()) return false;
    vector<CompleteID>::iterator itP = predecessors.begin();
    vector<unsigned char>::iterator itS;
    for (itS = scenarios.begin(); itS != scenarios.end(); ++itS)
    {
        if (*itS == 1 && ids.contains(*itP)) return true;
        ++itP;
    }
    return false;
}

string* Type14Entry::getByteSeq()
{
    if (isGood()) return &byteSeq;
    return nullptr;
}

bool Type14Entry::isGood()
{
    return liqui.getNotary()>0;
}

int Type14Entry::getType()
{
    return 14;
}

double Type14Entry::getTransferableAmount()
{
    return getTotalAmount() - getNonTransferAmount();
}

double Type14Entry::getNonTransferAmount()
{
    return max(nonTransferAmount, 0.0);
}

double Type14Entry::getTotalAmount()
{
    return max(totalAmount, 0.0);
}

CompleteID Type14Entry::getCurrencyOrObligation()
{
    return liqui;
}

CompleteID Type14Entry::getPredecessorClaim()
{
    if (predecessorClaimIndex<0) return CompleteID();
    else return predecessors.at(predecessorClaimIndex);
}

bool Type14Entry::outflowsDominate()
{
    if (!hasPredecessorClaim()) return false;
    return (impacts.at(predecessorClaimIndex)>totalAmount);
}

unsigned char Type14Entry::getScenario(unsigned short i)
{
    if (scenarios.size()<=i) return 10;
    return scenarios.at(i);
}

double Type14Entry::getImpact(unsigned short i)
{
    if (impacts.size()<=i) return 0;
    return impacts.at(i);
}

CompleteID Type14Entry::getPredecessor(unsigned short i)
{
    if (predecessors.size()<=i) return CompleteID();
    return predecessors.at(i);
}

bool Type14Entry::hasPredecessorClaim()
{
    return (predecessorClaimIndex>=0);
}

unsigned short Type14Entry::getPredecessorsCount()
{
    return predecessors.size();
}

list<CompleteID>* Type14Entry::getOutflowEntries()
{
    return &outflowEntries;
}

CompleteID Type14Entry::getOwnerID()
{
    return holder;
}

Entry* Type14Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type14Entry* out = new Type14Entry(str);
    return out;
}
