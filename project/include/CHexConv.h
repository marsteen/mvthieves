#ifndef CHEXCONV_H
#define CHEXCONV_H

class CHexConv
{
    public:

        int NibbleToInt(char a);
        int ByteToInt(unsigned char a, unsigned char b);
        float ByteToFloat(unsigned char a, unsigned char b);
        int StringToInt(const char* s);
};

#include <CHexConv.hpp>

#endif // CHEXCONV
