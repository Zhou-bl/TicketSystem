#ifndef TICKETSYSTEM_TOOLS_H
#define TICKETSYSTEM_TOOLS_H
#include <unordered_map>
using std::string;
using std::size_t;
static int monthSDay[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
static int monthDay[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class TIME {
public:
    int h{0}, m{0}, d{0};

public:
    TIME() { d = 0;}

    TIME(const string &t) {
        h = (t[0] - '0') * 10 + (t[1] - '0');
        m = (t[3] - '0') * 10 + (t[4] - '0');
        d = 0;
    }

    TIME operator+(const int &rhs) const {
        TIME ret;
        int total_time = (d * 24 + h) * 60 + m + rhs;
        ret.m = total_time % 60, total_time /= 60;
        ret.h = total_time % 24, total_time /= 24;
        ret.d = total_time;
        return ret;
    }

    friend bool operator<(const TIME &l, const TIME &r) {
        if (l.h == r.h) return l.m < r.m;
        return l.h < r.h;
    }

    bool operator == (const TIME &rhs) const {
        return h == rhs.h && m == rhs.m && d == rhs.d;
    }

    explicit operator string() const {
        string tmp = "00:00";
        tmp[0] += h / 10;
        tmp[1] += h % 10;
        tmp[3] += m / 10;
        tmp[4] += m % 10;
        return tmp;
    }

    explicit operator int() const { return (d * 24 + h) * 60 + m; }
};

class DATE {
    int m{0}, d{0};

    DATE(const int &m, const int &d) : m(m), d(d) {};

    friend int operator-(const DATE &lhs, const DATE &rhs) {
        return int(lhs) - int(rhs);
    }

public:
    DATE() {}

    DATE(const string &date) {
        m = (date[0] - '0') * 10 + date[1] - '0';
        d = (date[3] - '0') * 10 + date[4] - '0';
    }

    friend bool operator<(const DATE &lhs, const DATE &rhs) {
        if (lhs.m == rhs.m) return lhs.d < rhs.d;
        return lhs.m < rhs.m;
    }

    friend bool operator<=(const DATE &lhs, const DATE &rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const DATE &lhs, const DATE &rhs) { return rhs < lhs; }

    friend bool operator>=(const DATE &lhs, const DATE &rhs) {
        return !(lhs < rhs);
    }

    friend bool operator==(const DATE &lhs, const DATE &rhs) {
        return lhs.d == rhs.d && lhs.m == rhs.m;
    }

    friend bool operator==(const DATE &lhs, const string &rhs) {
        DATE r(rhs);
        return lhs.d == r.d && lhs.m == r.m;
    }

    friend bool operator!=(const DATE &lhs, const DATE &rhs) {
        return !(lhs == rhs);
    }

    DATE operator+(const int &day) {
        DATE tmp(m, d + day);
        if (tmp.d > monthDay[m]) { tmp.d -= monthDay[tmp.m], tmp.m++; }
        return tmp;
    }

    DATE operator-(const int &day) const {
        DATE tmp(m, d - day);
        if (tmp.d < 1) { tmp.d += monthDay[--tmp.m]; }
        return tmp;
    }

    explicit operator string() const {
        string tmp = "00-00";
        tmp[0] += m / 10;
        tmp[1] += m % 10;
        tmp[3] += d / 10;
        tmp[4] += d % 10;
        return tmp;
    }

    explicit operator int() const { return d + monthSDay[m - 1]; }
};

struct DateTime {
    DATE date;
    TIME time;

    DateTime() {}

    DateTime(const DATE &d, const TIME &t) : date{d}, time{t} {
        date = time.d > 0 ? date + time.d : date - time.d, time.d = 0;
    }

    explicit operator string() const { return string(date) + " " + string(time); }

    DateTime operator+(const int &rhs) const {
        DateTime ret{date, time + rhs};
        ret.date = ret.date + ret.time.d, ret.time.d = 0;
        return ret;
    }

    bool operator == (const DateTime &rhs) const {
        return  date == rhs.date && time == rhs.time;
    }

    friend int operator-(const DateTime &l, const DateTime &r) {
        return (l.date - r.date) * 1440 + (int(l.time) - int(r.time));
    }

    friend std::ostream &operator<<(std::ostream &l, const DateTime &r) {
        return l << string(r);
    }

};


template<int len>
class str {
    char STR[len + 1]{};
public:
    str() {};

    str(const string &s) {
        strcpy(STR, s.c_str());
    }

    explicit operator string() const { return STR; }

    char &operator[](const int &index) { return STR[index]; }

    const char &operator[](const int &index) const { return STR[index]; }

    friend bool operator<(const str<len> &l, const str<len> &r) {
        return string(l.STR) < string(r.STR);
    }

    friend bool operator<=(const str<len> &l, const str<len> &r) {
        return !(r < l);
    }

    friend bool operator>(const str<len> &l, const str<len> &r) {
        return r < l;
    }

    friend bool operator>=(const str<len> &l, const str<len> &r) {
        return !(l < r);
    }

    friend bool operator==(const str<len> &l, const str<len> &r) {
        return string(l.STR) == string(r.STR);
    }

    bool operator==(const string &rhs)const{
        return string(STR) == rhs;
    }

    friend bool operator!=(const str<len> &l, const str<len> &r) {
        return !(l == r);
    }

    friend std::ostream &operator<<(std::ostream &o, const str<len> &s) {
        return o << s.STR;
    }
};

template<class Iter, class Compare = std::less<typename Iter::value_type>>
void Sort(Iter first, Iter last, Compare Cmp = Compare{}) {
    if (first == last) return;
    Iter l{first}, r{last};
    std::swap(*l, *(l + rand() % (r - l)));
    typename Iter::value_type x{*l};
    for (--r; l != r;) {
        while (l != r && !Cmp(*r, x)) --r;
        *l = *r;
        while (l != r && !Cmp(x, *l)) ++l;
        *r = *l;
    }
    *l = x;
    Sort(first, l, Cmp);
    Sort(++l, last, Cmp);
}


struct FindOrders {
    str<21> uid;
    int timestamp;

    FindOrders() : uid("~~~"), timestamp(114514) {};

    FindOrders(const string &uid, const int &timestamp) : uid(uid), timestamp(timestamp) {};

    FindOrders(const FindOrders &fs) : uid(fs.uid), timestamp(fs.timestamp) {};

    friend bool operator<(const FindOrders &l, const FindOrders &r) {
        if (r.uid == str<21>("~~~") && r.timestamp == 114514)return false;
        if (l.uid == str<21>("~~~") && l.timestamp == 114514)return true;
        if (l.uid == r.uid)return l.timestamp < r.timestamp;
        return l.uid < r.uid;
    }

    friend bool operator==(const FindOrders &l, const FindOrders &r) {
        return l.uid == r.uid && l.timestamp == r.timestamp;
    }
    friend bool operator>(const FindOrders&l, const FindOrders &r) {
        return r<l;
    }

    friend bool operator>=(const FindOrders &l, const FindOrders &r) {
        return !(l<r);
    }

    friend bool operator<=(const FindOrders &l, const FindOrders &r) {
        return !(l>r);
    }
};

struct FindTickets {
public:
    DATE d;
    str<21> trainID;

    FindTickets() : d("99:99"), trainID("~~~") {};

    FindTickets(const DATE &d, const string &trainID) : d(d), trainID(trainID) {};

    FindTickets(const FindTickets &fs) : d(fs.d), trainID(fs.trainID) {};

    friend bool operator<(const FindTickets &l, const FindTickets &r) {
        if (l.d == "99:99" && l.trainID == str<21>("~~~")) return true;
        if (r.d == "99:99" && r.trainID == str<21>("~~~")) return false;
        if (l.trainID == r.trainID) {
            return l.d < r.d;
        }
        return l.trainID < r.trainID;
    }
    friend bool operator>(const FindTickets&l, const FindTickets &r) {
        return r<l;
    }

    friend bool operator>=(const FindTickets &l, const FindTickets &r) {
        return !(l<r);
    }

    friend bool operator<=(const FindTickets &l, const FindTickets &r) {
        return !(l>r);
    }

    friend bool operator==(const FindTickets &l, const FindTickets &r) { return l.d == r.d && l.trainID == r.trainID; }
};

struct FindStations {
    str<21> trainID;
    str<41> stationID;

    FindStations() : stationID(""), trainID("") {};

    FindStations(const FindStations &fs) : stationID(fs.stationID), trainID(fs.trainID) {};

    FindStations(const string &l, const string &r) : stationID(l), trainID(r) {};

    friend bool operator<(const FindStations &l, const FindStations &r) {
        if (l.stationID == r.stationID) return l.trainID < r.trainID;
        return l.stationID < r.stationID;
    }

    friend bool operator>(const FindStations &l, const FindStations &r) {
        return r<l;
    }

    friend bool operator>=(const FindStations &l, const FindStations &r) {
        return !(l<r);
    }

    friend bool operator<=(const FindStations &l, const FindStations &r) {
        return !(l>r);
    }

    friend bool operator==(const FindStations &l, const FindStations &r) {
        return l.stationID == r.stationID && l.trainID == r.trainID;
    }
};

struct FindPD {
    str<41> trainID;
    DATE dptDate;

    FindPD():trainID(""), dptDate("99:99"){}

    FindPD(const string &str, const DATE &date){
        trainID = str;
        dptDate = date;
    }

    bool operator<(const FindPD &rhs)const{
        if(trainID == "" && dptDate == "99:99")return true;
        if(trainID == rhs.trainID)
            return dptDate < rhs.dptDate;
        return trainID < rhs.trainID;
    }
    bool operator==(const FindPD &rhs)const{
        return trainID == rhs.trainID && dptDate == rhs.dptDate;
    }
    bool operator>(const FindPD &rhs)const{
        return !(*this < rhs) && !(*this == rhs);
    }
    bool operator<=(const FindPD &rhs)const{
        return !(*this > rhs);
    }
    bool operator>=(const FindPD &rhs)const{
        return !(*this < rhs);
    }
};

struct FindPending{
    FindPD PD;
    int timestamp;

    FindPending():PD(),timestamp(0){}

    FindPending(const FindPD &tmpPD, const int tmpT){
        PD = tmpPD;
        timestamp = tmpT;
    }

    FindPending &operator=(const FindPending &rhs){
        if(this == &rhs)
            return *this;
        PD = rhs.PD;
        timestamp = rhs.timestamp;
        return *this;
    }

    bool operator<(const FindPending &rhs)const{
        if(PD == rhs.PD)
            return timestamp < rhs.timestamp;
        return PD < rhs.PD;
    }
    bool operator==(const FindPending &rhs)const{
        return PD == rhs.PD && timestamp == rhs.timestamp;
    }
    friend bool operator>(const FindPending&l, const FindPending &r) {
        return r<l;
    }

    friend bool operator>=(const FindPending &l, const FindPending &r) {
        return !(l<r);
    }

    friend bool operator<=(const FindPending &l, const FindPending &r) {
        return !(l>r);
    }
};

template <int len>
struct PairHash {
    size_t operator()(const FindTickets &obj) const {
        size_t ret{0};
        for (int i = 0; i < len && obj.trainID[i]; ++i) ret = ret * 257 + obj.trainID[i];
        return ret + int(obj.d);
    }
};
#endif //TICKETSYSTEM_TOOLS_H
