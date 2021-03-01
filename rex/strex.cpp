//
// String Extention
// 
// Copyright (c) 2014 Sean Farrell <sean.farrell@rioki.org>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 

#include "strex.h"

#include <regex>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

namespace strex
{
    std::vector<std::string> explode(const std::string& str, const std::string& delimiter)
    {
        std::vector<std::string> gibs;
        size_t start = 0;
        size_t end = 0;

        while ((start != std::string::npos) && (start < str.size()))
        {
            end = str.find(delimiter, start);

            std::string gib;
            if (end == std::string::npos)
            {
                gib = str.substr(start);
                start = std::string::npos;
            }
            else
            {
                gib = str.substr(start, end - start);
                start = end + delimiter.size();
            }
            gibs.push_back(gib);
        }

        return gibs;
    }

    std::vector<std::string> tokenize(const std::string& str, const std::string& delimiter)
    {
        // TOOD string literals
        std::vector<std::string> gibs = explode(str, delimiter);

        std::vector<std::string> result;
        for (auto gib : gibs)
        {
            if (!gib.empty())
            {
                result.push_back(gib);
            }
        }

        return result;
    }

    std::regex invalid_chars("[^a-zA-Z0-9\\-]");
    std::regex multiple_spaces("\\s+");
    std::regex spaces("\\s");

    std::string slug(const std::string& str)
    {
        std::string t1 = std::regex_replace(str, invalid_chars, "");
        std::string t2 = std::regex_replace(t1, multiple_spaces, " ");
        std::string t3 = t2.substr(0, 45);
        return std::regex_replace(t3, spaces, "-");
    }

#ifdef _WIN32    
    std::wstring widen(const char* value)
    {
        return widen(std::string(value));
    }

    std::wstring widen(const std::string& value)
    {
        if (value.empty())
        {
            return std::wstring();
        }
        
        std::vector<wchar_t> buff(value.size() + 126);
        int r = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.c_str(), value.size(), &buff[0], buff.size());
        if (r == 0)
        {
            throw std::logic_error("widen failed");        
        }
        else
        {
            return std::wstring(&buff[0]);
        }    
    }

    std::string narrow(const wchar_t* value)
    {
        return narrow(std::wstring(value));
    }

    std::string narrow(const std::wstring& value)
    {
        if (value.empty())
        {
            return std::string();
        }
        
        std::vector<char> buff(value.size() * 2);
        int r = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), value.size(), &buff[0], buff.size(), NULL, NULL);
        if (r == 0)
        {
            throw std::logic_error("narrow failed");        
        }
        else
        {
            return std::string(&buff[0]);
        }
    }
#endif 

    std::string rtrim(const std::string& str, const std::string& chars)
    {
        size_t p = str.find_last_of(chars);
        if (p != std::string::npos)
        {
            return str.substr(0, p);
        }
        else
        {
            return str;
        }
    }

    std::string ltrim(const std::string& str, const std::string& chars)
    {
        size_t p = str.find_first_of(chars);
        if (p != std::string::npos)
        {
            return str.substr(p);
        }
        else
        {
            return str;
        }
    }

    std::string trim(const std::string& str, const std::string& chars)
    {
        return rtrim(ltrim(str, chars), chars);
    }

    std::string tolower(const std::string& str, const std::locale& loc)
    {
        std::string result(str.size(), '\0');

        std::transform(str.begin(), str.end(), result.begin(), [&] (char c) {
            return std::tolower(c, loc); 
        });

        return result; 
    }

    std::string toupper(const std::string& str, const std::locale& loc)
    {
        std::string result(str.size(), '\0');

        std::transform(str.begin(), str.end(), result.begin(), [&] (char c) {
            return std::toupper(c, loc); 
        });

        return result; 
    }
}

