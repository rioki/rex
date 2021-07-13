// Copyright 2011-2020 Sean Farrell
//
// This program is free software. It comes without any warranty, to
// the extent permitted by applicable law. You can redistribute it
// and/or modify it under the terms of the Do What The Fuck You Want
// To Public License, Version 2, as published by Sam Hocevar. See
// http://www.wtfpl.net/ for more details.
//

#ifndef _COMPOSE_H_
#define _COMPOSE_H_

#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

namespace impl
{
    template <typename C, typename T>
    std::basic_string<C> to_string(T value)
    {
        std::basic_stringstream<C> buff;
        buff << value;
        return buff.str();
    }

    template <>
    inline std::string to_string<char, bool>(bool value)
    {
        return value ? "true" : "false";
    }

    template <>
    inline std::wstring to_string<wchar_t, bool>(bool value)
    {
        return value ? L"true" : L"false";
    }

    template <>
    inline std::string to_string(const std::string& value)
    {
        return value;
    }

    inline int char_to_int(char c)
    {
        switch (c)
        {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        default:
            throw std::logic_error("not a number");
        }
    }

    inline int char_to_int(wchar_t c)
    {
        switch (c)
        {
        case L'0':
            return 0;
        case L'1':
            return 1;
        case L'2':
            return 2;
        case L'3':
            return 3;
        case L'4':
            return 4;
        case L'5':
            return 5;
        case L'6':
            return 6;
        case L'7':
            return 7;
        case L'8':
            return 8;
        case L'9':
            return 9;
        default:
            throw std::logic_error("not a number");
        }
    }

    template <typename C, typename Arg>
    void unpack(std::vector<std::basic_string<C>>& args, Arg value) noexcept
    {
        args.push_back(to_string<C>(value));
    }

    template <typename C, typename Arg, typename ... Args>
    void unpack(std::vector<std::basic_string<C>>& args, Arg value, Args ... remainder) noexcept
    {
        args.push_back(to_string<C>(value));
        unpack(args, remainder...);
    }

    template <typename C, typename ... Args>
    std::basic_string<C> basic_compose(const std::basic_string<C>& format, Args ... args) noexcept
    {
        auto sargs = std::vector<std::basic_string<C>>{};
        unpack(sargs, args...);

        auto result = std::basic_string<C>{};

        for (auto i = 0u; i < format.size(); i++)
        {
            if (format[i] == '%')
            {
                if (i + 1 < format.size())
                {
                    auto idx = char_to_int(format[i + 1]);
                    result += sargs.at(idx);
                    i++;
                }
            }
            else
            {
                result.push_back(format[i]);
            }
        }

        return result;
    }

    template <typename ... Args>
    std::string compose(const std::string& format, Args ... args) noexcept
    {
        return basic_compose<char, Args...>(format, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    std::wstring wcompose(const std::wstring& format, Args ... args) noexcept
    {
        return basic_compose<wchar_t, Args...>(format, std::forward<Args>(args)...);
    }
}

using impl::compose;
using impl::wcompose;

#endif
