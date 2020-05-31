#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <map>
#include "requestParser.h"

using namespace std;
using namespace net;
using namespace http;

//TODO 带问号的url处理

    requestHeader::requestHeader(string method, string path, string version, string body, map<string, string>reqkv):
        method_(method),
        path_(path),
        version_(version),
        body_(body),
        reqkv_(reqkv)
    {
        type_["css"] = "text/css";
        type_["js"] = "application/javascript";
        type_["html"] = "text/html";//TODO 指定charset=GBK ?
        type_["jpg"] = "image/jpeg";
        type_["png"] = "image/png";
        type_["gif"] = "image/gif";
        type_["ico"] = "image/x-icon";
        type_["json"] = "application/json";
        stat_["200"] = "OK";
        stat_["304"] = "Not Modified";
        stat_["404"] = "Not Found";

    }

    string requestHeader::toResp(string body)
    {
        /*
        int fsize = 0;
        string path = basepath + path_;
        char fsize_c[10] = {0};
        //cout << path << endl;
        int pos = path_.find(".");
        string type = string(path_, pos + 1);
        //cout << type << endl;
        int ffd = open(path.c_str(), O_RDONLY);
        
       
        if (ffd == -1)
        {
            state_ = "404";
            cout << "Not Found" << endl;
        }
        else
        {
            struct stat st;
            if (fstat(ffd, &st) == 0)
            {
                fsize = st.st_size;
                char fbuf[2 * fsize];
                memset(fbuf, 0, sizeof(fbuf));
                int ret = read(ffd, fbuf, fsize * 2);
                memcpy(fstr_, fbuf, fsize);
                //TODO 不能用string保存二进制文件,遇到0会出现问题
                //cout << "===========file byte: " << fstr_.size() << endl;
                //cout << fstr_ << "EOF" << endl;
                state_ = "200";
                sprintf(fsize_c, "%d", fsize);
            }
            else
            {
                state_ = "404";
                //TODO 错误处理
            }
        }
        */
        //logic judge
        state_ = "200";
        //TODO 有效率问题
        string data = body;
        char len[10];
        memset(len, 0, sizeof(len));
        sprintf(len, "%d", data.size());
        string resp;
        if (state_ == "200")
        {
            resp = version_ + " " + state_ + " " + stat_[state_] + "\r\n";
            resp += "Server: My Server\r\n";
            resp = resp + "Content-Type: " + type_["json"] + "\r\n";
            resp = resp + "Content-Length: " + string(len) + "\r\n\r\n";
            resp = resp + data;
            //memcpy(resp_, resp.c_str(), resp.size());
            //memcpy(resp_ + resp.size(), fstr_, fsize);
            //resplen_ = resp.size() + fsize;
            //cout << resp_ << endl;
        }
        else if (state_ == "404")
        {
            data = "<!DOCTYPE><html><head><title>404 Not Found</title></head><body>404 Not Found</body></html>";
            char len[10];
            memset(len, 0, sizeof(len));
            sprintf(len, "%d", data.size());
            //sprintf(fsize_c, "%d", s.size());
            resp = version_ + " " + state_ + " " + stat_[state_] + "\r\n";
            resp += "Server: My Server\r\n";
            resp = resp + "Content-Type: text/html\r\n";
            resp = resp + "Content-Length: " + string(len) + "\r\n\r\n";
            resp += data;
            //resp = resp + s;
            //memcpy(resp_, resp.c_str(), resp.size());
            //resplen_ = resp.size();
            //cout << resp_ << endl;
        }
        return resp;
    }

    string requestHeader::getResponse(string version, string statecode, string fullstate, string type, string body)
    {
        string data = body;
        char len[10];
        memset(len, 0, sizeof(len));
        sprintf(len, "%d", data.size());
        string resp;
        if (statecode == "404")
        {
            data = "<!DOCTYPE><html><head><title>404 Not Found</title></head><body>404 Not Found</body></html>";
            char len[10];
            memset(len, 0, sizeof(len));
            sprintf(len, "%d", data.size());
            //sprintf(fsize_c, "%d", s.size());
            resp = version + " " + statecode + " " + fullstate + "\r\n";
            resp = resp + "Content-Type: text/html\r\n";
            resp = resp + "Content-Length: " + string(len) + "\r\n\r\n";
            resp += data;
            return resp;
            //resp = resp + s;
            //memcpy(resp_, resp.c_str(), resp.size());
            //resplen_ = resp.size();
            //cout << resp_ << endl;
        }
        memset(len, 0, sizeof(len));
        sprintf(len, "%d", data.size());
        //sprintf(fsize_c, "%d", s.size());
        resp = version + " " + statecode + " " + fullstate + "\r\n";
        resp += "Server: My Server\r\n";
        resp += "Access-Control-Allow-Origin: *\r\n";
        resp += "Access-Control-Allow-Methods: *\r\n";
        resp += "Access-Control-Max-Age: 3600\r\n";
        resp += "Access-Control-Allow-Headers: *\r\n";
        resp += "Access-Control-Allow-Credentials: true\r\n";
        resp += "Connection: keep-alive\r\n";
        resp += "Transfer-Encoding: chunked";
        
        if (data.size() != 0)
        {
            resp = resp + "Content-Type: " + type + "\r\n";
            resp = resp + "Content-Length: " + string(len) + "\r\n\r\n";
            resp += data;
        }
        
        return resp;
    }

bool requestParser::parseBuffer()
{
    //处理请求body不完整的情况
    if (kfin == false)
    {
        auto iter = reqkv_.find("Content-Length");//有buf中只有一半body的可能
        if (iter != reqkv_.end())
        {
            int n = atoi(iter->second.c_str());
            string bodystr = buf_->preViewAsString(buf_->readable());
            if (bodystr.size() >= n)
            {
                body_ = string(bodystr, 0, n);
                cout << "body: " << body_ << endl;
                buf_->retrieve(n);
                kfin = true;
            }
            else
            {
                kfin = false;
                printf("请求body不完整\n");
                //函数出口未统一在最后
                return false;
            }
        }
        //printf("\n\n不完整body解析完成\n");
        //printf("Result:\nmethod: %s\npath: %s\nversion: %s\nbody: %s\n", method_.c_str(), path_.c_str(), version_.c_str(), body_.c_str());
        //for (auto &iter : reqkv_)
        //{
        //    printf("key: %s\nvalue: %s\n", iter.first.c_str(), iter.second.c_str());
        //}
        reqs.push_back(requestHeader(method_, path_, version_, body_, reqkv_));
        //函数出口未统一在最后
        return true;
    }

    //正常开始处理
    int begin = 0;
    int cur = 0;
    string s = buf_->preViewAsString(buf_->readable());
    //出口未统一
    if (s.size() == 0)
        return false;
    //cout << s << endl;
    int reqtail = s.find("\r\n\r\n", 0);
    if (reqtail != string::npos)
    {
        string headpart = string(s, 0, reqtail + 2);
        //cout << headpart.size() << endl;
        //printf("解析开始\n");
        while (true)
        {
            printf("debug\n");
            cur = headpart.find("\r\n", begin);
            if (cur != string::npos)
            {
                string line(s, begin, cur - begin);
                //cout << line << endl;
                int ret = line.find(':');
                if (ret == string::npos)
                {
                    parseHead(line);
                }
                else
                {
                    string key = string(line, 0, ret);
                    string value = string(line, ret + 2);
                    reqkv_[key] = value;
                    //cout << "key: " << key << endl;
                    //cout << "value: " << value << endl;
                }
                begin = cur + 2;
            }
            else
                break;
        }

        cout << "headpart: " << headpart << endl;
        
        buf_->retrieve(headpart.size() + 2);
        string bodyr = buf_->preViewAsString(buf_->readable());
        cout << "remain: " << bodyr << endl;
        for (auto iter : reqkv_)
        {
            //cout << iter.first << ":" << iter.second << endl;
        }
        for (auto pair : reqkv_)
        {
            cout << pair.first << ":" << pair.second << endl;
        }
        auto iter = reqkv_.find("Content-Length");//有buf中只有一半body的可能
        if (iter != reqkv_.end())
        {
            //cout << iter->second << endl;
            int n = atoi(iter->second.c_str());
            string bodystr = buf_->preViewAsString(buf_->readable());
            if (bodystr.size() >= n)
            {
                body_ = buf_->retrieveAsString(n);
                kfin = true;
            }
            else
            {
                kfin = false;
                //函数出口未统一在最后
                //printf("请求body不完整\n");
                return false;
            }
            //debug
        }
        //printf("解析完成\n");
        //printf("Result:\nmethod: %s\npath: %s\nversion: %s\nbody: %s\n", method_.c_str(), path_.c_str(), version_.c_str(), body_.c_str());
        //for (auto &iter : reqkv_)
        //{
        //   printf("key: %s\nvalue: %s\n", iter.first.c_str(), iter.second.c_str());
        //}
        reqs.push_back(requestHeader(method_, path_, version_, body_, reqkv_));
        method_ = ""; path_ = ""; version_ = ""; body_ = ""; reqkv_.clear();
        return true;
    }
    else//如果找不到\r\n\r\n直接返回,等下一次调用
    {
        //printf("未找到\\r\\n\\r\\n,header不完整\n");
        return false;
    }
}

void requestParser::parseHead(string line)
{
    int begin = 0;
    int cur = 0;
    cur = line.find(" ", begin);
    method_ = string(line, begin, cur - begin);
    begin = cur + 1;
    cur = line.find(" ", begin);
    path_ = string(line, begin, cur - begin);
    int pos = path_.find("?");
    if (pos != string::npos)
        path_ = string(path_, 0, pos);
    if (path_ == "/")
        path_ = "/index.html";
    begin = cur + 1;
    version_ = string(line, begin);
}
