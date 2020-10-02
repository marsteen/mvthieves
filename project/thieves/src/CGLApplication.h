#ifndef CGLAPPLICATION_H
#define CGLAPPLICATION_H

#include "CSdlApp.h"
#include "CVectorShape.h"
#include "CVectorUnit.h"
#include "CEnemyUnit.h"
#include "CBaitUnit.h"
#include "CVectorShot.h"
#include "CExplosion.h"
#include "SGameContext.h"
#include "CPlayerUnit.h"



class CSdlThiefs : public CSdlApp
{
    public:

        CGLApplication();

        const char* Name()    { return "thiefs"; }
        const char* Version() { return "1.0"; }

        void Draw3DObjects();
        void Draw2DObjects();
        void ManageInterface(CGL_Mouse* Mouse);
        void LeftMouseButtonDown();
        void RightMouseButtonDown();
        void LeftMouseButtonUp();
        void RightMouseButtonUp();
        void MouseWheel(int d);


        void MouseMotion(int x, int y);
        void MouseMotionLeft(int x, int y);
        void MouseMotionRight(int x, int y);

        void KeyboardAction(unsigned char key);
        void SpecialkeysAction(int key);
        void SetResolution(int w, int h);

        bool mFullscreen;

    protected:

        void InitResources();
        void Zoom(float f);
        void LoadEnvTexture(bool Anaglyph);


        COpenGL* mOpenGL;
        CG3DGlobals* mResGlobals;
        float mScale;
        const char* mActInfobox;
        bool mInitFlag;
        bool mShowInterface;


        /*******************************************
         *
         * GAME
         *
         ********************************************/

        void InitEnemies();
        void DrawEnemies();
        void TestInside();
        void ParseSvg(string& SVGstr, CPolylineObject* poly);
        void ReadShapes(const char* Filename);
        void CalcPolyCenter(CPolylineObject* Poly);
        CVectorShape* FindShape(const char* Shapename);
        bool ShotHit(CVectorUnit* shot, vector<CVector2<float> >* explo);
        void NewAttackWave();
        void InitBait();

        void RemoveInactive(list<CVectorUnit*>& ul);

        //void RemoveInactivelist(list<CVectorUnit*>& ulist);
        void Shot(int Player);
        void RunUnits();

        void DrawUnitList(list<CVectorUnit*>& ulist, bool Shotable);
        void DrawUnitPoints(list<CVectorUnit*>& ulist);

        SGameContext mGameContext;
        CPlayerUnit mPlayer1;
        CPlayerUnit mPlayer2;
        list<CVectorUnit*> mUnitList;
        list<CVectorUnit*> mBaitList;
        list<CVectorUnit*> mShotList;
        list<CVectorUnit*> mExploList;
        list<CVectorShape*> mShapeList;
        bool mInitEnemies;
};

#endif
