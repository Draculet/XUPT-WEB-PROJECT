#ifndef ___CONT_H___
#define ___CONT_H___

#include "../FastNet/include/Server.h"
#include "../WebService/headerParse/requestParser.h"
#include "DoctorOper.h"
#include "NoticeOper.h"
#include "DepartmentOper.h"
#include "Doctor_UncheckedOper.h"
#include <functional>
#include <memory>

using namespace std;
using namespace http;
/* 每个Server一个Controler,使用unique_ptr管理 */
class Controler
{
    public:
    Controler()
        :pool_(new DbConnPool("xuptcd_db"))
    {
        pool_->connect(10);
        /* url: http://49.233.166.221:8832/xuptcd/doctor/insert */
        /* 请求post json例子: 
        {
            "doctor_name":"医生",
            "doctor_gender":2,
            "department_name":"department",
            "doctor_title":"医生头衔",
            "doctor_photo":"www.url.com",
            "doctor_tel":"13543456333",
            "doctor_pwd":"pass",
            "doctor_info":"医生信息",
            "doctor_quota":3
        } */
        /* 响应 
        {
            "errCode":0,
            "errMsg":"Success"
        }
        {
            "errCode":1,
            "errMsg":"Insert Failed"
        }
        {
            "errCode":2,
            "errMsg":"Error Request"
        }*/
        opers_["/xuptcd/doctor/insert"] = bind(DoctorOper::insert, placeholders::_1, placeholders::_2);
        /* url: http://49.233.166.221:8832/xuptcd/doctor/modify */
        /* 请求post json例子: 
        {
            "doctor_name":"医生",
            "doctor_gender":2,
            "department_name":"department",
            "doctor_title":"医生头衔",
            "doctor_photo":"www.url.com",
            "doctor_tel":"13543456333",
            "doctor_pwd":"pass",
            "doctor_info":"医生信息",
            "doctor_quota":3
        } */
        /* 响应 
        {
            "errCode":0,
            "errMsg":"Success"
        }
        {
            "errCode":1,
            "errMsg":"Modify Failed"
        }
        {
            "errCode":2,
            "errMsg":"Error Request"
        }*/
        opers_["/xuptcd/doctor/modify"] = bind(DoctorOper::modify, placeholders::_1, placeholders::_2);
        /* url: http://49.233.166.221:8832/xuptcd/doctor/del */
        /* 请求post json例子: 
        {
            "doctor_id":"10006"
        } */
        /* 响应 
        {
            "errCode":0,
            "errMsg":"Success"
        }
        {
            "errCode":1,
            "errMsg":"Delete Failed"
        }
        {
            "errCode":2,
            "errMsg":"Error Request"
        }*/
        opers_["/xuptcd/doctor/del"] = bind(DoctorOper::del, placeholders::_1, placeholders::_2);

        //TODO ...
        /* url: http://49.233.166.221:8832/xuptcd/doctor/selectDepart */
        /* 请求post json例子: 
        {
            "department_name":"department"
        } 
        */
        /* 正常响应 响应的json比较奇怪待修改
        {
            "doctor1": {
                "doctor_id": 10030,
                "doctor_name": "医生",
                "doctor_gender": 2,
                "department_name": "department",
                "doctor_title": "医生头衔",
                "doctor_photo": "www.url.com",
                "doctor_tel": "13543456444",
                "doctor_pwd": "pass",
                "doctor_info": "医生信息",
                "doctor_quota": 3
            },
            "doctor2": {
                "doctor_id": 10031,
                "doctor_name": "医生",
                "doctor_gender": 2,
                "department_name": "department",
                "doctor_title": "医生头衔",
                "doctor_photo": "www.url.com",
                "doctor_tel": "110",
                "doctor_pwd": "pass",
                "doctor_info": "医生信息",
                "doctor_quota": 3
            }
        }
        当select无匹配时
        {"errCode":0,"errMsg":"Empty"}
        */
        opers_["/xuptcd/doctor/selectDepart"] = bind(DoctorOper::selectDepart, placeholders::_1, placeholders::_2);
        /*
        {
            "notice_msg":"通知内容",
            "notice_pic":"图片url"
        }

        */
        opers_["/xuptcd/notice/insert"] = bind(NoticeOper::insert, placeholders::_1, placeholders::_2);

        opers_["/xuptcd/notice/modify"] = bind(NoticeOper::modify, placeholders::_1, placeholders::_2);

        opers_["/xuptcd/notice/del"] = bind(NoticeOper::del, placeholders::_1, placeholders::_2);
        http://49.233.166.221:8832/xuptcd/notice/select
        opers_["/xuptcd/notice/select"] = bind(NoticeOper::select, placeholders::_1, placeholders::_2);
        //{"department_name":"呼吸科","department_pos":"西安邮电大学附属医院"}
        opers_["/xuptcd/department/insert"] = bind(DepartmentOper::insert, placeholders::_1, placeholders::_2);
        
        opers_["/xuptcd/department/modify"] = bind(DepartmentOper::modify, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/department/del"] = bind(DepartmentOper::del, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/department/select"] = bind(DepartmentOper::select, placeholders::_1, placeholders::_2);
        //{"doctor_name":"李默","doctor_gender":2,"department_name":"department","doctor_title":"医生头衔","doctor_photo":"www.url.com","doctor_tel":"13543456333","doctor_pwd":"pass","doctor_info":"医生信息"}
        opers_["/xuptcd/doctor_unchecked/insert"] = bind(Doctor_UncheckedOper::insert, placeholders::_1, placeholders::_2);
        //{"doctor_name":"改个名,电话号码是主键","doctor_gender":2,"department_name":"department","doctor_title":"医生头衔","doctor_photo":"www.url.com","doctor_tel":"13543456333","doctor_pwd":"pass","doctor_info":"医生信息"}
        opers_["/xuptcd/doctor_unchecked/modify"] = bind(Doctor_UncheckedOper::modify, placeholders::_1, placeholders::_2);
        
        opers_["/xuptcd/doctor_unchecked/del"] = bind(Doctor_UncheckedOper::del, placeholders::_1, placeholders::_2);
        //{"department_name":"department"}
        opers_["/xuptcd/doctor_unchecked/selectDepart"] = bind(Doctor_UncheckedOper::selectDepart, placeholders::_1, placeholders::_2);

        //...
    }

    void controlRoute(Request &req, string url)
    {
        printf("in control\n");
        MYSQL *sql = pool_->getConn();
        if (req.getMethod() == "OPTIONS")
        {
            string response = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Methods: OPTIONS, GET, HEAD, POST\r\nServer: My Server\r\nContent-Length: 0\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Credentials: true\r\nAccess-Control-Allow-Headers: X-Requested-With, accept, origin, content-type\r\n\r\n";
            req.setResp(response);
            return;
        }
        else if (opers_.find(url) != opers_.end() || req.getBody().size() == 0 || req.getMethod() == "GET")
        {
            auto cb = opers_[url];
            if (cb)
            {
                cb(req, sql);
                //不经过下面判断
            }
            else
            {
                printf("Error: Callback Not Found");
                string response = req.getResponse("HTTP/1.1", "404", "Not Found", "", "");
                req.setResp(response);
                return;
            }
        }
        else
        {
            printf("Error: Callback Not Found");
            string response = req.getResponse("HTTP/1.1", "404", "Not Found", "", "");
            req.setResp(response);
            return;
        }
        pool_->putBack(sql);
    }

    private:
    map<string, function<void(Request &, MYSQL *)> > opers_;
    unique_ptr<DbConnPool> pool_;
};

#endif