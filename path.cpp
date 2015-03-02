
#include "path.h"

#include <algorithm>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#define SEP  '\\'
#define WSEP '/'
#else
#define SEP  '/'
#define WSEP '\\'
#endif

namespace path
{
    std::vector<std::string> explode(const std::string& path)
    {
        std::vector<std::string> bits;
        size_t start = 0;
        size_t end = 0;

        while ((start != std::string::npos) && (start < path.size()))
        {
            end = path.find_first_of("\\/", start);

            std::string bit;
            if (end == std::string::npos)
            {
                bit = path.substr(start);
                start = std::string::npos;
            }
            else
            {
                bit = path.substr(start, end - start);
                start = end + 1;
            }
            bits.push_back(bit);
        }

        return bits;
    }

    std::string join(std::vector<std::string> bits)
    {
        std::string result;

        for (unsigned int i = 0; i < bits.size(); i++)
        {
            result += bits[i];
            if ((i + 1) != bits.size())
            {
                result += SEP;
            }
        }

        return result;
    }

    std::string normalize(const std::string& path)
    {
        std::vector<std::string> bits = explode(path);

        // We need to keep the ../ in the beginning of the path.
        // This flag indicates if we found something that is neither "." nore "..".
        bool first_file = false;

        auto i = bits.begin();
        while (i != bits.end())
        {
            if (*i == ".")
            {
                i = bits.erase(i);
            }
            else if (*i == ".." && first_file)
            {                                
                i--;
                i = bits.erase(i);
                i = bits.erase(i);
            }
            else
            {
                first_file = true;
                i++;
            }
        }

        return join(bits);
    }

    std::string canonicalize(const std::string& path)
    {
        #ifdef _WIN32
        char buffer[MAX_PATH];
        DWORD r = GetFullPathNameA(path.c_str(), MAX_PATH,  buffer, NULL);
        if (r != 0)
        {
            return std::string(buffer);
        }
        else
        {
            throw std::runtime_error("Failed to canocialize path.");
        }
        #else
        #error PORT ME
        #endif
    }

    std::string dirname(const std::string& file)
    {
        unsigned int i = file.find_last_of("\\/");
        if (i == std::string::npos)
        {
            return "";
        }
        else
        {
            return file.substr(0, i);
        }
    }

    std::string basename(const std::string& file)
    {
        unsigned int i = file.find_last_of("\\/");
        if (i == std::string::npos)
        {
            return file;
        }
        else
        {
            return file.substr(i + 1);
        }
    }

    std::string join(const std::string& a, const std::string& b)
    {
        if (a.empty())
        {
            return normalize(b);
        }
        if (b.empty())
        {
            return normalize(a);
        }
    
        std::string ac;
        std::string bc;
    
        if (a[a.size() - 1] == '/' || a[a.size() - 1] == '\\')
        {
            ac = a.substr(0, a.size() - 1);
        }
        else
        {
            ac = a;
        }
    
        if (b[0] == '/' || b[0] == '\\')
        {
            bc = b.substr(1);
        }
        else
        {
            bc = b;
        }
    
        return normalize(ac + SEP + bc);
    }

    std::string diff(const std::string& start, const std::string& target)
    {
        std::string cstart  = canonicalize(start);
        std::string ctarget = canonicalize(target);

        std::vector<std::string> start_bits  = explode(cstart);
        std::vector<std::string> target_bits = explode(ctarget);

        unsigned int i = 0;
        while (i < start_bits.size() && i < target_bits.size() && start_bits[i] == target_bits[i])
        {
            i++;
        }

        std::vector<std::string> result_bits;
        for (unsigned int j = i; j < start_bits.size(); j++)
        {
            result_bits.push_back("..");
        }

        for (unsigned int j = i; j < target_bits.size(); j++)
        {
            result_bits.push_back(target_bits[j]);
        }
        
        return join(result_bits);
    }

    std::string ext(const std::string& file)
    {
        size_t p = file.find_last_of('.');
        if (p == std::string::npos)
        {
            return std::string();
        }
        else
        {
            return file.substr(p + 1);
        }
    }

    std::string tempdir()
    {
    #ifdef _WIN32
        char tmp[MAX_PATH+1];
        DWORD r = GetTempPathA(MAX_PATH+1, tmp);
        if (r != 0)
        {
            return std::string(tmp);
        }
        else
        {
            throw std::runtime_error("Failed to get temp path.");
        }
    #else
    #error PORT ME
    #endif
    }

    std::string confdir(const std::string& name)
    {
    #ifdef _WIN32
        char tmp[MAX_PATH+1];
        BOOL r = SHGetSpecialFolderPathA(NULL, tmp, CSIDL_COMMON_APPDATA, FALSE);
        if (r == FALSE)
        {
            throw std::runtime_error("Failed to get APPDATA folder!");
        }

        return join(tmp, name);
    #else
    // $HOME/.<name>/
    #error PORT ME
    #endif
    }
}
