#ifndef TNTRNR_H
#define TNTRNR_H
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class TNtrNr
{
public:
    TNtrNr(string str);
    TNtrNr(unsigned short l, unsigned long n);
    TNtrNr();
    ~TNtrNr();
    unsigned long getNotaryNr();
    unsigned short getLineage();
    string toString();
    bool isZero();
    bool isGood();

    bool operator==(const TNtrNr& rhs)
    {
        return !cmp(*this, rhs) && !cmp(rhs, *this);
    }
    bool operator!=(const TNtrNr& rhs)
    {
        return cmp(*this, rhs) || cmp(rhs, *this);
    }
    bool operator<(const TNtrNr& rhs)
    {
        return cmp(*this, rhs);
    }
    bool operator>(const TNtrNr& rhs)
    {
        return cmp(rhs, *this);
    }
    bool operator<=(const TNtrNr& rhs)
    {
        return !cmp(rhs, *this);
    }
    bool operator>=(const TNtrNr& rhs)
    {
        return !cmp(*this, rhs);
    }

    struct CompareTNtrNrs
    {
        bool operator()(const TNtrNr& a, const TNtrNr& b) const
        {
            if (a.lineage < b.lineage) return true;
            else if (a.lineage > b.lineage) return false;

            return (a.notaryNr < b.notaryNr);
        }
    };

protected:
private:
    CompareTNtrNrs cmp;
    unsigned short lineage;
    unsigned long notaryNr;
};

#endif // TNTRNR_H
