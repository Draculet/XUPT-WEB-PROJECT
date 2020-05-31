#ifndef ___DOC_H___
#define ___DOC_H___

#include <string>

using namespace std;
/* 每个Server一个Controler,使用unique_ptr管理 */
class Doctor
{
    public:
    Doctor(){}
    Doctor(int doctor_id, string doctor_name, int doctor_gender, string department_name, string doctor_title, string doctor_photo,
        string doctor_tel, string doctor_pwd, string doctor_info, int doctor_quota)
    {
		doctor_id_ = doctor_id;
		doctor_name_ = doctor_name;
		doctor_gender_ = doctor_gender;
		department_name_ = department_name;
		doctor_title_ = doctor_title;
		doctor_photo_ = doctor_photo;
		doctor_tel_ = doctor_tel;
		doctor_pwd_ = doctor_pwd;
		doctor_info_ = doctor_info;
		doctor_quota_ = doctor_quota;
    }

	Doctor(string doctor_name, int doctor_gender, string department_name, string doctor_title, string doctor_photo,
        string doctor_tel, string doctor_pwd, string doctor_info, int doctor_quota)
    {
		doctor_id_ = -1;//AUTO INC
		doctor_name_ = doctor_name;
		doctor_gender_ = doctor_gender;
		department_name_ = department_name;
		doctor_title_ = doctor_title;
		doctor_photo_ = doctor_photo;
		doctor_tel_ = doctor_tel;
		doctor_pwd_ = doctor_pwd;
		doctor_info_ = doctor_info;
		doctor_quota_ = doctor_quota;
    }
    
    int getDoctor_id() {
		return doctor_id_;
	}

	void setDoctor_id(int doctor_id_) {
		doctor_id_ = doctor_id_;
	}

	string getDoctor_name() {
		return doctor_name_;
	}

	void setDoctor_name(string doctor_name_) {
		doctor_name_ = doctor_name_;
	}

	int getDoctor_gender() {
		return doctor_gender_;
	}

	void setDoctor_gender(int doctor_gender_) {
		doctor_gender_ = doctor_gender_;
	}

	string getDepartment_name() {
		return department_name_;
	}

	void setDepartment_name(string department_name_) {
		department_name_ = department_name_;
	}

	string getDoctor_title() {
		return doctor_title_;
	}

	void setDoctor_title(string doctor_title_) {
		doctor_title_ = doctor_title_;
	}

    string getDoctor_photo()
    {
        return doctor_photo_;
    }

    void setDoctor_photo(string doctor_photo_) {
		doctor_photo_ = doctor_photo_;
	}

	string getDoctor_tel() {
		return doctor_tel_;
	}

	void setDoctor_tel(string doctor_tel_) {
		doctor_tel_ = doctor_tel_;
	}

	string getDoctor_pwd() {
		return doctor_pwd_;
	}

	void setDoctor_pwd(string doctor_pwd_) {
		doctor_pwd_ = doctor_pwd_;
	}

	string getDoctor_info() {
		return doctor_info_;
	}

	void setDoctor_info(string doctor_info_) {
		doctor_info_ = doctor_info_;
	}

	int getDoctor_quota() {
		return doctor_quota_;
	}

	void setDoctor_quota(string doctor_quota_) {
		doctor_quota_ = doctor_quota_;
	}

    private:
    int doctor_id_;
    string doctor_name_;
    int doctor_gender_;
    string department_name_;
    string doctor_title_;
    string doctor_photo_;
    string doctor_tel_;
    string doctor_pwd_;
    string doctor_info_;
    int doctor_quota_;
    
};

#endif