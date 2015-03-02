
#include "fs.h"

#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else

#endif

#include "strex.h"
#include "compose.h"

namespace fs
{
    std::string read(const std::string& file)
    {
        std::string result;
        
        std::ifstream input(file.c_str(), std::ios::binary);
        if (!input.good())
        {
            std::stringstream msg;
            msg << "Failed to open file " << file << " for reading.";
            throw std::runtime_error(msg.str());
        }
        
        int c = input.get();
        while (c != EOF)
        {
            result.push_back(c);
            c = input.get();
        }
        
        return result;
    }

    bool exists(const std::string& file)
    {
    #ifdef _WIN32
        return PathFileExists(strex::widen(file).c_str()) == TRUE;
    #else
    #error PORT ME
    #endif
    }

    void unlink(const std::string& file)
    {
        #ifdef _WIN32
            BOOL r = DeleteFile(strex::widen(file).c_str());
            if (r == FALSE)
            {
                throw std::runtime_error(compose("Failed to unlink %0.", file));
            }
        #else
        #error PORT ME
        #endif
    }
}