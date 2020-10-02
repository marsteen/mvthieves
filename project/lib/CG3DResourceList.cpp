//---------------------------------------------------------------------------
//
// PROJECT : Die Planeten
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#include "CG3DResourceList.h"
#include "CG3DReslistInterface.h"

CG3DGlobals* gGlobals;
CG3DResourceList* gResourceList;

#ifdef _WIN32_
#define EXPORT    extern "C" __declspec(dllexport)
EXPORT int CALLBACK SendCommand(int Cmd, void* val)
#else
int gSendCommand(int Cmd, void* val)
#endif
{
    int retval = 0;

    switch (Cmd)
    {
        //
        // komplette Resource Liste zeichnen
        //

        case EG3DcomDraw:

            gResourceList->DrawList();
            break;

        //
        // Resource Liste neu ausrichten
        //

        case EG3DcomAdjust:

            gResourceList->Adjust();
            break;

        //
        // Resource Liste neu laden
        //

        case EG3DcomReload:

            gResourceList->Reload();
            break;

        //
        // Zeichen  an Textfeld senden
        //
        case EG3DcomSendChar:
            if (gResourceList->mTextField != NULL)
            {
                unsigned char* key = (unsigned char*)val;
                gResourceList->mTextField->SendChar(*key);
                retval = 1;
            }
            else
            {
                retval = 0;
            }
            break;

        //
        // Maus-Rollover verarbeiten
        //
        case EG3DcomRollover:
        {
            CVector2<int>* Mouse = (CVector2<int>*)val;
            retval = gResourceList->Rollover(Mouse);
        }
        break;

        //
        // Initialisieren
        //
        case EG3DcomInit:
        {
            if (gResourceList == NULL)
            {
                gResourceList = new CG3DResourceList;
            }
            string Filename = (char*)val;
            gResourceList->Read(Filename);
        }
        break;

        //
        // Globalen Parameterpointer setzen
        //
        case EG3DcomSetGlobals:
            gGlobals = (CG3DGlobals*)val;
            break;

        //
        // Knoten deaktivieren
        //
        case EG3DcomDeactivateKnot:
        {
            string KnotName = (const char*)val;

            CG3DResChild* cld = gResourceList->SearchChild(KnotName);
            if (cld != NULL)
            {
                cld->mStatus.Clr(ESTAT_ACTIVE);
                if (cld->mRef->mType == ERTYPE_KNOT)
                {
                    CG3DResourceKnot* kn = dynamic_cast<CG3DResourceKnot*> (cld->mRef);
                    kn->DisableRadioGroup();
                }
            }
        }
        break;

        case EG3DcomDrawString:
        {
            // mObject     - Zeiger auf Child
            // mPos        - Position
            // mChrParam   - Druck-String

            SG3DcomData* nr = (SG3DcomData*)val;

            if (nr->mObject != NULL)
            {
                CG3DResChild* cld = (CG3DResChild*)(nr->mObject);
                if (cld->mRef->mType == ERTYPE_STRING)
                {
                    CG3DResourceString* str = dynamic_cast<CG3DResourceString*> (cld->mRef);
                    if (nr->mChrParam != NULL)
                    {
                        str->Draw(nr->mChrParam, &nr->mPos);
                    }
                }
            }
        }
        break;

        //
        // Knoten aktivieren
        //
        case EG3DcomActivateKnot:
        {
            string KnotName = (const char*)val;

            //cout << "EG3DcomActivateKnot " << KnotName << endl;

            CG3DResChild* cld = gResourceList->SearchChild(KnotName);
            if (cld != NULL)
            {
                cld->mStatus.Set(ESTAT_ACTIVE);
                if (cld->mRef->mType == ERTYPE_KNOT)
                {
                    CG3DResourceKnot* kn = dynamic_cast<CG3DResourceKnot*> (cld->mRef);
                    kn->DisableRadioGroup();
                }
            }
            else
            {
                cout << "***** Object not found! " << KnotName << endl;
            }

            //cout << "EG3DcomActivateKnot OK" << endl;
        }
        break;


        //
        // Child-Skalierung setzen
        //
        case EG3DcomSetScale:
        {
            SG3DcomData* nr = (SG3DcomData*)val;
            if (nr->mObject != NULL)
            {
                CG3DResChild* cld = (CG3DResChild*)(nr->mObject);
                cld->mOrigParams.mScale = nr->mPos;
            }
        }
        break;

        //
        // Linke Maustaste verarbeiten
        //
        case EG3DcomLeftMouseClick:
        {
            CG3DResChild* ccld;
            SG3DcomData* scom = (SG3DcomData*)val;
            retval = gResourceList->ClickAction(&scom->mName, &ccld);

            if (retval)
            {
                //cout << "NAME=" << ccld->mRef->mName << " TYPE=" << ccld->mRef->mType << endl;

                scom->mFlags = 0;
                switch (ccld->mRef->mType)
                {
                    case ERTYPE_SLIDER:

                        scom->mFlags |= SPF_SLIDER;
                        break;

                    case ERTYPE_BUTTON:
                    {
                        CG3DResourceButton* bt = dynamic_cast<CG3DResourceButton*> (ccld->mRef);
                        if (bt->GetButtonStatus() == EBTSTAT_ACTIVE)
                        {
                            scom->mFlags |= 1;
                        }
                    }
                    break;
                }
                scom->mObject = ccld;
            }
            else
            {
                scom->mObject = NULL;
            }
        }
        break;

        //
        // Child-Objekt suchen
        //
        case EG3DcomSearchChild:
        {
            SG3DcomData* nr = (SG3DcomData*)val;
            nr->mObject = gResourceList->SearchChild(nr->mName);

            if (nr->mObject == NULL)
            {
                cout << "***** EG3DcomSearchChild NOT found:" << nr->mName << endl;
            }

            retval = (nr->mObject != NULL);
        }
        break;

        //
        // Child-Position ermitteln
        //
        case EG3DcomGetChildPos:
        {
            SG3DcomData* nr = (SG3DcomData*)val;
            if (nr->mObject != NULL)
            {
                CG3DResChild* cld = (CG3DResChild*)(nr->mObject);
                retval = (int)cld->mOrigParams.mPos.x;
            }
            else
            {
                retval = 0;
            }
        }
        break;

        //
        // Button setzen
        //
        case EG3DcomSetButton:
            if (gResourceList != NULL)
            {
                SG3DcomData* nr = (SG3DcomData*)val;
                CG3DResChild* cld = gResourceList->SearchChild(nr->mName);
                if (cld != NULL)
                {
                    if (cld->mRef->mType == ERTYPE_BUTTON)
                    {
                        CG3DResourceButton* bt = dynamic_cast<CG3DResourceButton*> (cld->mRef);
                        if (bt != NULL)
                        {
                            if (nr->mFlags & 1)
                            {
                                bt->SetButtonStatus(EBTSTAT_ACTIVE);
                            }
                            else
                            {
                                bt->SetButtonStatus(EBTSTAT_NORMAL);
                            }
                        }
                    }
                }
            }
            break;

        //
        // String setzen
        //
        case EG3DcomChangeString:
        {
            SG3DcomData* nr = (SG3DcomData*)val;
            if (nr != NULL)
            {
                if (nr->mObject != NULL)
                {
                    CG3DResChild* cld = (CG3DResChild*)(nr->mObject);
                    if (cld->mRef->mType == ERTYPE_STRING)
                    {
                        CG3DResourceString* str = dynamic_cast<CG3DResourceString*> (cld->mRef);

                        if (str != NULL)
                        {
                            //nr->mStrParam.clear();
                            //nr->mStrParam << "QQQ" << endl;



                            str->mText[nr->mFlags] = nr->mStrParam;
                            //str->mText[nr->mFlags] = "xxx"; //nr->mName;
                        }
                    }
                }
            }
        }
        break;

        //
        // Position des Child-Objekts setzen
        //
        case EG3DcomSetChildPos:
        {
            SG3DcomData* nr = (SG3DcomData*)val;
            if (nr->mObject != NULL)
            {
                CG3DResChild* cld = (CG3DResChild*)(nr->mObject);
                cld->SetPosition(&nr->mPos, nr->mFlags);
                retval = 1;
            }
            else
            {
                retval = 0;
            }
        }
        break;
    }
    return retval;
}


#ifdef _WIN32_
int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
    return TRUE;
}


#endif


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		~CG3DResourceList
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResourceList::~CG3DResourceList()
{
    //cout << "deleting resource list" << endl;

    for (list<CG3DResource*>::iterator res = begin();
        res != end();
        res++)
    {
        //cout << "Deleting " << (*res)->mName << endl;
        delete *res;
    }
    //cout << "ok" << endl;

    clear();
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		TestDoubleEntry
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::TestDoubleEntry(string& Name, CRecord* dr)
{
    for (list<CG3DResource*>::iterator res = begin();
        res != end();
        res++)
    {
        if ((*res)->mName == Name)
        {
            throw(CErrorMessage(EPERR_DOUBLE_NAME, Name, dr->mLineNumber, dr->mFilename));
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Show
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::Show()
{
    cout << "CG3DResourceList::Show START" << endl;

    if (mErrCode == EPERR_NOERROR)
    {
        for (list<CG3DResource*>::iterator res = begin();
            res != end();
            res++)
        {
            (*res)->Show();
        }
    }
    cout << "CG3DResourceList::Show OK" << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		CheckParamCount
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResourceList::CheckParamCount(CRecord& dr, int ExpectedParams)
{
    if (dr.size() != ExpectedParams)
    {
        stringstream es;

        es << "expected " << ExpectedParams << ", got " << dr.size();
        throw(CErrorMessage(EPERR_WRONG_PARAMETERCOUNT, es.str(), dr.mLineNumber, dr.mFilename));
        return false;
    }
    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Read
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CG3DResourceList::GetAlignStatus(string& AlignString, CRecord* dr)
{
    STag AlignList[] =
    {
        {											0, "LEFT_TOP"	   },
        { ERSTAT_VALIGN_CENTER,						   "LEFT_CENTER"   },
        { ERSTAT_VALIGN_BOTTOM,						   "LEFT_BOTTOM"   },
        { ERSTAT_VALIGN_BOTTOM2,					   "LEFT_BOTTOM2"  },
        { ERSTAT_HALIGN_CENTER,						   "CENTER_TOP"	   },
        { ERSTAT_HALIGN_CENTER | ERSTAT_VALIGN_CENTER, "CENTER_CENTER" },
        { ERSTAT_HALIGN_CENTER | ERSTAT_VALIGN_BOTTOM, "CENTER_BOTTOM" },
        { ERSTAT_HALIGN_RIGHT,						   "RIGHT_TOP"	   },
        { ERSTAT_HALIGN_RIGHT  | ERSTAT_VALIGN_CENTER, "RIGHT_CENTER"  },
        { ERSTAT_HALIGN_RIGHT  | ERSTAT_VALIGN_BOTTOM, "RIGHT_BOTTOM"  },
        { ERSTAT_HALIGN_RIGHTOBJECT,				   "RIGHT_OBJECT"  },
        { ERSTAT_VALIGN_BOTTOMOBJECT,				   "BOTTOM_OBJECT" },
        {											0, "VOID"		   },
        {											0, NULL			   },
    };
    CTagList TagFinder;
    int AlignStatus = TagFinder.Find(AlignString, AlignList);

    if (AlignStatus == -1)
    {
        throw(CErrorMessage(EPERR_UNKNOWN_TYPE, AlignString, dr->mLineNumber, dr->mFilename));
    }
    return AlignStatus;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		GetStretchMode
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CG3DResourceList::GetStretchMode(string& tag, CRecord* dr)
{
    STag StretchList[] =
    {
        {						0, "STRETCH_NONE" },
        { ESTRMODE_X,			   "STRETCH_X"	  },
        { ESTRMODE_Y,			   "STRETCH_Y"	  },
        { ESTRMODE_X | ESTRMODE_Y, "STRETCH_XY"	  },
        {						0, NULL			  }
    };
    CTagList TagFinder;
    int StretchMode = TagFinder.Find(tag, StretchList);

    if (StretchMode == -1)
    {
        throw(CErrorMessage(EPERR_UNKNOWN_TYPE, tag, dr->mLineNumber, dr->mFilename));
    }
    return StretchMode;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		ExtractBasePath
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::ExtractBasePath(string& FilePath)
{
    char* BasePathBuffer = new char[strlen(FilePath.c_str())+1];

    strcpy(BasePathBuffer, FilePath.c_str());

    char* p = strrchr(BasePathBuffer, '/');

    if (p != NULL)
    {
        mFilename = p + 1;

        p[1] = 0;
        char* BasePathBuffer2 = new char[strlen(BasePathBuffer) + 1];
        strcpy(BasePathBuffer2, BasePathBuffer);
        mBasePath = BasePathBuffer2;
        //cout << "BasePath=" << mBasePath << endl;
        //cout << "Filename=" << mFilename << endl;
    }
    else
    {
        mFilename = FilePath;
        delete[] BasePathBuffer;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Read
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResourceList::Read(string& Filepath)
{
    CStringTool stconv;
    CDatabase3 db3;
    CG3DResource* NewRes = NULL;
    CG3DResourceImage* ImgRes = NULL;
    CG3DResourceString* StrRes = NULL;
    CG3DResourceTableau* TabRes = NULL;
    CG3DResourceFont* FntRes = NULL;
    CG3DResourceFrame* FraRes = NULL;
    CG3DResourceButton* ButRes = NULL;
    CG3DResourceBox* BoxRes = NULL;
    CG3DResourceOButton* OptRes = NULL;
    CG3DResChild* CldRes = NULL;
    CG3DResourceTextfield* FldRes = NULL;
    CG3DResourceSlider* SldRes = NULL;
    CG3DResourceKnot* KntRes = NULL;
    CG3DGroup* Group = NULL;
    CG3DResourceClipframe* ClpRes = NULL;



    CRectT<int>* RectPtr = NULL;

    STag TagList[] =
    {
        { ERTAG_VOID,		  "VOID"		 },
        { ERTAG_NAME,		  "NAME"		 },
        { ERTAG_TYPE,		  "TYPE"		 },
        { ERTAG_POS,		  "POS"			 },
        { ERTAG_OPOS,		  "OPOS"		 },
        { ERTAG_REF,		  "REF"			 },
        { ERTAG_FREF,		  "FRAMEREF"	 },
        { ERTAG_PARENT,		  "PARENT"		 },
        { ERTAG_CHILD,		  "CHILD"		 },
        { ERTAG_IRECT,		  "IRECT"		 },
        { ERTAG_FRECT,		  "FRECT"		 },
        { ERTAG_TEXT,		  "TEXT"		 },
        { ERTAG_FILES,		  "FILES"		 },
        { ERTAG_SCALE,		  "SCALE"		 },
        { ERTAG_STATUS,		  "STATUS"		 },
        { ERTAG_COLOR,		  "COLOR"		 },
        { ERTAG_GROUP,		  "GROUP"		 },
        { ERTAG_LINE,		  "LINE"		 },
        { ERTAG_COLORREF,	  "COLORREF"	 },
        { ERTAG_PBUTT,		  "PBUTTON"		 },
        { ERTAG_STRETCH,	  "STRETCH"		 },
        { ERTAG_TPARAMS,	  "TPARAMS"		 },
        { ERTAG_JOIN,		  "JOIN"		 },
        { ERTAG_BTPROP,		  "BTPROP"		 },
        { ERTAG_CONTENT,	  "CONTENT"		 },
        { ERTAG_GPARENT,	  "GROUPPARENT"	 },
        { ERTAG_SLIDERLIMIT,  "SLIDERLIMIT"	 },
        { ERTAG_TOOLTIPP,	  "TOOLTIPP"	 },
        { ERTAG_TOOLTIPPTIME, "TOOLTIPPTIME" },
        { ERTAG_VOID,		  NULL			 }
    };
    CTagList TagFinder;


    ExtractBasePath(Filepath);


    try
    {
        clear();
        if (db3.Read(NULL, mBasePath, mFilename, '\t'))
        {
            //cout << "Zeilen:" << db3.size() << endl;

            for (list<CRecord>::iterator dr = db3.begin();
                dr != db3.end();
                dr++)
            {
                //cout << "TEST" << endl;
                //cout << "TAG=" << (*dr)[0] << endl;

                int tag = TagFinder.Find((*dr)[0], TagList);

                //cout << "tag=" << tag << endl;

                switch (tag)
                {
                    case ERTAG_NAME:
                        if (CheckParamCount(*dr, 3))
                        {
                            TestDoubleEntry((*dr)[2], &(*dr));

                            if ((*dr)[1] == "ROOT")
                            {
                                mRoot = new CG3DResourceRoot;
                                NewRes = mRoot;
                                NewRes->mType = ERTYPE_ROOT;

                                mRoot->InitRoot(gGlobals->mWindowWidth, gGlobals->mWindowHeight);

                                //cout << "ROOT name=" << (*dr)[2] << endl;
                            }
                            else
                            if ((*dr)[1] == "KNOT")
                            {
                                KntRes = new CG3DResourceKnot;
                                NewRes = KntRes;
                                NewRes->mType = ERTYPE_KNOT;
                                Group = KntRes;
                                //cout << "KNOT name=" << (*dr)[2] << endl;
                            }
                            else
                            if ((*dr)[1] == "BUTTON")
                            {
                                ButRes = new CG3DResourceButton;
                                NewRes = ButRes;
                                NewRes->mType = ERTYPE_BUTTON;
                                Group = ButRes;
                            }
                            else
                            if ((*dr)[1] == "SLIDER")
                            {
                                SldRes = new CG3DResourceSlider;
                                NewRes = SldRes;
                                NewRes->mType = ERTYPE_SLIDER;
                            }
                            else
                            if ((*dr)[1] == "OBUTTON")
                            {
                                OptRes = new CG3DResourceOButton;
                                NewRes = OptRes;
                                ButRes = OptRes;
                                NewRes->mType = ERTYPE_OBUTTON;
                            }
                            else
                            if ((*dr)[1] == "DIALOG")
                            {
                                NewRes = new CG3DResourceDialog;
                                NewRes->mType = ERTYPE_DIALOG;
                            }
                            else
                            if ((*dr)[1] == "STRING")
                            {
                                StrRes = new CG3DResourceString;
                                NewRes = StrRes;
                                NewRes->mType = ERTYPE_STRING;
                                FldRes = NULL;
                            }
                            else
                            if ((*dr)[1] == "IMAGE")
                            {
                                ImgRes = new CG3DResourceImage;
                                NewRes = ImgRes;
                                NewRes->mType = ERTYPE_IMAGE;
                            }
                            else
                            if ((*dr)[1] == "TABL")
                            {
                                TabRes = new CG3DResourceTableau;
                                NewRes = TabRes;
                                NewRes->mType = ERTYPE_TABLEAU;
                            }
                            else
                            if ((*dr)[1] == "FONT")
                            {
                                FntRes = new CG3DResourceFont;
                                NewRes = FntRes;
                                NewRes->mType = ERTYPE_FONT;
                            }
                            else
                            if ((*dr)[1] == "FRAME")
                            {
                                FraRes = new CG3DResourceFrame;
                                NewRes = FraRes;
                                NewRes->mType = ERTYPE_FRAME;

                                FraRes->mRect.Set(0, 0, gGlobals->mWindowWidth, gGlobals->mWindowHeight);
                                RectPtr = &(FraRes->mRect);
                            }
                            else
                            if ((*dr)[1] == "CLIPFRAME")
                            {
                                ClpRes = new CG3DResourceClipframe;
                                NewRes = ClpRes;
                                NewRes->mType = ERTYPE_CLIPFRAME;

                                ClpRes->mRect.Set(0, 0, gGlobals->mWindowWidth, gGlobals->mWindowHeight);
                                RectPtr = &(ClpRes->mRect);
                            }
                            else
                            if ((*dr)[1] == "BOX")
                            {
                                BoxRes = new CG3DResourceBox;
                                NewRes = BoxRes;
                                NewRes->mType = ERTYPE_BOX;
                                RectPtr = &(BoxRes->mBrect);

                                //cout << "BOX Name=" << (*dr)[2] << endl;
                            }
                            else
                            if ((*dr)[1] == "TEXTFIELD")
                            {
                                FldRes = new CG3DResourceTextfield;
                                NewRes = FldRes;
                                NewRes->mType = ERTYPE_TEXTFIELD;
                                StrRes = NULL;
                            }
                            else
                            {
                                throw(CErrorMessage(EPERR_UNKNOWN_TYPE, (*dr)[1], dr->mLineNumber, dr->mFilename));
                            }
                        }

                        NewRes->mName = (*dr)[2];
                        push_back(NewRes);            // An Liste anfügen
                        break;

                    case ERTAG_TOOLTIPPTIME:

                        if (CheckParamCount(*dr, 2))
                        {
                            gGlobals->mTooltippTime = stconv.StringTo<int>((*dr)[1]);
                        }
                        break;

                    case ERTAG_TOOLTIPP:
                    {
                        //cout << "TOOLTIPP:" << (*dr)[1] << endl;
                        CG3DResourceTooltipp* TtpRes = new CG3DResourceTooltipp;
                        TtpRes->mType = ERTYPE_TOOLTIPP;
                        TtpRes->mName = "_TT_";

                        CG3DResRef ResRef;
                        ResRef.mName = (*dr)[1];
                        TtpRes->mRefs.push_back(ResRef);


                        CG3DResChild ResChild;
                        ResChild.mName = "_TT_";
                        ResChild.mRef = TtpRes;
                        ResChild.mStatus.Set(ESTAT_BACKDRAW);
                        NewRes->mChilds.push_back(ResChild);

                        push_back(TtpRes);
                    }
                    break;

                    case ERTAG_SLIDERLIMIT:

                        if (SldRes != NULL)
                        {
                            SldRes->mSliderMin = stconv.StringTo<int>((*dr)[2]);
                            SldRes->mSliderMax = stconv.StringTo<int>((*dr)[3]);
                        }
                        if ((*dr)[1] == "VERTICAL")
                        {
                            SldRes->mSliderType = ESLIDER_VERTIKAL;
                        }
                        else
                        if ((*dr)[1] == "HORIZONTAL")
                        {
                            SldRes->mSliderType = ESLIDER_HORIZONTAL;
                        }
                        break;


                    case ERTAG_GPARENT:

                        Group->SetGroupParent(Find(NewRes, (*dr)[1], ERTYPE_ANY));
                        Group->SetProperty(EBTPROP_AUTOGROUP, false);
                        break;


                    case ERTAG_BTPROP:

                        for (int k = 1; k < dr->size(); k++)
                        {
                            if ((*dr)[k] == "TEXTROLL")
                            {
                                ButRes->SetProperty(EBTPROP_KEEPTXTROLL, true);
                            }
                            else
                            if ((*dr)[k] == "NOTEXTROLL")
                            {
                                ButRes->SetProperty(EBTPROP_TEXTROLLOVER, false);
                            }
                            else
                            if ((*dr)[k] == "NOACTIVE")
                            {
                                ButRes->SetProperty(EBTPROP_NOACTIVE, true);
                            }
                            else
                            {
                                throw(CErrorMessage(EPERR_NOTDEFINED, (*dr)[1], dr->mLineNumber, dr->mFilename));
                            }
                        }
                        break;



                    case ERTAG_STRETCH:

                        CldRes->mStretchMode = (EG3DStretchMode)GetStretchMode((*dr)[1], &(*dr));
                        break;

/*
 *                  case ERTAG_PBUTT:
 *
 *                      if (CheckParamCount(*dr, 2))
 *                      {
 *                          CG3DResourceOButton* PButton = dynamic_cast<CG3DResourceOButton*> (Find(ButRes, (*dr)[1], ERTYPE_OBUTTON));
 *                          if (PButton != NULL)
 *                          {
 *                              ButRes->SetPbutton(PButton);
 *                          }
 *                      }
 *                      break;
 */


                    case ERTAG_COLORREF:
                    {
                        if ((*dr)[1] == "STRCOLOR")
                        {
                            CheckParamCount(*dr, 3);
                        }
                        else
                        {
                            CheckParamCount(*dr, 4);
                        }

                        float* Color = mColorList.FindColor((*dr)[2]);
                        if (Color != NULL)
                        {
                            if ((*dr)[1] == "BOXCOLOR")
                            {
                                BoxRes->mStdColor = stconv.StringTo<int>((*dr)[3]);
                                BoxRes->mBoxColor[BoxRes->mStdColor] = Color;
                            }
                            else
                            if ((*dr)[1] == "FRAMECOLOR")
                            {
                                BoxRes->mStdColor = stconv.StringTo<int>((*dr)[3]);
                                BoxRes->mFrameColor[BoxRes->mStdColor] = Color;
                            }
                            else
                            if ((*dr)[1] == "STRCOLOR")
                            {
                                if (StrRes != NULL)
                                {
                                    StrRes->mColorRef = Color;
                                    StrRes->mOrigColor = Color;
                                }
                                if (FldRes != NULL)
                                {
                                    FldRes->mColorRef = Color;
                                }
                            }
                            else
                            {
                                throw(CErrorMessage(EPERR_NOTDEFINED, (*dr)[1], dr->mLineNumber, dr->mFilename));
                            }
                        }
                        else
                        {
                            //cout << "Farbe nicht gefunden: " << (*dr)[2] << " Zeile=" << dr->mLineNumber << endl;

                            throw(CErrorMessage(EPERR_COLORUNKNOWN, (*dr)[2], dr->mLineNumber, dr->mFilename));
                        }
                    }
                    break;


                    case ERTAG_COLOR:

                        if (dr->size() == 6)
                        {
                            CG3DColor Color;

                            Color.mName = (*dr)[1];
                            Color.mRGB[0] = stconv.StringTo<float>((*dr)[2]);
                            Color.mRGB[1] = stconv.StringTo<float>((*dr)[3]);
                            Color.mRGB[2] = stconv.StringTo<float>((*dr)[4]);
                            Color.mRGB[3] = stconv.StringTo<float>((*dr)[5]);

                            if (Color.mName == "ROLLOVER")
                            {
                                memcpy(gGlobals->mRolloverColor, Color.mRGB, sizeof(float) * 4);
                            }
                            else
                            if (Color.mName == "TT_FRAME")
                            {
                                memcpy(gGlobals->mTTframeColor, Color.mRGB, sizeof(float) * 4);
                            }
                            else
                            if (Color.mName == "TT_BOX")
                            {
                                memcpy(gGlobals->mTTboxColor, Color.mRGB, sizeof(float) * 4);
                            }
                            else
                            if (Color.mName == "TT_FONT")
                            {
                                memcpy(gGlobals->mTTfontColor, Color.mRGB, sizeof(float) * 4);
                            }

                            mColorList.push_back(Color); //
                        }
                        break;

                    case ERTAG_FRECT:

                        //cout << "ERTAG_FRECT 1" << endl;

                        if (CheckParamCount(*dr, 6))
                        {
                            //cout << "ERTAG_FRECT 2" << endl;

                            if (RectPtr == NULL)
                            {
                                cout << "***** ERTAG_FRECT RectPtr=NULL" << endl;
                            }
                            else
                            {
                                RectPtr->left = stconv.StringTo<int>((*dr)[1]);
                                RectPtr->top = stconv.StringTo<int>((*dr)[2]);
                                RectPtr->right = stconv.StringTo<int>((*dr)[3]);
                                RectPtr->bottom = stconv.StringTo<int>((*dr)[4]);


                                int AlignStatus = GetAlignStatus((*dr)[5], &(*dr));

                                if (AlignStatus & ERSTAT_HALIGN_RIGHT)
                                {
                                    RectPtr->left += gGlobals->mWindowWidth;
                                    RectPtr->right += gGlobals->mWindowWidth;
                                }

                                if (AlignStatus & ERSTAT_VALIGN_BOTTOM)
                                {
                                    RectPtr->top += gGlobals->mWindowHeight;
                                    RectPtr->bottom += gGlobals->mWindowHeight;
                                }
                            }
                        }
                        break;

                    case ERTAG_SCALE:
                        CldRes->mOrigParams.mScale.x = stconv.StringTo<float>((*dr)[1]);
                        CldRes->mOrigParams.mScale.y = stconv.StringTo<float>((*dr)[2]);
                        break;

                    case ERTAG_TPARAMS:
                        if (FldRes != NULL)
                        {
                            if (CheckParamCount(*dr, 5))
                            {
                                float x = stconv.StringTo<int>((*dr)[1]);
                                float y = stconv.StringTo<int>((*dr)[2]);
                                int tmax = stconv.StringTo<int>((*dr)[3]);
                                int freq = stconv.StringTo<int>((*dr)[4]);

                                FldRes->SetTextParams(x, y, tmax, freq);
                            }
                        }
                        break;

                    case ERTAG_OPOS:
                    {
                        float x = stconv.StringTo<float>((*dr)[1]);
                        float y = stconv.StringTo<float>((*dr)[2]);
                        float scx = stconv.StringTo<float>((*dr)[3]);
                        float scy = stconv.StringTo<float>((*dr)[4]);

                        OptRes->SetOptionParams(x, y, scx, scy);
                    }
                    break;

/*
 *                  case ERTAG_POS:
 *                  {
 *                      CStringConverter stconv;
 *                      CldRes->mPosition.x = stconv.StringTo<int>((*dr)[1]);
 *                      NewRes->mPosition.y = stconv.StringTo<int>((*dr)[2]);
 *                  }
 *                  break;
 */
                    case ERTAG_LINE:

                        if (CheckParamCount(*dr, 6))
                        {
                            static int sLineNum;
                            stringstream LineName;
                            CG3DResourceLine* Line = new CG3DResourceLine;
                            CG3DResChild ResChild;
                            LineName << "_LN_" << sLineNum++;
                            ResChild.mName = LineName.str();
                            Line->mName = LineName.str();
                            //ResChild.mOffPos = ResChild.mRefPos;

                            ResChild.mRef = Line;
                            Line->mType = ERTYPE_LINE;

                            //ResChild.mRefPos.Set(0,0);
                            //ResChild.mOffPos.Set(0,0);

                            Line->ParseColor((*dr)[1], Line->mColor);


                            Line->mLineStart.x = stconv.StringTo<int>((*dr)[2]);
                            Line->mLineStart.y = stconv.StringTo<int>((*dr)[3]);
                            Line->mLineEnd.x = stconv.StringTo<int>((*dr)[4]);
                            Line->mLineEnd.y = stconv.StringTo<int>((*dr)[5]);

                            NewRes->mChilds.push_back(ResChild);
                            CldRes = &(NewRes->mChilds.back());
                            //cout << "ERTAG_LINE!" << endl;
                        }
                        break;


                    case ERTAG_GROUP:

                        if (CheckParamCount(*dr, 2))
                        {
                            Group->SetGroup(stconv.StringTo<int>((*dr)[1]));
                        }
                        break;

                    case ERTAG_STATUS:

                        for (int i = 1; i < (*dr).size(); i++)
                        {
                            if ((*dr)[i] == "DEBUG")
                            {
                                CldRes->mStatus.Set(ESTAT_DEBUG);
                            }
                            else
                            if ((*dr)[i] == "DISABLED")
                            {
                                CldRes->mStatus.Set(ESTAT_DISABLED);
                            }
                            else
                            if ((*dr)[i] == "BACKDRAW")
                            {
                                CldRes->mStatus.Set(ESTAT_BACKDRAW);
                            }
                            else
                            if ((*dr)[i] == "FRONTDRAW")
                            {
                                CldRes->mStatus.Set(ESTAT_FRONTDRAW);
                            }
                            else
                            if ((*dr)[i] == "INACTIVE")
                            {
                                CldRes->mStatus.Clr(ESTAT_ACTIVE);
                            }
                            else
                            if ((*dr)[i] == "REKURSIV")
                            {
                                CldRes->mStatus.Set(ESTAT_ACTIVATE_CHILDREN);
                            }
                            else
                            if ((*dr)[i] == "INHERITSCALE")
                            {
                                CldRes->mStatus.Set(ESTAT_INHERITSCALE);
                            }
                            else
                            if ((*dr)[i] == "REFERENCE")
                            {
                                CldRes->mStatus.Set(ESTAT_REFERENCE);
                            }
                            else
                            {
                                throw(CErrorMessage(EPERR_NOTDEFINED, (*dr)[i], dr->mLineNumber, dr->mFilename));
                            }
                        }
                        break;


                    case ERTAG_FREF:

                        if (CheckParamCount(*dr, 6))
                        {
                            CG3DResChild ResChild;
                            ResChild.mName = (*dr)[1];
                            ResChild.mOrigParams.mPos.x = stconv.StringTo<int>((*dr)[2]);
                            ResChild.mOrigParams.mPos.y = stconv.StringTo<int>((*dr)[3]);

                            int AlignStatus = GetAlignStatus((*dr)[4], &(*dr));
                            if (AlignStatus > 0)
                            {
                                ResChild.mAlignment |= AlignStatus;
                            }

                            STag StretchList[] =
                            {
                                {						0, "STRETCH_NONE" },
                                { ESTRMODE_X,			   "STRETCH_X"	  },
                                { ESTRMODE_Y,			   "STRETCH_Y"	  },
                                { ESTRMODE_X | ESTRMODE_Y, "STRETCH_XY"	  },
                                {						0, NULL			  }
                            };

                            EG3DStretchMode StretchMode = (EG3DStretchMode)TagFinder.Find((*dr)[5], StretchList);
                            if (StretchMode > 0)
                            {
                                ResChild.mStretchMode = StretchMode;
                            }

                            NewRes->mChilds.push_back(ResChild);
                            CldRes = &(NewRes->mChilds.back());
                        }
                        break;

                    case ERTAG_CONTENT:
                    case ERTAG_CHILD:
                    {
                        CG3DResChild ResChild;
                        if (dr->size() == 2)
                        {
                            ResChild.mName = (*dr)[1];
                        }
                        else
                        if (CheckParamCount(*dr, 5))
                        {
                            ResChild.mName = (*dr)[1];
                            ResChild.mOrigParams.mPos.x = stconv.StringTo<int>((*dr)[2]);
                            ResChild.mOrigParams.mPos.y = stconv.StringTo<int>((*dr)[3]);

                            int AlignStatus = GetAlignStatus((*dr)[4], &(*dr));
                            if (AlignStatus > 0)
                            {
                                ResChild.mAlignment |= AlignStatus;
                            }
                        }
                        NewRes->mChilds.push_back(ResChild);
                        CldRes = &(NewRes->mChilds.back());

                        if (tag == ERTAG_CONTENT)
                        {
                            ButRes->SetContent(CldRes);
                        }
                    }
                    break;

                    case ERTAG_IRECT:
                        if (ImgRes != NULL)
                        {
                            CRectT<int>* rc = ImgRes->mIrect + stconv.StringTo<int>((*dr)[1]);
                            rc->left = stconv.StringTo<int>((*dr)[2]);
                            rc->top = stconv.StringTo<int>((*dr)[3]);
                            rc->right = stconv.StringTo<int>((*dr)[4]);
                            rc->bottom = stconv.StringTo<int>((*dr)[5]);
                        }
                        else
                        {
                            throw(CErrorMessage(EPERR_NOTDEFINED, "IMAGE", dr->mLineNumber, dr->mFilename));
                        }
                        break;

                    case ERTAG_REF:
                    {
                        CG3DResChild ResRef;
                        ResRef.mName = (*dr)[1];
                        NewRes->mRefs.push_back(ResRef);
                    }
                    break;

                    case ERTAG_TEXT:
                        if (StrRes != NULL)
                        {
                            int LangIndex;

                            if ((*dr)[1] == "DE")
                            {
                                LangIndex = ELANG_GERMAN;
                            }
                            else
                            if ((*dr)[1] == "EN")
                            {
                                LangIndex = ELANG_ENGLISH;
                            }
                            else
                            if ((*dr)[1] == "ES")
                            {
                                LangIndex = ELANG_SPANISH;
                            }
                            else
                            if ((*dr)[1] == "FR")
                            {
                                LangIndex = ELANG_FRENCH;
                            }
                            else
                            {
                                throw(CErrorMessage(EPERR_LANG_UNSUPPORTED, (*dr)[1], dr->mLineNumber, dr->mFilename));
                                break;
                            }
                            StrRes->mText[LangIndex] = (*dr)[2];
                        }
                        else
                        {
                            throw(CErrorMessage(EPERR_NOTDEFINED, "STRING", dr->mLineNumber, dr->mFilename));
                        }
                        break;

                    case ERTAG_VOID:

                        break;

                    case ERTAG_FILES:

                        if (NewRes->mType == ERTYPE_TABLEAU)
                        {
                            if (TabRes != NULL)
                            {
                                if (dr->size() == 2)
                                {
                                    string GrafFile = mBasePath + (*dr)[1];

                                    TabRes->LoadTextureTga(GrafFile.c_str(), false);
                                }
                                else
                                if (dr->size() == 3)
                                {
                                    string GrafFile = mBasePath + (*dr)[1];
                                    string AlphaFile = mBasePath + (*dr)[2];

                                    TabRes->LoadTextureTga2D(GrafFile.c_str(), AlphaFile.c_str(), false, false);
                                }
                            }
                            else
                            {
                                throw(CErrorMessage(EPERR_NOTDEFINED, "TABL", dr->mLineNumber, dr->mFilename));
                            }
                        }
                        else
                        if (NewRes->mType == ERTYPE_FONT)
                        {
                            string FontPath = mBasePath + (*dr)[1];

                            FntRes->Load(FontPath.c_str(), (*dr)[2].c_str(), gGlobals->mWindowHeight);
                        }
                        break;



                    case -1:

                        throw(CErrorMessage(EPERR_UNKNOWN_KEYWORD, (*dr)[0], dr->mLineNumber, dr->mFilename));
                        break;
                }
            }
            //cout << "CG3DResourceList::Read OK" << endl;
            ResolveChilds();
            //cout << "CG3DResourceList::Read ResolveChilds OK" << endl;
            ResolveRefs();
            //cout << "CG3DResourceList::Read ResolveRefs OK" << endl;
            Adjust();
            //cout << "CG3DResourceList::Read Adjust OK" << endl;
            //mRoot->SetLevel();
        }
    }
    catch (CErrorMessage& ErrMsg)
    {
        mErrCode = ErrMsg.mErrCode;
        ErrMsg.ShowError();
        exit(0);
    }
    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Reload
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::Reload()
{
    Read(mFilename);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Find
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::AdjustFonts()
{
    for (list<CG3DResource*>::iterator res = begin();
        res != end();
        res++)
    {
        if ((*res)->mType == ERTYPE_FONT)
        {
            (*res)->Adjust(NULL);
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Find
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResource* CG3DResourceList::Find(CG3DResource* Ref, string& Name, EResourceType rtype)
{
    for (list<CG3DResource*>::iterator res = begin();
        res != end();
        res++)
    {
        if ((*res)->mName == Name)
        {
            if ((rtype == ERTYPE_ANY) || ((*res)->mType == rtype))
            {
                return *res;
            }
        }
    }
    if (Ref != NULL)
    {
        string ErrTxt = string("from: ") + Ref->mName + string(" to: ") + Name;
        throw(CErrorMessage(EPERR_FINDERROR, ErrTxt));
    }
    else
    {
        string ErrTxt = string("not found: ") + Name;
        throw(CErrorMessage(EPERR_FINDERROR, ErrTxt));
    }


    return NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Resolve
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::ResolveChilds()
{
    for (list<CG3DResource*>::iterator res = begin();
        res != end();
        res++)
    {
        list<CG3DResChild>* ChildList = &((*res)->mChilds);
        if (!ChildList->empty())
        {
            for (list<CG3DResChild>::iterator cld = ChildList->begin();
                cld != ChildList->end();
                cld++)
            {
                if ((*res)->IsButton())
                {
                    cld->mStatus.Clr(ESTAT_ACTIVE);
                }

                if (cld->mRef == NULL)
                {
                    cld->mRef = Find(*res, (*cld).mName);
                    cld->mRef->mParent = *res;

                    switch (cld->mRef->mType)
                    {
                        case ERTYPE_STRING:

                            if ((*res)->IsButton())
                            {
                                CG3DResourceButton* bt = dynamic_cast<CG3DResourceButton*>(*res);
                                bt->SetContent(&(*cld));
                            }
                            break;


                        case ERTYPE_SLIDER:
                        case ERTYPE_BUTTON:
                        case ERTYPE_OBUTTON:
                        case ERTYPE_TEXTFIELD:
                            cld->mStatus.Set(ESTAT_TESTROLLOVER);
                            break;

                        case ERTYPE_DIALOG:
                        case ERTYPE_BOX:
                            cld->mStatus.Set(ESTAT_ACTIVATE_CHILDREN);
                            break;
                    }
                }
            }
        }
    }

    // Objekte der Ebene 0 aktivieren
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		ResolveRefs
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::ResolveRefs()
{
    for (list<CG3DResource*>::iterator res = begin();
        res != end();
        res++)
    {
        list<CG3DResRef>* RefList = &((*res)->mRefs);
        for (list<CG3DResRef>::iterator ref = RefList->begin();
            ref != RefList->end();
            ref++)
        {
            CG3DResource* RefRes = Find(*res, ref->mName, ERTYPE_ANY);
            ref->mRef = RefRes;

            if (RefRes != NULL)
            {
                (*res)->AddResRef(RefRes);
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Adjust
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::Adjust()
{
    //cout << "root type=" << mRoot->mType << endl;
    AdjustFonts();

    mRoot->InitRoot(gGlobals->mWindowWidth, gGlobals->mWindowHeight);
    mRoot->AdjustRoot();
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceList::DrawList()
{
    //mRoot->CalcAlignPosition();
    mRoot->FrontDraw();
    mRoot->DrawRoot();
    mRoot->BackDraw();
}


#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		DisableAll
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::DisableAll(CG3DResource* Except)
{
    for (list<CG3DResource*>::iterator res = begin();
        res != end();
        res++)
    {
        if (res->mType == Except)
        {
        }
    }
}


#endif

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResourceList::Rollover(CVector2<int>* Mouse)
{
    bool RolloverAction = false;

    mLastInsideObject = mInsideObject;

    try
    {
        if (mInsideObject != NULL)
        {
            mInsideObject->ActivateTooltipp(false);
        }
        mRoot->Rollover(Mouse);

        if (mInsideObject != NULL)
        {
            mInsideObject = NULL;
            RolloverAction = true;
        }
    }

    catch (CInsideObject& iobj)
    {
        if (mInsideObject != NULL)
        {
            if (mInsideObject != iobj.mInsideObject)
            {
                mInsideObject->Reset(ERESET_ROLLOVER);
                mTooltippTime = 0;
            }
            else
            {
                if (mTooltippTime++ > gGlobals->mTooltippTime)
                {
                    mInsideObject->ActivateTooltipp(true);
                }
            }
        }
        else
        {
            mTooltippTime = 0;
        }

        mInsideObject = iobj.mInsideObject;
        RolloverAction = true;
    }
    mLastInsideObject = mInsideObject;

    return RolloverAction;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		Draw
//
// Parameter:
//
// Return:    1 - Objekt geklickt
//            0 - kein Objekt geklickt
//
//---------------------------------------------------------------------------

int CG3DResourceList::ClickAction(string* ClickedName, CG3DResChild** ClickedChild)
{
    if (mInsideObject != NULL)
    {
        if (mInsideObject->IsButton())
        {
            CG3DResourceButton* bt = dynamic_cast<CG3DResourceButton*> (mInsideObject);
            bt->ClickAction();

            if (mTextField != NULL)
            {
                mTextField->mActive = false;
                mTextField = NULL;
            }
        }
        else
        if (mInsideObject->mType == ERTYPE_TEXTFIELD)
        {
            mTextField = dynamic_cast<CG3DResourceTextfield*> (mInsideObject);
            mTextField->mActive = true;
        }
        *ClickedName = mInsideObject->mName;
        *ClickedChild = SearchChild(mInsideObject->mName);
        Adjust();
        return 1;
    }
    return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
// Methode:		SearchChild
//
// Parameter:
//
//
//---------------------------------------------------------------------------

CG3DResChild* CG3DResourceList::SearchChild(string& Name)
{
    try
    {
        mRoot->SearchChild(Name);
    }
    catch (CSearchChild& scld)
    {
        return scld.mSearchChild;
    }
    return NULL;
}
