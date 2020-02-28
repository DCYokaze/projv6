#ifndef _STDPATCH_H_
#define _STDPATCH_H_
#include <string>
#include <sstream>

namespace patch
{
template <typename T>
std::string to_string(const T &n)
{
    std::ostringstream stm;
    stm << n;
    return stm.str();
}
} // namespace patch
/*
class commonLib{
private:
public:;
//*/
#endif