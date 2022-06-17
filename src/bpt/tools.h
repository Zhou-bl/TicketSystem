//
// Created by 陆逸凡 on 2022/6/13.
//

#ifndef TICKETSYSTEM_TOOLS_H
#define TICKETSYSTEM_TOOLS_H
using std::string;
static int monthSDay[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
static int monthDay[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class TIME {
    int h, m;

public:
    TIME() {}

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

    int d;

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
    int m, d;;

    DATE(const int &m, const int &d) : m(m), d(d) {};

    explicit operator int() const { return d + monthSDay[m - 1]; }

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
        return lhs.d == r.d && lhs.d == r.d;
    }

    friend bool operator!=(const DATE &lhs, const DATE &rhs) {
        return !(lhs == rhs);
    }

    DATE operator+(const int &day) {
        DATE tmp(m, d + day);
        if (tmp.d > monthDay[m]) { tmp.d -= monthDay[tmp.m], m++; }
        return tmp;
    }

    DATE operator-(const int &day) const {
        DATE tmp(m, d - day);
        if (tmp.d < 1) { tmp.d += monthDay[--tmp.m]; }
        return tmp;
    }

    explicit operator string() const {
        string tmp = "00:00";
        tmp[0] += m / 10;
        tmp[1] += m % 10;
        tmp[3] += d / 10;
        tmp[4] += d % 10;
        return tmp;
    }
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

    str(const string &s) { strcpy(STR, s.c_str()); }

    explicit operator string() const { return STR; }

    char &operator[](const int &index) { return STR[index]; }

    const char &operator[](const int &index) const { return STR[index]; }

    friend bool operator<(const str<len> &l, const str<len> &r) {
        for (int i = 0; i < len; i++) {
            if (l[i] < r[i]) return 1;
            if (r[i] < l[i]) return 0;
        }
        return 0;
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
        for (int i = 0; i < len && l[i] != '\0' && r[i] != '\0'; i++) if (l[i] != r[i])return 0;
        return 1;
    }

    friend bool operator!=(const str<len> &l, const str<len> &r) {
        return !(l == r);
    }

    friend std::ostream &operator<<(std::ostream &o, const str<len> &s) {
        return o << s.str;
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

#endif //TICKETSYSTEM_TOOLS_H
