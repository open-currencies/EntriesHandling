#include "Type1Entry.h"

Type1Entry::Type1Entry()
{
}

bool Type1Entry::createFromString(string &str)
{
    if (lineages.size() != 0 || extensions.size() != 0)
    {
        puts("preexisting data in type1entry");
        return false;
    }
    Util u;
    size_t pos = 1;
    size_t skip = 0; // number of chars to skip at the and
    size_t len = str.length();
    if (len < 3 || str.at(0) != 0x20) return false;
    unsigned long long lastkickoff = 0;
    unsigned long long maxPossibleFreshnessTime = 0;
    unsigned long long maxRunningExtension = 0;
    set<string> keyStringsAll;
    while (pos < len)
    {
        if (len-pos < 6)
        {
            return false;
        }
        LineageData *lineage = new LineageData();
        string t = str.substr(pos,2);
        lineage->maxActing = u.byteSeqAsUs(t);
        pos+=2;
        t = str.substr(pos,2);
        lineage->neededSignatures = u.byteSeqAsUs(t);
        pos+=2;
        t = str.substr(pos,2);
        lineage->maxWaiting = u.byteSeqAsUs(t);
        pos+=2;
        if (len-pos < 4 * lineage->maxActing)
        {
            delete lineage;
            deleteData();
            return false;
        }
        unsigned long* keyLengths = new unsigned long[lineage->maxActing];
        unsigned long totalKeysLength = 0;
        for (unsigned short i=0; i<lineage->maxActing; i++)
        {
            t = str.substr(pos,4);
            keyLengths[i] = u.byteSeqAsUl(t);
            totalKeysLength += keyLengths[i];
            pos+=4;
        }
        if (len-pos < totalKeysLength)
        {
            delete lineage;
            deleteData();
            return false;
        }
        for (unsigned short i=0; i<lineage->maxActing; i++)
        {
            t = str.substr(pos,keyLengths[i]);

            if (keyStringsAll.count(t)>0)
            {
                delete lineage;
                deleteData();
                return false;
            }
            else keyStringsAll.insert(t);

            lineage->keyStrings.push_back(t);
            pos+=keyLengths[i];
        }
        delete[] keyLengths;
        if (len-pos < 48)
        {
            delete lineage;
            deleteData();
            return false;
        }
        t = str.substr(pos,4);
        lineage->tenureDuration = u.byteSeqAsUl(t);
        pos+=4;
        t = str.substr(pos,4);
        lineage->maxNotarizationTime = u.byteSeqAsUl(t);
        pos+=4;
        t = str.substr(pos,4);
        lineage->freshnessTime = u.byteSeqAsUl(t);
        pos+=4;
        t = str.substr(pos,4);
        lineage->proposalProcessingTime = u.byteSeqAsUl(t);
        pos+=4;
        t = str.substr(pos,8);
        lineage->firstTenureStart = u.byteSeqAsUll(t);
        pos+=8;
        t = str.substr(pos,8);
        lineage->fee = u.byteSeqAsDbl(t);
        pos+=8;
        t = str.substr(pos,8);
        lineage->penaltyFactor1 = u.byteSeqAsDbl(t);
        pos+=8;
        t = str.substr(pos,8);
        lineage->penaltyFactor2 = u.byteSeqAsDbl(t);
        pos+=8;

        lineages.push_back(lineage);

        // skip some trailing chars if useless
        if (len-pos <= 3)
        {
            skip = len-pos;
            pos = len;
        }

        // get lineage transition parameters
        if (pos < len)
        {
            if (len-pos < 24)
            {
                deleteData();
                return false;
            }
            ExtensionData *ext = new ExtensionData();
            t = str.substr(pos,8);
            ext->cutoff = u.byteSeqAsUll(t);
            pos+=8;
            t = str.substr(pos,8);
            ext->extension = u.byteSeqAsUll(t);
            pos+=8;
            t = str.substr(pos,8);
            ext->kickoff = u.byteSeqAsUll(t);
            pos+=8;
            if (ext->cutoff < lastkickoff || ext->cutoff >= ext->kickoff)
            {
                delete ext;
                deleteData();
                return false;
            }
            maxPossibleFreshnessTime = max(maxPossibleFreshnessTime, ext->cutoff + ext->extension);
            ext->maxPossibleFreshnessTime = maxPossibleFreshnessTime;
            maxRunningExtension = max(maxRunningExtension,  ext->extension);
            ext->maxRunningExtension = maxRunningExtension;
            extensions.push_back(ext);
        }
    }

    if (pos != len || extensions.size() != lineages.size()-1)
    {
        deleteData();
        return false;
    }

    byteSequence = str.substr(0, len-skip);
    return true;
}

// NOTE: for mingw: replace "stat" with "_stat" for struct and function

Type1Entry::Type1Entry(const string& dirName)
{
    Util u;
    string fileName(dirName);
    fileName.append("/entry.t1e");
    struct stat fInfo;
    if (stat(fileName.c_str(), &fInfo) == 0)
    {
        ifstream f(fileName.c_str(), ios::binary);
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
        puts(u.c(fileName, " could not be read").c_str());
        f.close();
        return;
    }
    // try to load from json file
    string fileNameJ(dirName);
    fileNameJ.append("/entry.json");
    puts(u.c("attempting to read ", fileNameJ).c_str());
    struct stat fJInfo;
    if (stat(fileNameJ.c_str(), &fJInfo) != 0)
    {
        puts(u.c(fileNameJ, " could not be found").c_str());
        return;
    }
    ifstream fJ(fileNameJ.c_str());
    if (!fJ.good())
    {
        puts(u.c(fileNameJ, " could not be read").c_str());
        fJ.close();
        return;
    }
    json j;
    fJ >> j;
    fJ.close();

    // create str from json object j
    string str;
    str.push_back(0x20);
    unsigned short lineagesNr = j["lineagesNr"];
    for (unsigned short i=1; i<=lineagesNr; i++)
    {
        string k("lineage");
        k += to_string(i);
        puts(u.c("reading ", k).c_str());
        json lineage = j[k];
        // read lineage
        unsigned short maxActing = lineage["maxActing"];
        unsigned short neededSignatures = lineage["neededSignatures"];
        unsigned short maxWaiting = lineage["maxWaiting"];
        vector<string> keyStrings;
        for (unsigned short j=1; j<=maxActing; j++)
        {
            string fileNameK(dirName);
            fileNameK += "/(";
            fileNameK += to_string(i);
            fileNameK += ",";
            fileNameK += to_string(j);
            fileNameK += ").PBLCkey";
            puts(u.c("attempting to read ", fileNameK).c_str());
            struct stat fKInfo;
            if (stat(fileNameK.c_str(), &fKInfo) != 0)
            {
                puts(u.c(fileNameK, " could not be found").c_str());
                return;
            }
            ifstream fK(fileNameK.c_str(), ios::binary);
            if (!fK.good())
            {
                puts(u.c(fileNameK, " could not be read").c_str());
                fK.close();
                return;
            }
            ostringstream ostrm;
            ostrm << fK.rdbuf();
            string keyStr(ostrm.str());
            fK.close();
            keyStrings.push_back(keyStr);
        }
        unsigned long tenureDuration = lineage["tenureDuration"];
        unsigned long maxNotarizationTime = lineage["maxNotarizationTime"];
        unsigned long freshnessTime = lineage["freshnessTime"];
        unsigned long proposalProcessingTime = lineage["proposalProcessingTime"];
        unsigned long long firstTenureStart = lineage["firstTenureStart"];
        double fee = lineage["fee"];
        double penaltyFactor1 = lineage["penaltyFactor1"];
        double penaltyFactor2 = lineage["penaltyFactor2"];

        if (keyStrings.size() != maxActing)
        {
            perror(u.c(fileNameJ, " has bad format").c_str());
            return;
        }

        // write to str
        str.append(u.UsAsByteSeq(maxActing));
        str.append(u.UsAsByteSeq(neededSignatures));
        str.append(u.UsAsByteSeq(maxWaiting));
        for (unsigned short j=0; j<maxActing; j++)
        {
            str.append(u.UlAsByteSeq(keyStrings.at(j).size()));
        }
        for (unsigned short j=0; j<maxActing; j++)
        {
            str.append(keyStrings.at(j));
        }
        str.append(u.UlAsByteSeq(tenureDuration));
        str.append(u.UlAsByteSeq(maxNotarizationTime));
        str.append(u.UlAsByteSeq(freshnessTime));
        str.append(u.UlAsByteSeq(proposalProcessingTime));
        str.append(u.UllAsByteSeq(firstTenureStart));
        str.append(u.DblAsByteSeq(fee));
        str.append(u.DblAsByteSeq(penaltyFactor1));
        str.append(u.DblAsByteSeq(penaltyFactor2));

        // read cutoff etc.
        if (i<lineagesNr)
        {
            k = "cutoff";
            k += to_string(i);
            unsigned long long cutoff = j[k];
            k = "extension";
            k += to_string(i);
            unsigned long long extension = j[k];
            k = "kickoff";
            k += to_string(i);
            unsigned long long kickoff = j[k];

            str.append(u.UllAsByteSeq(cutoff));
            str.append(u.UllAsByteSeq(extension));
            str.append(u.UllAsByteSeq(kickoff));
        }
    }

    // load from string
    if (!createFromString(str))
    {
        puts("could not create Type1Entry from string");
        return;
    }

    // save str to file "entry.t1e"
    saveToFile(fileName);
}

void Type1Entry::saveToFile(string &fileName)
{
    // save str to file "entry.t1e"
    ofstream outfile(fileName.c_str(), ios::binary);
    outfile << byteSequence;
    outfile.close();
}

bool Type1Entry::isGood()
{
    return (lineages.size()>0 && extensions.size() == lineages.size()-1);
}

void Type1Entry::deleteData()
{
    unsigned short l = lineages.size();
    for (unsigned short j=0; j<l; j++)
    {
        delete lineages.at(j);
    }
    lineages.clear();

    l = extensions.size();
    for (unsigned short j=0; j<l; j++)
    {
        delete extensions.at(j);
    }
    extensions.clear();

    byteSequence = "";
}

Type1Entry::~Type1Entry()
{
    deleteData();
}

unsigned long long Type1Entry::getLatestMaxNotarizationTime()
{
    unsigned int lineage = lineages.size();
    if (lineage == 0) return 0;
    return lineages.at(lineage-1)->maxNotarizationTime;
}

unsigned long long Type1Entry::getMaxNotarizationTime(unsigned long long time)
{
    unsigned int lineage = getLineage(time);
    if (lineage == 0) return 0;
    return lineages.at(lineage-1)->maxNotarizationTime;
}

unsigned long long Type1Entry::getMaxNotarizationTimeByLineage(unsigned int lineage)
{
    return lineages.at(lineage-1)->maxNotarizationTime;
}

unsigned long long Type1Entry::getNotarizationTimeLimit(unsigned long long time)
{
    unsigned int lineage = getLineage(time);
    if (lineage == 0) return 0;
    unsigned long long maxNotarizationTime = lineages.at(lineage-1)->maxNotarizationTime;
    if (extensions.size() > lineage-1) return min(extensions.at(lineage-1)->cutoff, time + maxNotarizationTime);
    return time + maxNotarizationTime;
}

unsigned long Type1Entry::getFreshnessTime(unsigned int lineage)
{
    return lineages.at(lineage-1)->freshnessTime;
}

unsigned long Type1Entry::getNotaryTenure(unsigned int lineage)
{
    return lineages.at(lineage-1)->tenureDuration;
}

unsigned short Type1Entry::getMaxActing(unsigned int lineage)
{
    return lineages.at(lineage-1)->maxActing;
}

unsigned short Type1Entry::getMaxWaiting(unsigned int lineage)
{
    return lineages.at(lineage-1)->maxWaiting;
}

unsigned short Type1Entry::getNeededSignatures(unsigned int lineage)
{
    return lineages.at(lineage-1)->neededSignatures;
}

unsigned long Type1Entry::getProposalProcessingTime(unsigned int lineage)
{
    return lineages.at(lineage-1)->proposalProcessingTime;
}

double Type1Entry::getPenaltyFactor1(unsigned int lineage)
{
    return lineages.at(lineage-1)->penaltyFactor1;
}

double Type1Entry::getPenaltyFactor2(unsigned int lineage)
{
    return lineages.at(lineage-1)->penaltyFactor2;
}

unsigned long long Type1Entry::getCutOff(unsigned int lineage)
{
    return extensions.at(lineage-1)->cutoff;
}

unsigned long long Type1Entry::getExtension(unsigned int lineage)
{
    return extensions.at(lineage-1)->extension;
}

unsigned long long Type1Entry::getKickOff(unsigned int lineage)
{
    return extensions.at(lineage-1)->kickoff;
}

unsigned short Type1Entry::latestLin()
{
    return lineages.size();
}

string* Type1Entry::getPublicKey(unsigned int lineage, unsigned long notary)
{
    return &lineages.at(lineage-1)->keyStrings.at(notary-1);
}

unsigned long long Type1Entry::getNotaryTenureStartFormal(unsigned int lineage, unsigned long notary)
{
    unsigned long long notaryTenure = getNotaryTenure(lineage);
    notaryTenure *= 1000;
    double delta = ((double) notaryTenure) / getMaxActing(lineage);
    unsigned long long out = (unsigned long long)(delta * (notary-1));
    out += lineages.at(lineage-1)->firstTenureStart;

    return out;
}

unsigned long long Type1Entry::getNotaryTenureStartCorrected(unsigned int lineage, unsigned long notary)
{
    unsigned long long out = getNotaryTenureStartFormal(lineage, notary);

    if (lineage==1) return out;
    return max(out, extensions.at(lineage-2)->kickoff);
}

unsigned long long Type1Entry::getNotaryTenureEndFormal(unsigned int lineage, unsigned long notary)
{
    unsigned long long notaryTenure = getNotaryTenure(lineage);
    notaryTenure *= 1000;
    double delta = ((double) notaryTenure) / getMaxActing(lineage);
    unsigned long long out = (unsigned long long)(delta * (notary-1));
    out += lineages.at(lineage-1)->firstTenureStart;

    out += notaryTenure;

    return out;
}

unsigned long long Type1Entry::getNotaryTenureEndCorrected(unsigned int lineage, unsigned long notary)
{
    unsigned long long out = getNotaryTenureEndFormal(lineage, notary);

    if (lineage==latestLin()) return out;
    return min(out, extensions.at(lineage-1)->cutoff);
}

unsigned short Type1Entry::getLineage(unsigned long long time)
{
    unsigned short l = extensions.size();
    if (l == 0) return 1;
    unsigned int candidate = l+1; // candidate is the latest lineage
    for (unsigned short i=0; i<l; i++)
    {
        if (time < extensions.at(i)->cutoff)
        {
            candidate = i+1;
            break;
        }
    }
    if (candidate >= 2 && time < extensions.at(candidate-2)->kickoff) return 0;
    return candidate;
}

bool Type1Entry::isFresh(CompleteID &id, unsigned long long currentTime)
{
    if (id.getNotary() <= 0) return false;
    unsigned long long entryTime = id.getTimeStamp();
    unsigned short lineage = getLineage(entryTime);
    if (lineage == 0) return false;
    unsigned long long freshBefore = getFreshnessTime(lineage);
    freshBefore *= 1000;
    freshBefore += entryTime;
    freshBefore = getExtentedFreshness(lineage, freshBefore);
    return (currentTime < freshBefore);
}

unsigned long long Type1Entry::getExtentedFreshness(unsigned int lineage, unsigned long long freshBefore)
{
    unsigned int i = lineage-1;
    if (i <= extensions.size()) return freshBefore;
    if (freshBefore <= extensions.at(i)->cutoff) return freshBefore;
    return extensions.at(i)->cutoff + extensions.at(i)->extension;
}

bool Type1Entry::isActingNotary(unsigned long notaryNr, unsigned long long currentTime)
{
    if (notaryNr <= 0) return false;
    unsigned short lineageNr = getLineage(currentTime);
    if (lineageNr <= 0) return false;
    unsigned long N = getMaxActing(lineageNr);
    unsigned long long linStart = getNotaryTenureStartFormal(lineageNr,1);
    unsigned long long tenure = getNotaryTenure(lineageNr);
    tenure *= 1000;
    double delta = ((double) tenure) / N;
    unsigned long long tenureStart = linStart + (unsigned long long)(delta * (notaryNr-1));
    if (currentTime < tenureStart) return false;
    if (currentTime >= tenureStart + tenure) return false;
    return true;
}

TNtrNr Type1Entry::getTotalNotaryNr(unsigned long notaryNr, unsigned long long currentTime)
{
    unsigned short lineage = getLineage(currentTime);
    TNtrNr out(lineage, notaryNr);
    return out;
}

unsigned long long Type1Entry::getOldestPossibleFreshEntryTime(unsigned long long time)
{
    unsigned short L = extensions.size();
    unsigned int candidate = L+1;
    for (unsigned short i=0; i<L; i++)
    {
        if (time <= extensions.at(i)->cutoff)
        {
            candidate = i+1;
            break;
        }
    }
    unsigned long long d = lineages.at(candidate-1)->freshnessTime;
    d *= 1000;
    if (time <= d) return 0;
    unsigned long long oldest = time - d;
    if (candidate == 1) return oldest;
    unsigned int i = candidate-2;
    if (time < extensions.at(i)->maxPossibleFreshnessTime)
    {
        if (time > extensions.at(i)->maxRunningExtension)
            oldest = min(oldest, time - extensions.at(i)->maxRunningExtension);
        else oldest = 0;
    }
    return oldest;
}

double Type1Entry::getFee(unsigned int lineage)
{
    return lineages.at(lineage-1)->fee;
}

CompleteID Type1Entry::getConfirmationId()
{
    unsigned long long timeStamp = getNotaryTenureStartCorrected(latestLin(), 1);
    CompleteID id(1, 0, timeStamp);
    return id;
}

// returns:
// 0 for first signature of a block but not notarized version
// 1 for in notarization, but not first in block
// 2 for notarized version
// 3 for error
unsigned char Type1Entry::notarizationStatus(unsigned long signatureCount, unsigned long long time)
{
    if (signatureCount < 1) return 3;
    unsigned int lineage = getLineage(time);
    if (lineage == 0) return 3;
    unsigned long needed = lineages.at(lineage-1)->neededSignatures;
    if (signatureCount > needed) return 3;
    if (needed == signatureCount) return 2;
    if (signatureCount == 1) return 0;
    return 1;
}
