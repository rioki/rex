
#ifndef _FS_H_
#define _FS_H_

#include <string>

namespace fs
{
    std::string read(const std::string& file);

    bool exists(const std::string& file);

    void unlink(const std::string& file);
}

#endif
