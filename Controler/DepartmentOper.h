#ifndef ___DEPARTOPER_H___
#define ___DEPARTOPER_H___

#include "../FastNet/include/Server.h"
#include "IOper.h"
#include "../Model/Department.h"
#include "../WebService/headerParse/requestParser.h"
#include "../CJsonObject/CJsonObject.hpp"
#include "../DAO/DepartmentDAO.h"

using namespace std;
using namespace http;
using namespace neb;

class DepartmentOper 
{
    public:
    DepartmentOper(){}

    static void insert(Request &req, MYSQL *mysql)
    {
        if (req.getBody().size() != 0)
        {
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            string department_name;
            obj.Get("department_name", department_name);
            string department_pos;
            obj.Get("department_pos", department_pos);

            Department d(department_name, department_pos);
            string response;
            if (DepartmentDAO::insert(d, mysql))
            {
                response = req.getResponse("HTTP/1.1", "201", "Created", "application/json", "{\"errCode\":0,\"errMsg\":\"Success\"}");
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

    static void del(Request &req, MYSQL *mysql)
    {
        if (req.getBody().size() != 0)
        {
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            string department_name;
            obj.Get("department_name", department_name);
            string department_pos;
            obj.Get("department_pos", department_pos);

            Department d(department_name, department_pos);
            string response;
            if (DepartmentDAO::del(d, mysql))
            {
                response = req.getResponse("HTTP/1.1", "200", "OK", "application/json", "{\"errCode\":0,\"errMsg\":\"Success\"}");
            }
            else
            {
                response = req.getResponse("HTTP/1.1", "403", "Forbidden", "application/json", "{\"errCode\":1,\"errMsg\":\"Delete Failed\"}");
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
    static void modify(Request &req, MYSQL *mysql)
    {
        if (req.getBody().size() != 0)
        {
            //string doctor_id = obj.Get("doctor_id");
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            string department_name;
            obj.Get("department_name", department_name);
            string department_pos;
            obj.Get("department_pos", department_pos);

            Department d(department_name, department_pos);
            string response;
            if (DepartmentDAO::modify(d, mysql))
            {
                response = req.getResponse("HTTP/1.1", "200", "OK", "application/json", "{\"errCode\":0,\"errMsg\":\"Success\"}");
            }
            else
            {
                response = req.getResponse("HTTP/1.1", "403", "Forbidden", "application/json", "{\"errCode\":1,\"errMsg\":\"Modify Failed\"}");
            }
            //std::string version, std::string statecode, std::string fullstate, std::string fulltype, std::string body
            //printf("insert end\n");
            req.setResp(response);
        }
        else
        {
            string response = req.getResponse("HTTP/1.1", "403", "Forbidden", "application/json", "{\"errCode\":2,\"errMsg\":\"Error Requset\"}");
            req.setResp(response);
        }
    }

    static void select(Request &req, MYSQL *mysql)
    {
        if (req.getBody().size() == 0)
        {
            vector<Department> docs = DepartmentDAO::select("", mysql);
            CJsonObject jObj;
            string response;
            if (docs.size() != 0)
            {
                jObj.AddEmptySubArray("department");
                for (int i = 0; i < docs.size(); i++)
                {
                    printf("*debug4\n");
                    char index[10];
                    memset(index, 0, sizeof(index));
                    sprintf(index, "%d", i+1);
                    CJsonObject jObj2;
                    jObj2.Add("department_name", docs[i].getDepartment_name());
                    jObj2.Add("department_pos", docs[i].getDepartment_pos());
                    jObj["department"].Add(jObj2);
                    //jObj.AddEmptySubObject("department" + string(index));
                    //jObj["department" + string(index)].Add("department_name", docs[i].getDepartment_name());
                    //jObj["department" + string(index)].Add("department_pos", docs[i].getDepartment_pos());
                }
                //printf("%s\n", jObj.ToString().c_str());
                response = req.getResponse("HTTP/1.1", "200", "OK", "application/json", jObj.ToString());
                //printf("%s\n", response.c_str());
            }
            else
            {
                response = req.getResponse("HTTP/1.1", "403", "Forbidden", "application/json", "{\"errCode\":0,\"errMsg\":\"Empty\"}");
            }
            //std::string version, std::string statecode, std::string fullstate, std::string fulltype, std::string body
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

#endif