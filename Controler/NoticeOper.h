#ifndef ___NOTICEOPER_H___
#define ___NOTICEOPER_H___

#include "../FastNet/include/Server.h"
#include "IOper.h"
#include "../Model/Notice.h"
#include "../WebService/headerParse/requestParser.h"
#include "../CJsonObject/CJsonObject.hpp"
#include "../DAO/NoticeDAO.h"

using namespace std;
using namespace http;
using namespace neb;

class NoticeOper 
{
    public:
    NoticeOper(){}

    static void insert(Request &req, MYSQL *mysql)
    {
        if (req.getBody().size() != 0)
        {
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            string notice_msg;
            obj.Get("notice_msg", notice_msg);
            string notice_pic;
            obj.Get("notice_pic", notice_pic);

            Notice d(notice_msg, notice_pic);
            string response;
            if (NoticeDAO::insert(d, mysql))
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
            string notice_msg;
            obj.Get("notice_msg", notice_msg);
            string notice_pic;
            obj.Get("notice_pic", notice_pic);

            Notice d(notice_msg, notice_pic);
            string response;
            if (NoticeDAO::del(d, mysql))
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
            string notice_msg;
            obj.Get("notice_msg", notice_msg);
            string notice_pic;
            obj.Get("notice_pic", notice_pic);

            Notice d(notice_msg, notice_pic);
            string response;
            if (NoticeDAO::modify(d, mysql))
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
            vector<Notice> docs = NoticeDAO::select("", mysql);
            CJsonObject jObj;
            string response;
            if (docs.size() != 0)
            {
                for (int i = 0; i < docs.size(); i++)
                {
                    printf("*debug4\n");
                    char index[10];
                    memset(index, 0, sizeof(index));
                    sprintf(index, "%d", i+1);
                    jObj.AddEmptySubObject("notice" + string(index));
                    jObj["notice" + string(index)].Add("notice_msg", docs[i].getNotice_msg());
                    jObj["notice" + string(index)].Add("notice_pic", docs[i].getNotice_pic());
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