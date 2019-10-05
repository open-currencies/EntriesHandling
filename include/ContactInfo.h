#ifndef CONTACTINFO_H
#define CONTACTINFO_H
#include <string>
#include "Util.h"

using namespace std;

class ContactInfo
{
public:
    ContactInfo(string &str);
    ContactInfo(TNtrNr &t, string &i, unsigned long p, unsigned long long v, string &s);
    ~ContactInfo();
    string getIP();
    unsigned long getPort();
    unsigned long long getValidSince();
    string* getSignature();
    string* getByteSeq();
    TNtrNr getTotalNotaryNr();
protected:
private:
    TNtrNr tNtrNr;
    string ip;
    unsigned long port;
    unsigned long long validSince;
    string signature;
    void deleteData();

    string byteSeqence;
};

#endif // CONTACTINFO_H
