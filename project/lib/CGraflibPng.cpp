//------------------------------------------------------------------------------
//
// PROJECT : Imagon
//
// FILE    : CGraflibPng.cpp
//
// VERSION : 1.0
//
// AUTOR   : Martin Steen
//           Imagon GmbH
//
//
//
//------------------------------------------------------------------------------
//
// Inhalt: Deklaration der Klasse CGraflibPng
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 08.01.2001 erstellt - Martin Steen
//
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <CFileIO.h>
#include <libpng/png.h>


#include <SFileReadInfo.h>
#include <CGraflibPng.h>
//#include <libpng16/png.h>
#include <GlobalDebug.h>

using namespace std;

extern "C"
{
#include <PnglibInterface.h>
};

//---------------------------------------------------------------------------
//
// Klasse:    CGraflibPng
// Methode:   Read
//
//
//---------------------------------------------------------------------------

bool CGraflibPng::Read(const char* Filename, SFileReadInfo* fri)
{
    int r;

    GlobalDebug("CGraflibPng::Read START File=", Filename);


    PngInfoStruct pinfo;
    //int FileSize;
    //unsigned char* PicData = NULL;

    FILE* fp = (FILE*)fopen(Filename, "rb");

    GlobalDebug("  open ok");

    if (fp == NULL)
    {
        throw(CGraflibException(EGLIB_OPENWRITE, Filename));
        r = false;
    }
    else
    {
        read_png(fp, 0, &pinfo);
        GlobalDebug("  read_png ok");

        mWidth = pinfo.info_ptr->width;
        mHeight = pinfo.info_ptr->height;

        int ByteProPixel = pinfo.info_ptr->channels;
        mBits = ByteProPixel * 8;
        mData = (CPixel*)new unsigned char[pinfo.info_ptr->width *  pinfo.info_ptr->height * ByteProPixel];

        unsigned char* PicDataPtr = (unsigned char*)mData;

        stringstream mstr;
        mstr << " W=" << mWidth << " H=" << mHeight << " Bits=" << mBits;
        GlobalDebug(mstr);

        int BytesPerLine = mWidth * pinfo.info_ptr->channels;

        for (int i = 0; i < mHeight; i++)
        {
            memcpy(PicDataPtr, pinfo.info_ptr->row_pointers[i], BytesPerLine);
            PicDataPtr += BytesPerLine;
        }
        finish_read(&pinfo);
        r = true;
    }
    GlobalDebug("CGraflibPng::Read OK", Filename);

    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibPng
// Methode:   Write
//
//
//---------------------------------------------------------------------------

int CGraflibPng::Write(const char* Filename, CFileIO* fio)
{
#if 1
    switch (mBits)
    {
        case 8:
            write_pngGrayscale(Filename, mWidth, mHeight, mData, 1);
            break;

        case 16:
            write_pngGrayscale(Filename, mWidth, mHeight, mData, 2);
            break;

        case 24:
        case 32:
            write_png(Filename, mWidth, mHeight, mData);
            break;
    }
#endif
    return 0;
}
