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

using namespace std;

#include <fstream>
#include <string>
#include <CParser.h>

// ---------------------------------------------------------------------------
//
// METHODE       : SplitLine
//
// KLASSE        : CParserExt
//
// RETURN        : char** - Zeiger auf String-List
//
// PARAMETER     : char* line - Zeile mit Strings
//                 char  t    - Trenn-Charakter
//                 int*  AnzahlParams - Anzahl der Strings (Rückgabe)
//
// BESCHREIBUNG  :
//
// HISTORY       :
//
// 09.04.2002 erstellt
//
// ---------------------------------------------------------------------------

char** CParser::SplitLine(char* line, char t, int* AnzahlParams)
{
    char** ParamList = NULL;

    if (strlen(line) > 0)
    {
        *AnzahlParams = 1;

        for (char* l = line; *l != 0; l++)
        {
            if (*l == t)
            {
                (*AnzahlParams)++;
            }
        }

        char* ParamStr = new char[strlen(line) + 1];

        ParamList = new char*[*AnzahlParams+1];

        strcpy(ParamStr, line);

        ParamList[0] = ParamStr;

        int i = 0;

        for (char* l = ParamStr; *l != 0; l++)
        {
            if (*l == t)
            {
                *l = 0;
                ParamList[++i] = l + 1;
            }
        }
        ParamList[*AnzahlParams] = NULL;
    }
    else
    {
        *AnzahlParams = 0;
    }
    return ParamList;
}


// ---------------------------------------------------------------------------
//
// METHODE       : DeleteSplitLine
//
// KLASSE        : CParser
//
// RETURN        :
//
// PARAMETER     : char** sline - Zeile mit String-Tabelle
//
// BESCHREIBUNG  :
//
// HISTORY       :
//
// 09.04.2002 erstellt
//
// ---------------------------------------------------------------------------

void CParser::DeleteSplitLine(char** sline)
{
    delete sline[0];
    delete sline;
}
