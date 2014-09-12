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

#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

namespace strex
{
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
}

