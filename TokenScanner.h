//
// Created by 陆逸凡 on 2022/5/26.
//

#ifndef TICKETSYSTEM_TOKENSCANNER_H
#define TICKETSYSTEM_TOKENSCANNER_H

#include <iostream>

using std::string;

class TokenScanner {
public:
    TokenScanner() = default;

    TokenScanner(const string &str) : pos{0}, Buffer{str} {};

    TokenScanner(const TokenScanner &t) : pos{t.pos}, Buffer{t.Buffer} {};

    string NextToken(char delim = ' ') {
        if (pos >= Buffer.size()) {
            ++pos;
            return "";
        }
        size_t l;
        while (pos < Buffer.size() && Buffer[pos] == delim)pos++;
        l = pos;
        while (pos < Buffer.size() && Buffer[pos] != delim)pos++;
        return Buffer.substr(l, pos - l);
    }

    string NextToken(char delimL, char delimR, bool ck = 0) {
        if (pos == Buffer.size()) return "";
        while (pos < Buffer.size() && Buffer[pos] != delimL)++pos;
        size_t l = pos + 1 - ck;
        while (pos < Buffer.size() && Buffer[pos] != delimR)++pos;
        return Buffer.substr(l, pos++ - l);  // 修改：++
    }

    void Clear() {
        Buffer.clear();
        pos = 0;
    }

    string getLeft(char delim = ' ') {
        if (pos == Buffer.size()) return "";
        while (pos < Buffer.size() && Buffer[pos] == delim) pos++;
        if (pos == Buffer.size()) return "";
        size_t l = Buffer.size() - 1;
        while (l >= pos && Buffer[l] == delim)l--;
        return Buffer.substr(pos,l+1-pos);
    }

    size_t  pos = 0;
    string Buffer;
};

#endif //TICKETSYSTEM_TOKENSCANNER_H
