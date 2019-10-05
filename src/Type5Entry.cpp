#include "Type5Entry.h"

Type5Entry::Type5Entry(string &str)
{
    createFromString(str);
}

void Type5Entry::createFromString(string &str)
{
    const unsigned long long l = str.length();
    if (l < 3 || str.at(0) != 0x24) return;
    string dum;
    size_t pos = 1;

    // read firstRefereeId
    if (l-pos < 20)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,20);
    firstRefereeId = CompleteID(dum);
    firstRefereeIdAsList.push_back(firstRefereeId);
    pos+=20;

    // tenure length
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    refereeTenure = u.byteSeqAsUl(dum);
    pos+=4;

    // refereeApplProcessingTime
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    refereeApplProcessingTime = u.byteSeqAsUl(dum);
    pos+=4;

    // refereeApplStake
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    refereeApplStake = u.byteSeqAsDbl(dum);
    pos+=8;

    // maxAppointments
    if (l-pos < 2)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,2);
    maxAppointments = u.byteSeqAsUs(dum);
    pos+=2;

    // initialLiquidity
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    initialLiquidity = u.byteSeqAsDbl(dum);
    pos+=8;

    // maxLiquidityCreation
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    maxLiquidityCreation = u.byteSeqAsDbl(dum);
    pos+=8;

    // parentShare
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    parentShare = u.byteSeqAsDbl(dum);
    pos+=8;

    // minTransfer
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,8);
    minTransfer = u.byteSeqAsDbl(dum);
    pos+=8;

    // minProcessingTime
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    minProcessingTime = u.byteSeqAsUl(dum);
    pos+=4;

    // maxProcessingTime
    if (l-pos < 4)
    {
        deleteData();
        return;
    }
    dum = str.substr(pos,4);
    maxProcessingTime = u.byteSeqAsUl(dum);
    pos+=4;

    // read numOfAmounts
    if (l-pos < 2)
    {
        deleteData();
        return;
    }
    string numOfAmountsStr = str.substr(pos,2);
    unsigned short numOfAmounts = u.byteSeqAsUs(numOfAmountsStr);
    if (numOfAmounts<2)
    {
        deleteData();
        return;
    }
    pos+=2;
    for (unsigned short i=0; i<numOfAmounts; i++)
    {
        // amounts
        if (l-pos < 8)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        amounts.push_back(u.byteSeqAsDbl(dum));
        pos+=8;
        // minimalFees
        if (l-pos < 8)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        minimalFees.push_back(u.byteSeqAsDbl(dum));
        pos+=8;
        // maximalFees
        if (l-pos < 8)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        maximalFees.push_back(u.byteSeqAsDbl(dum));
        pos+=8;
        // amountBasedRefereeStakes
        if (l-pos < 8)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        amountBasedRefereeStakes.push_back(u.byteSeqAsDbl(dum));
        pos+=8;
        // test recently added data
        size_t lastPos = amounts.size()-1;
        if (amounts.at(lastPos)<0 || amountBasedRefereeStakes.at(lastPos)<0
                || minimalFees.at(lastPos)<0 || maximalFees.at(lastPos)<0
                || minimalFees.at(lastPos)>maximalFees.at(lastPos))
        {
            deleteData();
            return;
        }
        if (lastPos>0 && amounts.at(lastPos)<=amounts.at(lastPos-1))
        {
            deleteData();
            return;
        }
    }

    // read numOfFees
    if (l-pos < 2)
    {
        deleteData();
        return;
    }
    string numOfFeesStr = str.substr(pos,2);
    unsigned short numOfFees = u.byteSeqAsUs(numOfFeesStr);
    if (numOfFees<2)
    {
        deleteData();
        return;
    }
    pos+=2;
    for (unsigned short i=0; i<numOfFees; i++)
    {
        // fees
        if (l-pos < 8)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        fees.push_back(u.byteSeqAsDbl(dum));
        pos+=8;
        // feeBasedRefereeStakes
        if (l-pos < 8)
        {
            deleteData();
            return;
        }
        dum = str.substr(pos,8);
        feeBasedRefereeStakes.push_back(u.byteSeqAsDbl(dum));
        pos+=8;
        // test recently added data
        size_t lastPos = fees.size()-1;
        if (fees.at(lastPos)<0 || feeBasedRefereeStakes.at(lastPos)<0)
        {
            deleteData();
            return;
        }
        if (lastPos>0 && fees.at(lastPos)<=fees.at(lastPos-1))
        {
            deleteData();
            return;
        }
    }

    // read rate
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string rateStr = str.substr(pos,8);
    rate = u.byteSeqAsDbl(rateStr);
    pos+=8;

    // read description length
    if (l-pos < 8)
    {
        deleteData();
        return;
    }
    string lenOfDescriptionStr = str.substr(pos,8);
    unsigned long long lenOfDescription = u.byteSeqAsUll(lenOfDescriptionStr);
    pos+=8;
    // load description
    if (l-pos < lenOfDescription || lenOfDescription < 1 || lenOfDescription>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
    description = str.substr(pos,lenOfDescription);
    pos+=lenOfDescription;

    if (l-pos == 2)
    {
        if (str.at(pos) == 0x04 || str.at(pos) == 0x0A || str.at(pos) == 0x0D) pos+=1;
    }
    if (l-pos == 1)
    {
        if (str.at(pos) == 0x04 || str.at(pos) == 0x0A || str.at(pos) == 0x0D) pos+=1;
    }

    if (pos != l || description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }
}

// NOTE: for mingw: replace "stat" with "_stat" for struct and function

Type5Entry::Type5Entry(const string& dirName, const string& fileName)
{
    string t5eSuffix(".t5e");
    if (u.has_suffix(fileName, t5eSuffix))
    {
        string path(dirName);
        path.append(fileName);
        ifstream f(path.c_str(), ios::binary);
        if (f.good())
        {
            // load byte sequence
            ostringstream ostrm;
            ostrm << f.rdbuf();
            string str(ostrm.str());
            f.close();
            createFromString(str);
            return;
        }
        else
        {
            puts(u.c(path, " could not be read").c_str());
            f.close();
            deleteData();
            return;
        }
    }

    // try to load from json file
    string fileNameJ(dirName);
    fileNameJ.append(fileName);
    struct stat fJInfo;
    if (stat(fileNameJ.c_str(), &fJInfo) != 0)
    {
        puts(u.c(fileNameJ, " could not be found").c_str());
        deleteData();
        return;
    }
    ifstream fJ(fileNameJ.c_str());
    if (!fJ.good())
    {
        puts(u.c(fileNameJ, " could not be read").c_str());
        fJ.close();
        deleteData();
        return;
    }
    json j;
    fJ >> j;
    fJ.close();

    // load data
    string firstRefereeIdStr = j["firstRefereeId"];
    firstRefereeId = CompleteID(firstRefereeIdStr);
    if (firstRefereeId.getNotary()<=0)
    {
        deleteData();
        return;
    }
    firstRefereeIdAsList.push_back(firstRefereeId);
    refereeTenure = j["refereeTenure"];
    refereeApplProcessingTime = j["refereeApplProcessingTime"];
    refereeApplStake = j["refereeApplStake"];
    maxAppointments = j["maxAppointments"];
    initialLiquidity = j["initialLiquidity"];
    maxLiquidityCreation = j["maxLiquidityCreation"];
    parentShare = j["parentShare"];
    minTransfer = j["minTransfer"];
    minProcessingTime = j["minProcessingTime"];
    maxProcessingTime = j["maxProcessingTime"];

    // read amounts etc.
    json listJ = j["amounts"];
    for (json::iterator it = listJ.begin(); it != listJ.end(); ++it) amounts.push_back(*it);
    listJ = j["minimalFees"];
    for (json::iterator it = listJ.begin(); it != listJ.end(); ++it) minimalFees.push_back(*it);
    listJ = j["maximalFees"];
    for (json::iterator it = listJ.begin(); it != listJ.end(); ++it) maximalFees.push_back(*it);
    listJ = j["amountBasedRefereeStakes"];
    for (json::iterator it = listJ.begin(); it != listJ.end(); ++it) amountBasedRefereeStakes.push_back(*it);
    // some checks
    if (amounts.size()!=minimalFees.size() || amounts.size()!=maximalFees.size()
            || amounts.size()!=amountBasedRefereeStakes.size() || amounts.size()<2)
    {
        deleteData();
        return;
    }
    for (size_t i=0; i<amounts.size(); i++)
    {
        if (amounts.at(i)<0 || amountBasedRefereeStakes.at(i)<0
                || minimalFees.at(i)<0 || maximalFees.at(i)<0
                || minimalFees.at(i)>maximalFees.at(i))
        {
            deleteData();
            return;
        }
        if (i>0 && amounts.at(i)<=amounts.at(i-1))
        {
            deleteData();
            return;
        }
    }

    // read fees etc.
    listJ = j["fees"];
    for (json::iterator it = listJ.begin(); it != listJ.end(); ++it) fees.push_back(*it);
    listJ = j["feeBasedRefereeStakes"];
    for (json::iterator it = listJ.begin(); it != listJ.end(); ++it) feeBasedRefereeStakes.push_back(*it);
    // some checks
    if (fees.size()!=feeBasedRefereeStakes.size() || fees.size()<2)
    {
        deleteData();
        return;
    }
    for (size_t i=0; i<fees.size(); i++)
    {
        if (fees.at(i)<0 || feeBasedRefereeStakes.at(i)<0)
        {
            deleteData();
            return;
        }
        if (i>0 && fees.at(i)<=fees.at(i-1))
        {
            deleteData();
            return;
        }
    }

    // read discount rate
    rate = j["rate"];

    // read description from specified file
    string descriptionFileName(dirName);
    descriptionFileName.append(j["descriptionFile"]);
    struct stat fileInfo;
    if (stat(descriptionFileName.c_str(), &fileInfo) != 0)
    {
        puts(u.c(descriptionFileName, " could not be found").c_str());
        deleteData();
        return;
    }
    ifstream fstrm(descriptionFileName.c_str(), ios::binary);
    if (!fstrm.good())
    {
        puts(u.c(descriptionFileName, " could not be read").c_str());
        fstrm.close();
        deleteData();
        return;
    }
    ostringstream ostrm;
    ostrm << fstrm.rdbuf();
    description = ostrm.str();
    fstrm.close();

    if (description.length()>((size_t) maxDescriptionLengthInK)*1000)
    {
        deleteData();
        return;
    }

    // create byte sequence and check
    string *byteSequence = getByteSeq();
    if (byteSequence == nullptr)
    {
        deleteData();
        return;
    }
    Type5Entry tmp(*byteSequence);
    if (!tmp.isGood()) deleteData();
}

Type5Entry::~Type5Entry()
{
    deleteData();
}

double Type5Entry::getInitialLiquidity()
{
    return initialLiquidity;
}

CompleteID Type5Entry::getOwnerID()
{
    return firstRefereeId;
}

list<CompleteID>* Type5Entry::getReferencedEntries()
{
    return &firstRefereeIdAsList;
}

list<CompleteID>* Type5Entry::getCurrenciesAndObligations()
{
    return nullptr;
}

string* Type5Entry::getByteSeqTrunc()
{
    if (!isGood()) return nullptr;
    if (byteSeqTrunc.length()<2)
    {
        byteSeqTrunc = "";
        byteSeqTrunc.push_back(0x24);
        byteSeqTrunc.append(firstRefereeId.to20Char());
        byteSeqTrunc.append(u.UlAsByteSeq(refereeTenure));
        byteSeqTrunc.append(u.UlAsByteSeq(refereeApplProcessingTime));
        byteSeqTrunc.append(u.DblAsByteSeq(refereeApplStake));
        byteSeqTrunc.append(u.UsAsByteSeq(maxAppointments));
        byteSeqTrunc.append(u.DblAsByteSeq(initialLiquidity));
        byteSeqTrunc.append(u.DblAsByteSeq(maxLiquidityCreation));
        byteSeqTrunc.append(u.DblAsByteSeq(parentShare));
        byteSeqTrunc.append(u.DblAsByteSeq(minTransfer));
        byteSeqTrunc.append(u.UlAsByteSeq(minProcessingTime));
        byteSeqTrunc.append(u.UlAsByteSeq(maxProcessingTime));
        byteSeqTrunc.append(u.UsAsByteSeq(amounts.size()));
        for (unsigned short i = 0; i<amounts.size(); i++)
        {
            byteSeqTrunc.append(u.DblAsByteSeq(amounts.at(i)));
            byteSeqTrunc.append(u.DblAsByteSeq(minimalFees.at(i)));
            byteSeqTrunc.append(u.DblAsByteSeq(maximalFees.at(i)));
            byteSeqTrunc.append(u.DblAsByteSeq(amountBasedRefereeStakes.at(i)));
        }
        byteSeqTrunc.append(u.UsAsByteSeq(fees.size()));
        for (unsigned short i = 0; i<fees.size(); i++)
        {
            byteSeqTrunc.append(u.DblAsByteSeq(fees.at(i)));
            byteSeqTrunc.append(u.DblAsByteSeq(feeBasedRefereeStakes.at(i)));
        }
        byteSeqTrunc.append(u.DblAsByteSeq(rate));

        string nullDescription("null");
        byteSeqTrunc.append(u.UllAsByteSeq(nullDescription.length()));
        byteSeqTrunc.append(nullDescription);
    }
    return &byteSeqTrunc;
}

string* Type5Entry::getByteSeq()
{
    if (!isGood()) return nullptr;
    if (byteSeq.length()<2)
    {
        byteSeq = "";
        byteSeq.push_back(0x24);
        byteSeq.append(firstRefereeId.to20Char());
        byteSeq.append(u.UlAsByteSeq(refereeTenure));
        byteSeq.append(u.UlAsByteSeq(refereeApplProcessingTime));
        byteSeq.append(u.DblAsByteSeq(refereeApplStake));
        byteSeq.append(u.UsAsByteSeq(maxAppointments));
        byteSeq.append(u.DblAsByteSeq(initialLiquidity));
        byteSeq.append(u.DblAsByteSeq(maxLiquidityCreation));
        byteSeq.append(u.DblAsByteSeq(parentShare));
        byteSeq.append(u.DblAsByteSeq(minTransfer));
        byteSeq.append(u.UlAsByteSeq(minProcessingTime));
        byteSeq.append(u.UlAsByteSeq(maxProcessingTime));
        byteSeq.append(u.UsAsByteSeq(amounts.size()));
        for (unsigned short i = 0; i<amounts.size(); i++)
        {
            byteSeq.append(u.DblAsByteSeq(amounts.at(i)));
            byteSeq.append(u.DblAsByteSeq(minimalFees.at(i)));
            byteSeq.append(u.DblAsByteSeq(maximalFees.at(i)));
            byteSeq.append(u.DblAsByteSeq(amountBasedRefereeStakes.at(i)));
        }
        byteSeq.append(u.UsAsByteSeq(fees.size()));
        for (unsigned short i = 0; i<fees.size(); i++)
        {
            byteSeq.append(u.DblAsByteSeq(fees.at(i)));
            byteSeq.append(u.DblAsByteSeq(feeBasedRefereeStakes.at(i)));
        }
        byteSeq.append(u.DblAsByteSeq(rate));
        byteSeq.append(u.UllAsByteSeq(description.length()));
        byteSeq.append(description);
    }
    return &byteSeq;
}

string* Type5Entry::getDescription()
{
    return &description;
}

bool Type5Entry::isGood()
{
    return (firstRefereeId.getNotary()>0 && description.length()>0);
}

int Type5Entry::getType()
{
    return 5;
}

unsigned long Type5Entry::getRefereeTenure()
{
    return refereeTenure;
}

double Type5Entry::getParentShare()
{
    return parentShare;
}

double Type5Entry::getRefApplStake()
{
    return refereeApplStake;
}

unsigned long Type5Entry::getRefApplProcessingTime()
{
    return refereeApplProcessingTime;
}

bool Type5Entry::isGoodOpPrProcessingTime(unsigned long t)
{
    return (minProcessingTime<=t && t<=maxProcessingTime);
}

bool Type5Entry::isGoodFee(double amount, double fee)
{
    double minFee = getMinimalFee(amount);
    double maxFee = getMaximalFee(amount);
    return (minFee<=fee && fee<=maxFee);
}

double Type5Entry::getMinimalFee(double amount)
{
    size_t p = amounts.size()-1;
    if (amount <= amounts.at(0))
    {
        p = 0;
    }
    else if (amount < amounts.at(p))
    {
        vector<double>::iterator it = lower_bound(amounts.begin(), amounts.end(), amount);
        p = it-amounts.begin()-1;
    }
    else p=amounts.size()-2;

    double amount1 = amounts.at(p);
    double amount2 = amounts.at(p+1);
    double r = (amount-amount1)/(amount2-amount1);
    // linear interpolation
    double fee1 = minimalFees.at(p);
    double fee2 = minimalFees.at(p+1);
    return r*fee2+(1-r)*fee1;
}

double Type5Entry::getMaximalFee(double amount)
{
    size_t p = amounts.size()-1;
    if (amount <= amounts.at(0))
    {
        p = 0;
    }
    else if (amount < amounts.at(p))
    {
        vector<double>::iterator it = lower_bound(amounts.begin(), amounts.end(), amount);
        p = it-amounts.begin()-1;
    }
    else p=amounts.size()-2;

    double amount1 = amounts.at(p);
    double amount2 = amounts.at(p+1);
    double r = (amount-amount1)/(amount2-amount1);
    // linear interpolation
    double fee1 = maximalFees.at(p);
    double fee2 = maximalFees.at(p+1);
    return r*fee2+(1-r)*fee1;
}

double Type5Entry::getRefereeStake(double amount, double fee)
{
    // get stake by amount
    size_t p = amounts.size()-1;
    if (amount <= amounts.at(0))
    {
        p = 0;
    }
    else if (amount < amounts.at(p))
    {
        vector<double>::iterator it = lower_bound(amounts.begin(), amounts.end(), amount);
        p = it-amounts.begin()-1;
    }
    else p=amounts.size()-2;

    double amount1 = amounts.at(p);
    double amount2 = amounts.at(p+1);
    double r = (amount-amount1)/(amount2-amount1);
    double stake1 = amountBasedRefereeStakes.at(p);
    double stake2 = amountBasedRefereeStakes.at(p+1);
    double stakeByAmount = r*stake2+(1-r)*stake1;
    // get stake by fee
    p = fees.size()-1;
    if (fee <= fees.at(0))
    {
        p = 0;
    }
    else if (fee < fees.at(p))
    {
        vector<double>::iterator it = lower_bound(fees.begin(), fees.end(), fee);
        p = it-fees.begin()-1;
    }
    else p=fees.size()-2;

    double fee1 = fees.at(p);
    double fee2 = fees.at(p+1);
    r = (fee-fee1)/(fee2-fee1);
    stake1 = feeBasedRefereeStakes.at(p);
    stake2 = feeBasedRefereeStakes.at(p+1);
    double stakeByFee = r*stake2+(1-r)*stake1;
    // return max
    return max(stakeByFee, stakeByAmount);
}

Entry* Type5Entry::createCopy()
{
    string str;
    str.append(*getByteSeq());
    Type5Entry* out = new Type5Entry(str);
    return out;
}

double Type5Entry::getRate()
{
    return rate;
}

unsigned long Type5Entry::minOpPrProcessingTime()
{
    return minProcessingTime;
}

unsigned long Type5Entry::maxOpPrProcessingTime()
{
    return maxProcessingTime;
}

double Type5Entry::getLiquidityCreationLimit()
{
    return maxLiquidityCreation;
}

unsigned short Type5Entry::getRefAppointmentLimit()
{
    return maxAppointments;
}

double Type5Entry::getMinTransferAmount()
{
    return minTransfer;
}

void Type5Entry::deleteData()
{
    firstRefereeId=CompleteID();
    firstRefereeIdAsList.clear();
    description="";
    rate=0;
    byteSeq="";
    byteSeqTrunc="";
}
