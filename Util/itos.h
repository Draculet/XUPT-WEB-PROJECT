#ifndef __ITOS__
#define __ITOS__

#include <string>

std::string itos(int num)
{
    char tmp[20] = {0};
    sprintf(tmp, "%d", num);
    return std::string(tmp);
}

#endif