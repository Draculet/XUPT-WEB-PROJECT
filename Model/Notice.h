#ifndef __NOTICE_H__
#define __NOTICE_H__

#include <string>
using namespace std;

class Notice
{
    public:
    Notice(string notice_msg, string notice_pic)
        :notice_msg_(notice_msg),
        notice_pic_(notice_pic)
    {}
    
    string getNotice_msg() {
		return notice_msg_;
	}

	void setNotice_msg(string notice_msg) {
		notice_msg_ = notice_msg;
	}

	string getNotice_pic() {
		return notice_pic_;
	}

	void setNotice_pic(string notice_pic) {
		notice_pic_ = notice_pic;
	}


    private:
    string notice_msg_;
    string notice_pic_;
};

#endif