#ifndef UTIL_H
#define UTIL_H
#include <string>
#include "TNtrNr.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <limits.h>

using namespace std;
using json = nlohmann::json;

class Util
{
public:
    Util();
    ~Util();
    static int getType(string &entryStr);
    static bool descriptionTooLarge(string &descr);
    static unsigned int descriptionLimitInKB();
    static string UsAsByteSeq(unsigned short x);
    static string UlAsByteSeq(unsigned long x);
    static string UllAsByteSeq(unsigned long long x);
    static string UcAsByteSeq(unsigned char x);
    static string DblAsByteSeq(double x);
    static bool toHex(string &str, string &out);
    static bool fromHex(string &str, string &out);
    static unsigned long byteSeqAsUl(string &x);
    static unsigned long long byteSeqAsUll(string &x);
    static double byteSeqAsDbl(string &x);
    static unsigned short byteSeqAsUs(string &x);
    static unsigned char byteSeqAsUc(string x);
    static bool roughlyTheSameW(double x, double y);
    static bool roughlyTheSameS(double x, double y, double comparedTo);
    bool roughlyLargerOrEqual(double x, double y, double comparedTo);
    static string c(string &str1, const char * str2);
    static string c(const char * str1, string str2);
    static string c(const char * str1, const char * str2);
    static double getDiscountFactor(unsigned long long t, unsigned long long T, double r);
    static string epochToStrWithBr(unsigned long long timeInMs);
    static string epochToStr(unsigned long long timeInMs);
    static string epochToStr2(unsigned long long timeInMs);
    static unsigned long long str2ToEpoch(string str);
    static string splitInTheMiddle(string str);
    static string toString(double value, unsigned int precision);
    static string b61hide(string str);
    static string b61show(string str);
    static string flip(string str);
    static string correctColon(string str);
    static void breakAfter3Digits(string &str);
    static unsigned short getRange(double amount, double minim);
    static bool has_suffix(const string &str, const string &suffix);

    struct ServerConf
    {
        string dbDir;

        string ownIP;
        int ownPort;
        TNtrNr ownNotaryNr;
        string privateKeyFile;

        string otherServerIP;
        int otherServerPort;
        TNtrNr otherServerNotaryNr;
        unsigned long long otherServerValidSince;
        string otherServerPublicKeyFile;
    };
    static bool loadServerConf(string &fileName, ServerConf &conf);

    static bool LEAPYEAR(int y)
    {
        return (!((y) % 4) && (((y) % 100) || !((y) % 400)));
    };
    static unsigned long YEARSIZE(int y)
    {
        return (LEAPYEAR(y) ? 366 : 365);
    };
    static unsigned long DAYS(bool leap, unsigned int month)
    {
        if (month % 12 >= 7)
        {
            if ((month-7) % 2 == 0) return 31;
            return 30;
        }
        if (month==1)
        {
            if (leap) return 29;
            return 28;
        }
        if (month % 2 == 0) return 31;
        return 30;
    };
    static void gmtime(unsigned long long epochSecs, struct tm * timep);
    static unsigned long long countMs(struct tm * timeinfo);
    static bool removeEnding(string &str, string ending);
protected:
private:
    static const char* const zeroToF;
};

#endif // UTIL_H
