//
// Rioki's Option Parser
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

#include "Options.h"

#include <cassert>
#include <stdexcept>
#include <regex>

Options::Options(int argc, char* argv[]) 
{
    if (argc != 0)
    {
        command = argv[0];
    }

    std::regex option("--?[a-zA-Z0-9]+");
    std::regex long_option("--([a-zA-Z0-9]+)");
    std::regex short_option("-([a-zA-Z0-9]+)");
    std::smatch match;
        
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (std::regex_match(arg, match, long_option))
        {
            assert(match.size() == 2);
            std::string a = match[1].str();

            std::string value;
            if (i+1 < argc && std::regex_match(argv[i+1], option) == false)
            {
                value = argv[i+1];
                i++;
            }
                
            options[a] = value;
        }
        else if (std::regex_match(arg, match, short_option)) 
        {
            assert(match.size() == 2);
            std::string aaa = match[1].str();

            for (size_t j = 0; j < aaa.size(); j++)
            {
                std::string a = aaa.substr(j, 1);

                std::string value;
                if (j+1 == aaa.size()) // only the last short arg may have an option
                {
                    if (i+1 < argc && std::regex_match(argv[i+1], option) == false)
                    {
                        value = argv[i+1];
                        i++;
                    }
                }
                options[a] = value;
            }
        }
        else
        {
            files.push_back(arg);
        }
    }
}

Options::~Options() {}

const std::string& Options::get_command() const
{
    return command;
}

size_t Options::get_file_count() const
{
    return files.size();
}

const std::string& Options::get_file(size_t i) const
{
    return files.at(i);
}

bool Options::has_option(const std::string& id) const
{
    return options.find(id) != options.end();
}

const std::string& Options::get_option(const std::string& id) const
{
    auto i = options.find(id);
    if (i != options.end())
    {
        return i->second;
    }
    else
    {
        throw std::logic_error("invalid option id");
    }
}

