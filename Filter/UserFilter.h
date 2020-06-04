#include <map>
#include <vector>
#include <string>
#include "IFilter.h"
#include "../DBConnPool/DBConnPool.h"
#include "../WebService/headerParse/requestParser.h"
#include "../CJsonObject/CJsonObject.hpp"

using namespace std;
using namespace http;
using namespace neb;


class UserFilter : public IFilter
{
    public:
    UserFilter(string type){}

    bool doFilter(Request &req, DbConnPool *pool)
    {
        MYSQL *conn = pool->getConn();
        if (req.getBody().size() != 0)
        {
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            string cookie;
            obj.Get("x-safety-cookie", cookie);
            string token;
            obj.Get("x-safety-token", token);
            vector<string> to;
            string response;
            string sql = "select * from keys where id = \'" + token + "\'";
            printf("*debug* %s\n", sql.c_str());
            int res = mysql_query( conn, sql.c_str() );
            if (res)
            {
                printf("*ERROR* %s\n", mysql_error(conn));
                response = req.getResponse("HTTP/1.1", "403", "Forbidden", "application/json", "{\"errCode\":1,\"errMsg\":\"Forbiden\"}");
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
                string key = result_row[0];
                string value = result_row[1];

                to.push_back(key+value);
            }
            if (to.size() != 0)
            {
                return true;
            }
            else
            {
                response = req.getResponse("HTTP/1.1", "403", "Forbidden", "application/json", "{\"errCode\":1,\"errMsg\":\"Insert Failed\"}");
            }
            //td::string version, std::string statecode, std::string fullstate, std::string fulltype, std::string body
            //printf("insert end\n");
            req.setResp(response);
        }
        else
        {
            string response = req.getResponse("HTTP/1.1", "403", "Forbidden", "application/json", "{\"errCode\":2,\"errMsg\":\"Error Requset\"}");
            req.setResp(response);
        }
    }

};