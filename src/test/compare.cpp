#include <bits/stdc++.h>
using namespace std;
/*
 * This a comparer for test;
 */
int main(){
    int cnt = 0;
    system("g++ src/test/test.cpp src/bpt/bpt.hpp -o src/data/tree");
    system("g++ src/test/maptest.cpp -o src/data/map");
    while(true){
        ++cnt;
        system("g++ src/test/datamaker.cpp -o src/data/datamaker");
        system("./src/data/datamaker");
        system("./src/data/tree");
        system("./src/data/map");
        system("rm src/data/treedata");
        if (system("diff src/data/tree.txt src/data/map.txt -b")) //当 fc 返回1时，说明这时数据不一样
            break;
        else{
            cout << "第" << cnt << "次测试, 找不到差异" << endl;
        }
    }
}