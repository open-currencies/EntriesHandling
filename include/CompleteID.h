#ifndef COMPLETEID_H
#define COMPLETEID_H
#include <utility>
#include <string>
#include "Util.h"
#include "BigInt.h"

typedef unsigned char byte;
using namespace std;

class CompleteID
{
public:
    CompleteID();
    CompleteID(unsigned long n, unsigned long long i,  unsigned long long t);
    CompleteID(string &str);
    ~CompleteID();
    CompleteID maximum(CompleteID otherId);
    CompleteID minimum(CompleteID otherId);
    unsigned long getNotary();
    unsigned long long getTimeStamp();
    string to20Char();
    string toHex();
    string to27Char();
    void resetTo(CompleteID otherId);
    bool isZero();
    bool operator==(const CompleteID& rhs)
    {
        return !cmp(*this, rhs) && !cmp(rhs, *this);
    }
    bool operator!=(const CompleteID& rhs)
    {
        return cmp(*this, rhs) || cmp(rhs, *this);
    }
    bool operator<(const CompleteID& rhs)
    {
        return cmp(*this, rhs);
    }
    bool operator>(const CompleteID& rhs)
    {
        return cmp(rhs, *this);
    }
    bool operator<=(const CompleteID& rhs)
    {
        return !cmp(rhs, *this);
    }
    bool operator>=(const CompleteID& rhs)
    {
        return !cmp(*this, rhs);
    }

    struct CompareIDs
    {
        bool operator()(const CompleteID& a, const CompleteID& b) const
        {
            if (a.timeStamp < b.timeStamp) return true;
            else if (a.timeStamp > b.timeStamp) return false;

            if (a.notary < b.notary) return true;
            else if (a.notary > b.notary) return false;

            return (a.id < b.id);
        }
    };

    struct CompareIDIntPairs
    {
        bool operator()(const pair<CompleteID,int>& a, const pair<CompleteID,int>& b) const
        {
            CompareIDs comp;

            if (comp(a.first, b.first)) return true;
            else if (comp(b.first, a.first)) return false;

            return (a.second < b.second);
        }
    };

    struct LLComparePairs
    {
        bool operator()(const pair<unsigned long long,CompleteID>& a, const pair<unsigned long long,CompleteID>& b) const
        {
            CompareIDs comp;

            if (a.first < b.first) return true;
            else if (b.first < a.first) return false;

            return (comp(a.second, b.second));
        }
    };
protected:
private:
    Util u;
    CompareIDs cmp;
    volatile unsigned long notary;
    volatile unsigned long long id;
    volatile unsigned long long timeStamp;
    string underlyingStr;
    bool loadFrom20Char(string &str);
    bool loadFromHex(string &str);
};

#endif // COMPLETEID_H
