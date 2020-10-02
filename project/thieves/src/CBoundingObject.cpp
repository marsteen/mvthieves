#include <sstream>
#include <iostream>
#include <vector>



#include <GLinclude.h>
#include "CBoundingObject.h"

using namespace std;



//---------------------------------------------------------------------------
//
// Klasse:    CBoundingObject
// Methode:		CreateBounds
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CBoundingObject::CreateBounds(const SShapeParams& sp)
{
    mBounds.clear();

    for (vector<CVector2<float> >::iterator iv = mVerts.begin();
        iv != mVerts.end();
        ++iv)
    {
        CVector2<float> Bound;
        TransformVertex(*iv, Bound, sp);
        mBounds.push_back(Bound);

/*
 *      cout << "vx=" << iv->x << " vy=" << iv->y << endl;
 *      cout << "bx=" << Bound.x << " by=" << Bound.y << endl;
 */
    }
}


//---------------------------------------------------------------------------
//
// Klasse:		CBoundingObject
// Methode:		Print
//
//
//---------------------------------------------------------------------------

std::string CBoundingObject::Print() const
{
    stringstream mstr;

    for (int i = 0; i < mBounds.size(); i++)
    {
        mstr << i << "(x=" << mBounds[i].x << " y=" << mBounds[i].y << ") ";
    }
    return mstr.str();
}


//---------------------------------------------------------------------------
//
// Klasse:		CBoundingObject
// Methode:		Draw
//
//
//---------------------------------------------------------------------------

void CBoundingObject::DrawBounds(const SShapeParams& sp)
{
    //glColor4fv(mColor);

    //glPushMatrix();

    CreateBounds(sp);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < mBounds.size(); i++)
    {
        glVertex2f(mBounds[i].x, mBounds[i].y);
    }
    glEnd();

    //glPopMatrix();
}


//---------------------------------------------------------------------------
//
// Klasse:    CBoundingObject
// Methode:		Inside
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CBoundingObject::Inside(const CBoundingObject& pb) const
{
    int iCount = 0;                   // Zaehler fuer Linienschnitte

    for (vector<CVector2<float> >::const_iterator ib = pb.mBounds.begin();
        ib != pb.mBounds.end();
        ++ib)
    {
        //CVector2f* vb = pb->mVertex + j;  // Vector Array B

        int cCount = 0;

        //cout << "mBounds.size()=" << mBounds.size() << endl;

        for (int i = 0; i < mBounds.size()-1; i++)
        {
            const CVector2<float>& va1 = mBounds[i];
            const CVector2<float>& va2 = mBounds[i+1]; //(i == mBounds.size()-1) ? mBounds[0] : mBounds[i+1];

/*
 *
 *          if (i == mBounds.size()-1)
 *          {
 *              va2 = mBounds[0];
 *          }
 *          else
 *          {
 *              va2 = mBounds[i+1];
 *          }
 */
            //cout << "x1=" << va1.x << " x2=" << va2.x << endl;

            float dx = va2.x - va1.x;

            //cout << "dx=" << dx << endl;

            // Senkrechte Linien nicht ber�cksichtigen

            if (dx != 0)
            {
                float x1, x2; // x1 muss kleiner als x2 sein

                if (va1.x > va2.x)
                {
                    x1 = va2.x;
                    x2 = va1.x;
                }
                else
                {
                    x1 = va1.x;
                    x2 = va2.x;
                }



                // Liegt x vom Vector vb zwischen x1 und x2

                if ((ib->x >= x1) && (ib->x < x2))
                {
                    // wenn ja, dann Liniengleichung berechnen

                    float ml, cl;
                    float dy = va2.y - va1.y;

                    ml = dy / dx; // Steigung

                    cl = va1.y - (ml * va1.x);

                    // x vom Vector vb n Liniengleichung einsetzten:

                    if (((ml * ib->x) + cl) < ib->y)
                    {
                        // wenn Ergebnis (y) kleiner als vb->y, dann Linienschnitt = true

                        cCount++;
                    }
                }
            }
        }

        // Wenn cCount gerade liegt der Punkt im Polygon
        // Debug2 << "  cCount=" << cCount << DBLF;

        if (cCount & 1)
        {
            iCount++;
        }
    }

    // Anzahl der Punkte zur�ckgeben, die im Polygon liegen
    return iCount;
}


//---------------------------------------------------------------------------
//
// Klasse:    CBoundingObject
// Methode:		Inside
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CBoundingObject::Inside(float tx, float ty) const
{
    bool r = false;
    int cCount = 0;

    for (int i = 0; i < mBounds.size()-1; i++)
    {
        const CVector2<float>& va1 = mBounds[i];
        const CVector2<float>& va2 = mBounds[i+1]; //(i == mBounds.size()-1) ? mBounds[0] : mBounds[i+1];

        float dx = va2.x - va1.x;


        // Senkrechte Linien nicht beruecksichtigen

        if (dx != 0)
        {
            float x1, x2; // x1 muss kleiner als x2 sein

            if (va1.x > va2.x)
            {
                x1 = va2.x;
                x2 = va1.x;
            }
            else
            {
                x1 = va1.x;
                x2 = va2.x;
            }

            // Liegt x vom Vector vb zwischen x1 und x2

            if ((tx >= x1) && (tx < x2))
            {
                // wenn ja, dann Liniengleichung berechnen

                float ml, cl;
                float dy = va2.y - va1.y;

                ml = dy / dx; // Steigung

                cl = va1.y - (ml * va1.x);

                // x vom Vector vb n Liniengleichung einsetzten:

                if (((ml * tx) + cl) < ty)
                {
                    // wenn Ergebnis (y) kleiner als vb->y, dann Linienschnitt = true

                    cCount++;
                }
            }
        }

        // Wenn cCount gerade liegt der Punkt im Polygon
    }
    r = (cCount & 1);

    // Anzahl der Punkte zur�ckgeben, die im Polygon liegen
    return r;
}
