#ifndef CG3DRESOURCELIST
#define CG3DRESOURCELIST


#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <CDatabase3.h>
#include <CVector3T.h>
#include <CRectT.h>
#include <CStringConverter.h>
#include <CGL_Texture.h>
#include <CGL_FontTableau.h>
#include <CStringTool.h>

//---------------------------------------------------------------------------
//
// enum:    EG3DChildStatus
//
//---------------------------------------------------------------------------

enum EG3DChildStatus
{
    ERSTAT_HALIGN_NONE			= 0,
    ERSTAT_HALIGN_CENTER		= 1,
    ERSTAT_HALIGN_RIGHT			= 4,
    ERSTAT_VALIGN_CENTER		= 8,
    ERSTAT_VALIGN_BOTTOM		= 16,
    ERSTAT_HALIGN_RIGHTOBJECT	= 32,
    ERSTAT_VALIGN_BOTTOMOBJECT	= 64,
    ERSTAT_VALIGN_BOTTOM2		= 128
};

#define ERSTAT_HALIGN_LEFT    ERSTAT_HALIGN_NONE

//---------------------------------------------------------------------------
//
// enum:    EG3DStatus
//
//---------------------------------------------------------------------------

enum EG3DStatus
{
    ESTAT_TESTINSIDE		= 1,
    ESTAT_ACTIVE			= 2,
    ESTAT_ACTIVATE_CHILDREN = 4,
    ESTAT_DEBUG				= 8,
    ESTAT_DISABLED			= 16,
    ESTAT_BACKDRAW			= 32,   // als letztes zeichnen
    ESTAT_DRAWNOW			= 64,   //
    ESTAT_DRAW				= 128,  //
    ESTAT_INHERITSCALE		= 256,  // Skalierung vom Eltern-Object übernehmen
    ESTAT_TESTROLLOVER		= 512,
    ESTAT_FRONTDRAW			= 1024, // als erstes zeichnen
    ESTAT_REFERENCE			= 2048, // Alignment-Referenz
    ESTAT_TOOLTIPP			= 2048, // Child ist Tooltip
};

//---------------------------------------------------------------------------
//
// enum:    EG3DButtonStatus
//
//---------------------------------------------------------------------------

enum EG3DButtonStatus
{
    EBTSTAT_NORMAL,
    EBTSTAT_ROLLOVER,
    EBTSTAT_ACTIVE
};

//---------------------------------------------------------------------------
//
// enum:    EG3DResetMode
//
//---------------------------------------------------------------------------

enum EG3DResetMode
{
    ERESET_ROLLOVER,
    ERESET_DEACTIVATE,
    ERESET_INIT
};

//---------------------------------------------------------------------------
//
// enum:    EG3DButtonProp
//
//---------------------------------------------------------------------------

enum EG3DButtonProp
{
    EBTPROP_TEXTROLLOVER	= 1,
    EBTPROP_ROLLOVER		= 2,
    EBTPROP_ACTIVE			= 4,
    EBTPROP_KEEPTXTROLL		= 8,
    EBTPROP_AUTOGROUP		= 16,
    EBTPROP_KLAPPBT			= 32,
    EBTPROP_NOACTIVE		= 64 // Button wird nicht "aktiv"
};


//---------------------------------------------------------------------------
//
// enum:    EG3DStretchMode
//
//---------------------------------------------------------------------------

enum EG3DStretchMode
{
    ESTRMODE_NONE	= 0,
    ESTRMODE_X		= 1,
    ESTRMODE_Y		= 2
};

enum EG3DSliderType
{
    ESLIDER_HORIZONTAL,
    ESLIDER_VERTIKAL,
};

//---------------------------------------------------------------------------
//
// enum:    EResourceTag
//
//---------------------------------------------------------------------------

enum EResourceTag
{
    ERTAG_VOID,         // nichts
    ERTAG_NAME,         // Name
    ERTAG_TYPE,         // Typ
    ERTAG_POS,          // XY-Position
    ERTAG_OPOS,         // XY-Position, optionsfeld
    ERTAG_REF,          // Referenz
    ERTAG_PARENT,       // Mutterobjekt
    ERTAG_CHILD,        // Kindobjekt
    ERTAG_IRECT,        // Image-Rechteck
    ERTAG_FRECT,        // Frame-Rechteck
    ERTAG_TABL,         // Tableau
    ERTAG_TEXT,         // Text
    ERTAG_FILES,        // Grafikdatei- und Alphamaskendatei-namen
    ERTAG_SCALE,        // XY-Skalierung
    ERTAG_FREF,         // Frame-Referenz,
    ERTAG_STATUS,       // Status,
    ERTAG_COLOR,        // Farbe
    ERTAG_COLORREF,     // Farb-Verweis
    ERTAG_GROUP,        // Gruppe (für Radio-Buttons)
    ERTAG_LINE,         // Linie,
    ERTAG_HALIGN,       // horiz. Schrift-Ausrichtung) für Button-Schrift
    ERTAG_PBUTT,
    ERTAG_STRETCH,      // Stretchmodus
    ERTAG_TPARAMS,      // Texteingabefeld-Parameter
    ERTAG_JOIN,         // Verknüpfung mit anderem Objekt
    ERTAG_BTPROP,       // Button eigenschaft
    ERTAG_CONTENT,      // Button Content
    ERTAG_GPARENT,      // Group Parent (für Buttons);
    ERTAG_SLIDERLIMIT,  // Slider Limit
    ERTAG_TOOLTIPP,     // Tooltipp
    ERTAG_TOOLTIPPTIME  // Tooltipp-Zeit
};

//---------------------------------------------------------------------------
//
// enum:    EResourceType
//
//---------------------------------------------------------------------------

enum EResourceType
{
    ERTYPE_ANY,             // Beliebig
    ERTYPE_VOID,            // Ungültig (nicht gesetzt)
    ERTYPE_KNOT,            // Knoten
    ERTYPE_STRING,          // String
    ERTYPE_BUTTON,          // Button
    ERTYPE_DIALOG,          // Dialog
    ERTYPE_IMAGE,           // Image
    ERTYPE_TABLEAU,         // Grafik-Tableau
    ERTYPE_FONT,            // Zeichensatz
    ERTYPE_FRAME,           // Rahmen
    ERTYPE_BOX,             // Kasten
    ERTYPE_OBUTTON,         // Option-Button
    ERTYPE_ROOT,            // Wurzel
    ERTYPE_LINE,            // Linie
    ERTYPE_TEXTFIELD,       // Texteingabe-Feld
    ERTYPE_SLIDER,          // Schieberegler
    ERTYPE_TOOLTIPP,        // Tooltipp
    ERTYPE_CLIPFRAME        // Clipping Rechteck
};

//---------------------------------------------------------------------------
//
// enum:    EParseError
//
//---------------------------------------------------------------------------

enum EParseError
{
    EPERR_NOERROR,
    EPERR_UNKNOWN_KEYWORD,
    EPERR_UNKNOWN_TYPE,
    EPERR_NOTDEFINED,
    EPERR_FINDERROR,
    EPERR_WRONG_PARAMETERCOUNT,
    EPERR_LANG_UNSUPPORTED,
    EPERR_COLORUNKNOWN,
    EPERR_DOUBLE_NAME
};


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

class CG3DResourceFrame;

#include "CG3DStatus.h"
#include "CG3DGlobals.h"
#include "CG3DColor.h"
#include "SG3DDrawParams.h"
#include "CTagList.h"
#include "CErrorMessage.h"
#include "CG3DResRef.h"
#include "CG3DResChild.h"
#include "CG3DResource.h"
#include "CG3DGroup.h"
#include "CG3DResourceKnot.h"
#include "CG3DResourceRoot.h"
#include "CG3DResourceTableau.h"
#include "CG3DResourceFont.h"
#include "CG3DResourceString.h"
#include "CG3DResourceImage.h"
#include "CG3DResourceImgRef.h"
#include "CG3DResourceBox.h"
#include "CG3DResourceTooltipp.h"
#include "CG3DResourceButton.h"
#include "CG3DResourceSlider.h"
#include "CG3DResourceOButton.h"
#include "CG3DResourceDialog.h"
#include "CG3DResourceFrame.h"
#include "CG3DResourceClipframe.h"
#include "CG3DResourceLine.h"
#include "CG3DResourceTextfield.h"



class CInsideObject
{
    public:

        CInsideObject(CG3DResource* res) : mInsideObject(res) { }
        CG3DResource* mInsideObject;
};


class CSearchChild
{
    public:

        CSearchChild(CG3DResChild* cld) : mSearchChild(cld) { }
        CG3DResChild* mSearchChild;
};



//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceList
//
//---------------------------------------------------------------------------

class CG3DResourceList : public list<CG3DResource*>
{
    public:

        CG3DResourceList()
        {
            mErrCode = EPERR_NOERROR;
            mTextField = NULL;
            mTooltippTime = 0;
            mLastInsideObject = NULL;
            mInsideObject = NULL;
            mRoot = NULL;
        }


        ~CG3DResourceList();

        bool Read(string& Filename);
        void Reload();
        void Show();
        CG3DResource* Find(CG3DResource* Ref, string& Name, EResourceType rtype = ERTYPE_ANY);
        void DrawList();
        void Adjust();
        bool Rollover(CVector2<int>* Mouse);
        int ClickAction(string* clo, CG3DResChild** ccld);
        CG3DResChild* SearchChild(string& Name);

        EParseError mErrCode;
        CG3DResourceTextfield* mTextField;

    protected:



        CG3DResource* mInsideObject;
        CG3DResource* mLastInsideObject;
        CG3DColorList mColorList;
        CG3DResourceRoot* mRoot;
        int mTooltippTime;
        string mFilename;
        string mBasePath;

        bool CheckParamCount(CRecord& dr, int ExpectedParams);
        void ResolveChilds();
        void ResolveRefs();
        int GetAlignStatus(string& AlignString, CRecord* dr);
        int GetStretchMode(string& tag, CRecord* dr);
        void TestDoubleEntry(string& Name, CRecord* dr);
        void ExtractBasePath(string& FilePath);
        void AdjustFonts();
};

#endif // CG3DRESOURCELIST
