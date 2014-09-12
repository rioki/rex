// Copyright 2011 Sean Farrell

#ifndef _COMPOSE_H_
#define _COMPOSE_H_

#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

namespace impl
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

    inline
    int char_to_int(char c)
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

    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5, typename T6, typename T7, typename T8, typename T9>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                                   T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9)
    {
        std::vector<std::string> args(10);
        args[0] = to_string(arg0);
        args[1] = to_string(arg1);
        args[2] = to_string(arg2);
        args[3] = to_string(arg3);
        args[4] = to_string(arg4);
        args[5] = to_string(arg5);
        args[6] = to_string(arg6);
        args[7] = to_string(arg7);
        args[8] = to_string(arg8);
        args[9] = to_string(arg9);

        std::string result;

        for (unsigned int i = 0; i < format.size(); i++)
        {
            if (format[i] == '%')
            {
                if (i + 1 < format.size())
                {
                    int idx = char_to_int(format[i + 1]);
                    result += args.at(idx);
                    i++;
                }
                else
                {
                    throw std::logic_error("% at end of string.");
                }
            }
            else
            {
                result.push_back(format[i]);
            }
        }

        return result;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5, typename T6, typename T7, typename T8>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                                   T5 arg5, T6 arg6, T7 arg7, T8 arg8)
    {
        return compose(format, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, "");
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5, typename T6, typename T7>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                                   T5 arg5, T6 arg6, T7 arg7)
    {
        return compose(format, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, "", "");
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5, typename T6>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                                   T5 arg5, T6 arg6)
    {
        return compose(format, arg0, arg1, arg2, arg3, arg4, arg5, arg6, "", "", "");
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                                   T5 arg5)
    {
        return compose(format, arg0, arg1, arg2, arg3, arg4, arg5, "", "", "", "");
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
    {
        return compose(format, arg0, arg1, arg2, arg3, arg4, "", "", "", "", "");
    }

    template <typename T0, typename T1, typename T2, typename T3>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2, T3 arg3)
    {
        return compose(format, arg0, arg1, arg2, arg3, "", "", "", "", "", "");
    }

    template <typename T0, typename T1, typename T2>
    std::string compose(const std::string& format, T0 arg0, T1 arg1, T2 arg2)
    {
        return compose(format, arg0, arg1, arg2, "", "", "", "", "", "", "");
    }

    template <typename T0, typename T1>
    std::string compose(const std::string& format, T0 arg0, T1 arg1)
    {
        return compose(format, arg0, arg1, "", "", "", "", "", "", "", "");
    }

    template <typename T0>
    std::string compose(const std::string& format, T0 arg0)
    {
        return compose(format, arg0, "", "", "", "", "", "", "", "", "");
    }

    inline
    std::string compose(const std::string& format)
    {
        return compose(format, "", "", "", "", "", "", "", "", "", "");
    }
}

using impl::to_string;
using impl::from_string;
using impl::compose;

#endif
