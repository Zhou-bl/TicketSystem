#<font color=pink   >TicketSystem 开发文档</font>
|成员|任务|
|:---:|:---:|
|周秉霖|B + tree|
|陆逸凡|前端逻辑|
##<font color=pink>指令列表 & 出现频率</font>
```cpp
 1:add_user              *
 2:login                 ***
 3:logout                ***
 4:query_profile         *****
 5:modify_profile        ***
 
 6:add_train             *
 7:release_train         *
 8:query_train           *
 
 9:query_ticket          *****
 10:query_transfer       *
 11:buy_ticket           *****
 12:query_order          ***
 13:refund_ticket        *
 
 14:roll_back
 15:clear
 16:exit
```
###<font color=pink>指令内容 & 相关数据</font>
####<font color=pink>用户相关：</font>
|账户数据|相关说明|
|:----:|:----:|
|`username`|账户名，用户唯一标识串，长度1～20|
|`password`|可见字符，长度1～30|
|`name`|用户真实姓名，汉字，长度2～5|
|`mailAddr`|邮箱地址，长度1～30|
|`privilege`|优先级，为0～10中的一个整数|
* `add_user`
  * 参数列表：`-c -u -p -n -m -g`
* `login`
  * 参数列表：`-u -p`
* `logout`
  * 参数列表：`-u`
* `query_profile`
  * 参数列表：`-c -u`
* `modify_profile`
  * 参数列表：`-c -u (-p) (-n) (-m) (-g)`
####<font color=pink>车次相关：</font>
|车次数据|相关说明|
|:----:|:----:|
|`trainID`|车次唯一标识符，长度不超过20|
|`stationNum`|车次经过车站数量，整数，2～100|
|`station2`|共`stationNum`项车站名，不超过10个汉字|
|`seatNum`|车次的座位数，整数，不超过1e5|
|`prices`|两站间票价，共`(stationNum-1)`项，整数，不超过1e5|
|`startTime`|发车时间，格式`hh:mm`|
|`travelTimes`|两站间时间，共`(stationNum-1)`项，单位：分钟，整数：小于1e4|
|`stopoverTimes`|每站停留时间（不包括起始站、终点站），单位：分钟，整数：小于1e4|
|`saleDate`|车次售卖时间区间，两项时间，格式：`mm-dd`|
|`type`|列车类型，一个大写字母|
* `add_train`
  * 参数列表：`-i -n -m -s -p -x -t -o -d -y`
* `release_train`
  * 参数列表：`-i `
* `query_train`
  * 参数列表：`-i -d`
* `query_ticket`
  * 参数列表：`-s -t -d (-p time)`
* `query_transfer`
  * 参数列表：`-s -t -d (-p time)`
* `buy_ticket`
  *  参数列表：`-u -i -d -n -f -t (-q false)`
* `query_order`
  * 参数列表：`-u`
* `refund_ticket`
  * 参数列表：`-u (-n 1)`
* `rollback`
  * 参数列表：`-t`
* `clean`
  * 参数列表：` none`
* `exit`
  * 参数列表：`none`
##<font color=pink>头文件设计</font>
  * 若有更新，将标注在修改过程栏
### _main.cpp_
  ```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

#include "CommandLine.h"
#include "cUsers.h"
#include "cTrains.h"
#include "BPlusTree.h"

using std::string;
using std::cin;
//add some more

int main() {
    string commandLine;
    bool exit;
    while (getline(cin, commandLine)) {
        exit = operation(commandLine);
        if (!exit) break;
    }
    return 0;
}
```
###_CommandLine.h_
```cpp
#ifndef TICKETSYSTEM_COMMANDLINE_H
#define TICKETSYSTEM_COMMANDLINE_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "cUsers.h"
#include "cTrains.h"

using std::string;

class Command {
    long long timeStamp;
    //时间戳标记：需要记录且有回滚的需求
    //tips:各个指令间时间戳的值严格单调递增

    int command;
    //具体的操作：我的想法是 main.cpp 中 void operation() 读取这一指令，
    //不同的 string 对应不同的 int（操作种类有限），
    //便于后续 void operation() 进行switch
    /**
     * 1:add_user              *
     * 2:login                 ***
     * 3:logout                ***
     * 4:query_profile         *****
     * 5:modify_profile        ***
     * 6:add_train             *
     * 7:release_train         *
     * 8:query_train           *
     * 9:query_ticket          *****
     * 10:query_transfer       *
     * 11:buy_ticket           *****
     * 12:query_order          ***
     * 13:refund_ticket        *
     * 14:roll_back
     * 15:clear
     * 16:exit:
     */

    string totalLine;

    //先把整行都给记录下来，之后慢慢调 TLE
    Command() = default;

    Command(int &command, long long &timeStamp, char &_totalLine) :
            command(command), timeStamp(timeStamp) {}
    Command(string &_totalLine):totalLine(_totalLine) {}
    ~Command() {}
};

class CommandManager {
    Command *cmd;
};


#endif //TICKETSYSTEM_COMMANDLINE_H


```
###_TicketSystem.h_
```cpp
#ifndef TICKETSYSTEM_TICKETSYSTEM_H
#define TICKETSYSTEM_TICKETSYSTEM_H
#include "CommandLine.h"
#include "cTrains.h"
#include "cUsers.h"
class UserManager;
class CommandManager;
class TrainManager;

#endif //TICKETSYSTEM_TICKETSYSTEM_H
```
###_CommandParser.h_
```cpp
#ifndef TICKETSYSTEM_COMMANDPARSER_H
#define TICKETSYSTEM_COMMANDPARSER_H
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "cTrains.h"
#include "vector.h"
class TicketSystem;
class CommandParser {
private:
public:
    TicketSystem *China12306;
    void run();
    void parseAddUser(const sjtu::vector<std::string> &arg){};
    void parseLogin(const sjtu::vector<std::string> &arg){};
    void parseLogout(const sjtu::vector<std::string> &arg){};
    void parseQueryProfile(const sjtu::vector<std::string> &arg){};
    void parseModifyProfile(const sjtu::vector<std::string> &arg){};
    void parseAddTrain(const sjtu::vector<std::string> &arg){};
    void parseReleaseTrain(const sjtu::vector<std::string> &arg){};
    void parseQueryTrain(const sjtu::vector<std::string> &arg){};
    void parseQueryTicket(const sjtu::vector<std::string> &arg){};
    void parseQueryTransfer(const sjtu::vector<std::string> &arg){};
    void parseBuyTicket(const sjtu::vector<std::string> &arg){};
    void parseQueryOrder(const sjtu::vector<std::string> &arg){};
    void parseRefundTicket(const sjtu::vector<std::string> &arg){};
    void parseRollback(const sjtu::vector<std::string> &arg){};
    void parseClean(const sjtu::vector<std::string> &arg){};
    void parseExit(const sjtu::vector<std::string> &arg){};
private:
};
#endif //TICKETSYSTEM_COMMANDPARSER_H
```
###_cUser.h_
```cpp
#ifndef TICKETSYSTEM_CUSERS_H
#define TICKETSYSTEM_CUSERS_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "BPlusTree.h"

using std::string;

class UserManager;

//这个user就是存储user的全部数据
class User {
    friend class UserManager;

private:
    char *password;

public:
    char *userName;
    char *name;
    char *mailAdr;
    int privilege;

//构造函数：
    User() = default;

    User(char &_password, char &_userName, char &_name, char &_mailAdr, int &privilege) :
            privilege(privilege) {
        password = new char(_password);
        userName = new char(_userName);
        name = new char(_name);
        mailAdr = new char(_mailAdr);
    };

    ~User() {
        delete password;
        delete userName;
        delete name;
        delete mailAdr;
    };
};

class TicketSystem;

class UserManager {
private:
//TreeStorage<User> ~~~
//TreeStorage<Command> ~~~
public:
    TicketSystem *China12306;

    UserManager() = default;

    bool addUser() {};
    int login() {};
    int logout() {};
    int query_profile() {};
    int modify_profile() {};


};


#endif //TICKETSYSTEM_CUSERS_H
```
###_cTrains.h_
```cpp
#ifndef TICKETSYSTEM_CTRAINS_H
#define TICKETSYSTEM_CTRAINS_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

using std::string;

class TrainManager;

class Train {
public:
    string trainID;
//    车次唯一标识符
    int stationNum;
//    车次经过车站数量  2 <= n <= 100
    string station2[102];
//    车次经过所有车站名，共 stationNum 项
    int seatNum;
//    该车次座位数
    int prices[102];
//    两站之间票价，共 stationNum - 1 项
    int startHour;
//    列车每日发车时间
    int startMinute;
//    列车每日发车时间
    int travelTimes[102];
//    两站之间行车所用时间 共 stationNum - 1 项
    int stopoverTimes[102];
//    除始发站终点站，列车在每一站停留时间 共 stationNum - 2 项
    int saleDateM1, saleDateM2, saleDateD1, saleDateD2;
//    车次的售卖时间区间 :   M1-D1  M2-D2
    char type;

//    列车类型，一个大写字母
    Train() = default;

//TODO
    Train(char &trainID, int &stationNum, string &station2, int &seatNum, int &prices,
          int &startHour, int &startMinute, int *&travelTimes, int &stopoverTimes,
          int &saleDateM1, int &saleDateD1, int &saleDateM2, int &saleDateD2, char &type) {};
};

class TicketSystem;

class TrainManager {
private:
//TreeStorage<Train> ~~~
//TreeStorage<Command> ~~~
public:
    TicketSystem *China12306;

    TrainManager() = default;
    int add_train() {};
    int release_train() {};
    int query_train() {};
    int query_ticket() {};
    int query_transfer() {};
};


#endif //TICKETSYSTEM_CTRAINS_H
```
##<font color=pink>修改过程 & 相关说明</font>
|版本号|修改日期|修改内容|
|:----:|:-------:|:-----:|
|v1.0| 2022-05-03 |完成开发文档初稿|
*****

