#ifndef CDATABASE3
#define CDATABASE3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

class CRecord : public vector<string>
{
    public:

        int mLineNumber;
        string mFilename;

        void Show();
        void AddString(string& as);
};


class CDatabase3 : public list<CRecord>
{
    public:

        int Read(list<CRecord>* db, string& BasePath, string& Filename, char SplitChar);

    protected:

        void AddRecord(CRecord& rc);
        bool IsValid(int c);
};


#endif // CDATABASE3
