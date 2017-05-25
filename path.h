
#ifndef _PATH_H_
#define _PATH_H_

#include <string>
#include <vector>

namespace path
{
    
    std::vector<std::string> explode(const std::string& path);

    
    std::string join(std::vector<std::string> bits);

    
    std::string normalize(const std::string& path);

    /** 
     * Convert path to using only forward slashes /
     * 
     * This makes the path independent useful on all oses.
     **/
    std::string independent(const std::string& path);
    
    std::string canonicalize(const std::string& path);

    
    std::string dirname(const std::string& file);

    
    std::string basename(const std::string& file);

    
    std::string join(const std::string& a, const std::string& b);  
    
    
    std::string diff(const std::string& start, const std::string& target);

    std::string ext(const std::string& file);

    std::string corename(const std::string& file);
    
    std::string tempdir();

    std::string confdir(const std::string& name);
}

#endif
