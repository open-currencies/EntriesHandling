#include "BigInt.h"

BigInt::BigInt(string s, unsigned int b) : number(s), basis(b)
{
    // remove leading zeros
    while((unsigned char) number[0] == 0 && number.length()>0) number.erase(0,1);
}

BigInt::BigInt(BigInt n, unsigned int b) : basis(b)
{
    number="";
    string str=n.getString();
    BigInt oldBasis(n.basis, basis);
    for (unsigned int i=0; i<str.length(); i++)
    {
        number = multiply(number, oldBasis.number, basis);
        BigInt nextDigit((unsigned char) str[i], basis);
        number = add(number, nextDigit.number, basis);
    }
    // remove leading zeros
    while((unsigned char) number[0] == 0 && number.length()>0) number.erase(0,1);
}

BigInt::BigInt(unsigned long long n, unsigned int b) : basis(b)
{
    unsigned long long first = n / basis;
    unsigned char second = (unsigned char)(n - (first * basis));
    if (first > 0)
    {
        BigInt x(first, basis);
        number = x.number;
    }
    else number="";
    number.push_back((char)second);
    // remove leading zeros
    while((unsigned char) number[0] == 0 && number.length()>0) number.erase(0,1);
}

BigInt::~BigInt()
{
    //dtor
}

string BigInt::getString()
{
    return number;
}

bool BigInt::operator == (BigInt b)
{
    return equals((*this), b);
}

bool BigInt::operator != (BigInt b)
{
    return !equals((*this), b);
}

bool BigInt::operator > (BigInt b)
{
    return greater((*this), b);
}

bool BigInt::operator < (BigInt b)
{
    return less((*this), b);
}

bool BigInt::operator >= (BigInt b)
{
    return equals((*this), b) || greater((*this), b);
}

bool BigInt::operator <= (BigInt b)
{
    return equals((*this), b) || less((*this), b);
}

BigInt BigInt::operator + (BigInt b)
{
    BigInt out(add(getString(), b.getString(), basis), basis);
    return out;
}

BigInt BigInt::operator * (BigInt b)
{
    BigInt out(multiply(getString(), b.getString(), basis), basis);
    return out;
}

bool BigInt::equals(BigInt n1, BigInt n2)
{
    return n1.getString() == n2.getString();
}

bool BigInt::less(BigInt n1, BigInt n2)
{
    string str1=n1.getString();
    string str2=n2.getString();
    if(str1.length() < str2.length()) return true;
    if(str1.length() > str2.length()) return false;
    return str1 < str2;
}

bool BigInt::greater(BigInt n1, BigInt n2)
{
    return !equals(n1, n2) && !less(n1, n2);
}

string BigInt::add(string n1, string n2, unsigned int basis)
{
    unsigned char carry=0;
    // make sure lengths are the same
    int lenDiff = abs((int)(n1.size() - n2.size()));
    if (n1.size() > n2.size()) n2.insert(0, lenDiff, (char) carry);
    else if (n1.size() < n2.size()) n1.insert(0, lenDiff, (char) carry);
    // build new string
    string out;
    for(int i=n1.size()-1; i>=0; --i)
    {
        unsigned int tmp = 0;
        tmp+=carry;
        tmp+=(unsigned char) n1[i];
        tmp+=(unsigned char) n2[i];
        unsigned char correctedTmp;
        if (tmp>=basis)
        {
            correctedTmp = (unsigned char)(tmp-basis);
            carry = 1;
        }
        else
        {
            correctedTmp = (unsigned char)tmp;
            carry = 0;
        }
        out.insert(0, 1, (char) correctedTmp);
    }
    if (carry > 0)
    {
        out.insert(0, 1, (char) carry);
    }
    return out;
}

string BigInt::multiply(string n1, string n2, unsigned int basis)
{
    if(n1.length() > n2.length()) n1.swap(n2);
    string out;
    for(int i=n1.length()-1; i>=0; --i)
    {
        unsigned int carry = 0;
        // build next summand
        string nextSummand;
        for(int j=n2.length()-1; j>=0; --j)
        {
            unsigned int tmp=0;
            tmp+=(unsigned char)n2[j];
            tmp*=(unsigned char)n1[i];
            tmp+=(unsigned char)carry;

            carry = tmp/basis;
            unsigned char correctedTmp = (unsigned char)(tmp-basis*carry);
            nextSummand.insert(0, 1, (char) correctedTmp);
        }
        if (carry > 0)
        {
            nextSummand.insert(0, 1, (char) (unsigned char) carry);
        }
        unsigned char zero = 0;
        nextSummand.append(n1.length()-i-1, (char) zero);
        // add to running sum
        out = add(out, nextSummand, basis);
    }
    return out;
}
