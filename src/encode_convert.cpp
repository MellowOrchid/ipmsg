#include <iostream>
#include <iconv.h>
#include <vector>
#include "public.h"
#include "encode_convert.h"
#include "write_log.h"
using std::cerr, std::string, std::vector;

encode_convert::encode_convert() {}

encode_convert::~encode_convert() {}

/**
 * 将字符串从一种编码转换为另一种编码
 * @param str 要转换的字符串
 * @param toEncoding 目标编码
 * @param fromEncoding 源编码
 * @return 转换后的字符串
 */
string encode_convert::convertEncoding(const string &str, const char *toEncoding, const char *fromEncoding)
{
    iconv_t cd = iconv_open(toEncoding, fromEncoding);
    if (cd == (iconv_t)-1)
    {
        // 处理错误
        lmsg = "创建转换描述符失败";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        return "";
    }

    vector<char> inbuf(str.begin(), str.end());
    size_t inbytesleft = inbuf.size();
    size_t outbytesleft = inbytesleft * 4;
    vector<char> outbuf(outbytesleft);

    char *inptr = &inbuf[0];
    char *outptr = &outbuf[0];

    if (iconv(cd, &inptr, &inbytesleft, &outptr, &outbytesleft) == (size_t)-1)
    {
        // 处理错误
        lmsg = "转换失败";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        iconv_close(cd);
        return "";
    }

    iconv_close(cd);

    return string(&outbuf[0], outptr - &outbuf[0]);
}
