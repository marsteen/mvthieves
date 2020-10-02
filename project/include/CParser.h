class CParser
{
    public:

        char** SplitLine(char* line, char t, int* AnzahlParams);
        void DeleteSplitLine(char** sline);
};
