#ifndef FRIENDMODLE_H
#define FRIENDMODLE_H

#include "user.hpp"
#include <vector>
using namespace std;

//维护好友消息的操作接口方法
class FriendModel
{
public:
    //添加好友关系
    void insert(int userid, int friendid);

    //返回用户好友列表 friendid 其中还要从User表中查询friendid中的相关信息，所以要添加一个联合查询
    vector<User> query(int userid); //联合查询
};

#endif