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

#define APP_NAME    "Die Planeten"

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#include <GLinclude.h>
#include <math.h>
#include <CVector3T.h>
#include <COpenGL.h>
#include <CGL_GLU_Sphere.h>
#include <CGL_Object.h>
#include <CGL_Disk.h>
#include <CFileIO.h>
#include "CGL_Ellipsoid.h"
#include "CG3DGlobals.h"
#include "CG3DReslistInterface.h"
#include "CGLApplication.h"

static float sYrot;
static float sZrot;
static float sXrot;
static float sYrotAuto;


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       : CGLApplication
//
//
//
// ---------------------------------------------------------------------------

CGLApplication::CGLApplication()
{
    gErde = NULL;
    mActInfobox = "";
    mInitFlag = false;
    mAutoScale = false;
    mAutoRotate = true;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CPlanet
// METHODE       : Delete
//
//
//
// ---------------------------------------------------------------------------

void CPlanet::Delete()
{
    for (vector<CMond>::iterator im = mMonde.begin();
        im != mMonde.end();
        im++)
    {
        im->Delete();
    }
    CGL_EllipsoidPatched::Delete();

    mMonde.clear();
}


static SPlanetDesc sMerkurDesc[] =
{
    {
        "planeten/merkur.tga",
        0.0, 1.0, 1.0, 1.0
    },
    { NULL, 0, 0, 0, 0 }
};

static SPlanetDesc sVenusDesc[] =
{
    {
        "planeten/venus.tga",
        0.0, 1.0, 1.0, 1.0
    },
    { NULL, 0, 0, 0, 0 }
};

#define SIZE_ERDE    12756.0
static SPlanetDesc sErdeDesc[] =
{
    {
        "planeten/erde.tga",
        1.0, 1.0, 1.0, 1.0
    },
    {
        "planeten/monde/mond.tga",
        (384400.0 / SIZE_ERDE) * 20,
        3476.0 / SIZE_ERDE,
        0, 0.1 / 30.0
    },
    { NULL, 0, 0, 0, 0 }
};

static SPlanetDesc sMarsDesc[] =
{
    {
        "planeten/mars.tga",
        0.0, 1.0, 1.0, 1.0
    },
    { NULL, 0, 0, 0, 0 }
};

#define SIZE_JUPITER    142984.0
static SPlanetDesc sJupiterDesc[] =
{
    {
        "planeten/jupiter.tga",
        0.0, 1.0, 1.0, 1.0
    },
    {
        // IO
        "planeten/monde/io.tga",
        (420100.0 / SIZE_JUPITER) * 20,
        3643.0 / SIZE_JUPITER,
        2.1,
        0.12
    },
    {
        // Europa
        "planeten/monde/europa.tga",
        (664100.0 / SIZE_JUPITER) * 20,
        3121.0 / SIZE_JUPITER,
        1.2,
        0.06
    },
    {
        // Ganymed
        "planeten/monde/ganymed.tga",
        (1069000.0 / SIZE_JUPITER) * 20,
        5264.0 / SIZE_JUPITER,
        1.8,
        0.03
    },
    {
        // Kallisto
        "planeten/monde/kallisto.tga",
        (1869500.0 / SIZE_JUPITER) * 20,
        4820.0 / SIZE_JUPITER,
        1.2,
        0.02
    },
    { NULL, 0, 0, 0, 0 }
};

#define SIZE_SATURN    120536.0
static SPlanetDesc sSaturnDesc[] =
{
    {
        "planeten/saturn.tga",
        1.0, 1.0, 1.0, 1.0
    },
    {
        "planeten/monde/titan.tga",
        (1221830.0 / SIZE_SATURN) * 20,
        5150.0 / SIZE_SATURN,
        0.2, 1.0 / 60
    },
    {
        "planeten/monde/rhea.tga",
        (527040.0 / SIZE_SATURN) * 20,
        1528.0 / SIZE_SATURN,
        0.2, 1.0 / 60
    },
    {
        "planeten/monde/dione.tga",
        (377420.0 / SIZE_SATURN) * 20,
        1118.0 / SIZE_SATURN,
        0.2, 1.0 / 60
    },
    {
        "planeten/monde/tethys.tga",
        (294670.0 / SIZE_SATURN) * 20,
        1060.0 / SIZE_SATURN,
        0.2, 1.0 / 60
    },
    {
        "planeten/monde/iapetus.tga",
        (3561300.0 / SIZE_SATURN) * 20,
        1436.0 / SIZE_SATURN,
        0.2, 1.0 / 60
    },
    { NULL, 0, 0, 0, 0 }
};



static SPlanetDesc sUranusDesc[] =
{
    {
        "planeten/uranus.tga",
        0.0, 1.0, 1.0, 1.0
    },
    { NULL, 0, 0, 0, 0 }
};

static SPlanetDesc sNeptunDesc[] =
{
    {
        "planeten/neptun.tga",
        0.0, 1.0, 1.0, 1.0
    },
    { NULL, 0, 0, 0, 0 }
};


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       : FileExists
//
//
//
// ---------------------------------------------------------------------------

static bool FileExists(const char* f)
{
    ifstream k(f, ios::binary);

    if (k.good())
    {
        k.close();
        return true;
    }
    cout << "***** File not found: " << f << endl;
    return false;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       : InitPlanet
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::InitPlanet(const SPlanetDesc* PlanetDesc)
{
    CGL_Texture tx;

    if (gErde == NULL)
    {
        gErde = new CPlanet;

        mPlanet = EPLANET_ERDE;
        mScale = 0.7;
        mPlanetSize = 0.7;
    }
    else
    {
        gErde->Delete();
    }

    //string ErdTexture = "planeten/saturn.tga";
    //string ErdTexture = "level0.tga";
    float WasserMaterial[] = { 122.0 / 255, 219.0 / 255, 251.0 / 255, 1.0 };

    for (int i = 0; PlanetDesc[i].mTextur != NULL; i++)
    {
        string TextureName = PlanetDesc[i].mTextur;
        CMond mond;
        int sx, sy;
        CGL_EllipsoidPatched* thing;

        if (i == 0)
        {
            thing = gErde;
            sx = 6;
            sy = 3;
        }
        else
        {
            thing = &mond;
            sx = 2;
            sy = 1;
        }

        thing->CreateEllipsoid(sx * 10, 20 * PlanetDesc[i].mSize, sy * 10, 20.0 * PlanetDesc[i].mSize, WasserMaterial);
        unsigned int* ErdeTexHandles = tx.CreateSplitTextures(TextureName, sx, sy);
        thing->SetMultiTextures(ErdeTexHandles, sx, sy);
        thing->InitDisplayList();
        gOpenGL->mCamera.mStandort.z = -80.0;

        if (i > 0)
        {
            mond.mPlanetDesc = PlanetDesc + i;
            gErde->mMonde.push_back(mond);
        }
        //CGL_EllipsoidPatched
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::SetResolution(int w, int h)
{
    gResGlobals->mWindowWidth = w;
    gResGlobals->mWindowHeight = h;

    if (!mInitFlag)
    {
        InitResources();
        mInitFlag = true;
    }
    else
    {
        //gG3Dinterface->SendCommand(EG3DcomReload);
        gG3Dinterface->SendCommand(EG3DcomAdjust);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::InitApplication(int argc, char* argv[])
{
    gOpenGL = new COpenGL;
    gG3Dinterface = new CG3DReslistInterface;
    gResGlobals = new CG3DGlobals;

    /*
     * int ScreenWidth =  glutGet(GLUT_WINDOW_WIDTH);
     * int ScreenHeight = glutGet(GLUT_WINDOW_HEIGHT);
     *
     * cout << "ScreenW=" << ScreenWidth << endl;
     * cout << "ScreenH=" << ScreenHeight << endl;
     */

    gOpenGL->InitWindowGLUT(argc, argv, APP_NAME, 1024, 768, mFullscreen);
    gOpenGL->InitGLUTCallback(10);
    //cout << "InitWindowGLUT OK" << endl;

    gOpenGL->mCamera.mStandort.z = -80.0;
}


void CGLApplication::InitResources()
{
    float Scale = 0.7;

    bool b = gG3Dinterface->Init("Resource/resource0.r3d", gResGlobals);

    if (b)
    {
        //cout << "gG3Dinterface->Init OK" << endl;
    }
    else
    {
        cout << "***** gG3Dinterface->Init failed" << endl;
    }

    //gResourceList->Read("Resource/GlobusResource.r3d");

    if (b)  //gResourceList->mErrCode == EPERR_NOERROR)
    {
        CFileIO fio;

        //cout << "- NO Errors - " << endl;

        //gResourceList->Show();

        string SaturnRingFile = string("planeten/saturnring.tga");
        string SaturnRingMask = string("planeten/A_saturnring2.tga");
        string ErdeLevel0 = "Level0-SW.tga";

        if (!FileExists(SaturnRingFile.c_str()))
        {
            exit(0);
        }
        if (!FileExists(SaturnRingMask.c_str()))
        {
            exit(0);
        }

        gSaturnRing.LoadTextureTga1D(SaturnRingFile, SaturnRingMask);
        gSaturnRing.mSegments = 256;
        gSaturnRing.mRadius = 60.0 * Scale;
        gSaturnRing.Init();

        ActivatePlanet(EPLANET_ERDE);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CPlanet
// METHODE       : DrawMonde
//
//
//
// ---------------------------------------------------------------------------

void CPlanet::DrawMonde(bool AutoRotate)
{
    for (vector<CMond>::iterator im = mMonde.begin();
        im != mMonde.end();
        im++)
    {
        glPushMatrix();

        im->Umlauf(AutoRotate);

        glTranslatef(im->mPlanetDesc->mDistance, 0, 0);

        im->Rotation(AutoRotate);

        im->DrawDisplayList();
        glPopMatrix();
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::Draw3DObjects()
{
    if (!mInitFlag)
    {
        return;
    }


    glPushMatrix();


    glRotatef(sXrot, 1.0, 0.0, 0.0);
    glRotatef(sZrot, 0.0, 0.0, 1.0);
    glRotatef(sYrot, 0.0, 1.0, 0.0);

    glScalef(mScale, mScale, mScale);

    gErde->DrawMonde(mAutoRotate);

    glRotatef(sYrotAuto, 0.0, 1.0, 0.0);


    gErde->DrawDisplayList();

    if (mPlanet == EPLANET_SATURN)
    {
        glRotatef(270, 1.0, 0.0, 0.0);

        glDisable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_1D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gSaturnRing.Draw();
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_BLEND);
        glPopMatrix();
        glEnable(GL_CULL_FACE);
    }
    glPopMatrix();

    if (mAutoScale)
    {
        if (mScale < mPlanetSize)
        {
            mScale += 0.01;
            if (mScale >= mPlanetSize)
            {
                mAutoScale = false;
            }
        }
        if (mScale > mPlanetSize)
        {
            mScale -= 0.01;
            if (mScale <= mPlanetSize)
            {
                mAutoScale = false;
            }
        }
    }

    if (mAutoRotate)
    {
        sYrotAuto += 0.1;
        if (sYrotAuto > 360)
        {
            sYrotAuto -= 360;
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       : ActivateInfobox
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::MouseMotion(int dx, int dy)
{
    if (mScale >= 1.0)
    {
        sXrot += (float)dy / (mScale * mScale * 3);
        sYrot += (float)dx / (mScale * mScale * -3);
    }
    else
    {
        sXrot += (float)dy / 3;
        sYrot += (float)dx / -3;
    }

    if (sXrot > 85)
    {
        sXrot = 85;
    }
    if (sXrot < -85)
    {
        sXrot = -85;
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       : ActivateInfobox
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::ActivatePlanet(EPlanet p)
{
    const char* TexturFile;
    SPlanetDesc* pd;

    gG3Dinterface->SendCommand(EG3DcomDeactivateKnot, (void*)mActInfobox);

    switch (p)
    {
        case EPLANET_MERKUR:

            mActInfobox = "MerkurInfoBox";
            TexturFile = "planeten/merkur.tga";
            pd = sMerkurDesc;
            break;

        case EPLANET_VENUS:

            mActInfobox = "VenusInfoBox";
            TexturFile = "planeten/venus.tga";
            pd = sVenusDesc;
            break;

        case EPLANET_ERDE:

            mActInfobox = "EarthInfoBox";
            TexturFile = "planeten/erde.tga";
            pd = sErdeDesc;
            break;

        case EPLANET_MARS:

            mActInfobox = "MarsInfoBox";
            TexturFile = "planeten/mars.tga";
            pd = sMarsDesc;
            break;

        case EPLANET_JUPITER:

            mActInfobox = "JupiterInfoBox";
            TexturFile = "planeten/jupiter.tga";
            pd = sJupiterDesc;
            break;

        case EPLANET_SATURN:

            mActInfobox = "SaturnInfoBox";
            TexturFile = "planeten/saturn.tga";
            pd = sSaturnDesc;
            break;

        case EPLANET_URANUS:

            mActInfobox = "UranusInfoBox";
            TexturFile = "planeten/uranus.tga";
            pd = sUranusDesc;
            break;

        case EPLANET_NEPTUN:

            mActInfobox = "NeptunInfoBox";
            TexturFile = "planeten/neptun.tga";
            pd = sNeptunDesc;
            break;
    }

    InitPlanet(pd);
    gG3Dinterface->SendCommand(EG3DcomActivateKnot, (void*)mActInfobox);
    //cout << "EG3DcomActivateKnot OK2" << endl;

    mPlanet = p;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       : MouseWheel
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::MouseWheel(int d)
{
    if (d == 0)
    {
        mScale += 0.05;
        if (mScale > 3)
        {
            mScale = 3;
        }
    }
    else
    {
        mScale -= 0.05;
        if (mScale < 0.05)
        {
            mScale = 0.05;
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::LeftMouseButtonDown()
{
    SG3DcomData scom;

    if (gG3Dinterface->SendCommand(EG3DcomLeftMouseClick, &scom))
    {
        if (scom.mFlags & SPF_SLIDER)
        {
            gSlider.mName = scom.mName;
            gSlider.mObject = scom.mObject;
        }
        else
        {
            gSlider.mObject = NULL;
        }

        if (scom.mName == "RotateOnButton")
        {
            mAutoRotate = true;
        }
        else
        if (scom.mName == "RotateOffButton")
        {
            mAutoRotate = false;
        }
        else
        if (scom.mName == "GermanButton")
        {
            gResGlobals->mLanguage = ELANG_GERMAN;
            gG3Dinterface->SendCommand(EG3DcomAdjust);
        }
        else
        if (scom.mName == "EnglishButton")
        {
            gResGlobals->mLanguage = ELANG_ENGLISH;
            gG3Dinterface->SendCommand(EG3DcomAdjust);
        }
        else
        if (scom.mName == "LargeButton")
        {
            mPlanetSize = 1.5;
            mAutoScale = true;
        }
        else
        if (scom.mName == "NormalButton")
        {
            mPlanetSize = 0.7;
            mAutoScale = true;
        }
        else
        if (scom.mName == "MerkurButton")
        {
            ActivatePlanet(EPLANET_MERKUR);
        }
        else
        if (scom.mName == "VenusButton")
        {
            ActivatePlanet(EPLANET_VENUS);
        }
        else
        if (scom.mName == "EarthButton")
        {
            ActivatePlanet(EPLANET_ERDE);
        }
        else
        if (scom.mName == "MarsButton")
        {
            ActivatePlanet(EPLANET_MARS);
        }
        else
        if (scom.mName == "JupiterButton")
        {
            ActivatePlanet(EPLANET_JUPITER);
        }
        else
        if (scom.mName == "SaturnButton")
        {
            ActivatePlanet(EPLANET_SATURN);
        }
        else
        if (scom.mName == "UranusButton")
        {
            ActivatePlanet(EPLANET_URANUS);
        }
        else
        if (scom.mName == "NeptunButton")
        {
            ActivatePlanet(EPLANET_NEPTUN);
        }
        else
        if (scom.mName == "QuitButton")
        {
            exit(0);
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::KeyboardAction(unsigned char key)
{
    if (gG3Dinterface->SendCommand(EG3DcomSendChar, &key) == 0)
    {
        switch (key)
        {
            case 'r':

                gG3Dinterface->SendCommand(EG3DcomReload);
                break;

            case 'e':

                gResGlobals->mLanguage = ELANG_ENGLISH;
                gG3Dinterface->SendCommand(EG3DcomAdjust);
                break;

            case 'd':

                gResGlobals->mLanguage = ELANG_GERMAN;
                gG3Dinterface->SendCommand(EG3DcomAdjust);
                break;
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGLApplication
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CGLApplication::ManageInterface(CGL_Mouse* Mouse)
{
    if (mInitFlag)
    {
        gG3Dinterface->SendCommand(EG3DcomRollover, Mouse);
        gG3Dinterface->SendCommand(EG3DcomDraw);
    }
}
