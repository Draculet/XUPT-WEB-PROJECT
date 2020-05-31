#ifndef ___IOPER_H___
#define ___IOPER_H___

#include <vector>
using namespace std;
/* 每个Server一个Controler,使用unique_ptr管理 */
template<typename T>
class Oper
{
    public:
    virtual void insert(T &obj) = 0;
    virtual void del(int id) = 0;
    virtual void modify(T &obj) = 0;
    vector<T> select(string cond) = 0;
};

#endif