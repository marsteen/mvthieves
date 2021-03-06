#ifndef CGLAPPLICATION_H
#define CGLAPPLICATION_H

#include <string>
#include <vector>
#include <list>

#include <CGraflibPng.h>
#include <CGL_Texture.h>
#include <CGL_Patch2d.h>
#include "CSdlApp.h"
#include "CVectorShape.h"
#include "CVectorUnit.h"
#include "CEnemyUnit.h"
#include "CBaitUnit.h"
#include "CVectorShot.h"
#include "CExplosion.h"
#include "SGameContext.h"
#include "CPlayerUnit.h"
#include "CSdlSound.h"
#include "CDebrisUnit.h"
#include "CSaveScreenGL.h"


enum
{
    SOUND_EXPLO1,
    SOUND_EXPLO2,
    SOUND_SCHUSS,
    SOUND_MUSIC
};

enum EGameStatus
{
    EGAMESTATUS_WAITING,    // Warten dass was passiert
    EGAMESTATUS_GAMEOVER,   // Warten dass was passiert
    EGAMESTATUS_PLAYING     // Spiel laeuft
};

class CSdlThiefs : public CSdlApp
{
    public:

        CSdlThiefs();
        ~CSdlThiefs();

        const char* Name()    { return "thieves"; }
        const char* Version() { return "2.0"; }

        void Draw3DObjects();
        void Draw2DObjects();

        //void ManageInterface(CGL_Mouse* Mouse);
        bool ParseKeys(SDLKey key, bool down);
        void ParseMouseRel(float xrel, float yrel);

        void LeftMouseButtonDown();
        void RightMouseButtonDown();
        void LeftMouseButtonUp();
        void RightMouseButtonUp();
        void MouseWheel(int d);


        void MouseMotion(int x, int y);
        void MouseMotionLeft(int x, int y);
        void MouseMotionRight(int x, int y);

        bool mFullscreen;
        void GameLoop();

    protected:

        void SetResolution(int w, int h);
        void Zoom(float f);
        void LoadEnvTexture(bool Anaglyph);
        void InitGame();
        void FinishGame();


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
        void DrawDebris();
        void TestInside();
        void ParseSvg(std::string& SVGstr, CPolylineObject* poly, int Modifier);
        void ReadShapes(const char* Filename);
        void CalcPolyCenter(CPolylineObject* Poly);
        CVectorShape* FindShape(const char* Shapename);
        bool ShotHit(CVectorUnit* shot, std::vector<CVector2<float> >* explo);
        void EnemyShotHit();
        void NewAttackWave(bool Reset = false);
        void NewDebris(float xpos, float ypos, int Fakt, const float* Color, const CVector2<float>& ddir);
        void InitBait();
        void RunPlayers();
        void TestAvoid();
        void TestEnemyShoot();
        void StartGame();
        void GotoWaitStatus();


        void RemoveInactive(std::list<CVectorUnit*>& ul);
        void ClearList(std::list<CVectorUnit*>& ulist);

        //void RemoveInactivelist(list<CVectorUnit*>& ulist);
        void Shot(int Player);
        void ShotEnemy(CVectorUnit* who);
        void RunUnits();
        bool TestCrash(CVectorUnit* vu);
        bool TestPlayerCrash();
        void DrawCenterPatch(CGL_Patch2d* pat);
        void PlayerExplodes(int p);
        void BonusScore();


        void DrawUnitList(std::list<CVectorUnit*>& ulist, int HitStatus);
        void DrawUnitPoints(std::list<CVectorUnit*>& ulist);
        void DrawScore(int xpos, int Score, const float* Color);

        //void DrawCenterHighScore(const float* Color);
        void DrawCenterScore(int xpos, int Score, const float* Color);
        void DrawLevel();
        int Digits(int k);

        void WriteHighScore();
        void ReadHighScore();

        SGameContext mGameContext;
        EGameStatus mGameStatus;
        CPlayerUnit mPlayer[2];
        int mScore[2];
        int mHighScore;
        int mLevel;
        std::list<CVectorUnit*> mUnitList;
        std::list<CVectorUnit*> mBaitList;
        std::list<CVectorUnit*> mShotList;
        std::list<CVectorUnit*> mEnemyShotList;
        std::list<CVectorUnit*> mExploList;
        std::list<CVectorShape*> mShapeList;
        std::list<CDebrisUnit*> mDebrisList;
        bool mInitEnemies;
        CVectorUnit mZiffern[10];
        CGL_Patch2d mMainTex;                       // Titelbild-Textur
        CGL_Patch2d mGameOverTex;                   // "Game Over"-Textur
        CSdlSound mSdlSound;
        int mGameOverTime;                          // Zeit fuer GameOver-Status
        bool mExploded;
        float mLevelDrawScale;                      // Scalierung der Levelanzeige
        int mLevelDrawWait;                         // Verzoegerung fuer Levelanzeige
        CSaveScreenGL mSaveScreenGL;                // Bildschirm abspeichern
        bool mScreenSaveMode;
};

#endif
