#ifndef ___NOTICEDAO_H___
#define ___NOTICEDAO_H___

#include "../Model/Notice.h"
#include "../DBConnPool/DBConnPool.h"
#include <vector>
using namespace std;


class NoticeDAO // : public Oper<Doctor>
{
    public:
    NoticeDAO(){}

    static bool insert(Notice &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "insert into notice values(\'" + d.getNotice_msg() + "\', \'" + d.getNotice_pic() + "\')";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }

    static bool del(Notice &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "delete from notice where notice_msg = \'" + d.getNotice_msg() + "\'";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }

        return true;
    }
    static bool modify(Notice &d, MYSQL *conn)
    {
        if (conn == nullptr)
            return -1;
        string sql = "update notice set notice_msg=\'" + d.getNotice_msg() + "\', notice_pic=\'" + d.getNotice_pic() + "\' where notice_msg = \'" + d.getNotice_msg() + "\'";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query(conn,sql.c_str());
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return false;
        }
        return true;
    }

    static vector<Notice> select(string cond, MYSQL *conn)
    {
        vector<Notice> docs;
        if (conn == nullptr)
            return vector<Notice>();
        string sql = "select * from notice";
        printf("*debug* %s\n", sql.c_str());
        int res = mysql_query( conn, sql.c_str() );
        if (res)
        {
            printf("*ERROR* %s\n", mysql_error(conn));
            return vector<Notice>();
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
            string notice_msg = result_row[0];
            string notice_pic = result_row[1];

            docs.push_back( Notice(notice_msg, notice_pic) );
        }
        return docs;
    }
};

#endif