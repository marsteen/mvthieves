#ifndef CGRAFLIB
#define CGRAFLIB

#include <CDataRect.h>
#include <SFileReadInfo.h>
#include <CFixpointT.h>
#include <CPixel24.h>
#include <CPixel32.h>

enum EGraflibExceptionNr
{
    EGLIB_VOID,
    EGLIB_OPENREAD,
    EGLIB_OPENWRITE
};


class CGraflibException
{
    public:

        CGraflibException(EGraflibExceptionNr er, const char* fn) : mErrnum(er), mFilename(fn)
        {
        }


        EGraflibExceptionNr mErrnum;
        const char* mFilename;
};

template<int Tbits, typename Tpixel>
struct SScaleParams
{
    int					mOrigWidth;
    int					mOrigHeight;
    CFixpointT<Tbits>	mXstep;
    CFixpointT<Tbits>	mYstep;
    CFixpointT<Tbits>	mXYstep;
    Tpixel*				mScaledData;
};


class CGraflib : public CDataRect
{
    public:

        void ShowInfo();
        virtual bool Read(const char* Filename, SFileReadInfo* fri = NULL) = 0;
        virtual int Write(const char* Filename, CFileIO* fio) = 0;

        virtual bool OpenReadLine(const char* Filename) = 0;
        virtual void ReadLine(void* LineBuffer) = 0;
        virtual void CloseReadLine() = 0;

        virtual bool OpenWriteLine(const char* Filename) = 0;
        virtual void WriteLine(void* LineBuffer) = 0;
        virtual void CloseWriteLine() = 0;


        void Yflip();
        void SwapRedBlue();
        void SwapBigEndian();
        void SwapWords(void* Words, int WordCount);
        void SwapWords();
        void ShiftWordsSigned();
        void ShiftWordsUnsigned();
        void* DownScaleSigned(int NewWidth, int NewHeight);
        void* DownScale(int NewWidth, int NewHeight);
        void* DownScaleFp(int NewWidth, int NewHeight, int Threads);
        void* DownScaleFix2(int NewWidth, int NewHeight);


        void DownScaleLine2(void* LineBuffer, void* ErgBuffer, int LineOffset);
        void DownScaleLine4(void* LineBuffer, void* ErgBuffer, int LineOffset);

        void DownScaleLine2(void* LineBuffer, void* ErgBuffer, int Width, int LineOffset, int b);
        void DownScaleLine4(void* LineBuffer, void* ErgBuffer, int Width, int LineOffset, int b);
        void DownScaleThread(void* LineBuffer, void* ErgBuffer, int Width, int LineOffset, int ScaleFaktor);

        bool CopyScaled2(CGraflib* DstGraf, const char* SrcFilename, const char* DstFilename);
        void ShowError(CGraflibException ge);

        bool InterleaveAlpha(CGraflib* Alpha);

        void Rotate90();
        void SplitBitplanes(char** red, char** green, char** blue);
        void JoinBitplanes(char* red, char* green, char* blue);

        // Offset zum Anfang der Daten
        unsigned int mFileOffset;

    protected:

        template<class TPixelType>
        TPixelType* GetPixelAdrT(TPixelType* p, int w, int x, int y);

        template<class TPixelType>
        void Rotate90T();


        template<int Tbits, typename Tfixp, typename Tpixel>
        void GetScaledPicturePixel(SScaleParams<Tbits, Tpixel>* sp, int x, int y);

        template<int Tbits, typename Tfixp, typename Tpixel>
        void DownScalePicture(Tpixel* ScaledData, int NewWidth, int NewHeight);
};
#endif
