#ifndef SG3DCOMDATA_H
#define SG3DCOMDATA_H

struct SG3DcomData
{
    SG3DcomData()
    {
        mFlags = 0;
        mObject = NULL;
        mChrParam = NULL;
    }


    string			mName;
    void*			mObject;
    CVector2<float> mPos;
    unsigned int	mFlags;
    string			mStrParam;
    const char*		mChrParam;
};

#endif
