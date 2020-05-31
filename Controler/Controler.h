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
        opers_["/xuptcd/doctor/insert"] = bind(DoctorOper::insert, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/doctor/modify"] = bind(DoctorOper::modify, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/doctor/del"] = bind(DoctorOper::del, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/doctor/selectDepart"] = bind(DoctorOper::selectDepart, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/notice/insert"] = bind(NoticeOper::insert, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/notice/modify"] = bind(NoticeOper::modify, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/notice/del"] = bind(NoticeOper::del, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/notice/select"] = bind(NoticeOper::select, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/department/insert"] = bind(DepartmentOper::insert, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/department/modify"] = bind(DepartmentOper::modify, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/department/del"] = bind(DepartmentOper::del, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/department/select"] = bind(DepartmentOper::select, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/doctor_unchecked/insert"] = bind(Doctor_UncheckedOper::insert, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/doctor_unchecked/modify"] = bind(Doctor_UncheckedOper::modify, placeholders::_1, placeholders::_2);
        opers_["/xuptcd/doctor_unchecked/del"] = bind(Doctor_UncheckedOper::del, placeholders::_1, placeholders::_2);
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