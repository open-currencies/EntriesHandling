#include "Util.h"
#include "Entry.h"

#define equalityThresholdW 0.00005
#define equalityThresholdS 0.000000000001

#define YEAR0 1900
#define EPOCH_YR 1970
#define SECS_DAY 86400
#define SECS_YEAR 31557600

const char* const Util::zeroToF = "0123456789ABCDEF";

Util::Util()
{
    //ctor
}

Util::~Util()
{
    //dtor
}

bool Util::has_suffix(const string &str, const string &suffix)
{
    return (str.size() >= suffix.size() &&
            str.compare(str.size()-suffix.size(), suffix.size(), suffix) == 0);
}

bool Util::descriptionTooLarge(string &descr)
{
    return descr.length() > ((size_t) maxDescriptionLengthInK)*1000;
}

unsigned int Util::descriptionLimitInKB()
{
    return maxDescriptionLengthInK;
}

// returns string of length 4
string Util::UlAsByteSeq(unsigned long x)
{
    const int l = sizeof(unsigned long);
    char *seq = new char[l];
    memcpy(seq, &x, l);
    string out;
    out.push_back(seq[0]);
    out.push_back(seq[1]);
    out.push_back(seq[2]);
    out.push_back(seq[3]);
    delete[] seq;
    return out;
}

string Util::UsAsByteSeq(unsigned short x)
{
    const int l = sizeof(unsigned short);
    char *seq = new char[l];
    memcpy(seq, &x, l);
    string out;
    out.push_back(seq[0]);
    out.push_back(seq[1]);
    delete[] seq;
    return out;
}

string Util::UcAsByteSeq(unsigned char x)
{
    const int l = sizeof(unsigned char);
    char *seq = new char[l];
    memcpy(seq, &x, l);
    string out;
    out.push_back(seq[0]);
    delete[] seq;
    return out;
}

string Util::UllAsByteSeq(unsigned long long x)
{
    const int l = sizeof(unsigned long long);
    char *seq = new char[l];
    memcpy(seq, &x, l);
    string out;
    out.push_back(seq[0]);
    out.push_back(seq[1]);
    out.push_back(seq[2]);
    out.push_back(seq[3]);
    out.push_back(seq[4]);
    out.push_back(seq[5]);
    out.push_back(seq[6]);
    out.push_back(seq[7]);
    delete[] seq;
    return out;
}

string Util::DblAsByteSeq(double x)
{
    const int l = sizeof(double);
    char *seq = new char[l];
    memcpy(seq, &x, l);
    string out;
    out.push_back(seq[0]);
    out.push_back(seq[1]);
    out.push_back(seq[2]);
    out.push_back(seq[3]);
    out.push_back(seq[4]);
    out.push_back(seq[5]);
    out.push_back(seq[6]);
    out.push_back(seq[7]);
    delete[] seq;
    return out;
}

unsigned char Util::byteSeqAsUc(string x)
{
    const int l = sizeof(unsigned char);
    char *seq = new char[l];
    const int sl = x.length();
    const int d = max(0,l-sl);
    for (int i=0; i<l-d; i++)
    {
        seq[i]=x.at(i);
    }
    for (int i=l-d; i<l; i++)
    {
        seq[i]=0;
    }
    unsigned char out;
    memcpy(&out, seq, l);
    delete[] seq;
    return out;
}

unsigned short Util::byteSeqAsUs(string &x)
{
    const int l = sizeof(unsigned short);
    char *seq = new char[l];
    const int sl = x.length();
    const int d = max(0,l-sl);
    for (int i=0; i<l-d; i++)
    {
        seq[i]=x.at(i);
    }
    for (int i=l-d; i<l; i++)
    {
        seq[i]=0;
    }
    unsigned short out;
    memcpy(&out, seq, l);
    delete[] seq;
    return out;
}

unsigned long Util::byteSeqAsUl(string &x)
{
    const int l = sizeof(unsigned long);
    char *seq = new char[l];
    const int sl = x.length();
    const int d = max(0,l-sl);
    for (int i=0; i<l-d; i++)
    {
        seq[i]=x.at(i);
    }
    for (int i=l-d; i<l; i++)
    {
        seq[i]=0;
    }
    unsigned long out;
    memcpy(&out, seq, l);
    delete[] seq;
    return out;
}

unsigned long long Util::byteSeqAsUll(string &x)
{
    const int l = sizeof(unsigned long long);
    char *seq = new char[l];
    const int sl = x.length();
    const int d = max(0,l-sl);
    for (int i=0; i<l-d; i++)
    {
        seq[i]=x.at(i);
    }
    for (int i=l-d; i<l; i++)
    {
        seq[i]=0;
    }
    unsigned long long out;
    memcpy(&out, seq, l);
    delete[] seq;
    return out;
}

double Util::byteSeqAsDbl(string &x)
{
    const int l = sizeof(double);
    char *seq = new char[l];
    const int sl = x.length();
    const int d = max(0,l-sl);
    for (int i=0; i<l-d; i++)
    {
        seq[i]=x.at(i);
    }
    for (int i=l-d; i<l; i++)
    {
        seq[i]=0;
    }
    double out;
    memcpy(&out, seq, l);
    delete[] seq;
    return out;
}

bool Util::fromHex(string &str, string &out)
{
    if (str.length() % 2 != 0 || out.length() > 0) return false;
    for(size_t i=0; i<str.length(); i+=2)
    {
        char a = str.at(i);
        char b = str.at(i+1);
        const char* p = lower_bound(zeroToF, zeroToF + 16, a);
        const char* q = lower_bound(zeroToF, zeroToF + 16, b);
        if (*p != a || *q != b) return false;
        out.push_back(((p - zeroToF) << 4) | (q - zeroToF));
    }
    return true;
}

bool Util::toHex(string &str, string &out)
{
    if (out.length() > 0) return false;
    for (size_t i=0; i<str.length(); ++i)
    {
        const unsigned char c = (unsigned char) str.at(i);
        out.push_back(zeroToF[c >> 4]);
        out.push_back(zeroToF[c & 15]);
    }
    return true;
}

int Util::getType(string &entryStr)
{
    if (entryStr.length()<1) return 0;
    return entryStr.at(0)-0x20+1;
}

string Util::c(string &str1, const char * str2)
{
    return c(str1.c_str(), str2);
}

string Util::c(const char * str1, string str2)
{
    return c(str1, str2.c_str());
}

string Util::c(const char * str1, const char * str2)
{
    string out(str1);
    out.append(str2);
    return out;
}

string Util::flip(string str)
{
    size_t halfLen = str.length()/2;
    for (size_t i = 0; i < halfLen; i++) swap(str[i], str[str.length() - i - 1]);
    return str;
}

bool Util::roughlyTheSameW(double x, double y)
{
    if (x>y) return false;
    if (x == 0) return (y==0);
    if (y == 0) return (x==0);
    if (x<0 && y>0) return false;
    if (x>0 && y<0) return false;
    double z = (x-y) * 2 / (x+y);
    return (z<equalityThresholdW && -z<equalityThresholdW);
}

bool Util::roughlyTheSameS(double x, double y, double comparedTo)
{
    if (x==y) return true;
    if (comparedTo <= 0) comparedTo = (abs(x)+abs(y)) / 2;
    if (comparedTo <= 0) return false;
    double z = (x-y) / comparedTo;
    return (z<equalityThresholdS && -z<equalityThresholdS);
}

bool Util::roughlyLargerOrEqual(double x, double y, double comparedTo)
{
    return (x>=y || roughlyTheSameS(x, y, comparedTo));
}

double Util::getDiscountFactor(unsigned long long t, unsigned long long T, double r)
{
    if (t>=T) return 1;
    double expFactor=log(r*0.01+1);
    double timeDiffInYears = (T-t);
    timeDiffInYears = timeDiffInYears/1000/SECS_YEAR;
    return exp(timeDiffInYears*expFactor);
}

bool Util::loadServerConf(string &fileName, ServerConf &conf)
{
    // read the JSON file
    ifstream i(fileName);
    json j;
    i >> j;
    i.close();

    // check that everything is set
    if (j.count("dbDir") != 1)
    {
        puts("dbDir not specified");
        return false;
    }
    if (j.count("ownIP") != 1)
    {
        puts("ownIP not specified");
        return false;
    }
    if (j.count("ownPort") != 1)
    {
        puts("ownPort not specified");
        return false;
    }
    if (j.count("ownNotaryNr") != 1)
    {
        puts("ownNotaryNr not specified");
        return false;
    }
    if (j.count("otherServerIP") != 1)
    {
        puts("otherServerIP not specified");
        return false;
    }
    if (j.count("otherServerPort") != 1)
    {
        puts("otherServerPort not specified");
        return false;
    }
    if (j.count("otherServerNotaryNr") != 1)
    {
        puts("otherServerNotaryNr not specified");
        return false;
    }
    if (j.count("otherServerValidSince") != 1)
    {
        puts("otherServerValidSince not specified");
        return false;
    }
    if (j.count("privateKeyFile") != 1)
    {
        puts("privateKeyFile not specified");
        return false;
    }

    // load into conf
    conf.dbDir=j["dbDir"];
    conf.ownIP=j["ownIP"];
    conf.ownPort=j["ownPort"];
    conf.ownNotaryNr=TNtrNr(j["ownNotaryNr"].get<string>());
    if (conf.ownNotaryNr.isZero())
    {
        puts("not a notary");
    }
    else if (!conf.ownNotaryNr.isGood())
    {
        puts("ownNotaryNr has bad format");
        return false;
    }
    conf.otherServerIP=j["otherServerIP"];
    conf.otherServerPort=j["otherServerPort"];
    conf.otherServerNotaryNr=TNtrNr(j["otherServerNotaryNr"].get<string>());
    if (!conf.otherServerNotaryNr.isGood())
    {
        puts("otherServerNotaryNr has bad format");
        return false;
    }
    conf.otherServerPublicKeyFile=j["otherServerPublicKeyFile"];
    if (conf.otherServerPublicKeyFile.length()<=0)
    {
        puts("otherServerPublicKeyFile not supplied");
        return false;
    }
    conf.otherServerValidSince=j["otherServerValidSince"];
    conf.privateKeyFile=j["privateKeyFile"];
    if (conf.privateKeyFile.length()<=0 && !conf.ownNotaryNr.isZero())
    {
        puts("privateKeyFile not supplied, but non-zero ownNotaryNr specified");
        return false;
    }
    if (conf.privateKeyFile.length()>0 && conf.ownNotaryNr.isZero())
    {
        puts("privateKeyFile supplied, but no ownNotaryNr specified");
        return false;
    }

    return true;
}

bool Util::removeEnding(string &str, string ending)
{
    if (ending.length()>str.length()) return false;
    size_t diff=str.length()-ending.length();
    if (str.substr(diff,ending.length()).compare(ending)!=0) return false;
    str = str.substr(0, diff);
    return true;
}

string Util::correctColon(string str)
{
    size_t pos = str.find(":");
    while (pos != string::npos)
    {
        str.replace(pos, 1, "&#58;");
        pos = str.find(":", pos);
    }
    return str;
}

string Util::epochToStr2(unsigned long long timeInMs)
{
    unsigned long long timeInS = timeInMs/1000;
    if (timeInMs - timeInS*1000 >= 500) timeInS+=1;
    struct tm timeinfo;
    gmtime(timeInS, &timeinfo);
    char buffer [80];
    size_t strLen = strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &timeinfo);
    string out(buffer, strLen);
    out.append(" UTC");
    return out;
}

string Util::epochToStr(unsigned long long timeInMs)
{
    unsigned long long timeInS = timeInMs/1000;
    if (timeInMs - timeInS*1000 >= 500) timeInS+=1;
    struct tm timeinfo;
    gmtime(timeInS, &timeinfo);
    char buffer [80];
    size_t strLen = strftime(buffer, 80, "%d %b %Y %H:%M:%S", &timeinfo);
    string out(buffer, strLen);
    out.append(" UTC");
    return out;
}

string Util::epochToStrWithBr(unsigned long long timeInMs)
{
    unsigned long long timeInS = timeInMs/1000;
    if (timeInMs - timeInS*1000 >= 500) timeInS+=1;
    struct tm timeinfo;
    gmtime(timeInS, &timeinfo);
    char buffer [80];
    size_t strLen = strftime (buffer, 80, "%d %b %Y <br>\n %H:%M:%S", &timeinfo);
    string out(buffer, strLen);
    out.append(" UTC");
    return out;
}

unsigned long long Util::str2ToEpoch(string str)
{
    while (removeEnding(str, " "));
    while (removeEnding(str, " UTC"));
    while (removeEnding(str, " "));
    string format("%Y-%m-%d");
    if (str.find(" ") != string::npos) format="%Y-%m-%d %H:%M:%S";
    tm timeinfo = {};
    istringstream ss(str.c_str());
    ss>>get_time(&timeinfo, format.c_str());
    if (ss.fail()) return 0;
    if (format.compare("%Y-%m-%d")==0)
    {
        timeinfo.tm_hour=0;
        timeinfo.tm_min=0;
        timeinfo.tm_sec=0;
    }
    return countMs(&timeinfo);
}

unsigned long long Util::countMs(struct tm * timeinfo)
{
    unsigned long long out=0;

    // count days in past years
    for (int year = EPOCH_YR; year < YEAR0 + timeinfo->tm_year; year++) out+=YEARSIZE(year);

    // count days for this year
    bool leap = LEAPYEAR(YEAR0 + timeinfo->tm_year);
    for (int mon = 0; mon < timeinfo->tm_mon; mon++) out+=DAYS(leap, mon);
    out+=(timeinfo->tm_mday-1);

    // count ms for this day
    out*=24;
    out+=timeinfo->tm_hour;
    out*=60;
    out+=timeinfo->tm_min;
    out*=60;
    out+=timeinfo->tm_sec;
    out*=1000;

    return out;
}

void Util::gmtime(unsigned long long epochSecs, struct tm * timep)
{
    // determine clock and day in week
    unsigned long dayclock = (unsigned long)epochSecs % SECS_DAY;
    unsigned long dayno = (unsigned long)epochSecs / SECS_DAY;
    timep->tm_sec = dayclock % 60;
    timep->tm_min = (dayclock % 3600) / 60;
    timep->tm_hour = dayclock / 3600;
    timep->tm_wday = (dayno + 4) % 7; // day 0 was a Thursday

    // determine the year
    int year = EPOCH_YR;
    while (dayno >= YEARSIZE(year))
    {
        dayno -= YEARSIZE(year);
        year++;
    }
    timep->tm_year = year - YEAR0;
    timep->tm_yday = dayno;

    // determine month and day in month
    timep->tm_mon = 0;
    unsigned long d = DAYS(LEAPYEAR(year), timep->tm_mon);
    while (dayno >= d)
    {
        dayno -= d;
        timep->tm_mon++;
        d = DAYS(LEAPYEAR(year), timep->tm_mon);
    }
    timep->tm_mday = dayno + 1;
    timep->tm_isdst = 0;
}

string Util::splitInTheMiddle(string str)
{
    size_t middlepos = str.length()/2;
    string out=str.substr(0, middlepos);
    string str2=str.substr(middlepos, str.length());
    out.append("-<br>\n");
    out.append(str2);
    return out;
}

string Util::toString(double value, unsigned int precision)
{
    stringstream out;
    out << fixed << setprecision(precision) << value;
    return out.str();
}

void Util::breakAfter3Digits(string &str)
{
    if (str.length()<7) return;
    if (str.at(str.length()-3) != '.') return;
    for (size_t pos = str.length()-6; pos>0; pos-=3)
    {
        str.insert(pos, " ");
        if (pos <= 3) break;
    }
}

unsigned short Util::getRange(double amount, double minim)
{
    if (minim<=0 || amount<=minim) return 0;
    unsigned short i = 0;
    for (; i<100; i++)
    {
        minim*=10;
        if (amount < minim) return i;
    }
    return i;
}

string Util::b61hide(string str)
{
    string out;
    for (size_t i=0; i<str.length(); i++)
    {
        int value;
        if (str[i]<='9') value = (str[i]-'1');
        else if (str[i]<='Z') value = 9 + (str[i]-'A');
        else value = 35 + (str[i]-'a');
        out.push_back((char) (unsigned char) (unsigned int) value);
    }
    return out;
}

string Util::b61show(string str)
{
    string out;
    for (size_t i=0; i<str.length(); i++)
    {
        unsigned char oldVal = (unsigned char) str[i];
        char value = '1' + oldVal;
        if (oldVal>8) value += 7;
        if (oldVal>34) value += 6;
        out.push_back(value);
    }
    return out;
}
