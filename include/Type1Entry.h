#ifndef TYPE1ENTRY_H
#define TYPE1ENTRY_H
#include "CompleteID.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "nlohmann/json.hpp"
#include "Util.h"
#include <sys/stat.h>

using namespace std;
using json = nlohmann::json;

class Type1Entry
{
public:
    Type1Entry(const string& dirName);
    Type1Entry();
    ~Type1Entry();
    unsigned long long getNotaryTenureStartFormal(unsigned int lineage, unsigned long notary);
    unsigned long long getNotaryTenureStartCorrected(unsigned int lineage, unsigned long notary);
    unsigned long long getNotaryTenureEndFormal(unsigned int lineage, unsigned long notary);
    unsigned long long getNotaryTenureEndCorrected(unsigned int lineage, unsigned long notary);
    unsigned long long getNotarizationTimeLimit(unsigned long long time);
    unsigned long long getMaxNotarizationTime(unsigned long long time);
    unsigned long long getLatestMaxNotarizationTime();
    unsigned long getFreshnessTime(unsigned int lineage);
    unsigned long getNotaryTenure(unsigned int lineage);
    unsigned short getMaxActing(unsigned int lineage);
    unsigned short getMaxWaiting(unsigned int lineage);
    unsigned short getNeededSignatures(unsigned int lineage);
    unsigned long getProposalProcessingTime(unsigned int lineage);
    unsigned long long getMaxNotarizationTimeByLineage(unsigned int lineage);
    unsigned long long getCutOff(unsigned int lineage);
    unsigned long long getExtension(unsigned int lineage);
    unsigned long long getKickOff(unsigned int lineage);
    unsigned short latestLin();
    unsigned short getLineage(unsigned long long time);
    string* getPublicKey(unsigned int lineage, unsigned long notary);
    double getFee(unsigned int lineage);
    double getPenaltyFactor1(unsigned int lineage);
    double getPenaltyFactor2(unsigned int lineage);
    unsigned char notarizationStatus(unsigned long signatureCount, unsigned long long time);
    CompleteID getConfirmationId();
    unsigned long long getOldestPossibleFreshEntryTime(unsigned long long time);
    bool isGood();
    unsigned long long getExtentedFreshness(unsigned int lineage, unsigned long long freshBefore);
    bool isFresh(CompleteID &id, unsigned long long currentTime);
    bool isActingNotary(unsigned long notaryNr, unsigned long long currentTime);
    TNtrNr getTotalNotaryNr(unsigned long notaryNr, unsigned long long currentTime);
    bool createFromString(string &str);
    void saveToFile(string &fileName);
protected:
private:
    void deleteData();

    string byteSequence;

    struct LineageData
    {
        unsigned short maxActing;
        unsigned short neededSignatures;
        unsigned short maxWaiting;
        vector<string> keyStrings;
        unsigned long tenureDuration;
        unsigned long maxNotarizationTime;
        unsigned long freshnessTime;
        unsigned long proposalProcessingTime;
        unsigned long long firstTenureStart;
        double fee;
        double penaltyFactor1;
        double penaltyFactor2;
    };

    struct ExtensionData
    {
        unsigned long long cutoff;
        unsigned long long extension;
        unsigned long long kickoff;

        unsigned long long maxPossibleFreshnessTime;
        unsigned long long maxRunningExtension;
    };

    vector<LineageData*> lineages;
    vector<ExtensionData*> extensions;
};

#endif // TYPE1ENTRY_H
