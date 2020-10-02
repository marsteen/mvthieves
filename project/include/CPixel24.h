#ifndef CPIXEL24_H
#define CPIXEL24_H

#include <CPixel.h>

class CPixel24 : public CPixel
{
    public:

        unsigned char rgb[3];

        void operator=(const CPixel24& p2)
        {
            rgb[0] = p2.rgb[0];
            rgb[1] = p2.rgb[1];
            rgb[2] = p2.rgb[2];
        }


        void operator=(int i)
        {
            rgb[0] = i >> 16;
            rgb[1] = (i >> 8) & 0xFF;
            rgb[2] = i & 0xFF;
        }


        unsigned int GetInt()
        {
            return (rgb[2] << 16) | (rgb[1] << 8) | rgb[0];
        }


        void MixPixel(CPixel24* p2, unsigned char Alpha);



        bool operator==(CPixel24& p2);
        bool operator!=(CPixel24& p2);
        void Set(unsigned char r, unsigned char g, unsigned char b);
        void Set(unsigned int b);
        void SwapRedBlue();
        int Brightness();
};


#endif
