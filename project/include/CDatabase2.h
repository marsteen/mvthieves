#ifndef CDATABASE2
#define CDATABASE2

#include <CList.h>
#include <CList.hpp>

enum ERecType
{
    ERT_INT,
    ERT_FLOAT,
    ERT_DOUBLE,
    ERT_STRING
};

struct CRecField
{
    char*	mTag;
    int		mTagType;
    void*	mVar;
    bool	mFound;
};

class CRecord : public CListElem<CRecord>
{
    public:

        CRecord();
        ~CRecord();


        char** mText;
        int mFields;
};

class CDatabase2 : public CList<CRecord>
{
    public:

        int Read(char* Filename, char SplitChar);
        bool ParseConfigFile(char* Filename, CRecField* rf, char Separator, bool TagTest = false);


        int mRecordCount;

    protected:

        bool IsValid(int c);

        int GetLineSize(int* SkipLineEnd);
        char* ReadLine(int* LineSize);


        CFileIO mFio;
        bool mEndOfFile;
};


#endif
