//------------------------------------------------------------------------------
//
// PROJECT : Imagon
//
// FILE    : CGraflibTga.h
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
// Inhalt: Deklaration der Klasse CGraflibTga
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 12.05.2006 erstellt - Martin Steen
//
//---------------------------------------------------------------------------

#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CDataRect.h>
#include <CGraflib.h>

#ifndef CGRAFLIBTGA
#define CGRAFLIBTGA

enum ETgaItype
{
    ETGA_ITYPE_8BIT_UNCOMPRESSED			= 1,
    ETGA_ITYPE_8BIT_UNCOMPRESSED_MONOCHROME = 3,
    ETGA_ITYPE_RGB_UNCOMPRESSED				= 2,
    ETGA_ITYPE_8BIT_COMPRESSED				= 9,
    ETGA_ITYPE_RGB_COMPRESSED				= 10,
    ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME	= 11,
};


struct STgaHeader
{
    unsigned char	mIdent;     /* Anzahl der Zeichen im Identificationsfeld */
    unsigned char	mCtype;     /* Color Map Type  */
    unsigned char	mItype;     /* Image Type Code */
    unsigned char	mCmap[5];   /* Color Map Specification */
    short			mXorigin;
    short			mYorigin;
    short			mWidth;
    short			mHeight;
    unsigned char	mPsize; /* Number of Bits in a stored pixel index */
    unsigned char	mIbyte; /* Image Descriptor Byte */
};

//---------------------------------------------------------------------------
//
// Klasse:    CTgaDecompressor
//
//---------------------------------------------------------------------------

class CTgaKompbase
{
    public:

        virtual unsigned int Compress(void* src, unsigned char* dest, int Width, int Height, int LineOffset) = 0;
};

template<class TPixelType>
class CTgaKompressor : public CTgaKompbase
{
    public:


        TPixelType* Decompress(unsigned char* src, void* DstData, unsigned int psize);
        unsigned int Compress(void* src, unsigned char* dest, int Width, int Height, int LineOffset);
};


class CGraflibTga : public CGraflib
{
    public:



        bool Read(const char* Filename, SFileReadInfo* fri = NULL);
        int Write(const char* Filename, CFileIO* fio = NULL);

        bool OpenReadLine(const char* Filename) { return false; }
        void ReadLine(void* LineBuffer)  { }
        void CloseReadLine() { }

        bool OpenWriteLine(const char* Filename);
        void WriteLine(void* LineBuffer);

        void CloseWriteLine()
        {
            mFio.CloseFile();
        }


        int WriteBytes(const unsigned char* Buffer, int Size);

    protected:

        CFileIO mFio;
};

#endif
