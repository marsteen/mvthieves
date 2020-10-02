//------------------------------------------------------------------------------
//
// PROJECT : 3D Globus
//
// FILE    : GlobalDebug.cpp
//
// VERSION : 1.0
//
// AUTOR   : Martin Steen
//           Imagon GmbH
//
//
//
//------------------------------------------------------------------------------
//
// Inhalt:
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 10.09.2009 erstellt - Martin Steen
//
//------------------------------------------------------------------------------

#define GLOBAL_DEBUG_LEVEL    0
#define DEBUG_LEVEL_SELECT

//#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GlobalDebug.h>
//#include "CGlobalDefines.h"

#define DEBUG_FILE0		"debug0.txt"
#define DEBUG_FILE1		"debug1.txt"

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

static char DebugFile0[256];
static char DebugFile1[256];
static EInitStage sInitStage;

void GlobalSetInitStage(EInitStage s)
{
    sInitStage = s;
}


EInitStage GlobalGetInitStage()
{
    return sInitStage;
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalShowError
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalShowError(const char* ErrMessage)
{
#if 0
    EInitStage OldInitStage = GlobalGetInitStage();

    GlobalSetInitStage(EINITSTAGE_HALT);
    GlobalDebug("***** GlobalShowError: ", ErrMessage);
    QString MsgCaption(APP_NAME);
    QString MsgText(ErrMessage);

    QMessageBox MsgBox(QMessageBox::Warning, MsgCaption, MsgText, QMessageBox::Ok);
    MsgBox.exec();
    GlobalSetInitStage(OldInitStage);
    //exit(0);
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalShowError
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalShowError(const stringstream& mstr)
{
    GlobalShowError(mstr.str().c_str());
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebugInit
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebugInit()
{
    strcpy(DebugFile0, DEBUG_FILE0);
    strcpy(DebugFile1, DEBUG_FILE1);
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebugEmpty
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebugEmpty()
{
    ofstream debug;

    debug.open(DebugFile0, ios::app);
    debug.close();
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebugEmpty
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------


inline static bool LevelValid(int DebugLevel)
{
#ifdef GLOBAL_DEBUG_LEVEL
#ifndef DEBUG_LEVEL_SELECT
    return (DebugLevel >= GLOBAL_DEBUG_LEVEL) || (DebugLevel == 0);
#else
    return DebugLevel == GLOBAL_DEBUG_LEVEL;
#endif
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebug
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebug(const char* txt, int DebugLevel)
{
#ifdef GLOBAL_DEBUG_LEVEL
    if (LevelValid(DebugLevel))
    {
        ofstream debug;
        debug.open(DebugFile0, ios::app);
        debug << txt << endl;
        debug.close();
    }
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebug
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebug(const char* txt, const char* txt2, int DebugLevel)
{
#ifdef GLOBAL_DEBUG_LEVEL
    if (LevelValid(DebugLevel))
    {
        ofstream debug;
        debug.open(DebugFile0, ios::app);
        debug << txt << txt2 << endl;
        debug.close();
    }
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebug
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebug1(const char* txt, int DebugLevel)
{
#ifdef GLOBAL_DEBUG_LEVEL
    if (LevelValid(DebugLevel))
    {
        ofstream debug;
        debug.open(DebugFile1, ios::app);
        debug << txt << endl;
        debug.close();
    }
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebug
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebug1(const char* txt, const char* txt2, int DebugLevel)
{
#ifdef GLOBAL_DEBUG_LEVEL
    if (LevelValid(DebugLevel))
    {
        ofstream debug;
        debug.open(DebugFile1, ios::app);
        debug << txt << txt2 << endl;
        debug.close();
    }
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebug
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebug(const stringstream& mstr, int DebugLevel)
{
#ifdef GLOBAL_DEBUG_LEVEL
    if (LevelValid(DebugLevel))
    {
        ofstream debug;
        debug.open(DebugFile0, ios::app);
        debug << mstr.str() << endl;
        debug.close();
    }
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebug1
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalDebug1(const stringstream& mstr, int DebugLevel)
{
#ifdef GLOBAL_DEBUG_LEVEL
    if (LevelValid(DebugLevel))
    {
        ofstream debug;
        debug.open(DebugFile1, ios::app);
        debug << mstr.str() << endl;
        debug.close();
    }
#endif
}


//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:		GlobalDebug1
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void GlobalShowOpenGLError(const char* txt)
{
}
