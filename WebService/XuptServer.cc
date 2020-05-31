#include "../FastNet/include/Server.h"
#include "../FastNet/include/Connection.h"
#include "../FastNet/include/Buffer.h"
#include "headerParse/requestParser.h"
#include <map>
#include <memory>
#include "../CJsonObject/CJsonObject.hpp"
#include "../Controler/Controler.h"

using namespace http;
using namespace std;
using namespace net;
using namespace base;
using namespace neb;

class XuptServer
{
    public:
    
    XuptServer(int threadnum, int port)
        :control_(new Controler()),
        parserMap_(),
        serv_(threadnum, port)
    {
        serv_.setReadCallback(bind(&XuptServer::onRead, this, placeholders::_1, placeholders::_2));
    }

    void start()
    {
        serv_.start();    
    }

    void onRead(Buffer *buf, shared_ptr<Connection> conn)
    {
        printf("Header:\n%s\n", buf->preViewAsString(buf->readable()).c_str() );
        requestParser *parser;
        if (parserMap_.find(conn) == parserMap_.end())
        {
            parserMap_[conn] = new requestParser(buf);
            parser = parserMap_[conn];
        }
        else
        {
            parser = parserMap_[conn];
        }
        while (parser->parseBuffer()){printf("error\n");}
        if (parser->size() > 0)
        {
            for (int i = 0; i < parser->size(); i++)
            {
                Request req = parser->reqs[i];
                control_->controlRoute(req, req.getPath());
                printf("ToSend: %s\n", req.getResp().c_str());
                conn->send(req.getResp());
                //printf("send\n");
            }
            parser->reqs.clear();
        }
    }

    private:
    unique_ptr<Controler> control_;
    map<shared_ptr<Connection>, requestParser *> parserMap_;
    net::Server serv_;
};

int main(void)
{
    XuptServer s(4, 8832);
    s.start();
    while(1){}
}

/*
//FIXME 用法有误,requestParser需要是类成员! 否则无法解析不完整body
void onRead(map<shared_ptr<Connection>, requestParser *> *parserMap, Buffer *buf, shared_ptr<Connection> conn)
{
    requestParser *reqhead_;
    if (parserMap->find(conn) == parserMap->end())
    {
        (*parserMap)[conn] = new requestParser(buf);
        reqhead_ = (*parserMap)[conn];
    }
    else
    {
        reqhead_ = (*parserMap)[conn];
    }
    while (reqhead_->parseBuffer()){}
    if (reqhead_->size() > 0)
    {
        //http1.1复用连接但仍是串行处理,所以不会有buffer里请求数大于1的情况
        //printf("In Conn Fd %d : request size %d\n", conn->chann_.getFd(), conn->reqhead_->size());
        for (int i = 0; i < reqhead_->size(); i++)
        {
            cout << reqhead_->reqs[i].getBody() << endl;
            CJsonObject obj( reqhead_->reqs[i].getBody() );
            //cout << obj.ToString() << endl;
            
            string k;
            obj.Get("userName", k);
            cout << "userName: " << k << endl;
            obj.Get("userTel", k);
            cout << "userTel: " << k << endl;
            obj.Get("userPwd", k);
            cout << "UserPwd: " << k << endl;

            //cout << "FromPeer Path: " << reqhead_->reqs[i].getPath() << endl;
            //cout << "FromPeer Method: " << reqhead_->reqs[i].getMethod() << endl;
            string str =  reqhead_->reqs[i].toResp("{\"errCode\":1,\"errMsg\":\"该电话号码已被注册\"}");
            cout << "test: " << str << endl;
            conn->send(str);
            //string data = string(r.resptr_, r.len_);
            //conn->send(data);
            //printf("send %d bytes\n", r.len_);
        }
        reqhead_->reqs.clear();
    }
}
*/