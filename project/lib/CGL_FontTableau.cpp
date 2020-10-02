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

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <CVector3T.h>
#include <CGL_FontTableau.h>
#include <GSystemFunctions.h>

//---------------------------------------------------------------------------
//
// Klasse:    CGL_FontTableau
// Methode:		Load
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CGL_FontTableau::Load(const char* TabFile, const char* AlphaFile, const char* RectFile, int wh)
{
    /*
     * cout << "Load Font" << endl;
     * cout << "  File =" << TabFile << endl;
     * cout << "  Alpha=" << AlphaFile << endl;
     * cout << "  Rect =" << RectFile << endl;
     */

    if (LoadTextureTga2D(TabFile, AlphaFile, false, false))
    {
        ifstream fin(RectFile, ios::binary);

        mWindowHeight = wh;

        if (fin.good())
        {
            fin.read((char*)mCharRect, sizeof(CRectT<int>) * 256);
            fin.close();

            for (int i = 0; i < 256; i++)
            {
                CRectT<int>* crect = mCharRect + i;
                CRectT<float>* trect = mTexRect + i;

                if (GsysBigEndian())
                {
                    GsysSwapDword(&crect->left);
                    GsysSwapDword(&crect->right);
                    GsysSwapDword(&crect->top);
                    GsysSwapDword(&crect->bottom);
                }


                trect->left = (float)crect->left   / mTexWidth;
                trect->right = (float)crect->right  / mTexWidth;
                trect->bottom = (float)crect->top    / mTexHeight;
                trect->top = (float)crect->bottom / mTexHeight;

                /*
                 *      if ((i >= 'A') && (i <= 'Z'))
                 *      {
                 *          cout << (char) i
                 *               << " " << trect->left
                 *               << " " << trect->top
                 *               << " " << trect->right
                 *               << " " << trect->bottom << endl;
                 *      }
                 */

                /*
                 *      trect->left  = 0;
                 *      trect->right = 1;
                 *      trect->top   = 1;
                 *      trect->bottom= 0;
                 */
            }
            //cout << "Font loading OK" << endl;
            return true;
        }
        else
        {
            cout << "***** file error: " << RectFile << endl;
            exit(0);
        }
    }
    cout << "***** Font loading failed" << endl;
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_FontTableau
// Methode:		Load
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CGL_FontTableau::Load(const char* Folder, const char* FileBase, int wh)
{
    string TabFile = string(Folder) + "/"   + FileBase + ".tga";
    string RectFile = string(Folder) + "/R_" + FileBase + string(".bin");
    string AlphaFile = string(Folder) + "/A_" + FileBase + string(".tga");

    return Load(TabFile.c_str(), AlphaFile.c_str(), RectFile.c_str(), wh);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_FontTableau
// Methode:		Width
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CGL_FontTableau::FontWidth(const char* Text)
{
    int w = 0;

    for (const char* c = Text; *c != 0; c++)
    {
        unsigned char index = (unsigned char)*c;
        CRectT<int>* crc = mCharRect + index;
        w += crc->Width();
    }
    return w;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_FontTableau
// Methode:		Height
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CGL_FontTableau::FontHeight()
{
    return mCharRect[' '].Height();
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_FontTableau
// Methode:		DrawString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CGL_FontTableau::DrawString(const char* Text, int x, int y)
{
    //GLfloat WhiteMaterial[] = { 1.0, 1.0, 1.0, 1.0 };

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glMaterialfv(GL_FRONT, GL_DIFFUSE, WhiteMaterial);

    y = mWindowHeight - y - mCharRect[' '].Height();
    DrawStringRaw(Text, x, y);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_FontTableau
// Methode:		DrawString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CGL_FontTableau::DrawStringRaw(const char* Text, float x, float y)
{
    for (const char* c = Text; *c != 0; c++)
    {
        CVector3<float> Vertex;
        //unsigned char  index = (unsigned char) *c;
        unsigned char index = (unsigned char)*c;


        CRectT<float>* trc = mTexRect + index;
        CRectT<int>* crc = mCharRect + index;

        glBindTexture(GL_TEXTURE_2D, mTexHandle);
        glBegin(GL_TRIANGLE_STRIP);

        Vertex.x = x;
        Vertex.y = y;
        Vertex.z = 0;
        glTexCoord2f(trc->left, trc->top);
        glVertex3fv(&Vertex.x);

        Vertex.x = x + crc->Width();
        Vertex.y = y;
        Vertex.z = 0;
        glTexCoord2f(trc->right, trc->top);
        glVertex3fv(&Vertex.x);

        Vertex.x = x;
        Vertex.y = y + crc->Height();
        Vertex.z = 0;
        glTexCoord2f(trc->left, trc->bottom);
        glVertex3fv(&Vertex.x);

        Vertex.x = x + crc->Width();
        Vertex.y = y + crc->Height();
        Vertex.z = 0;
        glTexCoord2f(trc->right, trc->bottom);
        glVertex3fv(&Vertex.x);

        glEnd();

        x += crc->Width();
    }
}
