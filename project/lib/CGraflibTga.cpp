//------------------------------------------------------------------------------
//
// PROJECT : Imagon
//
// FILE    : CGraflibTga.cpp
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
// 08.01.2001 erstellt - Martin Steen
//
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CGraflibTga.h>
#include <CPixel24.h>
#include <CPixel32.h>

using namespace std;

//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Read
//
// Parameter: Filename
//
// Return:
//
//---------------------------------------------------------------------------

template<class TPixelType>
TPixelType* CTgaKompressor<TPixelType>::Decompress(unsigned char* src, void* PixelBuffer, unsigned int psize)
{
    unsigned int count = 0;
    unsigned char c;
    TPixelType w;


    //cout  << "Uncompressing.." << endl;

    if (PixelBuffer == NULL)
    {
        PixelBuffer = new TPixelType[psize];
    }
    TPixelType* dest = (TPixelType*)PixelBuffer;

    //cout.flags(ios::hex);

    while (count < psize)
    {
        c = *src++;

        if (c & 0x80) /* komprimierte Daten */
        {
            c &= 0x7F;
            w = *((TPixelType*)src);
            src += sizeof(TPixelType);

            for (int l = 0; l <= c; l++)
            {
                *(dest++) = w;
            }
            count += c + 1;
        }
        else /* unkomprimierte Daten */
        {
            c += 1;
            memcpy(dest, src, c * sizeof(TPixelType));
            src += c * sizeof(TPixelType);
            dest += c;
            count += c;
        }
    }

    //cout.flags(ios::dec);

    //cout  << "Uncompressing ok" << endl;
    return (TPixelType*)PixelBuffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CTgaKompressor
// Methode:		Compress
//
// Return: Groeße der komprimierten Daten in Bytes
//
//---------------------------------------------------------------------------

template<class TPixelType>
unsigned int CTgaKompressor<TPixelType>::Compress(void* Pixels, unsigned char* dest, int Width, int Height, int LineOffset)
{
    TPixelType* src = (TPixelType*)Pixels;

    int ycount, xcount;
    unsigned char* sdest;


    sdest = dest; /* Anfang des Zielspeichers */

    for (ycount = 0; ycount < Height; ycount++)
    {
        xcount = 0;
        while (xcount < Width)
        {
            if (src[0] == src[1]) /* Zwei Pixel gleich */
            {
                int x = 2;
                TPixelType cwert = src[0];

                while ((src[x] == cwert) && (x < 127))
                {
                    x++;
                }

                if (x + xcount > Width)
                {
                    x = (int)(Width - xcount);
                }

                src += x;
                xcount += x;

                x |= 0x80;

                *dest++ = (unsigned char)x - 1; /* Zaehler speichern */
                *((TPixelType*)dest) = cwert;   /* Pixelwert speichern */
                dest += sizeof(TPixelType);
            }
            else
            {
                int i, x = 0;

                while ((src[x] != src[x+1]) && (x < 127))
                {
                    x++;
                }

                if (x + xcount > Width)
                {
                    x = (int)(Width - xcount);
                }

                *dest++ = (unsigned char)x - 1;

                memcpy(dest, src, sizeof(TPixelType) * x);
                dest += sizeof(TPixelType) * x;

                src += x;
                xcount += x;
            }
        }

        src += LineOffset;
    }
    return dest - sdest;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   OpenWriteLine
//
//
//---------------------------------------------------------------------------

bool CGraflibTga::OpenWriteLine(const char* Filename)
{
    if (mFio.OpenFileWrite(Filename, ios::binary))
    {
        STgaHeader TgaHeader;

        memset(&TgaHeader, 0, sizeof(STgaHeader));

        TgaHeader.mPsize = mBits;
        TgaHeader.mWidth = mWidth;
        TgaHeader.mHeight = mHeight;
        TgaHeader.mPsize = mBits;
        TgaHeader.mIbyte = 0x20;

        switch (mBits)
        {
            case 8:
                if (mPalette != NULL)
                {
                    TgaHeader.mItype = ETGA_ITYPE_8BIT_COMPRESSED;
                    TgaHeader.mCtype = 1;
                    TgaHeader.mCmap[3] = 1;
                    TgaHeader.mCmap[4] = 24;
                }
                else
                {
                    TgaHeader.mItype = ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME;
                }
                break;

            case 16:
                TgaHeader.mItype = ETGA_ITYPE_RGB_COMPRESSED;
                break;

            case 24:
                TgaHeader.mItype = ETGA_ITYPE_RGB_COMPRESSED;
                break;
        }
        mFio.WriteBytes(&TgaHeader, sizeof(STgaHeader));

        if ((mBits == 8) && (mPalette != NULL))
        {
            mFio.WriteBytes(mPalette, 256 * 3);
        }

        return true;
    }
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Write
//
// Parameter: Filename - Dateiname
//            SFileReadInfo* fri - Zeiger auf Datei-Informationen (oder NULL)
//
// Return:
//
//---------------------------------------------------------------------------

void CGraflibTga::WriteLine(void* UncompressedData)
{
    CTgaKompbase* TgaComp = NULL;
    unsigned char* CompressedBuffer = new unsigned char[mWidth * (mBits/8) * 2];

    switch (mBits)
    {
        case 8:

            TgaComp = new CTgaKompressor<unsigned char>;
            break;

        case 16:

            TgaComp = new CTgaKompressor<unsigned short>;
            break;

        case 24:

            TgaComp = new CTgaKompressor<CPixel24>;
            break;
    }


    int CompSize = TgaComp->Compress(UncompressedData, CompressedBuffer, mWidth, 1, 0);

    mFio.WriteBytes(CompressedBuffer, CompSize);

    delete[] CompressedBuffer;
    delete   TgaComp;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   WriteBytes
//
//
//---------------------------------------------------------------------------

int CGraflibTga::WriteBytes(const unsigned char* Buffer, int Size)
{
    mFio.WriteBytes(Buffer, Size);
    return 1;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Write
//
// Parameter: Filename - Dateiname
//            SFileReadInfo* fri - Zeiger auf Datei-Informationen (oder NULL)
//
// Return:
//
//---------------------------------------------------------------------------

int CGraflibTga::Write(const char* Filename, CFileIO* fio)
{
    CFileIO fioRead;
    CFileIO* fioPtr;
    STgaHeader TgaHeader;


    if (fio != NULL)
    {
        fioPtr = fio;
    }
    else
    {
        fioPtr = &fioRead;
        if (!fioPtr->OpenFileWrite(Filename, ios::binary))
        {
            // Datei konnte nicht zum Schreiben ge�ffent werden
            return false;
        }
    }

    memset(&TgaHeader, 0, sizeof(STgaHeader));

    TgaHeader.mPsize = mBits;
    TgaHeader.mWidth = mWidth;
    TgaHeader.mHeight = mHeight;
    TgaHeader.mPsize = mBits;
    TgaHeader.mIbyte = 0x20;

    unsigned char* CompressedBuffer = new unsigned char[mWidth * mHeight * (mBits/8) * 2];
    int CompSize;

    CTgaKompbase* TgaComp = NULL;

    switch (mBits)
    {
        case 8:
            TgaComp = new CTgaKompressor<unsigned char>;

            if (mPalette != NULL)
            {
                TgaHeader.mItype = ETGA_ITYPE_8BIT_COMPRESSED;
                TgaHeader.mCtype = 1;
                TgaHeader.mCmap[3] = 1;
                TgaHeader.mCmap[4] = 24;
            }
            else
            {
                TgaHeader.mItype = ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME;
            }
            break;

        case 16:
            TgaComp = new CTgaKompressor<unsigned short>;
            TgaHeader.mItype = ETGA_ITYPE_RGB_COMPRESSED;
            break;

        case 24:
            TgaComp = new CTgaKompressor<CPixel24>;
            TgaHeader.mItype = ETGA_ITYPE_RGB_COMPRESSED;
            break;
    }

    CompSize = TgaComp->Compress(mData, CompressedBuffer, mWidth, mHeight, 0);
    delete[] TgaComp;

    fioPtr->WriteBytes(&TgaHeader, sizeof(STgaHeader));

    if ((mBits == 8) && (mPalette != NULL))
    {
        fioPtr->WriteBytes(mPalette, 256 * 3);
    }
    fioPtr->WriteBytes(CompressedBuffer, CompSize);
    delete[] CompressedBuffer;

    if (fio == NULL)
    {
        fioPtr->CloseFile();
    }
    return CompSize + sizeof(TgaHeader);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Read
//
// Parameter: Filename - Dateiname
//            SFileReadInfo* fri - Zeiger auf Datei-Informationen (oder NULL)
//
// Return:
//
//---------------------------------------------------------------------------

bool CGraflibTga::Read(const char* Filename, SFileReadInfo* fri)
{
    //Debug2 << "CGraflibTga::Read START" << DBLF;

    CFileIO fio;
    CFileIO* fioPtr;
    unsigned int FileSize;
    STgaHeader TgaHeader;
    void* DstData;
    bool bCloseFile = false;
    bool r = true;

    if (fri != NULL)
    {
        //cout << "x" << endl;

        if (fri->mFio == NULL)
        {
            fioPtr = &fio;
            if (!fio.OpenFileRead(Filename, ios::binary))
            {
                return false;
            }
            bCloseFile = true;
        }
        else
        {
            fioPtr = fri->mFio;
        }

        FileSize = fri->mFileSize;
        DstData = fri->mData;

        fioPtr->SetReadPos(fri->mFilePosition);
    }
    else
    {
        FileSize = fio.GetFileSize(Filename);
        fioPtr = &fio;
        DstData = NULL;
        if (!fioPtr->OpenFileRead(Filename, ios::binary))
        {
            return false;
        }
        bCloseFile = true;
    }
    fioPtr->ReadBytes(&TgaHeader, sizeof(STgaHeader));


    mWidth = TgaHeader.mWidth;
    mHeight = TgaHeader.mHeight;
    mBits = TgaHeader.mPsize;


    int PicSize = mWidth * mHeight;

    /*
     * Debug3 << "  Width  =" << mWidth << DBLF;
     * Debug3 << "  Height =" << mHeight << DBLF;
     * Debug3 << "  Bits   =" << mBits << DBLF;
     */


    unsigned int DataLenght = FileSize - sizeof(STgaHeader);

/*
 *  cout   << " CGraflibTga::Read Bits=" << (int) TgaHeader.mPsize
 *         << " Itype=" << (int) TgaHeader.mItype << '\n'
 *         << " Width=" << mWidth << '\n'
 *         << " Height=" << mHeight << endl;
 *
 * if (fri != NULL)
 * {
 *      cout << " Filepos=" << (int) fri->mFilePosition << '\n'
 *         << " Filesize=" << (int) fri->mFileSize << endl;
 *  }
 */
/*
 *  Debug3 << " CGraflibTga::Read Bits=" << (int) TgaHeader.mPsize
 *         << " Itype=" << (int) TgaHeader.mItype << '\n'
 *         << " Width=" << mWidth << '\n'
 *         << " Height=" << mHeight << '\n'
 *         << " Filepos=" << (int) fri->mFilePosition << '\n'
 *         << " Filesize=" << (int) fri->mFileSize << DBLF;
 */
    switch (TgaHeader.mItype)
    {
        case ETGA_ITYPE_8BIT_UNCOMPRESSED:
        case ETGA_ITYPE_8BIT_UNCOMPRESSED_MONOCHROME:
        case ETGA_ITYPE_8BIT_COMPRESSED:
        case ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME:


            break;

        case ETGA_ITYPE_RGB_UNCOMPRESSED:

            //Debug3 << "Image type=ETGA_ITYPE_RGB_UNCOMPRESSED" << DBLF;
            break;

        case ETGA_ITYPE_RGB_COMPRESSED:

            //Debug3 << "Image type=ETGA_ITYPE_RGB_COMPRESSED" << DBLF;
            break;

        default:

            //Debug3 << "***** CGraflibTga::Read unsupported image type" << DBLF;
            r = false;
            break;
    }

    if (r)
    {
        switch (mBits)
        {
            // 8 Bit Graustufen oder Farbpaletten-Pixeldaten

            case 8:

                if (TgaHeader.mCtype == 1) // Farbpalette vorhanden
                {
                    mPalette = (CPixel24*)fioPtr->ReadBytes(256 * 3);
                    DataLenght -= 256 * 3;
                }
                switch (TgaHeader.mItype)
                {
                    case ETGA_ITYPE_8BIT_UNCOMPRESSED:              // Color Map unkomprimiert
                    case ETGA_ITYPE_8BIT_UNCOMPRESSED_MONOCHROME:   // Monochrom unkomprimiert

                        mData = (CPixel*)fioPtr->ReadBytes(DataLenght);
                        break;

                    case ETGA_ITYPE_8BIT_COMPRESSED:
                    case ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME:
                    {
                        unsigned char* CompressedData = (unsigned char*)fioPtr->ReadBytes(DataLenght);
                        CTgaKompressor<unsigned char> Decomp8;
                        mData = (CPixel*)Decomp8.Decompress(CompressedData, DstData, PicSize);
                        delete[] CompressedData;
                    }
                    break;
                }
                break;

            // 16 Bit Pixeldaten

            case 16:

                switch (TgaHeader.mItype)
                {
                    case ETGA_ITYPE_RGB_UNCOMPRESSED:

                        mData = (CPixel*)fioPtr->ReadBytes(DataLenght);
                        break;

                    case ETGA_ITYPE_RGB_COMPRESSED:
                    {
                        //Debug3 << "DataLenght=" << DataLenght << DBLF;
                        unsigned char* CompressedData = (unsigned char*)fioPtr->ReadBytes(DataLenght);

                        CTgaKompressor<short> Decomp16;
                        mData = (CPixel*)Decomp16.Decompress(CompressedData, DstData, PicSize);

                        //Debug3 << " CGraflibTga::Read mData=" << (int) mData << DBLF;

                        delete[] CompressedData;
                    }
                    break;
                }
                break;

            // 24 Bit Pixeldaten

            case 24:

                switch (TgaHeader.mItype)
                {
                    case ETGA_ITYPE_RGB_UNCOMPRESSED:

                        mData = (CPixel*)fioPtr->ReadBytes(DataLenght);
                        break;

                    case ETGA_ITYPE_RGB_COMPRESSED:
                    {
                        unsigned char* CompressedData = (unsigned char*)fioPtr->ReadBytes(DataLenght);
                        CTgaKompressor<CPixel24> Decomp24;
                        mData = (CPixel*)Decomp24.Decompress(CompressedData, DstData, PicSize);
                        delete[] CompressedData;
                    }
                    break;
                }
                SwapRedBlue();
                break;

            // 24 Bit Pixeldaten

            case 32:

                switch (TgaHeader.mItype)
                {
                    case ETGA_ITYPE_RGB_UNCOMPRESSED:

                        mData = (CPixel*)fioPtr->ReadBytes(DataLenght);
                        break;

                    case ETGA_ITYPE_RGB_COMPRESSED:
                    {
                        unsigned char* CompressedData = (unsigned char*)fioPtr->ReadBytes(DataLenght);
                        CTgaKompressor<CPixel32> Decomp32;
                        mData = (CPixel*)Decomp32.Decompress(CompressedData, DstData, PicSize);
                        delete[] CompressedData;
                    }
                    break;
                }
                SwapBigEndian();
                break;



            default:

                r = false;
                break;
        }

        if (r)
        {
            if ((TgaHeader.mIbyte & 0x30) == 0) // Ursprung der Grafik unten links?
            {
                Yflip();
            }
        }

        if (bCloseFile)
        {
            fioPtr->CloseFile();
        }
    }

    //Debug3 << "CGraflibTga::Read OK r=" << r<< DBLF;

    return r;
}
