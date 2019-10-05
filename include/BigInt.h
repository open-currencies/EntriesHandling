#ifndef BIGINT_H
#define BIGINT_H

#include <string>

using namespace std;

class BigInt
{
private:
    string number;
    unsigned int basis; // should not exceed 256
public:
    BigInt(string s, unsigned int b);
    BigInt(BigInt n, unsigned int b);
    BigInt(unsigned long long n, unsigned int b);
    ~BigInt();
    string getString();
    bool operator == (BigInt b);
    bool operator != (BigInt b);
    bool operator > (BigInt b);
    bool operator < (BigInt b);
    bool operator >= (BigInt b);
    bool operator <= (BigInt b);
    BigInt operator + (BigInt b);
    BigInt operator * (BigInt b);
private:
    static bool equals(BigInt n1, BigInt n2);
    static bool less(BigInt n1, BigInt n2);
    static bool greater(BigInt n1, BigInt n2);

    static string add(string n1, string n2, unsigned int basis);
    static string multiply(string n1, string n2, unsigned int basis);
};

#endif // BIGINT_H
