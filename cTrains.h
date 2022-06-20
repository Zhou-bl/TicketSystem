#ifndef TICKETSYSTEM_CTRAINS_H
#define TICKETSYSTEM_CTRAINS_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include "vector.h"
#include "bpt.h"
#include "TokenScanner.h"
#include "tools.h"
#include "cUsers.h"

using std::string;
//string->char :
//  char str[length];
//  strcpy(str, obj.c_str());

struct Train {
    str<21> trainID;
    int stationNum = -1;
    int seatNum = -1;
    str<41> stations[102];
    int prices[102] = {0};
    //price[i]:从第一站到第i+1站的票价;
    int arvTime[102] = {0};
    //
    int dptTime[102] = {0};
    TIME st;
    //每日发车时间
    DATE startSale, endSale;
    char type;
    bool release = false;

    Train() {};


    bool operator==(const Train &t) const {
        return trainID == t.trainID;
    }

    bool operator<(const Train &t) const { return false; }

};

struct trainDate {
    int stationNum{-1}, seatNum{-1};
    DATE begin, end;

    bool operator==(const trainDate &t) const {
        return stationNum == t.stationNum && seatNum == t.seatNum
        && begin == t.begin && end == t.end;
    }

    bool operator<(const trainDate &t) const { return false; }
};

struct station {
    str<21> trainID;
    TIME arvTime, dptTime;
    int sumPrice;
    int order{-1};//这个车站是列车途经的第几个车展
    station():order(-1),sumPrice(0){};
    station(const string &trainID,const TIME &arv,const TIME &dpt,const int &sumPrice,const int &order):
    trainID(trainID),arvTime(arv),dptTime(dpt),sumPrice(sumPrice),order(order){};
    bool operator==(const station &t) const {
        return trainID == t.trainID && arvTime == t.arvTime && dptTime == t.dptTime && sumPrice == t.sumPrice
        && order == t.order;
    }

    bool operator<(const station &t) const { return false; }
};

struct ticket {
    str<21> trainID;
    DateTime startTime, endTime;
    int cost, seat, time;

    static bool CmpTime(const ticket &lhs, const ticket &rhs) {
        if (lhs.time == rhs.time) return lhs.trainID < rhs.trainID;
        return lhs.time < rhs.time;
    }

    static bool CmpCost(const ticket &lhs, const ticket &rhs) {
        if (lhs.cost == rhs.cost) return lhs.trainID < rhs.trainID;
        return lhs.cost < rhs.cost;
    }

    bool operator==(const ticket &t) const {
        return trainID == t.trainID && startTime == t.startTime && endTime == t.endTime
        && cost == t.cost && seat == t.seat && time == t.time;
    }

    bool operator<(const ticket &t) const { return false; }


};


class seat {
public:
    int cnt[100];

public:
    int stationNum, seatNum;

    seat() : seatNum{-1} , stationNum{0}{
        memset(cnt, 0, sizeof(cnt));
    };

    seat(const int &num, const int &seat) : stationNum{num}, seatNum{seat} {
        for (int i = 0; i < stationNum - 1; ++i) cnt[i] = seat;
    }

    seat &operator=(const seat &rhs){
        if(this == &rhs) return *this;
        stationNum = rhs.stationNum;
        seatNum = rhs.seatNum;
        for(int i = 0; i < stationNum; ++i){
            cnt[i] = rhs.cnt[i];
        }
        return *this;
    }

//从l->r共有多少票
    int QueryTicket(int l, int r) const {
        int ans{cnt[l]};
        for (int i = l + 1; i <= r; ++i) ans = std::min(ans, cnt[i]);
        return ans;
    }

//从l->r减去这么多座位数
    void BuyTickets(int l, int r, int v) {
        for (int i = l; i <= r; ++i) cnt[i] -= v;
    }

    bool operator==(const seat &t) const {
        if(stationNum != t.stationNum || seatNum != t.seatNum) return false;
        for(int i = 0; i < stationNum; ++i)
            if(cnt[i] != t.cnt[i]) return false;
        return true;
    }
    bool operator<(const seat &t) const { return false; }

};

struct order {
    int status;//1:success , 2:pending , 3:refunded
    int timestamp, cost, cnt;
    DATE dptDate;
    str<21> userName, trainID;
    str<41> dpt, arv;
    int order1, order2;
    DateTime dptTime, arvTime;

    explicit operator string() const {
        string stts;
        if (status == 1) stts = "[success]";
        else if (status == 2) stts = "[pending]";
        else if (status == 3) stts = "[refunded]";
        return stts + ' ' + string(trainID) + ' ' + string(dpt) + ' ' + string(dptTime) + " -> " + string(arv) + ' ' +
               string(arvTime) + ' ' + std::to_string(cost) + ' ' + std::to_string(cnt);
    }

    bool operator==(const order &t) const {
        return userName == t.userName && timestamp == t.timestamp;
    }

    bool operator<(const order &t) const { return false; }
};

//struct FindOrders {
//    string uid;
//    int timestamp;
//
//    FindOrders() : uid("~~~"), timestamp(114514) {};
//
//    FindOrders(const string &uid, const int &timestamp) : uid(uid), timestamp(timestamp) {};
//
//    FindOrders(const FindOrders &fs) : uid(fs.uid), timestamp(fs.timestamp) {};
//
//    friend bool operator<(const FindOrders &l, const FindOrders &r) {
//        if (r.uid == "~~~" && r.timestamp == 114514)return false;
//        if (l.uid == "~~~" && l.timestamp == 114514)return true;
//        if (l.uid == r.uid)return l.timestamp < r.timestamp;
//        return l.uid < r.uid;
//    }
//
//    friend bool operator==(const FindOrders &l, const FindOrders &r) {
//        return l.uid == r.uid && l.timestamp == r.timestamp;
//    }
//    friend bool operator>(const FindOrders &l, const FindOrders &r) {return !(l<r);}
//
//    };

//struct FindTickets {
//    DATE d;
//    string trainID;
//
//    FindTickets() : d("99:99"), trainID("~~~") {};
//
//    FindTickets(const DATE &d, const string &trainID) : d(d), trainID(trainID) {};
//
//    FindTickets(const FindTickets &fs) : d(fs.d), trainID(fs.trainID) {};
//
//    friend bool operator<(const FindTickets &l, const FindTickets &r) {
//        if (l.d == "99:99" && l.trainID == "~~~") return true;
//        if (r.d == "99:99" && l.trainID == "~~~") return false;
//        if (l.trainID == r.trainID) {
//            return l.d < r.d;
//        }
//        return l.trainID < r.trainID;
//    }
//
//    friend bool operator==(const FindTickets &l, const FindTickets &r) { return l.d == r.d && l.trainID == r.trainID; }
//    friend bool operator>(const FindTickets &l, const FindTickets &r) {return r<l;}
//
//};
//
//
//struct FindStations {
//    string trainID;
//    string stationID;
//
//    FindStations() : stationID(""), trainID("") {};
//
//    FindStations(const FindStations &fs) : stationID(fs.stationID), trainID(fs.trainID) {};
//
//    FindStations(const string &l, const string &r) : stationID(l), trainID(r) {};
//
//    friend bool operator<(const FindStations &l, const FindStations &r) {
//        if (l.stationID == r.stationID) return l.trainID < r.trainID;
//        return l.stationID < r.stationID;
//    }
//
//    friend bool operator==(const FindStations &l, const FindStations &r) {
//        return l.stationID == r.stationID && l.trainID == r.trainID;
//    }
//    friend bool operator>(const FindStations &l, const FindStations &r) {return r<l;}
//
//};

struct pending {
    int timestamp, need, order1, order2;
    str<21> userID;
    str<21> trainID;

    bool operator==(const pending &t) const {
        return timestamp == t.timestamp && userID == t.userID;
    }

    bool operator<(const pending &t) const { return false; }
};
//struct FindPending {
//    string trainID;
//    DATE dptDate;
//    int timestamp;
//};
//
//struct FindPD {
//    string trainID;
//    DATE dptDate;
//};

class TrainManager {
    B_PLUS_TREE::BPlusTree<str<21>, Train, 32768 / sizeof(Train) > trains;
    B_PLUS_TREE::BPlusTree<str<21>, trainDate, 32768 / sizeof(trainDate) > dates;
    B_PLUS_TREE::BPlusTree<FindStations, station, 32768 / sizeof(station) > station2;
//    B_PLUS_TREE::BPlusTree<BPlusTree<string, station> station1;
    B_PLUS_TREE::BPlusTree<FindTickets, seat, 32768 / sizeof(seat) > tickets;
//    B_PLUS_TREE::BPlusTree<string, order> orders;
    B_PLUS_TREE::BPlusTree<FindOrders, order, 32768 / sizeof(order) > orders;
    B_PLUS_TREE::BPlusTree<FindPending, pending, 32768 / sizeof(pending)> pendings;
//    B_PLUS_TREE::BPlusTree<FindPD, pending> pendings2;
public:
    TrainManager() : trains{"traindata"}, dates{"datedata"}, station2{"station2data"}, tickets{"ticketdata"},
                     orders{"orderdata"}, pendings{"pendingdata"} {};

    bool add_train(const string &i, const string &n, const string &m, const string &s, const string &p, const string &x,
                   const string &t, const string &o, const string &d, const string &y) {
        vector <Train> tmpTrain;
        trains.Find(i, tmpTrain);
        if(!tmpTrain.empty()){
            return 0;
        }
        Train newTrain;
        TokenScanner tmp{s};
        newTrain.trainID = i;
        newTrain.stationNum = std::stoi(n);
        newTrain.seatNum = std::stoi(m);
        int cnt = 0;
        std::string getToke;
        for (; tmp.pos <= tmp.Buffer.size();) {
            getToke = tmp.NextToken('|');
            if(getToke != ""){
                newTrain.stations[cnt++] = getToke;
                getToke.clear();
            }
        }
        tmp = p;
        cnt = 0;
        for (; tmp.pos <= tmp.Buffer.size();) {
            getToke = tmp.NextToken('|');
            if(getToke != ""){
                int k = std::stoi(getToke);
                if(k < 0){
                    cout << "hhhhhhhhhhhhhhhhhhhh" <<endl;
                }
                int a = newTrain.prices[cnt];
                int b = a + k;
                newTrain.prices[++cnt] = b;
                getToke.clear();
            }
        }
        newTrain.st = x;
        tmp = t;
        cnt = 0;
        for (; tmp.pos <= tmp.Buffer.size();) {
            getToke = tmp.NextToken('|');
            if(getToke != ""){
                newTrain.arvTime[++cnt] = std::stoi(getToke);
                getToke.clear();
            }
        }
        tmp = o;
        cnt = 0;
        for (; tmp.pos <= tmp.Buffer.size();) {
            getToke = tmp.NextToken('|');
            if (getToke != "_" && getToke != "")newTrain.dptTime[++cnt] = std::stoi(getToke);
        }
        tmp = d;
        newTrain.startSale = tmp.NextToken('|');
        newTrain.endSale = tmp.NextToken('|');
        newTrain.type = y[0];
        for (int i = 1; i < newTrain.stationNum; i++) {
            newTrain.arvTime[i] += newTrain.dptTime[i - 1];

            //arvTime[i]:到达第i+1站时间。
            newTrain.dptTime[i] += newTrain.arvTime[i];

            //dptTime[i]:从第i+1站出发时间.
        }
        trains.Insert(i, newTrain);
        //cout << "add_train : " << newTrain.seatNum << " ";
        return 1;
    };

    bool delete_train(const string &i) {
        std::vector<Train> tmpTrain;
        trains.Find(i, tmpTrain);
        if (tmpTrain.empty() || tmpTrain[0].release) return false;
        trains.Delete(i, tmpTrain[0]);
        return true;
    };

    bool release_train(const string &i) {
        std::vector<Train> tmpTrain;
        station tmpStation;
        trains.Find(i, tmpTrain);
        if (tmpTrain.empty()|| tmpTrain[0].release ) return false;
        trains.Delete(i, tmpTrain[0]);
        tmpTrain[0].release = true;
        //UPDATE
        trains.Insert(i, tmpTrain[0]);
        for (int it = 0; it < tmpTrain[0].stationNum; it++) {
            tmpStation = station{i, tmpTrain[0].st + tmpTrain[0].arvTime[it], tmpTrain[0].st + tmpTrain[0].dptTime[it],
                                 tmpTrain[0].prices[it], it};
            station2.Insert({string(tmpTrain[0].stations[it]), i}, tmpStation);
        }
        dates.Insert(i,{tmpTrain[0].stationNum,tmpTrain[0].seatNum,tmpTrain[0].startSale,tmpTrain[0].endSale});

        /*
        seat tmp;
        for(int i = 0; i < tmpTrain[0].stationNum; ++i){
            tmp.cnt[i] = tmpTrain[0].seatNum;
        }
        tmp.stationNum = tmpTrain[0].stationNum;
        tmp.seatNum = tmpTrain[0].seatNum;
        for(DATE i = tmpTrain[0].startSale; i <= tmpTrain[0].endSale; i = i + 1){
            tickets.Insert({i, string(tmpTrain[0].trainID)}, tmp);
        }

*/
        return 1;
    };

    string query_train(const DATE &d, const string &i) {
        string ret{"-1"};
        std::vector<Train> tmpTrain;
        std::vector<seat> tmpSeats;
        trains.Find(i, tmpTrain);
        if (!tmpTrain.empty()) {
            if (d < tmpTrain[0].startSale || d > tmpTrain[0].endSale) return "-1";
            ret = i + ' ' + tmpTrain[0].type + '\n';
            TIME dpt = tmpTrain[0].st;
            seat s;
            if (tmpTrain[0].release) {
                tickets.Find({d,i},tmpSeats);
                if(tmpSeats.empty()) tmpSeats.push_back(seat());
                s = tmpSeats[0];
            }

            for (int it = 0; it < tmpTrain[0].stationNum; ++it) {
                ret += string(tmpTrain[0].stations[it] ) + ' ';
                if (it)
                    ret += string(DateTime{d, dpt + tmpTrain[0].arvTime[it]}) + ' ';
                else
                    ret += "xx-xx xx:xx ";
                ret += "-> ";
                if (it + 1 < tmpTrain[0].stationNum)
                    ret += string(DateTime{d, dpt + tmpTrain[0].dptTime[it]}) + ' ';
                else
                    ret += "xx-xx xx:xx ";
                ret += std::to_string(tmpTrain[0].prices[it]) + ' ';
                if (it + 1 == tmpTrain[0].stationNum)
                    ret += 'x';
                else if (s.seatNum != -1)
                    ret += std::to_string(s.QueryTicket(it, it)) + '\n';
                else
                    ret += std::to_string(tmpTrain[0].seatNum) + '\n';
            }
        }
        return ret;
    }

    string query_ticket(const string &s, const string &t, const DATE &d, const string &p) {
        string ret, i;
        DATE D{d};
        std::vector<station> dptTrains;
        std::vector<station> arvTrains;
        std::vector<ticket> tk;
        std::vector<seat> st;
        station2.FindStations(s, dptTrains);
        station2.FindStations(t, arvTrains);
        for (auto it = dptTrains.begin(), itt = arvTrains.begin(); it != dptTrains.end(); it++) {
            while (itt != arvTrains.end() && itt->trainID < it->trainID) ++itt;
            if (itt == arvTrains.end()) break;
            if (it->trainID == itt->trainID && it->order < itt->order) {
                i = string(it->trainID);
                DATE dpt{D - it->dptTime.d};
                std::vector<Train> tmpTrains;
                trains.Find(i, tmpTrains);
                if(tmpTrains.empty()) continue;
                if (dpt < tmpTrains[0].startSale || tmpTrains[0].endSale < dpt) continue;
                st.clear();
                tickets.Find({dpt, i}, st);
                if(st.empty())st.push_back(seat());
                tk.push_back({it->trainID, DateTime{dpt, it->dptTime},
                              DateTime{dpt, itt->arvTime},
                              itt->sumPrice - it->sumPrice,
                              ~st[0].seatNum
                              ? st[0].QueryTicket(it->order, itt->order - 1)
                              : tmpTrains[0].seatNum,int(itt->arvTime) - int(it->dptTime)
                              });
            }
        }
        if (p == "time")Sort(tk.begin(), tk.end(), ticket::CmpTime);
        else if (p == "cost")Sort(tk.begin(), tk.end(), ticket::CmpCost);
        ret = std::to_string(tk.size());
        for (auto it: tk)
            ret += "\n" + string(it.trainID) + " " + s + " " + string(it.startTime) + " -> " + t + " " +
                   string(it.endTime) + " " + std::to_string(it.cost) + " " + std::to_string(it.seat);
        return ret;
    };

    string query_transfer(const string &s, const string &t, const DATE &d, const string &p) {
        vector<Train> tmpTrains;
        vector<seat> tmpSeats;
        string ret{"0"}, mid_str;
        TIME s_time, t_time;
        DATE s_dpt, t_dpt;
        DateTime mid_s, mid_t;
        vector<station> dept_trains;
        station2.FindStations(s, dept_trains);
        vector<station> arr_trains;
        station2.FindStations(t, arr_trains);
        ticket train1, train2;
        int the_time{INT32_MAX}, the_cost{INT32_MAX}, cur_cost, cur_time;
        // 减少到达列车的 I/O 次数。
        std::unordered_map<string, Train> t_trains;
        std::unordered_map<FindTickets, seat, PairHash<21>> t_tickets;
        if (arr_trains.empty()) return ret;
        for (auto s_it: dept_trains) {
            s_dpt = d - s_it.dptTime.d;
            trains.Find(string(s_it.trainID), tmpTrains);
            Train s_train = tmpTrains[0], t_train;
            tmpTrains.clear();
            if (s_dpt < s_train.startSale || s_train.endSale < s_dpt) continue;
            tickets.Find({s_dpt, string(s_it.trainID)}, tmpSeats);
            if(tmpSeats.empty()){
                tmpSeats.push_back(seat());
            }
            seat s_ticket = tmpSeats[0], t_ticket;
            tmpSeats.clear();
            std::unordered_map<string, int> stat_order;
            for (int i = s_it.order + 1; i < s_train.stationNum; ++i)
                stat_order[string(s_train.stations[i])] = i;
            for (auto t_it: arr_trains) {
                if (s_it.trainID == t_it.trainID) continue;
                if (t_trains.find(string(t_it.trainID)) != t_trains.end())
                    t_train = t_trains[string(t_it.trainID)];
                else {
                    trains.Find(string(t_it.trainID), tmpTrains);
                    t_trains[string(t_it.trainID)] = t_train = tmpTrains[0];
                    tmpTrains.clear();
                }
                for (int i = 0; i < t_it.order; ++i) {
                    string trans{string(t_train.stations[i])};
                    if (stat_order.find(trans) == stat_order.end()) continue;
                    s_time = s_train.st + s_train.arvTime[stat_order[trans]];
                    mid_s = {s_dpt, s_time};
                    // 接下来算出下一班车的理论最早发车日期。
                    t_time = t_train.st + t_train.dptTime[i];
                    t_dpt = mid_s.date - t_time.d + (t_time < s_time);
                    // 时间的小于号不看 day.
                    t_dpt = std::max(t_dpt, t_train.startSale);
                    if (t_dpt > t_train.endSale) continue;
                    if (t_tickets.find({t_dpt, string(t_it.trainID)}) != t_tickets.end())
                        t_ticket = t_tickets[{t_dpt, string(t_it.trainID)}];
                    else {
                        tickets.Find({t_dpt, string(t_it.trainID)}, tmpSeats);
                        if(tmpSeats.empty())tmpSeats.push_back(seat());
                        t_tickets[{t_dpt, string(t_it.trainID)}] = t_ticket =
                                tmpSeats[0];
                        tmpSeats.clear();
                    }
                    mid_t = {t_dpt, t_time};
                    cur_cost = s_train.prices[stat_order[trans]] - s_it.sumPrice +
                               t_it.sumPrice - t_train.prices[i];
                    cur_time = s_train.arvTime[stat_order[trans]] -
                               s_train.dptTime[s_it.order] +
                               t_train.arvTime[t_it.order] - t_train.dptTime[i] +
                               (mid_t - mid_s);
                    bool is_better{0};
                    if (p=="time") {
                        if (cur_time == the_time)
                            if (cur_cost == the_cost)
                                if (train1.trainID == s_it.trainID)
                                    is_better = t_it.trainID < train2.trainID;
                                else
                                    is_better = s_it.trainID < train1.trainID;
                            else
                                is_better = cur_cost < the_cost;
                        else
                            is_better = cur_time < the_time;
                    } else {
                        if (cur_cost == the_cost)
                            if (cur_time == the_time)
                                if (train1.trainID == s_it.trainID)
                                    is_better = t_it.trainID < train2.trainID;
                                else
                                    is_better = s_it.trainID < train1.trainID;
                            else
                                is_better = cur_time < the_time;
                        else
                            is_better = cur_cost < the_cost;
                    }

                    if (is_better) {
                        the_time = cur_time, the_cost = cur_cost;
                        mid_str = trans;
                        train1 = {s_it.trainID,
                                  DateTime{s_dpt, s_it.dptTime},
                                  mid_s,

                                  s_train.prices[stat_order[trans]] - s_it.sumPrice,
                                  ~s_ticket.seatNum ? s_ticket.QueryTicket(
                                          s_it.order, stat_order[trans] - 1)
                                                    : s_train.seatNum,
                                  0
                                  };
                        train2 = {t_it.trainID,
                                  mid_t,
                                  DateTime{t_dpt, t_it.arvTime},
                                  t_it.sumPrice - t_train.prices[i],
                                  ~t_ticket.seatNum ? t_ticket.QueryTicket(i, t_it.order - 1)
                                                    : t_train.seatNum,
                                  0  // 没有必要计算时间。
                                  };
                    }
                }
            }
        }
        if (mid_str.length()) {
            ret = string(train1.trainID) + ' ' + s + ' ' +
                  string(train1.startTime) + " -> " + mid_str + ' ' +
                  string(train1.endTime) + ' ' + std::to_string(train1.cost) + ' ' +
                  std::to_string(train1.seat);
            ret += '\n' + string(train2.trainID) + ' ' + mid_str + ' ' +
                   string(train2.startTime) + " -> " + t + ' ' +
                   string(train2.endTime) + ' ' + std::to_string(train2.cost) +
                   ' ' + std::to_string(train2.seat);
        }
        return ret;
    };

    string buy_ticket(const string &u, const string &i, const DATE &d, const int &n, const string &f, const string &t,
                      const bool &q, const int &timestamp) {
        string ret;
        vector<station> sit, tit;
        vector<trainDate> seg;
        vector<seat> seats;
        dates.Find(i, seg);
        station2.Find({f, i}, sit);
        station2.Find({t, i}, tit);
        if(loginUser.find(u) == loginUser.cend()) return "-1";
        if ( sit.empty() || tit.empty() || sit[0].order==-1 || tit[0].order==-1 || sit[0].order >= tit[0].order) return "-1";
        DATE dptDate{d - sit[0].dptTime.d};
        if(seg.empty()) return "-1";
        if (dptDate < seg[0].begin || seg[0].end < dptDate || seg[0].seatNum < n) return "-1";
        tickets.Find({dptDate, i}, seats);
        if(seats.empty()){
            seats.push_back(seat());
        }
        seat tmpseat = seats[0];
        //seats.push_back(seats[0]);
        int left;
        bool buy{seats[0].seatNum != -1};
        if (!buy) seats[0] = seat{seg[0].stationNum, seg[0].seatNum}, left = seg[0].seatNum;//发布后没买过
        else left = seats[0].QueryTicket(sit[0].order, tit[0].order - 1);//发布后买过
        if(left < n && !q) return "-1";
        order newOrder{1, timestamp, tit[0].sumPrice - sit[0].sumPrice, n, dptDate, u, i, f, t, sit[0].order,
                       tit[0].order, DateTime{dptDate, sit[0].dptTime}, DateTime{dptDate, tit[0].arvTime}};
        //  ORDERORDERORDERORDERORDERORDERORDERORDERORDERORDERORDERORDERORDERORDER

        if (left >= n) {
            seats[0].BuyTickets(sit[0].order, tit[0].order - 1, n);

            if (buy){
                if(tickets.Update({dptDate, i}, tmpseat, seats[0]) == 0){
                    cout << "wwwwwwwwwwwwwwwwwwwwwwwwwwww" << endl;
                    cout << tmpseat.seatNum << endl;
                }
            }

            else {
                vector<seat> tmp;
                tickets.Find({dptDate, i}, tmp);
                if(!tmp.empty()){
                    if(!tickets.Update({dptDate, i}, tmp[0], seats[0])){
                        cout << "wwwwwwwwwwwwwwwwwwwwwwwwwwww" << endl;
                        cout << tmpseat.seatNum << endl;
                    }
                }
                else{
                    if(seats[0].seatNum == -1){
                        cout << "-----------------------------------" << endl;
                    }
                    tickets.Insert({dptDate, i}, seats[0]);
                }
            }
            ret = std::to_string(1ll * n * (tit[0].sumPrice - sit[0].sumPrice));

            //cout << "left>=n" << ": " << string(dptDate) << endl;
            //for(int i = 0; i<=seats[1].stationNum;i++)
                //cout << seats[1].cnt[i] << " " << seats[0].cnt[i] <<endl;

        } else {
            newOrder.status = 2;
            pending pending1{timestamp, n, sit[0].order, tit[0].order, u, i};
            pendings.Insert({{i, dptDate}, timestamp}, pending1);
            ret = "queue";
        }
        orders.Insert({u,-timestamp},newOrder);



        return ret;
    };

    string query_order(const string &u) {
        string ret;
        vector<order> userOrders;
        //orders.Find(u, userOrders);
        if(loginUser.find(u) == loginUser.cend()) return "-1";


        orders.FindOrders(u, userOrders);
        if(userOrders.empty()) return "0";
        ret = std::to_string(userOrders.size());
        for (auto it: userOrders) ret += '\n' + string(it);

        return ret;
    };

    /*
     * status:
     * 1:success;
     * 2:failed;
     * 3:refunded;
     */

    bool refund_ticket(const string &u, const int &n) {
        vector<order> userOrders;
        vector<pending> tmpPending;
//        orders.Find(u, userOrders);
        orders.FindOrders(u, userOrders);
        if (n > userOrders.size() || userOrders[n - 1].status == 3 || loginUser.find(u) == loginUser.cend()) {
            return false;}
        order newOrder = userOrders[n - 1];
        int prvStt = newOrder.status;
        newOrder.status = 3;
        DATE prvDptDate = newOrder.dptDate;
        orders.Update({u, -userOrders[n - 1].timestamp}, userOrders[n - 1], newOrder);
        if (prvStt == 2) {
//            pendings.Find({string(newOrder.trainID), prvDptDate, newOrder.timestamp}, tmpPending);
//            pendings.Delete({string(newOrder.trainID), prvDptDate, newOrder.timestamp}, tmpPending[0]);
//            tmpPending.clear();
//            pendings2.Find({string(newOrder.trainID), prvDptDate}, tmpPending);
//            pendings.Delete({string(newOrder.trainID), prvDptDate}, tmpPending[0]);
//            tmpPending.clear();
            pendings.Find({{string(newOrder.trainID), prvDptDate}, newOrder.timestamp}, tmpPending);
            if(!tmpPending.empty()) pendings.Delete({{string(newOrder.trainID), prvDptDate}, newOrder.timestamp}, tmpPending[0]);

        } else {
            vector<seat> tmpTk;
            tickets.Find({prvDptDate, string(newOrder.trainID)}, tmpTk);
            if(tmpTk.empty()){
                tmpTk.push_back(seat());
            }
            seat tmp = tmpTk[0];
            tmpTk[0].BuyTickets(newOrder.order1, newOrder.order2 - 1, -newOrder.cnt);
            vector<pending> pending2;
//            pendings2.Find({string(newOrder.trainID), prvDptDate}, pending2);
            pendings.FindPending(FindPD(string(newOrder.trainID), prvDptDate), pending2);
            for (auto it: pending2) {
                if (tmpTk[0].QueryTicket(it.order1, it.order2 - 1) >= it.need) {
                    vector<order> tmpOrder;
                    orders.Find({string(it.userID), -it.timestamp}, tmpOrder);
                    newOrder = tmpOrder[0];

//                    vector<order> tmpOrder;
//                    orders.Find(it.userID, tmpOrder);
//                    orders.FindOrders(string(it.userID), tmpOrder);
                    newOrder.status = 1;
                    tmpTk[0].BuyTickets(it.order1, it.order2 - 1, it.need);
                    orders.Update({string(it.userID), -it.timestamp}, tmpOrder[0], newOrder);
                    pendings.Find({{string(newOrder.trainID), prvDptDate}, it.timestamp}, tmpPending);
                    pendings.Delete({{string(newOrder.trainID), prvDptDate}, it.timestamp}, tmpPending[0]);
                    tmpPending.clear();
                    //cout << " refund test : "<< it.order1 << " " << it.order2 << " " <<it.need << " " << newOrder.dptTime << " " << string(newOrder.dptDate) <<endl;
                }
            }
            if(!tickets.Update({prvDptDate, string(newOrder.trainID)}, tmp, tmpTk[0])){
                cout << "wwwwwwwwwwwwwwwwwwwwwwwwwwww" << endl;
                cout << tmp.seatNum << endl;
            }

        }
        return 1;
    };

    void clear() {
        trains.Clear();
        dates.Clear();
        tickets.Clear();
        station2.Clear();
        orders.Clear();
        pendings.Clear();
    };
};


#endif //TICKETSYSTEM_CTRAINS_H
