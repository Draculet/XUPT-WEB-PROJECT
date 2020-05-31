#ifndef ___DOCDAO_H___
#define ___DOCDAO_H___

#include "../Model/Doctor.h"
#include "../DBConnPool/DBConnPool.h"
#include <vector>
using namespace std;

string itos(int num)
{
    char tmp[20] = {0};
    sprintf(tmp, "%d", num);
    return string(tmp);
}

class DoctorDAO // : public Oper<Doctor>
{
    public:
    DoctorDAO(){}

    static bool insert(Doctor &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "insert into doctor values(null, " + string("\'") + d.getDoctor_name() + "\', " + itos(d.getDoctor_gender()) + ", \'" + d.getDepartment_name() + "\', \'" + d.getDoctor_title() + "\'";
                sql += ", \'" + d.getDoctor_photo() + "\', \'" + d.getDoctor_tel() + "\', \'" +  d.getDoctor_pwd() + "\', \'" + d.getDoctor_info() + "\', " + itos(d.getDoctor_quota()) + ")";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }
    static bool del(int id, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "delete from doctor where doctor_id = " + itos(id);
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }
    static bool modify(Doctor &d, MYSQL *conn)
    {
        if (del(d.getDoctor_id(), conn))
        {
            insert(d, conn);
            return true;
        }
        return false;
    }
    static vector<Doctor> select(string cond, MYSQL *conn)
    {
        vector<Doctor> docs;
        if (conn == nullptr)
            return vector<Doctor>();
        string sql = "select * from doctor where " + cond;
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query( conn, sql.c_str() );
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return vector<Doctor>();
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
            int doctor_id = atoi(result_row[0]);
            string doctor_name = result_row[1];
            int doctor_gender = atoi(result_row[2]);
            string department_name = result_row[3];
            string doctor_title = result_row[4];
            string doctor_photo = result_row[5];
            string doctor_tel = result_row[6];
            string doctor_pwd = result_row[7];
            string doctor_info = result_row[8];
            int doctor_quota = atoi(result_row[9]);
            docs.push_back( Doctor(doctor_id, doctor_name, doctor_gender, department_name, doctor_title, doctor_photo,
                doctor_tel, doctor_pwd, doctor_info, doctor_quota) );
        }
        return docs;
    }
};

#endif