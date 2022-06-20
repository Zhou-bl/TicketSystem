#ifndef TICKETSYSTEM_COMMANDPARSER_H
#define TICKETSYSTEM_COMMANDPARSER_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "cTrains.h"
#include "cUsers.h"
#include "TokenScanner.h"
#include "tools.h"

class CommandParser {
    UserManager USER;
    TrainManager TRAIN;
public:
    std::string run(const string &t) {
        TokenScanner token{t};
        string key = token.NextToken('[', ']');
        if (!key.length())return "";
        int timeStamp = std::stoi(key);
        key = token.NextToken();
        if (key == "exit") return "[" + std::to_string(timeStamp) + "] " +  "bye";
        else if (key == "add_user") return "[" + std::to_string(timeStamp) + "] "+ parseAddUser(token);
        else if (key == "login") return "[" + std::to_string(timeStamp) + "] "+ parseLogin(token);
        else if (key == "logout") return "[" + std::to_string(timeStamp) + "] "+ parseLogout(token);
        else if (key == "query_profile") return "[" + std::to_string(timeStamp) + "] "+ parseQueryProfile(token);
        else if (key == "modify_profile") return "[" + std::to_string(timeStamp) + "] "+ parseModifyProfile(token);
        else if (key == "add_train") return "[" + std::to_string(timeStamp) + "] "+ parseAddTrain(token);
        else if (key == "delete_train") return "[" + std::to_string(timeStamp) + "] "+ parseDeleteTrain(token);
        else if (key == "release_train") return "[" + std::to_string(timeStamp) + "] "+ parseReleaseTrain(token);
        else if (key == "query_train") return "[" + std::to_string(timeStamp) + "] "+ parseQueryTrain(token);
        else if (key == "query_ticket") return "[" + std::to_string(timeStamp) + "] "+ parseQueryTicket(token);
        else if (key == "query_transfer") return "[" + std::to_string(timeStamp) + "] "+ parseQueryTransfer(token);
        else if (key == "buy_ticket") return "[" + std::to_string(timeStamp) + "] "+ parseBuyTicket(timeStamp,token);
        else if (key == "query_order") return "[" + std::to_string(timeStamp) + "] "+ parseQueryOrder(token);
        else if (key == "refund_ticket") return "[" + std::to_string(timeStamp) + "] "+ parseRefundTicket(token);
        else if (key == "rollback") return "[" + std::to_string(timeStamp) + "] "+ parseRollback(token);
        else if (key == "clean") return "[" + std::to_string(timeStamp) + "] "+ parseClean(token);
        else return "other cmd-----------------------------------------------------------!!!!!!!!!!!";
    };

//    bool add_user(const string &c, const string &u, const string &p, const string &n, const string &m, int g) {
    std::string parseAddUser(TokenScanner &token) {
        string key = token.NextToken(), c, u, p, n, m;
        int g;
        //key != ""
        while (!key.empty()) {
            if (key == "-c") c = token.NextToken();
            else if (key == "-u") u = token.NextToken();
            else if (key == "-p") p = token.NextToken();
            else if (key == "-n") n = token.NextToken();
            else if (key == "-m") m = token.NextToken();
            else if (key == "-g") g = std::stoi(token.NextToken());
            key = token.NextToken();
        }
        return USER.add_user(c, u, p, n, m, g) ? "0" : "-1";
    };

//    bool login(const string &u, const string &p) {
    std::string parseLogin(TokenScanner &token) {
        string key = token.NextToken(), u, p;
        while (!key.empty()) {
            if (key == "-u") u = token.NextToken();
            else if (key == "-p") p = token.NextToken();
            key = token.NextToken();
        }
        return USER.login(u, p) ? "0" : "-1";
    };

//    bool logout(const string &u) {
    std::string parseLogout(TokenScanner &token) {
        string key = token.NextToken(), u;
        while (!key.empty()) {
            if (key == "-u") u = token.NextToken();
            key = token.NextToken();
        }
        return USER.logout(u) ? "0" : "-1";
    };

//    string query_profile(const string &c, const string &u) {
    std::string parseQueryProfile(TokenScanner &token) {
        string key = token.NextToken(), c, u;
        while (!key.empty()) {
            if (key == "-c") c = token.NextToken();
            else if (key == "-u") u = token.NextToken();
            key = token.NextToken();
        }
        return USER.query_profile(c, u);
    };

//    string modify_profile(const string &c, const string &u, const string &p, const string &n, const string &m, int g) {
    std::string parseModifyProfile(TokenScanner &token) {
        string key = token.NextToken(), c, u, p = "", n = "", m = "";
        int g = -1;
        while (!key.empty()) {
            if (key == "-c") c = token.NextToken();
            else if (key == "-u") u = token.NextToken();
            else if (key == "-p") p = token.NextToken();
            else if (key == "-n") n = token.NextToken();
            else if (key == "-m") m = token.NextToken();
            else if (key == "-g") g = std::stoi(token.NextToken());
            key = token.NextToken();
        }
        return USER.modify_profile(c, u, p, n, m, g);
    };

//  bool add_train(const string &i, const string &n, const string &m, const string &s, const string &p, const string &x,
//                   const string &t, const string &o, const string &d, const string &y) {
    std::string parseAddTrain(TokenScanner &token) {
        string key = token.NextToken(), i, n, m, s, p, x, t, o, d, y;
        while (!key.empty()) {
            if (key == "-i") i = token.NextToken();
            else if (key == "-n") n = token.NextToken();
            else if (key == "-m") m = token.NextToken();
            else if (key == "-s") s = token.NextToken();
            else if (key == "-p") p = token.NextToken();
            else if (key == "-x") x = token.NextToken();
            else if (key == "-t") t = token.NextToken();
            else if (key == "-o") o = token.NextToken();
            else if (key == "-d") d = token.NextToken();
            else if (key == "-y") y = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.add_train(i, n, m, s, p, x, t, o, d, y) ? "0" : "-1";
    };
//    bool delete_train(const string &i) {

    std::string parseDeleteTrain(TokenScanner &token) {
        string key = token.NextToken(), i;
        while (!key.empty()) {
            if (key == "-i") i = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.delete_train(i) ? "0" : "-1";
    };
//    bool release_train(const string &i) {

    std::string parseReleaseTrain(TokenScanner &token) {
        string key = token.NextToken(), i;
        while (!key.empty()) {
            if (key == "-i") i = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.release_train(i) ? "0" : "-1";
    };
    //    string query_train(const DATE &d, const string &i) {

    std::string parseQueryTrain(TokenScanner &token) {
        DATE d;
        string key = token.NextToken(), i;
        while (!key.empty()) {
            if (key == "-d") d = token.NextToken();
            else if (key == "-i") i = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.query_train(d, i);
    };
    //   string query_ticket(const string &s, const string &t, const DATE &d, const string &p) {

    std::string parseQueryTicket(TokenScanner &token) {
        DATE d;
        string key = token.NextToken(), s, t, p;
        while (!key.empty()) {
            if (key == "-s") s = token.NextToken();
            else if (key == "-t") t = token.NextToken();
            else if (key == "-d") d = token.NextToken();
            else if (key == "-p") p = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.query_ticket(s, t, d, p);
    };

//    string query_transfer(const string &s, const string &t, const DATE &d, const string &p) {
    std::string parseQueryTransfer(TokenScanner &token) {
        DATE d;
        string key = token.NextToken(), s, t, p;
        while (!key.empty()) {
            if (key == "-s") s = token.NextToken();
            else if (key == "-t") t = token.NextToken();
            else if (key == "-d") d = token.NextToken();
            else if (key == "-p") p = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.query_transfer(s, t, d, p);
    };

//string buy_ticket(const string &u, const string &i, const DATE &d, const int &n, const string &f, const string &t,
//                      const bool &q, const int &timestamp) {
    std::string parseBuyTicket(const int &timestamp, TokenScanner &token) {
        string key = token.NextToken(), u, i, f, t, qValue;
        int n;
        bool q;
        DATE d;
        while (!key.empty()) {
            if (key == "-u") u = token.NextToken();
            else if (key == "-i") i = token.NextToken();
            else if (key == "-d") d = token.NextToken();
            else if (key == "-n") n = std::stoi(token.NextToken());
            else if (key == "-f") f = token.NextToken();
            else if (key == "-t") t = token.NextToken();
            else if (key == "-q") {
                qValue = token.NextToken();
                if (qValue == "true")q = true;
                else q = false;
            } else if (key == "-d") d = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.buy_ticket(u, i, d, n, f, t, q, timestamp);
    };

//    string query_order(const string &u) {
    std::string parseQueryOrder(TokenScanner &token) {
        string key = token.NextToken(), u;
        while (!key.empty()) {
            if (key == "-u") u = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.query_order(u);
    };

//    bool refund_ticket(const string &u, const int &n) {
    std::string parseRefundTicket(TokenScanner &token) {
        string key = token.NextToken(), u, n;
        while (!key.empty()) {
            if (key == "-u") u = token.NextToken();
            else if (key == "-n") n = token.NextToken();
            key = token.NextToken();
        }
        return TRAIN.refund_ticket(u, std::stoi(n)) ? "0" : "-1";
    };

    std::string parseRollback(TokenScanner &token) {
        string key = token.NextToken();
        int t;
        while (!key.empty()) {
            if (key == "-t") t = std::stoi(token.NextToken());
        }
        return "0";
    };

//    void clear() {
    std::string parseClean(TokenScanner &token) {
        USER.clear();
        TRAIN.clear();
        return "0";
    };

    std::string parseExit(TokenScanner &token) {
        return "bye";
    };


};

#endif //TICKETSYSTEM_COMMANDPARSER_H
