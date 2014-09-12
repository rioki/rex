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

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <map>
#include <vector>
#include <string>

class Options
{
public:
    Options(int argc, char* argv[]);

    ~Options();

    const std::string& get_command() const;

    size_t get_file_count() const;

    const std::string& get_file(size_t i = 0) const;

    bool has_option(const std::string& id) const;

    const std::string& get_option(const std::string& id) const;

private:
        
    std::string command;
    std::vector<std::string> files;
    std::map<std::string, std::string> options;

    Options(const Options&) = delete;
    const Options& operator = (const Options&) = delete;
};

#endif
