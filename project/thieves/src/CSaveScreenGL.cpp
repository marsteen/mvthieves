#include <iostream>
#include "CSaveScreenGL.h"
#include <GLinclude.h>
#include <CGraflibTga.h>

using namespace std;

static unsigned char CompBuffer[1024000];

void CSaveScreenGL::SaveScreen(int Width, int Height)
{
    CPixel24* ScreenBuffer = new CPixel24[Width * Height];

    glReadPixels(0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, ScreenBuffer);
    CTgaKompressor<CPixel24> TgaKompressor;

    {
        CGraflibTga tgaTmp;
        tgaTmp.mWidth = Width;
        tgaTmp.mHeight = Height;
        tgaTmp.mBits = 24;
        tgaTmp.mData = ScreenBuffer;
        tgaTmp.Yflip();
        tgaTmp.SwapRedBlue();
    }



    int CompSize = TgaKompressor.Compress(ScreenBuffer, CompBuffer, Width, Height, 0);

    unsigned char* NewCompBuffer = new unsigned char[CompSize];

    memcpy(NewCompBuffer, CompBuffer, CompSize);



    cout << "n=" << mScreenVec.size() <<  "Size: " << CompSize << endl;

    SCompInfo CompInfo;

    CompInfo.mBuffer = NewCompBuffer;
    CompInfo.mBufferSize = CompSize;

    mScreenVec.push_back(CompInfo);
    delete[] ScreenBuffer;

    mWidth = Width;
    mHeight = Height;
}


void CSaveScreenGL::Write()
{
    for (int i = 0; i < mScreenVec.size(); i++)
    {
        CGraflibTga tga;
        CFileIO fio;

        char TgaFilename[256];

        tga.mWidth = mWidth;
        tga.mHeight = mHeight;
        tga.mBits = 24;

        sprintf(TgaFilename, "movie/%s.tga", fio.Fill0(i+1, 6));


        if (tga.OpenWriteLine(TgaFilename))
        {
            tga.WriteBytes(mScreenVec[i].mBuffer, mScreenVec[i].mBufferSize);
            delete[] mScreenVec[i].mBuffer;
        }
    }
    mScreenVec.clear();
}
