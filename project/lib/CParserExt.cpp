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

#include <fstream.h>
#include <windows.h>
#include <CList.h>
#include <CList.hpp>
#include <CFileIO.h>
#include <CParser.h>
#include <CParserExt.h>
#include <CDebug.h>

extern CDebug Debug2;

// ---------------------------------------------------------------------------
//
// METHODE       : SplitCommandLine
//
// KLASSE        : CParserExt
//
// RETURN        : char** - Zeiger auf Parameterliste
//
// PARAMETER     : char* CmdLine - Zeiger auf String mit Commandozeile
//
// BESCHREIBUNG  : Zerlegen der Kommandozeile in eine String-Liste
//
// HISTORY       :
//
// 28.11.2001 erstellt
//
// ---------------------------------------------------------------------------

char** CParserExt::SplitCommandLine(char* CmdLine, bool ParseBackslash)
{
    int AnzahlParams = 0;

    if (strlen(CmdLine) == 0)
    {
        return NULL;
    }

    SetLine(CmdLine);

    while (strlen(GetStringParam(ParseBackslash)) > 0)
    {
        AnzahlParams++;
    }

    //Debug2 << "AnzahlParams=" << AnzahlParams << DBLF;

    SetLine(CmdLine);

    char** CmdLineParams = new char*[AnzahlParams+1];

    for (int i = 0; i < AnzahlParams; i++)
    {
        CmdLineParams[i] = CopyString(GetStringParam(ParseBackslash));
    }
    CmdLineParams[AnzahlParams] = NULL;

    return CmdLineParams;
}


// ---------------------------------------------------------------------------
//
// METHODE       : DeleteStringList
//
// KLASSE        : CParserExt
//
// RETURN        :
//
// PARAMETER     : char** StringList - Zeiger auf String-Liste
//
// BESCHREIBUNG  : Löschen einer String-Liste aus dem Speicher
//
// HISTORY       :
//
// 28.11.2001 erstellt
//
// ---------------------------------------------------------------------------

void CParserExt::DeleteStringList(char** StringList)
{
    for (char** cs = StringList; *cs != NULL; cs++)
    {
        delete *cs;
    }
    delete StringList;
}


// ---------------------------------------------------------------------------
//
// METHODE       : ResolveWildCards
//
// KLASSE        : CParserExt
//
// RETURN        : char** - Zeiger auf Dateiliste (endet mit NULL)
//
// PARAMETER     : char* WildCardString - String mit Wildcards (*, ?)
//
// BESCHREIBUNG  : Erstellen einer Datei-Liste aus einem Wildcard-String
//
// HISTORY       :
//
// 28.11.2001 erstellt
//
// ---------------------------------------------------------------------------

struct SFileList
{
    char*		Filename;
    SFileList*	next;
};

char** CParserExt::ResolveWildCards(char* WildCardString, int* nFiles)
{
    WIN32_FIND_DATA FindData;
    int AnzahlFiles = 0;

    HANDLE fhand = FindFirstFile(WildCardString, &FindData);

    if (fhand != INVALID_HANDLE_VALUE)
    {
        SFileList* root = new SFileList;
        SFileList* last = root;

        do
        {
//			cout << "fname=" << FindData.cFileName << endl;

            last->Filename = new char[strlen(FindData.cFileName) + 1];
            strcpy(last->Filename, FindData.cFileName);

            last->next = new SFileList;
            last = last->next;
            last->next = NULL;

            AnzahlFiles++;
        }while (FindNextFile(fhand, &FindData));

        char** FileList = new char*[AnzahlFiles + 1];

        SFileList* dnext;
        int i = 0;
        for (SFileList* dr = root; dr != NULL; dr = dnext)
        {
            FileList[i++] = dr->Filename;
            dnext = dr->next;
            delete dr;
        }
        FileList[AnzahlFiles] = NULL;

        if (nFiles != NULL)
        {
            *nFiles = AnzahlFiles;
        }
        return FileList;
    }

    char** FileList = new char*[1];

    FileList[0] = NULL;

    if (nFiles != NULL)
    {
        *nFiles = AnzahlFiles;
    }

    return FileList;
}


// ---------------------------------------------------------------------------
//
// METHODE       : ListSize
//
// KLASSE        : CParserExt
//
// RETURN        : Anzahl der Elemente der durch NULL begrenzten Zeigerliste
//
//
// ---------------------------------------------------------------------------

int CParserExt::ListSize(char** list)
{
    int Anzahl = 0;

    for (int i = 0; list[i] != NULL; i++)
    {
        Anzahl += 1;
    }
    return Anzahl;
}


// ---------------------------------------------------------------------------
//
// METHODE       : MergeList
//
// KLASSE        : CParserExt
//
// RETURN        :
//
//
// ---------------------------------------------------------------------------

char** CParserExt::MergeList(char** list1, char** list2)
{
    char** NewList = new char*[ListSize(list1) + ListSize(list2) + 1];

    int i;

    for (i = 0; list1[i] != NULL; i++)
    {
        NewList[i] = list1[i];
    }
    for (int j = 0; list2[j] != NULL; j++)
    {
        NewList[i++] = list2[j];
    }
    delete[] list1;
    delete[] list2;

    NewList[i] = NULL;

    return NewList;
}


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

char** CParserExt::SplitLine(char* line, char t, int* AnzahlParams)
{
    char** ParamList = NULL;

    if (line == NULL)
    {
        line = LineBuffer;
    }
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
// KLASSE        : CParserExt
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

void CParserExt::DeleteSplitLine(char** sline)
{
    delete sline[0];
    delete sline;
}


// ---------------------------------------------------------------------------
//
// METHODE       : SortFileList
//
// KLASSE        : CParserExt
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


static int CmpStrings(const void* v1, const void* v2)
{
    char** s1 = (char**)v1;
    char** s2 = (char**)v2;

    return strcmp(*s1, *s2);
}


static int CmpStringsInvers(const void* v1, const void* v2)
{
    char** s1 = (char**)v1;
    char** s2 = (char**)v2;

    return strcmp(*s2, *s1);
}


// ---------------------------------------------------------------------------
//
// METHODE       : SortFileList
//
// KLASSE        : CParserExt
//
// RETURN        :
//
// PARAMETER     :
//
// BESCHREIBUNG  :
//
// HISTORY       :
//
// 09.04.2002 erstellt
//
// ---------------------------------------------------------------------------

int CParserExt::SortFileList(char** FileList, bool Invers)
{
    int AnzFiles;

    for (AnzFiles = 0; FileList[AnzFiles] != NULL; AnzFiles++)
    {
    }

    if (Invers)
    {
        qsort(FileList, AnzFiles, 4, CmpStringsInvers);
    }
    else
    {
        qsort(FileList, AnzFiles, 4, CmpStrings);
    }
    return AnzFiles;
}


// ---------------------------------------------------------------------------
//
// METHODE       : ReadSplitLine
//
// KLASSE        : CParserExt
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

char** CParserExt::ReadSplitLine()
{
    if (ReadLine())
    {
        char** Args = SplitCommandLine(LineBuffer, false);
        return Args;
    }
    return NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReplaceAllStrings
//
// char* SrcString = String, in dem Ersetzt werden soll
//
//
//---------------------------------------------------------------------------

class CStrpos : public CListElem<CStrpos>
{
    public:

        char* mPos;
};

class CStrposList : public CList<CStrpos>
{
};


static void CopyStringBytes(char* dst, char* src, int num)
{
    int i;

    for (i = 0; (i < num) && (*src != 0); i++)
    {
        *(dst++) = *(src++);
    }
    *dst = 0;
}


char* CParserExt::ReplaceAllStrings(char* SrcString, char* SearchString, char* SubString, int* Count)
{
    CStrposList slist;
    int cnt = 0;
    char* pos;
    char* StartPos = SrcString;
    int LenSearch = strlen(SearchString);
    int LenSub = strlen(SubString);
    int LenNew;
    char* NewString = NULL;

    do
    {
        pos = strstr(StartPos, SearchString);
        if (pos != NULL)
        {
            CStrpos* se = slist.NewListElem();
            se->mPos = pos;
            StartPos = pos + LenSearch;
            cnt++;
        }
    }while (pos != NULL);

    if (Count != NULL)
    {
        *Count = cnt;
    }
    //cout << "Search=" << SearchString << " Sub=" << SubString << " Count=" << cnt << endl;


    if (cnt > 0)
    {
        LenNew = strlen(SrcString) + (cnt * (LenSub - LenSearch)) + 16;

        //cout << "LenNew = " << LenNew << endl;

        NewString = new char[LenNew+1];
        char* NewStringPos = NewString;

        StartPos = SrcString;
        for (CStrpos* se = slist.GetFirst();
            se != NULL;
            se = slist.GetNext())
        {
            int len = se->mPos - StartPos;
            CopyStringBytes(NewStringPos, StartPos, len);
            NewStringPos += len;
            CopyStringBytes(NewStringPos, SubString, LenSub);
            NewStringPos += LenSub;

            StartPos = se->mPos + LenSearch;
        }
        CopyStringBytes(NewStringPos, StartPos, strlen(StartPos));
        slist.ClearList();

        //cout << "strlen(NewString)=" << strlen(NewString) << endl;
        //cout << endl;
    }

    return NewString;
}


//---------------------------------------------------------------------------
//
// Klasse:    CParserExt
// Methode:   ReplaceStringsList
//
//
//---------------------------------------------------------------------------

char* CParserExt::ReplaceStringsList(char* SrcString, char** SearchList, int* cnt)
{
    //cout << "CParserExt::ReplaceStringsList" << endl;

    char* NewStr = SrcString;

    *cnt = 0;
    for (int i = 0; SearchList[i] != NULL; i += 2)
    {
        int LastCount;
        char* NewString2 = ReplaceAllStrings(NewStr, SearchList[i], SearchList[i+1], &LastCount);

        *cnt += LastCount;

        if (NewString2 != NULL)
        {
            if (NewStr != SrcString)
            {
                delete[] NewStr;
            }
            NewStr = NewString2;
        }
    }
    //cout << "CParserExt::ReplaceStringsList OK cnt=" << *cnt << endl;
    return NewStr;
}
