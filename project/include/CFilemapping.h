#ifndef CFILEMAPPING_H
#define CFILEMAPPING_H

class CFilemapping
{
    public:

        CFileMapping()
        {
            mMapHandle = NULL;
            mMapPointer = NULL;
        }

        void* OpenRead(char* Filename);
        void Close();

        int mFilesize;

    protected:

        HANDLE mMapHandle;
        HANDLE mFileHandle;
        LPVOID mMapPointer;
};

#endif
