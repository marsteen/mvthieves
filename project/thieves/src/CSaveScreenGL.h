#ifndef CSAVESCREENGL_H
#define CSAVESCREENGL_H

#include <vector>

struct SCompInfo
{
    unsigned char*	mBuffer;
    int				mBufferSize;
};


class CSaveScreenGL
{
    public:

        void SaveScreen(int Width, int Height);
        void Write();

    protected:

        std::vector<SCompInfo> mScreenVec;
        int mWidth;
        int mHeight;
};

#endif
