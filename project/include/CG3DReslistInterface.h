#ifndef CG3DRESLISTINTERFACE
#define CG3DRESLISTINTERFACE

#include "EG3DInterfaceCommand.h"
#include "SG3DcomData.h"

/*
 * typedef void (CALLBACK* DLLFUNC_charP)(char*);
 * typedef void (CALLBACK* DLLFUNC_void)();
 * typedef void (CALLBACK* DLLFUNC_intint)(int, int);
 * typedef void (CALLBACK* DLLFUNC_CG3DGlobalsP)(CG3DGlobals*);
 * typedef void (CALLBACK* DLLFUNC_ucharP)(unsigned char*);
 */

#ifdef _WIN32
typedef void (CALLBACK* DLLFUNC_intvoidP)(int, void*);
#else
//typedef void (*DLLFUNC_intvoidP)(int void*);
#endif

enum
{
    SPF_X_ONLY	= 1,
    SPF_Y_ONLY	= 2,
    SPF_SLIDER	= 4
};


struct EG3DError
{
    EG3DError(int e) : mErrnum(e) { }
    int mErrnum;
};



class CG3DReslistInterface
{
    public:

        bool Init(const char* ResFileName, CG3DGlobals* gbl);
        int SendCommand(EG3DInterfaceCommand cmd, void* val = NULL);

    protected:

        int (* mSendCommand)(int a, void* b);

        //DLLFUNC_intvoidP     mSendCommand;
};

#endif
