#ifndef ___DOCUNDAO_H___
#define ___DOCUNDAO_H___

#include "../Model/Doctor_Unchecked.h"
#include "../DBConnPool/DBConnPool.h"
#include <vector>
#include "../Util/itos.h"
using namespace std;

class Doctor_UncheckedDAO
{
    public:
    Doctor_UncheckedDAO(){}

    static bool insert(Doctor_Unchecked &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "insert into doctor_unchecked values(" + string("\'") + d.getDoctor_name() + "\', " + itos(d.getDoctor_gender()) + ", \'" + d.getDepartment_name() + "\', \'" + d.getDoctor_title() + "\'";
                sql += ", \'" + d.getDoctor_photo() + "\', \'" + d.getDoctor_tel() + "\', \'" +  d.getDoctor_pwd() + "\', \'" + d.getDoctor_info() + "\' )";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }
    static bool del(string doctor_tel, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "delete from doctor_unchecked where doctor_tel = \'" + doctor_tel + "\'";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }

    //TODO 修改
    static bool modify(Doctor_Unchecked &d, MYSQL *conn)
    {
        if (del(d.getDoctor_tel(), conn))
        {
            insert(d, conn);
            return true;
        }
        return false;
    }
    static vector<Doctor_Unchecked> select(string cond, MYSQL *conn)
    {
        vector<Doctor_Unchecked> docs;
        if (conn == nullptr)
            return vector<Doctor_Unchecked>();
        string sql = "select * from doctor where " + cond;
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query( conn, sql.c_str() );
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return vector<Doctor_Unchecked>();
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
            string doctor_name = result_row[0];
            int doctor_gender = atoi(result_row[1]);
            string department_name = result_row[2];
            string doctor_title = result_row[3];
            string doctor_photo = result_row[4];
            string doctor_tel = result_row[5];
            string doctor_pwd = result_row[6];
            string doctor_info = result_row[7];
            docs.push_back( Doctor_Unchecked(doctor_name, doctor_gender, department_name, doctor_title, doctor_photo,
                doctor_tel, doctor_pwd, doctor_info) );
        }
        return docs;
    }
};

#endif