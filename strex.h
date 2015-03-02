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

#ifndef _STREX_H_
#define _STREX_H_

#include <string>
#include <sstream>
#include <vector>

namespace strex
{
    template <typename T>
    std::string to_string(T value)
    {
        std::stringstream buff;
        buff << value;
        return buff.str();
    }

    template <>
    inline std::string to_string(bool value)
    {
        return value ? "true" : "false";
    }

    template <typename T>
    T from_string(const std::string& str)
    {
        std::stringstream buff(str);
        T value;
        buff >> value;
        return value;
    }

    template <>
    inline bool from_string(const std::string& str)
    {
        if (str == "true")
            return true;
        if (str == "false")
            return false;
        throw std::logic_error("No a boolean value");
    }

    std::vector<std::string> explode(const std::string& str, const std::string& delimiter);

    std::string slug(const std::string& str);    

#ifdef _WIN32
    std::wstring widen(const char* value);
    std::wstring widen(const std::string& value);
    
    std::string narrow(const wchar_t* value);
    std::string narrow(const std::wstring& value);
#endif
}

#endif

