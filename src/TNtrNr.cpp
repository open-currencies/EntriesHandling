#include "TNtrNr.h"

TNtrNr::TNtrNr(string str)
{
    lineage = 0;
    notaryNr = 0;
    if (str.length() < 5)
    {
        return;
    }
    char *cstr = new char[str.length()+1];
    strcpy(cstr, str.c_str());
    if (cstr[0] != '(')
    {
        delete[] cstr;
        return;
    }
    if (cstr[str.length()-1] != ')')
    {
        delete[] cstr;
        return;
    }
    size_t pos = str.find(",");
    if (pos == string::npos)
    {
        delete[] cstr;
        return;
    }
    string lineageStr = str.substr(1, pos-1);
    string notaryNrStr = str.substr(pos+1, str.length()-2-pos);
    lineage = stoul(lineageStr);
    notaryNr = stoul(notaryNrStr);
    delete[] cstr;
}

TNtrNr::TNtrNr(unsigned short l, unsigned long n) : lineage(l), notaryNr(n)
{
    //ctor
}

TNtrNr::TNtrNr()
{
    lineage = 0;
    notaryNr = 0;
}

TNtrNr::~TNtrNr()
{
    //dtor
}

string TNtrNr::toString()
{
    string out("(");
    out.append(to_string(lineage));
    out.append(",");
    out.append(to_string(notaryNr));
    out.append(")");
    return out;
}

bool TNtrNr::isZero()
{
    return (notaryNr == 0 && lineage == 0);
}

bool TNtrNr::isGood()
{
    return (notaryNr > 0 && lineage > 0);
}

unsigned long TNtrNr::getNotaryNr()
{
    return notaryNr;
}

unsigned short TNtrNr::getLineage()
{
    return lineage;
}
