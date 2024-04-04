#ifndef _ENCODE_CONVERT_H_
#define _ENCODE_CONVERT_H_

#define NO_CNV 0
#define to_UTF8 1
#define to_GBK 2

#include <string>
using std::string;

class encode_convert
{
private:
public:
    encode_convert();
    ~encode_convert();
    static string convertEncoding(const string &str, const char *toEncoding, const char *fromEncoding);
};

#endif
