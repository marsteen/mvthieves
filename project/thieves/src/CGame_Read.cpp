//---------------------------------------------------------------------------
//
// PROJECT : RIP OFF 2
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#define APP_NAME    "ripoff2"

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <cstdlib>


using namespace std;

#include <GLinclude.h>
#include <math.h>
#include <CVector3T.h>
#include <CFileIO.h>
#include <CStringTool.h>
#include <CException.h>

#include "CG3DGlobals.h"
#include "CSdlThiefs.h"

enum EReadMode
{
    EREADMODE_NONE,     // 0
    EREADMODE_POLY,     // 1
    EREADMODE_BOUND,    // 2
    EREADMODE_OBJECT    // 3
};

#define MOD_MIRROR_X	1
#define MOD_MIRROR_Y	2

extern float gGlobalScale;

// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ParseSvg
//
// Parsen eines SVG-Strings aus INKSCAPE
//
// ---------------------------------------------------------------------------

void CSdlThiefs::ParseSvg(string& SVGstr, CPolylineObject* poly, int Modifier)
{
    CStringTool st;
    vector<string> ResultVec1;

    st.SplitString(SVGstr, &ResultVec1, 'L');

    if (ResultVec1.size() > 0)
    {
        int i = 0;
        //cout << "SVGstr=" << SVGstr << endl;
        //cout << " Parts=" << ResultVec1.size() << endl;

        for (vector<string>::iterator it = ResultVec1.begin();
            it != ResultVec1.end();
            ++it)
        {
            vector<string> ResultVec2;
            st.SplitString(*it, &ResultVec2, ',');

            //cout << "    ResultVec2.size()=" << ResultVec2.size() << endl;

            if (ResultVec2.size() == 2)
            {
                float x, y;

                x = st.StringTo<float>(ResultVec2[0]);
                y = st.StringTo<float>(ResultVec2[1]);

                if (Modifier & MOD_MIRROR_Y)
                {
                    y *= -1;
                }
                if (Modifier & MOD_MIRROR_X)
                {
                    x *= -1;
                }

                poly->AddVertex(x, y);
                i++;
            }
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : ShowStack
//
//
// ---------------------------------------------------------------------------

static void ShowStack(vector<EReadMode>& Stack)
{
    cout << "Stack:";
    for (int i = 0; i < Stack.size(); i++)
    {
        cout << ' ' << Stack[i];
    }
    cout << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:		CSdlThiefs
// Methode:		ReadShapes
//
//
//---------------------------------------------------------------------------

void CSdlThiefs::ReadShapes(const char* Filename)
{
    CFileIO Infile;

    if (Infile.OpenFileRead(GsysPath(Filename), ios::binary))
    {
        string Line;
        CVectorShape* Vshape = NULL;
        CPolylineObject Poly;
        CBoundingObject Bound;
        vector<EReadMode> ModeStack;
        float ObjectScale = 1.0;
        //float ObjectRotation = 1.0;
        int Modifier = 0;
        float Color[4];

        EReadMode ReadMode = EREADMODE_NONE;

        ModeStack.push_back(EREADMODE_NONE);

        while (Infile.ReadLine(&Line))
        {
            CStringTool StringTool;
            vector<string> LineParts;

            StringTool.SplitString(Line, &LineParts, '\t');

            if (LineParts.size() > 0)
            {
                if (LineParts[0] == "OBJECT")
                {
                    Vshape = new CVectorShape;
                    Vshape->SetName(LineParts[1]);
                    ReadMode = EREADMODE_OBJECT;
                    ObjectScale = 1.0;
                    Modifier = 0;
                    //ObjectRotation = 0;

                    ModeStack.push_back(ReadMode);

                    //cout << "Object:" << LineParts[1] << endl;
                    //cout << "ReadMode=" << ReadMode << endl;
                }
                else
                if (LineParts[0] == "ORIGIN")
                {
                    Vshape->mOrigin.x = StringTool.StringTo<float>(LineParts[1]);
                    Vshape->mOrigin.y = StringTool.StringTo<float>(LineParts[2]);
                }
                else
                if (LineParts[0] == "POLY")
                {
                    Poly.Clear();
                    ReadMode = EREADMODE_POLY;
                    ModeStack.push_back(ReadMode);
                }
                else
                if (LineParts[0] == "BOUND")
                {
                    Bound.Clear();
                    ReadMode = EREADMODE_BOUND;
                    ModeStack.push_back(ReadMode);
                }
                else
                if (LineParts[0] == "SCALE")
                {
                    ObjectScale = StringTool.StringTo<float>(LineParts[1]);
                }
                else
                if (LineParts[0] == "ROTATION")
                {
                    //ObjectRotation = StringTool.StringTo<float>(LineParts[1]);
                }
                else
                if (LineParts[0] == "COLOR")
                {
                    for (int i = 0; i < 4; i++)
                    {
                        Color[i] = StringTool.StringTo<float>(LineParts[i+1]);
                    }
                }
                else
                if (LineParts[0] == "SVG")
                {
                    ParseSvg(LineParts[1], &Poly, Modifier);
                }
                else
                if (LineParts[0] == "MIRRORX")
                {
                    Modifier = MOD_MIRROR_X;
                }
                else
                if (LineParts[0] == "MIRRORY")
                {
                    Modifier = MOD_MIRROR_Y;
                }
                else
                if (LineParts[0] == "VERT")
                {
                    float x = StringTool.StringTo<float>(LineParts[1]);
                    float y = StringTool.StringTo<float>(LineParts[2]);

                    switch (ReadMode)
                    {
                        case EREADMODE_POLY:

                            Poly.AddVertex(x, y);
                            break;

                        case EREADMODE_BOUND:

                            Bound.AddVertex(x, y);
                            break;
                    }
                }
                else
                if (LineParts[0] == "END")
                {
                    //cout << "Readmode=" << ReadMode << endl;
                    //ShowStack(ModeStack);
                    switch (ReadMode)
                    {
                        case EREADMODE_POLY:

                            Vshape->AddPoly(Poly);
                            break;

                        case EREADMODE_BOUND:

                            //Vshape->SetBound(Bound);
                            break;

                        case EREADMODE_OBJECT:

                            //cout << "Add Object:" << Vshape->mName << endl;
                            Vshape->CenterShape();
                            Vshape->Scale(ObjectScale * gGlobalScale);
                            Vshape->SetColor(Color[0], Color[1], Color[2], Color[3]);
                            mShapeList.push_back(Vshape);
                            break;
                    }
                    ModeStack.pop_back();
                    ReadMode = ModeStack.back();
                }
            }
        }
    }
    else
    {
        throw(CException(1, "Shapefile not found"));
    }
}
