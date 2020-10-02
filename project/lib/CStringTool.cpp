//------------------------------------------------------------------------------
//
// PROJECT : 3D Globus
//
// FILE    : CStringTool.cpp
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
// Inhalt: Implementation von CStringTool
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 02.11.2007 erstellt - Martin Steen
//
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include "CStringTool.h"

using namespace std;



//---------------------------------------------------------------------------
//
// Klasse:    CStringTool
// Methode:   SplitString
//
//
//---------------------------------------------------------------------------

int CStringTool::SplitString(const string& InputString, vector<string>* ResultVector, char SplitChar)
{
    int OldIndex = 0;
    int NewIndex;

    do
    {
        NewIndex = InputString.find(SplitChar, OldIndex);

        if (NewIndex != string::npos)
        {
            ResultVector->push_back(InputString.substr(OldIndex, NewIndex - OldIndex));

            //cout << "String=" <<  *(--rc.end()) << endl;
            OldIndex = NewIndex + 1;
        }
        else
        {
            ResultVector->push_back(InputString.substr(OldIndex, InputString.size() - OldIndex));
            //cout << "LastString=" <<  *(--rc.end()) << endl;
        }
    }while (NewIndex != string::npos);

    return ResultVector->size();
}


//---------------------------------------------------------------------------
//
// Klasse:    CStringTool
// Methode:   TrimString
//
// Entfernen von " im String
//
//---------------------------------------------------------------------------

void CStringTool::TrimString(string* InputString)
{
    string outstring = *InputString;

    if ((outstring[0] == '"') &&
        (outstring[outstring.size()-1] == '"'))
    {
        outstring = InputString->substr(1, InputString->size()-2);
        *InputString = outstring;
    }
}
