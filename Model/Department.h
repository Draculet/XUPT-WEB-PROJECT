#ifndef __DEPART_H__
#define __DEPART_H__

#include <string>
using namespace std;

class Department
{
    public:
    Department(string department_name, string department_pos)
        :department_name_(department_name),
        department_pos_(department_pos)
    {}
    
    string getDepartment_name() {
		return department_name_;
	}

	void setDepartment_name(string department_name) {
		department_name_ = department_name;
	}

	string getDepartment_pos() {
		return department_pos_;
	}

	void setDepartment_pos(string department_pos) {
		department_pos_ = department_pos;
	}


    private:
    string department_name_;
    string department_pos_;
};

#endif