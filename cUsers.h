#ifndef TICKETSYSTEM_CUSERS_H
#define TICKETSYSTEM_CUSERS_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include "vector.h"
#include "bpt.h"

using std::string;
using std::unordered_map;

class UserManager;

class User {
    friend class UserManager;

public:
    str<21> userName;
    str<31> password;
    str<21> name;
    str<31> mailAdr;
    int privilege;
public:
    User() : privilege(-1) {};

    User(const string &u, const string &p, const string &n, const string &m, int &g) : userName{u}, password{p},
                                                                                       name{n}, mailAdr{m},
                                                                                       privilege{g} {}

    User(const User &user) : userName(user.userName), password(user.password), name(user.name), mailAdr(user.mailAdr),
    privilege(user.privilege) {}

    friend bool operator == (const User &l, const User &r) {
        return l.userName == r.userName;
    }
    friend bool operator < (const User &l, const User &r) {
        return false;
    }

};

std::unordered_map<string, int> loginUser;

class UserManager {
public:
    B_PLUS_TREE::BPlusTree<str<21>,User> users;

public:
    UserManager() : users{"userdata"} {};

    bool add_user(const string &c, const string &u, const string &p, const string &n, const string &m, int g) {
        if (!users.GetNum()) g = 10;
        else if (loginUser.find(c) == loginUser.end() || loginUser[c] <= g) return false;
        User new_user{u, p, n, m, g};
        users.Insert(u, new_user);
        return true;
    };

    bool login(const string &u, const string &p) {
        //用户已经登录，则登陆失败
        if (loginUser.find(u) != loginUser.cend()) return false;
        std::vector<User> tmpUser;
        users.Find(u, tmpUser);
        //若用户不存在，则登陆失败
        if (tmpUser.empty()) return false;
        //密码错误，则登陆失败
        if (tmpUser[0].password != p) return false;
        return loginUser[u] = tmpUser[0].privilege, true;
    };

    bool logout(const string &u) {
        return loginUser.erase(u);
    };

    string query_profile(const string &c, const string &u) {
        //c已经登陆
        if (loginUser.find(c) == loginUser.cend()) return "-1";
        std::vector<User> tmpUser;
        users.Find(u, tmpUser);
        //c.privilege >= u.privilege
        if(tmpUser.empty()) return "-1";
        if (c != u && loginUser[c] <= tmpUser[0].privilege) return "-1";
        return u + " " + string(tmpUser[0].name) + " " + string(tmpUser[0].mailAdr) + " " + std::to_string(tmpUser[0].privilege);
    };

    string modify_profile(const string &c, const string &u, const string &p, const string &n, const string &m, int g) {
        //c已登录
        if (loginUser.find(c) == loginUser.cend()) return "-1";
        std::vector<User> tmpUser;
        User preUser;
        users.Find(u, tmpUser);
        if(tmpUser.empty()) return "-1";
        preUser = tmpUser[0];
        //-c的权限大于-u的权限，或是-c和-u相同，且-g需低于-c的权限

        if (!(g < loginUser[c]) || !( (loginUser[c] > tmpUser[0].privilege) || (c == u) )) return "-1";
        if(g != -1) tmpUser[0].privilege = g;
        if(m != "") tmpUser[0].mailAdr = m;
        if(n != "") tmpUser[0].name = n;
        if(p != "") tmpUser[0].password = p;
        users.Update(u, preUser, tmpUser[0]);
        return u + " " + string(tmpUser[0].name) + " " + string(tmpUser[0].mailAdr) + " " + std::to_string(tmpUser[0].privilege);
    };

    void clear() {
        users.Clear();
        loginUser.clear();
    }

};


#endif //TICKETSYSTEM_CUSERS_H
