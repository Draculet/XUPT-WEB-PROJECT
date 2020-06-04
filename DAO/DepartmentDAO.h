#ifndef ___DEPARTDAO_H___
#define ___DEPARTDAO_H___

#include "../Model/Department.h"
#include "../DBConnPool/DBConnPool.h"
#include <vector>
using namespace std;


class DepartmentDAO // : public Oper<Doctor>
{
    public:
    DepartmentDAO(){}

    static bool insert(Department &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "insert into department values(\'" + d.getDepartment_name() + "\', \'" + d.getDepartment_pos() + "\')";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }

    static bool del(Department &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "delete from department where department_name = \'" + d.getDepartment_name() + "\'";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }
    static bool modify(Department &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "update department set department_pos=\'" + d.getDepartment_pos() + "\' where department_name = \'" + d.getDepartment_name() + "\'";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }
        return true;
    }

    static vector<Department> select(string cond, MYSQL *conn)
    {
        vector<Department> docs;
        if (conn == nullptr)
            return vector<Department>();
        string sql = "select * from department";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query( conn, sql.c_str() );
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return vector<Department>();
        }
        MYSQL_RES *res_ptr = mysql_store_result(conn);
        MYSQL_ROW result_row;
        while (1)
        {
            result_row = mysql_fetch_row(res_ptr);
            if (!result_row)
            {
                mysql_free_result(res_ptr);
                break;
            }
            string department_name = result_row[0];
            string department_pos = result_row[1];

            docs.push_back( Department(department_name, department_pos) );
        }
        return docs;
    }
};

#endif