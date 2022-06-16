//
// Created by zbl on 2022/6/9.
//
#include "../bpt/bpt.hpp"
#include <iostream>
#include <vector>
using namespace std;
using B_PLUS_TREE::BPlusTree;
class T{
public:
    int a, b;
    T():a(0), b(0){}
    T(int aa, int bb){
        a = aa;
        b = bb;
    }
    T &operator=(const T &rhs){
        if(this == &rhs) return *this;
        a = rhs.a;
        b = rhs.b;
        return *this;
    }
    bool operator==(const T &rhs) const {
        return a == rhs.a && b == rhs.b;
    }
    bool operator<(const T &rhs) const {
        return a < rhs.a;
    }
    friend ostream &operator << (ostream &ou, const T &obj){
        cout << obj.a << " " << obj.b;
        return ou;
    }

};
int main(){

    freopen("src/data/data.txt","r",stdin);
    freopen("src/data/tree.txt","w",stdout);

    BPlusTree<int, T> tree("treedata");
    int n, k;
    vector<T> ans;
    string op;
    int s;
    cin >> n;
    for(int i = 1; i <= n; ++i){
        cout << "for" << i << " ";
        //cout << "root : "<<tree.get_root_index() << " ";
        cin >> op;
        if(op == "insert"){
            int aa , bb;
            cin >> s >> aa >>bb;
            tree.Insert(s, T(aa, bb));
            cout << endl;
        }
        if(op == "find"){
            cin >> s;
            tree.Find(s, ans);
            if(ans.size()){
                for(int j = 0; j < ans.size(); ++j)
                    cout << ans[j] << " ";
                ans.clear();
            }
            else{
                cout << "null";
            }
            cout << endl;
        }
        if(op == "delete"){
            int aa, bb;
            cin >> s >> aa >> bb;
            tree.Delete(s, T(aa, bb));
            cout << endl;
        }
    }





    return 0;
}