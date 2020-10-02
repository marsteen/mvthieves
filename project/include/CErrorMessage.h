#ifndef CERRORMESSAGE_H
#define CERRORMESSAGE_H

class CErrorMessage
{
    public:

        CErrorMessage() { }


        CErrorMessage(EParseError ec, string msg) : mErrCode(ec), mErrMsg(msg)
        {
            mLineNumber = 0;
        }


        CErrorMessage(EParseError ec, string msg, int lnum, string str) :
            mErrCode(ec),
            mErrMsg(msg),
            mLineNumber(lnum),
            mFilename(str)
        {
        }


        void ShowError();

        EParseError mErrCode;

    protected:

        string mErrMsg;
        string mFilename;
        int mLineNumber;
};

#endif
