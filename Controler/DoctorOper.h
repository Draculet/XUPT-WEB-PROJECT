#ifndef ___DOCCONT_H___
#define ___DOCCONT_H___

#include "../FastNet/include/Server.h"
#include "IOper.h"
#include "../Model/Doctor.h"
#include "../WebService/headerParse/requestParser.h"
#include "../CJsonObject/CJsonObject.hpp"
#include "../DAO/DoctorDAO.h"

using namespace std;
using namespace http;
using namespace neb;

class DoctorOper // : public Oper<Doctor>
{
    public:
    DoctorOper(){}

    static void insert(Request &req, MYSQL *mysql)
    {
        if (req.getBody().size() != 0)
        {
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            string doctor_name;
            obj.Get("doctor_name", doctor_name);
            int doctor_gender;
            obj.Get("doctor_gender", doctor_gender);
            string department_name;
            obj.Get("department_name", department_name);
            string doctor_title;
            obj.Get("doctor_title", doctor_title);
            string doctor_photo;
            obj.Get("doctor_photo", doctor_photo);
            string doctor_tel;
            obj.Get("doctor_tel", doctor_tel);
            string doctor_pwd;
            obj.Get("doctor_pwd", doctor_pwd);
            string doctor_info;
            obj.Get("doctor_info", doctor_info);
            int doctor_quota;
            obj.Get("doctor_quota", doctor_quota);
            Doctor d(doctor_name, doctor_gender, department_name, doctor_title, doctor_photo, doctor_tel,
                doctor_pwd, doctor_info, doctor_quota);
            string response;
            if (DoctorDAO::insert(d, mysql))
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
            int doctor_id;
            obj.Get("doctor_id", doctor_id);
            
            string response;
            if (DoctorDAO::del(doctor_id, mysql))
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
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            int doctor_id;
            obj.Get("doctor_id", doctor_id);
            string doctor_name;
            obj.Get("doctor_name", doctor_name);
            int doctor_gender;
            obj.Get("doctor_gender", doctor_gender);
            string department_name;
            obj.Get("department_name", department_name);
            string doctor_title;
            obj.Get("doctor_title", doctor_title);
            string doctor_photo;
            obj.Get("doctor_photo", doctor_photo);
            string doctor_tel;
            obj.Get("doctor_tel", doctor_tel);
            string doctor_pwd;
            obj.Get("doctor_pwd", doctor_pwd);
            string doctor_info;
            obj.Get("doctor_info", doctor_info);
            int doctor_quota;
            obj.Get("doctor_quota", doctor_quota);
            Doctor d(doctor_name, doctor_gender, department_name, doctor_title, doctor_photo, doctor_tel,
                doctor_pwd, doctor_info, doctor_quota);
            string response;
            if (DoctorDAO::modify(d, mysql))
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
    static void selectDepart(Request &req, MYSQL *mysql)
    {
        if (req.getBody().size() != 0)
        {
            CJsonObject obj( req.getBody() );
            //string doctor_id = obj.Get("doctor_id");
            string department_name;
            obj.Get("department_name", department_name);
            string cond = "department_name = " + string("\'") + department_name + "\'"; 
            string response;
            vector<Doctor> docs = DoctorDAO::select(cond, mysql);
            CJsonObject jObj;
            if (docs.size() != 0)
            {
                for (int i = 0; i < docs.size(); i++)
                {
                    printf("*debug4\n");
                    char index[10];
                    memset(index, 0, sizeof(index));
                    sprintf(index, "%d", i+1);
                    jObj.AddEmptySubObject("doctor" + string(index));
                    jObj["doctor" + string(index)].Add("doctor_id", docs[i].getDoctor_id());
                    jObj["doctor" + string(index)].Add("doctor_name", docs[i].getDoctor_name());
                    jObj["doctor" + string(index)].Add("doctor_gender", docs[i].getDoctor_gender());
                    jObj["doctor" + string(index)].Add("department_name", docs[i].getDepartment_name());
                    jObj["doctor" + string(index)].Add("doctor_title", docs[i].getDoctor_title());
                    jObj["doctor" + string(index)].Add("doctor_photo", docs[i].getDoctor_photo());
                    jObj["doctor" + string(index)].Add("doctor_tel", docs[i].getDoctor_tel());
                    jObj["doctor" + string(index)].Add("doctor_pwd", docs[i].getDoctor_pwd());
                    jObj["doctor" + string(index)].Add("doctor_info", docs[i].getDoctor_info());
                    jObj["doctor" + string(index)].Add("doctor_quota", docs[i].getDoctor_quota());
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