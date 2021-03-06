#ifndef CSTRINGTOOL_H
#define CSTRINGTOOL_H

#include <string>
#include <vector>

class CStringTool
{
    public:

        static int SplitString(const std::string& InputString, std::vector<std::string>* ResultVector, char SplitChar);
        void TrimString(std::string* InputString);

        template<typename T>
        static T StringTo(const std::string& str);
};

#include <CStringTool.hpp>

#endif
