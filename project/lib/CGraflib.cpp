//------------------------------------------------------------------------------
//
// PROJECT : Imagon
//
// FILE    : CGraflib.h
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
// Inhalt: Deklaration der Klasse CGraflib
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 08.01.2001 erstellt - Martin Steen
// 07.10.2009 Version 2.0 - Martin Steen
//
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>

using namespace std;

#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CPixel24.h>
#include <CDataRect.h>
#include <CGraflib.h>
#include <CPixel24fixT.h>

//#include <CFixpoint.h>
//#include <CFilterBilinearFloat.h>
//#include <CFilterBilinearFloat.hpp>
//#include <CFilterBilinear.h>


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		ShowError
//
//---------------------------------------------------------------------------

void CGraflib::ShowError(CGraflibException ge)
{
    const char* ErrString[] =
    {
        "UNKNOWN ERROR",
        "OPENREAD ERROR",
        "OPENWRITE ERROR"
    };

    cout << ErrString[ge.mErrnum] << " File=" << ge.mFilename << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		SwapBigEndian
//
//---------------------------------------------------------------------------

void CGraflib::SwapBigEndian()
{
    CPixel32* PixelPtr = (CPixel32*)mData;

    for (int y = 0; y < mHeight; y++)
    {
        for (int x = 0; x < mWidth; x++)
        {
            PixelPtr->SwapBigEndian();
            PixelPtr++;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		SwapRedBlue
//
//---------------------------------------------------------------------------

void CGraflib::SwapRedBlue()
{
    CPixel24* PixelPtr = (CPixel24*)mData;

    for (int y = 0; y < mHeight; y++)
    {
        for (int x = 0; x < mWidth; x++)
        {
            PixelPtr->SwapRedBlue();
            PixelPtr++;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		Yflip
//
//---------------------------------------------------------------------------

void CGraflib::Yflip()
{
    unsigned char* PixelData = (unsigned char*)mData;
    int LineWidth = (mWidth * mBits) / 8; // Breite des Bildes in Byte
    unsigned char* LineBuffer = new unsigned char[LineWidth];

    for (int y = (mHeight / 2) - 1; y >= 0; y--)
    {
        int off1 = y * LineWidth;
        int off2 = ((mHeight-1) - y) * LineWidth;

        memcpy(LineBuffer, PixelData + off2, LineWidth);
        memcpy(PixelData + off2, PixelData + off1, LineWidth);
        memcpy(PixelData + off1, LineBuffer, LineWidth);
    }
    delete[] LineBuffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		ShowInfo
//
//---------------------------------------------------------------------------

void CGraflib::ShowInfo()
{
/*
 *  cout << "Width =" << mWidth << endl;
 *  cout << "Height=" << mHeight << endl;
 *  cout << "Bits  =" << mBits << endl;
 *  cout << "Data  =" << (int) mData << endl;
 */
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		SwapWords
//
//---------------------------------------------------------------------------

void CGraflib::SwapWords(void* Words, int WordCount)
{
    unsigned char* CharPtr = (unsigned char*)Words;
    unsigned char SwapByte;

    for (int i = 0; i < WordCount; i++)
    {
        SwapByte = CharPtr[0];
        CharPtr[0] = CharPtr[1];
        CharPtr[1] = SwapByte;
        CharPtr += 2;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		ShiftWordsUnsigned
//
//---------------------------------------------------------------------------

void CGraflib::ShiftWordsUnsigned()
{
    unsigned short* UData = (unsigned short*)mData;
    short* SData = (short*)mData;
    int WordCount = mWidth * mHeight;

    cout << "WordCount=" << WordCount << endl;


    for (int i = 0; i < WordCount; i++)
    {
        int k = *(SData++);
        UData[i] = 0xFF00;  //(k + 32768);
    }

/*
 *  UData[0] = 0;
 *  UData[WordCount-1] = 0xFFFF;
 */
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		ShiftWordsSigned
//
//---------------------------------------------------------------------------

void CGraflib::ShiftWordsSigned()
{
    unsigned short* UData = (unsigned short*)mData;
    short* SData = (short*)mData;
    int WordCount = mWidth * mHeight;

    for (int i = 0; i < WordCount; i++)
    {
        int k = *(UData++);
        *(SData++) = (short)(k - 32768);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		SwapWords
//
//---------------------------------------------------------------------------

void CGraflib::SwapWords()
{
    if (mBits == 16)
    {
        SwapWords(mData, mWidth * mHeight);
    }
    else
    {
        cout << "*****  CGraflib::SwapWords Error: mBits=" << mBits << endl;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScaleLine2
//
// Skalieren einer Zeile um den Faktor 2
//
// LineOffset = Offset zur nächsten Zeile in Bytes
//
//---------------------------------------------------------------------------

void CGraflib::DownScaleLine2(void* LineBuffer, void* ErgBuffer, int LineOffset)
{
/*
 * cout << "Width=" << mWidth << endl;
 * cout << "Height=" << mHeight << endl;
 *  cout << "LineOffset=" << LineOffset << endl;
 */

    int k = mBits / 8;

    //int k = (mBits == 24) ? 3 : 1;
    for (int b = 0; b < k; b++)
    {
        //cout << "b=" << b << endl;

        DownScaleLine2(LineBuffer, ErgBuffer, mWidth, LineOffset, b);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScaleLine4
//
// Skalieren einer Zeile um den Faktor 4
//
// LineOffset = Offset zur nächsten Zeile in Bytes
//
//---------------------------------------------------------------------------

void CGraflib::DownScaleLine4(void* LineBuffer, void* ErgBuffer, int LineOffset)
{
    int k = (mBits == 24) ? 3 : 1;

    for (int b = 0; b < k; b++)
    {
        DownScaleLine4(LineBuffer, ErgBuffer, mWidth, LineOffset, b);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScaleLine2
//
// Skalieren einer Zeile um den Faktor 2
//
// LineOffset = Offset zur nächsten Zeile in Bytes
//
//---------------------------------------------------------------------------

void CGraflib::DownScaleLine2(void* LineBuffer, void* ErgBuffer, int Width, int LineOffset, int b)
{
    int w2 = Width / 2;

    switch (mBits)
    {
        case 8:
        {
            unsigned char* LinePtr0 = (unsigned char*)LineBuffer;
            unsigned char* LinePtr1 = ((unsigned char*)LineBuffer) + LineOffset;
            unsigned char* ErgPtr = (unsigned char*)ErgBuffer;

            for (int i = 0; i < w2; i++)
            {
                *(ErgPtr++) = (LinePtr0[0] + LinePtr0[1] + LinePtr1[0] + LinePtr1[1]) >> 2;
                LinePtr0 += 2;
                LinePtr1 += 2;
            }
        }
        break;

        case 16:
        {
            unsigned short* LinePtr0 = (unsigned short*)LineBuffer;
            unsigned short* LinePtr1 = ((unsigned short*)LineBuffer) + LineOffset;
            unsigned short* ErgPtr = (unsigned short*)ErgBuffer;

            for (int i = 0; i < w2; i++)
            {
                *(ErgPtr++) = (LinePtr0[0] + LinePtr0[1] + LinePtr1[0] + LinePtr1[1]) >> 2;
                LinePtr0 += 2;
                LinePtr1 += 2;
            }
        }
        break;

        case 24:
        {
            unsigned char* LinePtr0 = (unsigned char*)LineBuffer + b;
            unsigned char* LinePtr1 = ((unsigned char*)LineBuffer) + LineOffset + b;
            unsigned char* ErgPtr = (unsigned char*)ErgBuffer + b;

            for (int i = 0; i < w2; i++)
            {
                //cout << "  i=" << i << endl;

                *ErgPtr = (LinePtr0[0] + LinePtr0[3] + LinePtr1[0] + LinePtr1[3]) >> 2;
                LinePtr0 += 6;
                LinePtr1 += 6;
                ErgPtr += 3;
            }
        }
        break;

        case 32:
        {
            unsigned char* LinePtr0 = (unsigned char*)LineBuffer + b;
            unsigned char* LinePtr1 = ((unsigned char*)LineBuffer) + LineOffset + b;
            unsigned char* ErgPtr = (unsigned char*)ErgBuffer + b;

            for (int i = 0; i < w2; i++)
            {
                //cout << "  i=" << i << endl;

                *ErgPtr = (LinePtr0[0] + LinePtr0[4] + LinePtr1[0] + LinePtr1[4]) >> 2;
                LinePtr0 += 8;
                LinePtr1 += 8;
                ErgPtr += 4;
            }
        }
        break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScaleLine4
//
// Skalieren einer Zeile um den Faktor 4
//
// LineOffset = Offset zur nächsten Zeile in Bytes
//
//---------------------------------------------------------------------------

void CGraflib::DownScaleLine4(void* LineBuffer, void* ErgBuffer, int Width, int LineOffset, int b)
{
    switch (mBits)
    {
        case 24:
        {
            unsigned char* LinePtr0 = (unsigned char*)LineBuffer + b;
            unsigned char* LinePtr1 = ((unsigned char*)LineBuffer) + LineOffset + b;
            unsigned char* LinePtr2 = ((unsigned char*)LineBuffer) + (LineOffset * 2) + b;
            unsigned char* LinePtr3 = ((unsigned char*)LineBuffer) + (LineOffset * 3) + b;
            unsigned char* ErgPtr = (unsigned char*)ErgBuffer + b;

            int w4 = Width / 4;

            for (int i = 0; i < w4; i++)
            {
                *ErgPtr = (LinePtr0[0] + LinePtr0[3] + LinePtr0[6] + LinePtr0[9] +
                    LinePtr1[0] + LinePtr1[3] + LinePtr1[6] + LinePtr1[9] +
                    LinePtr2[0] + LinePtr2[3] + LinePtr2[6] + LinePtr2[9] +
                    LinePtr3[0] + LinePtr3[3] + LinePtr3[6] + LinePtr3[9]) >> 4;

                LinePtr0 += 12;
                LinePtr1 += 12;
                LinePtr2 += 12;
                LinePtr3 += 12;

                ErgPtr += 3;
            }
        }
        break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		CopyScaled2
//
//
//---------------------------------------------------------------------------

bool CGraflib::CopyScaled2(CGraflib* DstGraf, const char* SrcFilename, const char* DstFilename)
{
    if (OpenReadLine(SrcFilename))
    {
        DstGraf->mWidth = mWidth / 2;
        DstGraf->mHeight = mHeight / 2;
        DstGraf->mBits = mBits;

        if (DstGraf->OpenWriteLine(DstFilename))
        {
            int SrcLinesize = (mWidth * mBits) / 8;
            int DstLinesize = (DstGraf->mWidth  * mBits) / 8;
            char* LineBuffer = new char[SrcLinesize * 2];
            char* DstBuffer = new char[DstLinesize];

            for (int y = 0; y < DstGraf->mHeight; y++)
            {
                ReadLine(LineBuffer);
                ReadLine(LineBuffer + SrcLinesize);

                DownScaleLine2(LineBuffer, DstBuffer, SrcLinesize);

                DstGraf->WriteLine(DstBuffer);
            }
            CloseReadLine();
            DstGraf->CloseWriteLine();

            delete LineBuffer;
            delete DstBuffer;
            return true;
        }
    }
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:   InterleaveAlpha
//
//
//
//---------------------------------------------------------------------------

bool CGraflib::InterleaveAlpha(CGraflib* Alpha)
{
    if ((mBits == 24) && (Alpha->mBits == 8))
    {
        if ((mWidth == Alpha->mWidth) && (mHeight == Alpha->mHeight))
        {
            unsigned char* AlphaDataPtr = (unsigned char*)Alpha->mData;
            unsigned char* PixelDataPtr = (unsigned char*)mData;

            int Size = mWidth * mHeight;

            unsigned char* C32Data = new unsigned char[Size * 4];
            unsigned char* C32DataPtr = C32Data;

            for (int i = 0; i < Size; i++)
            {
                C32DataPtr[0] = PixelDataPtr[0];
                C32DataPtr[1] = PixelDataPtr[1];
                C32DataPtr[2] = PixelDataPtr[2];
                C32DataPtr[3] = *AlphaDataPtr;

                PixelDataPtr += 3;
                AlphaDataPtr += 1;
                C32DataPtr += 4;
            }
            delete[] mData;
            mData = (CPixel32*)C32Data;
            mBits = 32;
            return true;
        }
    }
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:   GetPixelAdr
//
//
//
//---------------------------------------------------------------------------

template<class TPixelType>
TPixelType* CGraflib::GetPixelAdrT(TPixelType* p, int w, int x, int y)
{
    return p + ((y * w) + x);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:   Rotate90T
//
//
//
//---------------------------------------------------------------------------


template<class TPixelType>
void CGraflib::Rotate90T()
{
    TPixelType* AltPixel = (TPixelType*)mData;
    TPixelType* NeuPixel = new TPixelType[mWidth * mHeight];

    for (int y = 0; y < mHeight; y++)
    {
        for (int x = 0; x < mWidth; x++)
        {
            *GetPixelAdrT<TPixelType>(NeuPixel, mHeight, y, x) =
                *GetPixelAdrT<TPixelType>(AltPixel, mWidth, mWidth - x - 1, y);
        }
    }
    delete[] AltPixel;
    mData = (CPixel*)NeuPixel;

    int Swap = mHeight;

    mHeight = mWidth;
    mWidth = Swap;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:   Rotate90
//
//
//
//---------------------------------------------------------------------------

void CGraflib::Rotate90()
{
    switch (mBits)
    {
        case 8:

            Rotate90T<char>();
            break;

        case 16:

            Rotate90T<short>();
            break;

        case 24:

            Rotate90T<CPixel24>();
            break;

        case 32:

            Rotate90T<int>();
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:   SplitBitplanes
//
//
//
//---------------------------------------------------------------------------


void CGraflib::SplitBitplanes(char** red, char** green, char** blue)
{
    if (mBits == 24)
    {
        char* DataPtr = (char*)mData;

        *red = new char[mWidth * mHeight];
        *green = new char[mWidth * mHeight];
        *blue = new char[mWidth * mHeight];

        char* RedPtr = *red;
        char* GreenPtr = *green;
        char* BluePtr = *blue;

        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                *(RedPtr++) = *(DataPtr++);
                *(GreenPtr++) = *(DataPtr++);
                *(BluePtr++) = *(DataPtr++);
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:   JoinBitplanes
//
//
//
//---------------------------------------------------------------------------


void CGraflib::JoinBitplanes(char* red, char* green, char* blue)
{
    if (mBits == 24)
    {
        char* DataPtr = new char[mWidth * mHeight * 3];

        char* RedPtr = red;
        char* GreenPtr = green;
        char* BluePtr = blue;

        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                *(DataPtr++) = *(RedPtr++);
                *(DataPtr++) = *(GreenPtr++);
                *(DataPtr++) = *(BluePtr++);
            }
        }
        delete[] mData;
        mData = (CPixel24*)DataPtr;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		GetScaledPicturePixel
//
// Parameter: sp -
//
// Template-Parameter: Tbits  - Anzahl der Fixpoint-Bits
//                     Tfixp  - Fixpoint-Pixeltyp (CPixel1fixT, CPixel3fixT
//                              oder CPixel4fixT)
//                     Tpixel - Pixeltyp (SPixel1i<T>, SPixel3i<T>
//                              oder SPixel4i<T>)
//
//
//---------------------------------------------------------------------------

template<int Tbits, typename Tfixp, typename Tpixel>
void CGraflib::GetScaledPicturePixel(SScaleParams<Tbits, Tpixel>* sp, int x, int y)
{
    Tfixp FixPixel;

    CFixpointT<Tbits> LeftXfix = sp->mXstep * x;
    unsigned int LeftXint = (unsigned int)LeftXfix;
    CFixpointT<Tbits> RightXfix = LeftXfix + sp->mXstep;
    unsigned int RightXint = (unsigned int)RightXfix;

    CFixpointT<Tbits> TopYfix = sp->mYstep * y;
    unsigned int TopYint = (unsigned int)TopYfix;
    CFixpointT<Tbits> BottomYfix = TopYfix + sp->mYstep;
    unsigned int BottomYint = (unsigned int)BottomYfix;

    CFixpointT<Tbits> FaktorXstart = LeftXfix.GetFract2();
    CFixpointT<Tbits> FaktorXend = RightXfix.GetFract();

    CFixpointT<Tbits> FaktorYstart = TopYfix.GetFract2();
    CFixpointT<Tbits> FaktorY = FaktorYstart;

    Tpixel* PixelData = (Tpixel*)mData;
    Tpixel* PixelLinePtr = PixelData + (TopYint * mWidth) + LeftXint;

    FixPixel.Init();

    if (BottomYint > sp->mOrigHeight-1)
    {
        BottomYint = sp->mOrigHeight-1;
    }

    // Y-Vergroesserung?
    if (TopYint == BottomYint)
    {
        FaktorY += BottomYfix.GetFract() - 1;
    }

    for (int yko = TopYint; yko <= BottomYint; yko++)
    {
        Tpixel* PixelPtr = PixelLinePtr;
        CFixpointT<Tbits> FaktorX = FaktorXstart;

        // X-Vergroesserung?
        if (LeftXint == RightXint)
        {
            FaktorX += FaktorXend - 1;
        }

        for (int xko = LeftXint; xko <= RightXint; xko++)
        {
            FixPixel.Add(PixelPtr, FaktorX * FaktorY);

            PixelPtr++;
            if (xko >= RightXint-1)
            {
                FaktorX = FaktorXend;
            }
            else
            {
                FaktorX = 1;
            }
        }

        if (yko >= BottomYint-1)
        {
            FaktorY = BottomYfix.GetFract();
        }
        else
        {
            FaktorY = 1;
        }
        PixelLinePtr += mWidth;
    }

    FixPixel.Div(sp->mXYstep);
    FixPixel.CopyTo(sp->mScaledData);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScalePicture
//
// Template-Parameter: Tbits  - Anzahl der Fixpoint-Bits
//                     Tfixp  - Fixpoint-Pixeltyp (CPixel1fixT, CPixel3fixT
//                              oder CPixel4fixT)
//                     Tpixel - Pixeltyp (SPixel1i<T>, SPixel3i<T>
//                              oder SPixel4i<T>)
//
//---------------------------------------------------------------------------

template<int Tbits, typename Tfixp, typename Tpixel>
void CGraflib::DownScalePicture(Tpixel* ScaledData, int NewWidth, int NewHeight)
{
    SScaleParams<Tbits, Tpixel> sp;

    //cout << "sizeof(Tpixel)=" << sizeof(Tpixel) << endl;

    sp.mOrigWidth = mWidth;
    sp.mOrigHeight = mHeight;
    sp.mXstep.SetInt(mWidth, NewWidth);
    sp.mYstep.SetInt(mHeight, NewHeight);
    sp.mXYstep = sp.mXstep * sp.mYstep;
    sp.mScaledData = ScaledData;

    for (int y = 0; y < NewHeight; y++)
    {
        for (int x = 0; x < NewWidth; x++)
        {
            GetScaledPicturePixel<Tbits, Tfixp, Tpixel>(&sp, x, y);
            sp.mScaledData++;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScaleFix2
//
//---------------------------------------------------------------------------

void* CGraflib::DownScaleFix2(int NewWidth, int NewHeight)
{
    bool r = false;
    void* ScaledData = NULL;

    //
    // 8 Bit unsigned char (1 Kanal)
    //
    switch (mBits)
    {
        case 8:
        {
            SPixel1i<unsigned char>* ScaledData8 = new SPixel1i<unsigned char>[NewWidth * NewHeight];
            float ScaleFakt = (float(mWidth) / NewWidth)
                * (float(mHeight) / NewHeight);

            if (ScaleFakt < 16.0)
            {
                DownScalePicture<16, CPixel1fixT<16> >(ScaledData8, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 256.0)
            {
                DownScalePicture<8, CPixel1fixT<8> >(ScaledData8, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 1024.0)
            {
                DownScalePicture<4, CPixel1fixT<4> >(ScaledData8, NewWidth, NewHeight);
            }
            else
            {
                DownScalePicture<2, CPixel1fixT<2> >(ScaledData8, NewWidth, NewHeight);
            }
            ScaledData = ScaledData8;
            r = true;
        }
        break;

        //
        // 16 Bit SIGNED short (1 Kanal)
        //
        case 16:
        {
            SPixel1i<short>* ScaledData16 = new SPixel1i<short>[NewWidth * NewHeight];
            float ScaleFakt = (float(mWidth) / NewWidth)
                * (float(mHeight) / NewHeight);

            if (ScaleFakt < 16.0)
            {
                DownScalePicture<16, CPixel1fixT<16> >(ScaledData16, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 256.0)
            {
                DownScalePicture<8, CPixel1fixT<8> >(ScaledData16, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 1024.0)
            {
                DownScalePicture<4, CPixel1fixT<4> >(ScaledData16, NewWidth, NewHeight);
            }
            else
            {
                DownScalePicture<2, CPixel1fixT<2> >(ScaledData16, NewWidth, NewHeight);
            }
            ScaledData = ScaledData16;
            r = true;
        }
        break;

        //
        // 8 Bit unsigned char (3 Kanaele)
        //
        case 24:
        {
            SPixel3i<unsigned char>* ScaledData24 = new SPixel3i<unsigned char>[NewWidth * NewHeight];
            float ScaleFakt = (float(mWidth) / NewWidth)
                * (float(mHeight) / NewHeight);

            cout << "ScaleFakt=" << ScaleFakt << endl;

            if (ScaleFakt < 16.0)
            {
                DownScalePicture<16, CPixel3fixT<16> >(ScaledData24, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 256.0)
            {
                DownScalePicture<8, CPixel3fixT<8> >(ScaledData24, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 1024.0)
            {
                DownScalePicture<4, CPixel3fixT<4> >(ScaledData24, NewWidth, NewHeight);
            }
            else
            {
                DownScalePicture<2, CPixel3fixT<2> >(ScaledData24, NewWidth, NewHeight);
            }
            ScaledData = ScaledData24;
            r = true;
        }
        break;

        //
        // 8 Bit unsigned char (4 Kanaele)
        //
        case 32:
        {
            SPixel4i<unsigned char>* ScaledData32 = new SPixel4i<unsigned char>[NewWidth * NewHeight];
            float ScaleFakt = (float(mWidth) / NewWidth)
                * (float(mHeight) / NewHeight);

            if (ScaleFakt < 16.0)
            {
                DownScalePicture<16, CPixel4fixT<16> >(ScaledData32, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 256.0)
            {
                DownScalePicture<8, CPixel4fixT<8> >(ScaledData32, NewWidth, NewHeight);
            }
            else
            if (ScaleFakt < 1024.0)
            {
                DownScalePicture<4, CPixel4fixT<4> >(ScaledData32, NewWidth, NewHeight);
            }
            else
            {
                DownScalePicture<2, CPixel4fixT<2> >(ScaledData32, NewWidth, NewHeight);
            }
            ScaledData = ScaledData32;
            r = true;
        }
        break;

        //
        // nicht unterstuetzt
        //

        default:

            cout << "***** unsupported bitcount:" << mBits << endl;
            r = false;
            break;
    }


    if (r)
    {
        delete mData;
        mData = (CPixel*)ScaledData;
        mWidth = NewWidth;
        mHeight = NewHeight;
    }

    return ScaledData;
}
