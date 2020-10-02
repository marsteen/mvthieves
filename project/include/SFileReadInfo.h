#ifndef SFILEREADINFO
#define SFILEREADINFO
struct SFileReadInfo
{
    CFileIO*		mFio;
    unsigned int	mFilePosition;
    unsigned int	mFileSize;
    void*			mData;
};
#endif
