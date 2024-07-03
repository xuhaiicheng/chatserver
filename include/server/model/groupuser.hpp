#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

//群组用户，多一个role角色信息，从User类直接继承，复用User的其他信息
class GroupUser: public User
{
public: 
    void setRole(string role) { this->role = role; }
    string getRole() { return this->role; }

private:  //派生类新增的特殊的成员变量
    string role;

};

#endif