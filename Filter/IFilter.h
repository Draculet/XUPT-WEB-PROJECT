#include <map>
#include <vector>
#include <string>
#include <memory>
#include "../WebService/headerParse/requestParser.h"
using namespace std;
using namespace http;

class IFilter
{
    public:
    IFilter(string type){}
    IFilter(){}

    virtual bool doFilter(Request &req) = 0;
    virtual bool setFunc() = 0;
    virtual map<string, function<void()> > &getFilter() = 0;
    virtual bool getFunc(string key) = 0;
    private:
    map<string, function<void()> > filterFunc_;
};