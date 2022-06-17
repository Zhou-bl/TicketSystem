#ifndef TRAINTICKETSYSTEM_BPT_HPP
#define TRAINTICKETSYSTEM_BPT_HPP

/*
 * This B+Tree is for any type ValueType;
 * And this B+Tree support any KeyType to ValueType;
 * But KeyType() must be the minus in all KeyType;
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <fstream>
#include <type_traits>
#include <vector> //临时使用
#include "file.hpp"
#include "tools.h"

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::vector;
using std::is_same;

namespace B_PLUS_TREE {

    const int MAX_CHILDREN = 50;
    const int MIN_CHILDREN = MAX_CHILDREN >> 1;

    /*
     * T类型需要:默认构造函数T(),[operator=],[operator==],[operator<](随便写,只要有就行);
     */

    struct FindOrders {
        str<22> uid;
        int timestamp;

        FindOrders() : uid("~~~"), timestamp(114514) {};

        FindOrders(const string &uid, const int &timestamp) : uid(uid), timestamp(timestamp) {};

        FindOrders(const FindOrders &fs) : uid(fs.uid), timestamp(fs.timestamp) {};

        friend bool operator<(const FindOrders &l, const FindOrders &r) {
            if (r.uid == str<22>("~~~") && r.timestamp == 114514)return false;
            if (l.uid == str<22>("~~~") && l.timestamp == 114514)return true;
            if (l.uid == r.uid)return l.timestamp < r.timestamp;
            return l.uid < r.uid;
        }

        friend bool operator==(const FindOrders &l, const FindOrders &r) {
            return l.uid == r.uid && l.timestamp == r.timestamp;
        }
    };

    struct FindTickets {
        DATE d;
        str<21> trainID;

        FindTickets() : d("99:99"), trainID("~~~") {};

        FindTickets(const DATE &d, const string &trainID) : d(d), trainID(trainID) {};

        FindTickets(const FindTickets &fs) : d(fs.d), trainID(fs.trainID) {};

        friend bool operator<(const FindTickets &l, const FindTickets &r) {
            if (l.d == "99:99" && l.trainID == str<21>("~~~")) return true;
            if (r.d == "99:99" && l.trainID == str<21>("~~~")) return false;
            if (l.trainID == r.trainID) {
                return l.d < r.d;
            }
            return l.trainID < r.trainID;
        }

        friend bool operator==(const FindTickets &l, const FindTickets &r) { return l.d == r.d && l.trainID == r.trainID; }
    };

    struct FindStations {
        str<21> trainID;
        str<32> stationID;

        FindStations() : stationID(""), trainID("") {};

        FindStations(const FindStations &fs) : stationID(fs.stationID), trainID(fs.trainID) {};

        FindStations(const string &l, const string &r) : stationID(l), trainID(r) {};

        friend bool operator<(const FindStations &l, const FindStations &r) {
            if (l.stationID == r.stationID) return l.trainID < r.trainID;
            return l.stationID < r.stationID;
        }

        friend bool operator==(const FindStations &l, const FindStations &r) {
            return l.stationID == r.stationID && l.trainID == r.trainID;
        }
    };

    struct FindPD {
        string trainID;
        DATE dptDate;

        FindPD():trainID(""), dptDate("99:99"){}

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
    };

    template<typename KeyType, typename ValueType>
    struct KeyValue {//键值对
        KeyType key;
        ValueType value;

        KeyValue() : key(KeyType()), value(ValueType()){}

        KeyValue(const ValueType &Value) : key(KeyType()) {
            value = Value;
        }

        KeyValue(const KeyType &Key, const ValueType &Value) {
            key = Key;
            value = Value;
        }

        KeyValue &operator=(const KeyValue &rhs) {
            if (this == &rhs) return *this;
            key = rhs.key;
            value = rhs.value;
            return *this;
        }

        bool operator==(const KeyValue &rhs) const {
            return value == rhs.value && key == rhs.key;
        }

        bool operator!=(const KeyValue &rhs) const {
            return !(*this == rhs);
        }

        bool operator<(const KeyValue &rhs) const {
            KeyType lkey = key, rkey = rhs.key;
            if (lkey == rkey) return value < rhs.value;
            return lkey < rkey;
        }

        bool operator>(const KeyValue &rhs) const {
            return !(*this < rhs) && *this != rhs;
        }

        bool operator<=(const KeyValue &rhs) const {
            return !(*this > rhs);
        }

        bool operator>=(const KeyValue &rhs) const {
            return !(*this < rhs);
        }

    };

    template<typename KeyType, typename ValueType>
    class Node {//B+树的结点,叶子和非叶结点都是Node
    private:
        bool is_leaf;
        //判断是否是叶子结点
        int num;
        //分支数目 或 键值对数目;
        int index, next, pre, fa;
        //index:该节点在文件中的位置; next & pre:for叶子结点的顺序访问;
        KeyValue<KeyType, ValueType> children[MAX_CHILDREN + 5];
        //存储关键字
        int chIndex[MAX_CHILDREN + 5];
        //子节点的位置,当为叶子节点时，无意义;
    public:
        template<typename KeyName, typename ValueName>
        friend class BPlusTree;

        Node(bool type = 0) {
            is_leaf = type;
            num = 0;
            next = pre = fa = -1;
        }

    };

    template<typename KeyType, typename ValueType>
    class BPlusTree {
    private:
        MemoryRiver<Node<KeyType, ValueType>, 6> tree;
        int root_index, head_index, tail_index, num;
        //root_index 用于索引查找,对应memoryRiver中的第1个int; leaf_index用于顺序访问,对应memoryRiver的第2个int;
        //root_index 记录根节点在文件中的位置; leaf_index记录关键字最小的叶结点的位置;
    private:
        void Initialize() {
            tree.get_info(root_index, 3);
            tree.get_info(head_index, 4);
            tree.get_info(tail_index, 5);
            tree.get_info(num, 6);
            if (!root_index && !head_index && !tail_index) {//空文件,新建根节点和叶子结点;
                Node<KeyType, ValueType> new_root(0), new_head(1), new_tail(1);
                new_root.index = root_index = tree.write(new_root);
                new_head.index = head_index = tree.write(new_head);
                new_tail.index = tail_index = tree.write(new_tail);
                new_head.next = tail_index;
                new_tail.pre = head_index;
                new_head.fa = root_index;
                new_tail.fa = root_index;
                tree.write_info(root_index, 3);
                tree.write_info(head_index, 4);
                tree.write_info(tail_index, 5);
                tree.update(new_root, root_index);
                tree.update(new_head, head_index);
                tree.update(new_tail, tail_index);
            }
        }

        void SplitLeaf(Node<KeyType, ValueType> tmp) {//叶子结点
            Node<KeyType, ValueType> newLeaf(1);
            newLeaf.index = tree.write(newLeaf);
            newLeaf.fa = tmp.fa;
            tmp.num = MAX_CHILDREN + 1 >> 1;
            for (int i = MAX_CHILDREN + 1 >> 1; i <= MAX_CHILDREN; ++i) {
                newLeaf.children[newLeaf.num++] = tmp.children[i];
                tmp.children[i] = KeyValue<KeyType, ValueType>();
            }
            if (tmp.index == tail_index) {
                tail_index = newLeaf.index;
                tmp.next = newLeaf.index;
                newLeaf.pre = tmp.index;
                tree.update(tmp, tmp.index);
                tree.update(newLeaf, newLeaf.index);
                tree.write_info(tail_index, 5);
            } else {
                Node<KeyType, ValueType> nextLeaf;
                tree.read(nextLeaf, tmp.next);
                newLeaf.next = nextLeaf.index;
                newLeaf.pre = tmp.index;
                tmp.next = newLeaf.index;
                nextLeaf.pre = newLeaf.index;
                tree.update(tmp, tmp.index);
                tree.update(newLeaf, newLeaf.index);
                tree.update(nextLeaf, nextLeaf.index);
            }
            UpInsert(newLeaf.fa, newLeaf.children[0], newLeaf.index); //向根节点中插入新的关键字和index;
        }

        void SplitNonLeaf(Node<KeyType, ValueType> tmp) {
            Node<KeyType, ValueType> newNode(0);
            newNode.index = tree.write(newNode);
            tmp.num = MAX_CHILDREN + 1 >> 1;
            newNode.fa = tmp.fa;
            for (int i = MAX_CHILDREN + 1 >> 1; i <= MAX_CHILDREN; ++i) {
                newNode.children[newNode.num] = tmp.children[i];
                tmp.children[i] = KeyValue<KeyType, ValueType>();
                newNode.chIndex[newNode.num] = tmp.chIndex[i];
                ChangeFa(newNode.chIndex[newNode.num], newNode.index);
                newNode.num++;
            }
            ////newNode.children[MAX_CHILDREN + 1 >> 1] = KeyValue<KeyType, ValueType>(); //新节点的第一个键值对赋为初始值(最小值);
            newNode.children[0] = KeyValue<KeyType, ValueType>();
            tree.update(tmp, tmp.index);
            tree.update(newNode, newNode.index);
            KeyValue<KeyType, ValueType> minKeyValue = FindMin(newNode);
            UpInsert(newNode.fa, minKeyValue, newNode.index);
        }

        void SplitRoot(Node<KeyType, ValueType> tmp) {
            Node<KeyType, ValueType> newNode(0), newRoot(0);
            newNode.index = tree.write(newNode);
            newRoot.index = tree.write(newRoot);
            tmp.num = MAX_CHILDREN + 1 >> 1;
            newNode.fa = newRoot.index;
            tmp.fa = newRoot.index;
            for (int i = MAX_CHILDREN + 1 >> 1; i <= MAX_CHILDREN; ++i) {
                newNode.children[newNode.num] = tmp.children[i];
                tmp.children[i] = KeyValue<KeyType, ValueType>();
                newNode.chIndex[newNode.num] = tmp.chIndex[i];
                ChangeFa(newNode.chIndex[newNode.num], newNode.index);
                newNode.num++;
            }
            ////newNode.children[MAX_CHILDREN + 1 >> 1] = KeyValue<KeyType, ValueType>();
            newNode.children[0] = KeyValue<KeyType, ValueType>();
            KeyValue<KeyType, ValueType> minKey = FindMin(newNode);
            newRoot.num = 2;
            newRoot.children[1] = minKey;
            newRoot.chIndex[0] = tmp.index;
            newRoot.chIndex[1] = newNode.index;
            root_index = newRoot.index;
            tree.write_info(root_index, 3);
            tree.update(newRoot, newRoot.index);
            tree.update(newNode, newNode.index);
            tree.update(tmp, tmp.index);
        }

        KeyValue<KeyType, ValueType> FindMin(Node<KeyType, ValueType> tmp) {
            if (tmp.is_leaf) {
                return tmp.children[0];
            } else {
                Node<KeyType, ValueType> minCh;
                tree.read(minCh, tmp.chIndex[0]);
                return FindMin(minCh);
            }
        }

        void ChangeFa(int chPos, int faPos) {
            Node<KeyType, ValueType> ch;
            tree.read(ch, chPos);
            ch.fa = faPos;
            tree.update(ch, ch.index);
        }

        void UpInsert(int index, KeyValue<KeyType, ValueType> newPair, int newIndex) {//在非叶结点中插入新的键值对;
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            int l = 0, r = tmp.num - 1, mid, pos;
            while (l <= r) {
                mid = l + r >> 1;
                if (newPair >= tmp.children[mid]) {
                    pos = mid + 1;
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
            for (int i = ++tmp.num - 1; i > pos; --i) {
                tmp.children[i] = tmp.children[i - 1];
                tmp.chIndex[i] = tmp.chIndex[i - 1];
            }
            tmp.children[pos] = newPair;
            tmp.chIndex[pos] = newIndex;
            tree.update(tmp, tmp.index);
            if (tmp.num > MAX_CHILDREN) {
                if (tmp.fa != -1) {
                    SplitNonLeaf(tmp);
                } else {
                    SplitRoot(tmp);
                }
            }
        }

        void UpChange(int index, KeyValue<KeyType, ValueType> las, KeyValue<KeyType, ValueType> cur){
            if(index == -1) return; //边界条件;
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            int l = 1, r = tmp.num - 1, mid, pos = -1;
            while(l <= r){
                mid = l + r >> 1;
                if(tmp.children[mid] == las){
                    pos = mid;
                    break;
                }
                if(tmp.children[mid] < las){
                    l = mid + 1;
                }
                else{
                    r = mid - 1;
                }
            }
            if(pos != -1){
                tmp.children[pos] = cur;
                tree.update(tmp, tmp.index);
                return;
            }
            else{
                UpChange(tmp.fa, las, cur);
            }
        }

        bool Check(int index){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            return tmp.fa == -1 && tmp.num == 2;
        }

        bool LeafBorrow(Node<KeyType, ValueType> tmp){//记得update;
            Node<KeyType, ValueType> preNode, nextNode;
            KeyValue<KeyType, ValueType> borrowCh;
            if(tmp.pre != -1){
                tree.read(preNode, tmp.pre);
                if(preNode.num > MIN_CHILDREN){
                    borrowCh = preNode.children[preNode.num - 1];
                    preNode.num--; tmp.num++;
                    preNode.children[preNode.num] = KeyValue<KeyType, ValueType>();
                    for(int i = tmp.num - 1; i > 0; --i){
                        tmp.children[i] = tmp.children[i - 1];
                    }
                    tmp.children[0] = borrowCh;
                    tree.update(preNode, preNode.index);
                    tree.update(tmp, tmp.index);
                    UpChange(tmp.fa, tmp.children[1], borrowCh);
                    return true;
                }
            }
            if(tmp.next != -1){
                tree.read(nextNode, tmp.next);
                if(nextNode.num > MIN_CHILDREN){
                    borrowCh = nextNode.children[0];
                    nextNode.num--; tmp.num++;
                    for(int i = 0; i < nextNode.num; ++i){
                        nextNode.children[i] = nextNode.children[i + 1];
                    }
                    nextNode.children[nextNode.num] = KeyValue<KeyType, ValueType>();
                    tmp.children[tmp.num - 1] = borrowCh;
                    tree.update(tmp, tmp.index);
                    tree.update(nextNode, nextNode.index);
                    UpChange(nextNode.fa, borrowCh, nextNode.children[0]);
                    return true;
                }
            }
            return false;
        }

        bool NoneLeafBorrow(Node<KeyType, ValueType> tmp){
            Node<KeyType, ValueType> preNode, nextNode, faNode;
            KeyValue<KeyType, ValueType> aimPair = FindMin(tmp);
            int prePos, nextPos;
            tree.read(faNode, tmp.fa);
            int l = 0, r = faNode.num - 1, mid, pos = 0;
            while(l <= r){
                mid = l + r >> 1;
                if(faNode.children[mid] == aimPair){
                    pos = mid;
                    break;
                }
                if(faNode.children[mid] < aimPair){
                    l = mid + 1;
                }
                else{
                    r = mid - 1;
                }
            }
            prePos = pos - 1; nextPos = pos + 1;
            if(prePos != -1){
                tree.read(preNode, faNode.chIndex[prePos]);
                if(preNode.num > MIN_CHILDREN){
                    Node<KeyType, ValueType> borrowCh;
                    KeyValue<KeyType, ValueType> tmpMin, newMin;
                    int borrowChIndex;
                    tree.read(borrowCh, preNode.chIndex[preNode.num - 1]);
                    borrowChIndex = borrowCh.index;
                    tmpMin = FindMin(tmp);
                    newMin = FindMin(borrowCh);
                    ChangeFa(borrowChIndex, tmp.index);
                    preNode.num--; tmp.num++;
                    for(int i = tmp.num - 1; i > 0; --i){
                        tmp.children[i] = tmp.children[i - 1];
                        tmp.chIndex[i] = tmp.chIndex[i - 1];
                    }
                    Node<KeyType, ValueType> tmpCh1;
                    tree.read(tmpCh1, tmp.chIndex[1]);
                    tmp.children[1] = FindMin(tmpCh1); //不能赋成0,因为0处是初始值;
                    preNode.children[preNode.num] = KeyValue<KeyType, ValueType>();
                    tmp.children[0] = KeyValue<KeyType, ValueType>();
                    tmp.chIndex[0] = borrowChIndex;
                    tree.update(preNode, preNode.index);
                    tree.update(tmp, tmp.index);
                    UpChange(tmp.fa, tmpMin, newMin);
                    return true;
                }
            }
            if(nextPos < faNode.num){
                tree.read(nextNode, faNode.chIndex[nextPos]);
                if(nextNode.num > MIN_CHILDREN){
                    Node<KeyType, ValueType> borrowCh;
                    KeyValue<KeyType, ValueType> nextMin, newMin;
                    int borrowChIndex;
                    tree.read(borrowCh, nextNode.chIndex[0]);
                    borrowChIndex = borrowCh.index;
                    nextMin = FindMin(nextNode);
                    ChangeFa(borrowChIndex, tmp.index);
                    nextNode.num--, tmp.num++;
                    for(int i = 0; i < nextNode.num; ++i){
                        nextNode.children[i] = nextNode.children[i + 1];
                        nextNode.chIndex[i] = nextNode.chIndex[i + 1];
                    }
                    nextNode.children[0] = KeyValue<KeyType, ValueType>();
                    nextNode.children[nextNode.num] = KeyValue<KeyType, ValueType>();
                    tmp.children[tmp.num - 1] = nextMin;
                    tmp.chIndex[tmp.num - 1] = borrowChIndex;
                    newMin = FindMin(nextNode);
                    tree.update(nextNode, nextNode.index);
                    tree.update(tmp, tmp.index);
                    UpChange(nextNode.fa, nextMin, newMin);
                    return true;
                }
            }
            return false;
        }

        void UpDelete(int index, KeyValue<KeyType, ValueType> aimPair){//在非叶结点中删除关键字;
            if(index == -1){
                cout << "To Root !!!" << endl;
                return;
            }
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            int l = 0, r = tmp.num - 1, mid, pos = 0;
            while(l <= r){
                mid = l + r >> 1;
                if(tmp.children[mid] == aimPair){
                    pos = mid;
                    break;
                }
                if(tmp.children[mid] < aimPair){
                    l = mid + 1;
                }
                else {
                    r = mid - 1;
                }
            }
            tmp.num--;
            for(int i = pos; i < tmp.num; ++i){
                tmp.children[i] = tmp.children[i + 1];
                tmp.chIndex[i] = tmp.chIndex[i + 1];
            }
            tmp.children[0] = KeyValue<KeyType, ValueType>();
            tmp.children[tmp.num] = KeyValue<KeyType, ValueType>();
            tree.update(tmp, tmp.index);
            if(!pos){
                UpChange(tmp.fa, aimPair, FindMin(tmp));
            }
            if(tmp.fa == -1 && tmp.num == 1){//可去掉根节点;
                Node<KeyType, ValueType> newRoot;
                tree.read(newRoot, tmp.chIndex[0]);
                newRoot.fa = -1;
                root_index = newRoot.index;
                tree.write_info(root_index, 3);
                tree.update(newRoot, newRoot.index);
                tree.Delete(tmp.index);
                return;
            }
            if(tmp.fa != -1 && tmp.num < MIN_CHILDREN){//普通结点采取与叶子结点同样的处理方式
                if(!NoneLeafBorrow(tmp)){
                    MergeNonLeaf(tmp);
                }
            }
        }

        void MergeNonLeaf(Node<KeyType, ValueType> tmp){//同样统一向左合并;tmp -> pre, next -> tmp;
            Node<KeyType, ValueType> preNode, nextNode, faNode;
            KeyValue<KeyType, ValueType> aimPair = FindMin(tmp);
            tree.read(faNode, tmp.fa);
            int l = 0, r = faNode.num - 1, mid, pos = 0, prePos, nextPos;
            while(l <= r){
                mid = l + r >> 1;
                if(faNode.children[mid] == aimPair){
                    pos = mid;
                    break;
                }
                if(faNode.children[mid] < aimPair){
                    l = mid + 1;
                }
                else{
                    r = mid - 1;
                }
            }
            prePos = pos - 1, nextPos = pos + 1;
            if(prePos != -1){//有左邻居;
                tree.read(preNode, faNode.chIndex[prePos]);
                KeyValue<KeyType, ValueType> tmpMin = FindMin(tmp);
                preNode.num++;
                preNode.children[preNode.num - 1] = tmpMin;
                preNode.chIndex[preNode.num - 1] = tmp.chIndex[0];
                ChangeFa(tmp.chIndex[0], preNode.index);
                for(int i = 1; i < tmp.num; ++i){
                    preNode.num++;
                    preNode.children[preNode.num - 1] = tmp.children[i];
                    preNode.chIndex[preNode.num - 1] = tmp.chIndex[i];
                    ChangeFa(tmp.chIndex[i], preNode.index);
                }
                tree.update(preNode, preNode.index);
                tree.Delete(tmp.index);
                UpDelete(tmp.fa, tmpMin);
            }
            else{//只有右邻居;
                tree.read(nextNode, faNode.chIndex[nextPos]);
                KeyValue<KeyType, ValueType> nextMin = FindMin(nextNode);
                tmp.num++;
                tmp.children[tmp.num - 1] = nextMin;
                tmp.chIndex[tmp.num - 1] = nextNode.chIndex[0];
                ChangeFa(nextNode.chIndex[0], tmp.index);
                for(int i = 1; i < nextNode.num; ++i){
                    tmp.num++;
                    tmp.children[tmp.num - 1] = nextNode.children[i];
                    tmp.chIndex[tmp.num - 1] = nextNode.chIndex[i];
                    ChangeFa(nextNode.chIndex[i], tmp.index);
                }
                tree.update(tmp, tmp.index);
                tree.Delete(nextNode.index);
                UpDelete(nextNode.fa, nextMin);
            }
        }

        void MergeLeaf(Node<KeyType, ValueType> tmp){//统一向前合并;
            Node<KeyType, ValueType> preNode, nextNode, nextNextNode;
            if(tmp.pre != -1){//有左邻居,则向左邻居合并;
                tree.read(preNode, tmp.pre);
                if(tmp.next != -1){
                    tree.read(nextNode, tmp.next);
                    nextNode.pre = tmp.pre;
                    tree.update(nextNode, nextNode.index);
                }
                else{
                    tail_index = preNode.index;
                    tree.write_info(tail_index, 5);
                }
                for(int i = 0; i < tmp.num; ++i){
                    preNode.children[++preNode.num - 1] = tmp.children[i];
                }
                preNode.next = tmp.next;
                tree.update(preNode, preNode.index);
                tree.Delete(tmp.index);
                UpDelete(tmp.fa, tmp.children[0]);
            }
            else{//只有右邻居,则右邻居向该节点合并;
                tree.read(nextNode, tmp.next);
                if(nextNode.next == -1){
                    for(int i = 0; i < nextNode.num; ++i){
                        tmp.children[++tmp.num - 1] = nextNode.children[i];
                    }
                    tmp.next = nextNode.next;
                    tail_index = tmp.index;
                    tree.write_info(tail_index, 5);
                    tree.update(tmp, tmp.index);
                    tree.Delete(nextNode.index);
                    UpDelete(nextNode.fa, nextNode.children[0]);
                }
                else{
                    tree.read(nextNextNode, nextNode.next);
                    for(int i = 0; i < nextNode.num; ++i){
                        tmp.children[++tmp.num - 1] = nextNode.children[i];
                    }
                    tmp.next = nextNode.next;
                    nextNextNode.pre = nextNode.pre;
                    tree.update(tmp, tmp.index);
                    tree.update(nextNextNode, nextNextNode.index);
                    tree.Delete(nextNode.index);
                    UpDelete(nextNode.fa, nextNode.children[0]);
                }
            }
        }

    public:
        BPlusTree() = default;

        BPlusTree(const string &file_name) : tree(file_name) {
            Initialize();
        }

        int GetNum(){
            return num;
        }

        bool IsEmpty(){
            return !num;
        }

        void Clear(){
            tree.clear();
            Initialize();
        }

        void Find(const KeyType &aimKey, vector<ValueType> &res){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, root_index);
            while(!tmp.is_leaf){
                int l = 0, r = tmp.num - 1, mid;
                KeyType midKey;
                while(l < r){
                    mid = l + r >> 1;
                    midKey = tmp.children[mid].key;
                    if(aimKey > midKey){
                        l = mid + 1;
                    }
                    else{
                        r = mid;
                    }
                }
                if(l){
                    midKey = tmp.children[l].key;
                    if(midKey >= aimKey) --l;
                }
                tree.read(tmp, tmp.chIndex[l]);
            }
            KeyType tmpString;
            bool flag = 0;
            while(true){
                for(int i = 0; i < tmp.num; ++i){
                    tmpString = tmp.children[i].key;
                    if(tmpString == aimKey){
                        res.push_back(tmp.children[i].value);
                    }
                    else if(tmpString > aimKey){
                        flag = 1;
                        break;
                    }
                }
                if(tmp.next == -1 || flag) break;
                tree.read(tmp, tmp.next);
            }
        }

        void Insert(const KeyType &Key, const ValueType &Value) {
            KeyValue<KeyType, ValueType> newPair(Key, Value);
            Node<KeyType, ValueType> tmp, tail;
            tree.read(tmp, root_index);
            if (!tmp.num) {//空的B+树
                tree.read(tail, tail_index);
                tmp.num = 2;
                tmp.children[1] = newPair;
                tmp.chIndex[0] = head_index;
                tmp.chIndex[1] = tail_index;
                tail.num = 1;
                tail.children[0] = newPair;
                tree.update(tmp, tmp.index);
                tree.update(tail, tail.index);
                num++;
                tree.write_info(num, 6);
                return;
            }

            if(tmp.num == 2 && tmp.children[1] == KeyValue<KeyType, ValueType>()){ //删空之后特判;
                Node<KeyType, ValueType> ch;
                tree.read(ch, tmp.chIndex[1]);
                tmp.children[1] = newPair;
                ch.num++;
                ch.children[ch.num - 1] = newPair;
                tree.update(tmp, tmp.index);
                tree.update(ch, ch.index);
                return;
            }

            while (!tmp.is_leaf) {//直到找到叶子结点;
                int l = 0, r = tmp.num - 1, mid, nextChild = 0;
                while (l <= r) {//二分找孩子;
                    mid = l + r >> 1;
                    if (newPair >= tmp.children[mid]) {
                        nextChild = mid;
                        l = mid + 1;
                    } else {
                        r = mid - 1;
                    }
                }
                tree.read(tmp, tmp.chIndex[nextChild]);
            }
            int l = 0, r = tmp.num - 1, mid, pos = 0;
            while (l <= r) {
                mid = l + r >> 1;
                if (newPair > tmp.children[mid]) {
                    pos = mid + 1;
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
            for (int i = ++tmp.num - 1; i > pos; --i) {
                tmp.children[i] = tmp.children[i - 1];
            }
            tmp.children[pos] = newPair;
            tree.update(tmp, tmp.index);
            if(!pos && tmp.num > 1){//插入也别忘记改父节点关键字;
                UpChange(tmp.fa, tmp.children[1], tmp.children[0]);
            }
            if (tmp.num > MAX_CHILDREN) {
                if(tmp.index == head_index && tmp.next == tail_index){//head结点特判;
                    Node<KeyType, ValueType> tailNode;
                    tree.read(tailNode, tmp.next);
                    if(tailNode.num < MIN_CHILDREN){
                        int chNum = std::min(MIN_CHILDREN - tailNode.num, tmp.num);
                        tailNode.num += chNum; tmp.num -= chNum;
                        for(int i = tailNode.num - 1; i >= chNum; --i){
                            tailNode.children[i] = tailNode.children[i - chNum];
                            tailNode.children[i - chNum] = KeyValue<KeyType, ValueType>();
                        }
                        for(int i = 0; i < chNum; ++i){
                            tailNode.children[i] = tmp.children[i + tmp.num];
                            tmp.children[i + tmp.num] = KeyValue<KeyType, ValueType>();
                        }
                        tree.update(tmp, tmp.index);
                        tree.update(tailNode, tailNode.index);
                        UpChange(tailNode.fa, tailNode.children[chNum], tailNode.children[0]);
                        return;
                    }
                }
                SplitLeaf(tmp); //该节点分裂;
            }
            num++;
            tree.write_info(num, 6);
        }

        void Delete(const KeyType &aimKey, const ValueType &aimValue){
            KeyValue<KeyType, ValueType> aimPair(aimKey, aimValue);
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, root_index);
            while(!tmp.is_leaf){
                int l = 0, r = tmp.num - 1, mid, nextChild = 0;
                while(l <= r){
                    mid = l + r >> 1;
                    if(aimPair >= tmp.children[mid]){
                        nextChild = mid;
                        l = mid + 1;
                    }
                    else{
                        r = mid - 1;
                    }
                }
                tree.read(tmp, tmp.chIndex[nextChild]);
            }
            int l = 0, r = tmp.num - 1, mid, pos = -1;
            while(l <= r){
                mid = l + r >> 1;
                if(aimPair == tmp.children[mid]){
                    pos = mid;
                    break;
                }
                if(aimPair > tmp.children[mid]){
                    l = mid + 1;
                }
                else{
                    r = mid - 1;
                }
            }
            if(pos == -1){////删除的条目不存在
                return;
            }
            tmp.num--;
            for(int i = pos; i < tmp.num; ++i){
                tmp.children[i] = tmp.children[i + 1];
            }
            tmp.children[tmp.num] = KeyValue<KeyType, ValueType>(); //设为空节点;
            tree.update(tmp, tmp.index);
            if(!pos){//如果删除的是第一个元素,改变祖先节点的关键字;
                KeyValue<KeyType, ValueType> newPair;
                if(tmp.num){
                    newPair = tmp.children[0];
                }
                else{
                    newPair = KeyValue<KeyType, ValueType>();
                }
                UpChange(tmp.fa, aimPair, newPair);
            }
            if(tmp.num < MIN_CHILDREN && !Check(tmp.fa)){
                if(!LeafBorrow(tmp)){//没有成功领养则考虑合并;
                    MergeLeaf(tmp);
                }
            }
            else{
                if(!tmp.num && tmp.index == tail_index){//保证2号结点永远不空;
                    Node<KeyType, ValueType> preNode;
                    tree.read(preNode, tmp.pre);
                    if(preNode.num){
                        preNode.num--;
                        tmp.num++;
                        tmp.children[0] = preNode.children[preNode.num];
                        preNode.children[preNode.num] = KeyValue<KeyType, ValueType>();
                        tree.update(preNode, preNode.index);
                        tree.update(tmp, tmp.index);
                        UpChange(tmp.fa, KeyValue<KeyType, ValueType>(), tmp.children[0]);
                    }
                }
            }
            num--;
            tree.write_info(num, 6);
        }

        void Update(const KeyType &aimKey, const ValueType &preValue, const ValueType &newValue){
            Delete(aimKey, preValue);
            Insert(aimKey, newValue);
        }

        void FindOrders(const string &aimUid, vector<ValueType> &res){
            if(!std::is_same<KeyType, struct FindOrders>()){
                cout << "Type FindOrders Wrong!" << endl;
                return;
            }
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, root_index);
            while(!tmp.is_leaf){
                int l = 0, r = tmp.num - 1, mid;
                struct FindOrders midKey;
                while(l < r){
                    mid = l + r >> 1;
                    midKey = tmp.children[mid].key;
                    if(aimUid > midKey.uid){
                        l = mid + 1;
                    }
                    else{
                        r = mid;
                    }
                }
                if(l){
                    midKey = tmp.children[l].key;
                    if(midKey.uid >= aimUid) --l;
                }
                tree.read(tmp, tmp.chIndex[l]);
            }
            /*
            KeyType tmpString;
            bool flag = 0;
            while(true){
                for(int i = 0; i < tmp.num; ++i){
                    tmpString = tmp.children[i].key;
                    if(tmpString == aimUid){
                        res.push_back(tmp.children[i].value);
                    }
                    else if(tmpString > aimKey){
                        flag = 1;
                        break;
                    }
                }
                if(tmp.next == -1 || flag) break;
                tree.read(tmp, tmp.next);
            }
             */
            struct FindOrders tmpKey;
            bool flag = 0;
            while(true){
                for(int i = 0; i < tmp.num; ++i){
                    tmpKey = tmp.children[i].key;
                    if(tmpKey.uid == aimUid){
                        res.push_back(tmp.children[i].value);
                    }
                    else if(tmpKey.uid > aimUid){
                        flag = 1;
                        break;
                    }
                }
                if(tmp.next == -1 || flag) break;
                tree.read(tmp, tmp.next);
            }
        }

        void FindTickets(const string &aimID, vector<ValueType> &res){
            if(!std::is_same<KeyType, struct FindTickets>()){
                cout << "Type FindTickets Wrong!" << endl;
                return;
            }

            Node<KeyType, ValueType> tmp;
            tree.read(tmp, root_index);
            while(!tmp.is_leaf){
                int l = 0, r = tmp.num - 1, mid;
                struct FindTickets midKey;
                while(l < r){
                    mid = l + r >> 1;
                    midKey = tmp.children[mid].key;
                    if(aimID > midKey.trainID){
                        l = mid + 1;
                    }
                    else{
                        r = mid;
                    }
                }
                if(l){
                    midKey = tmp.children[l].key;
                    if(midKey.trainID >= aimID) --l;
                }
                tree.read(tmp, tmp.chIndex[l]);
            }
            struct FindTickets tmpKey;
            bool flag = 0;
            while(true){
                for(int i = 0; i < tmp.num; ++i){
                    tmpKey = tmp.children[i].key;
                    if(tmpKey.trainID == aimID){
                        res.push_back(tmp.children[i].value);
                    }
                    else if(tmpKey.trainID > aimID){
                        flag = 1;
                        break;
                    }
                }
                if(tmp.next == -1 || flag) break;
                tree.read(tmp, tmp.next);
            }
        }

        void FindStations(const string &aimStation, vector<ValueType> &res){
            if(!std::is_same<KeyType, struct FindStations>()){
                cout << "Type FindStations Wrong!" << endl;
                return;
            }

            Node<KeyType, ValueType> tmp;
            tree.read(tmp, root_index);
            while(!tmp.is_leaf){
                int l = 0, r = tmp.num - 1, mid;
                struct FindStations midKey;
                while(l < r){
                    mid = l + r >> 1;
                    midKey = tmp.children[mid].key;
                    if(aimStation > midKey.stationID){
                        l = mid + 1;
                    }
                    else{
                        r = mid;
                    }
                }
                if(l){
                    midKey = tmp.children[l].key;
                    if(midKey.stationID >= aimStation) --l;
                }
                tree.read(tmp, tmp.chIndex[l]);
            }

            struct FindStations tmpKey;
            bool flag = 0;
            while(true){
                for(int i = 0; i < tmp.num; ++i){
                    tmpKey = tmp.children[i].key;
                    if(tmpKey.stationID== aimStation){
                        res.push_back(tmp.children[i].value);
                    }
                    else if(tmpKey.stationID > aimStation){
                        flag = 1;
                        break;
                    }
                }
                if(tmp.next == -1 || flag) break;
                tree.read(tmp, tmp.next);
            }
        }

        void FindPending(const FindPD &aimPD, vector<ValueType> &res){
            if(!std::is_same<KeyType, struct FindPending>()){
                cout << "Type FindPD Wrong!" << endl;
                return;
            }

            Node<KeyType, ValueType> tmp;
            tree.read(tmp, root_index);
            while(!tmp.is_leaf){
                int l = 0, r = tmp.num - 1, mid;
                struct FindPending midKey;
                while(l < r){
                    mid = l + r >> 1;
                    midKey = tmp.children[mid].key;
                    if(aimPD > midKey.PD){
                        l = mid + 1;
                    }
                    else{
                        r = mid;
                    }
                }
                if(l){
                    midKey = tmp.children[l].key;
                    if(midKey.PD >= aimPD) --l;
                }
                tree.read(tmp, tmp.chIndex[l]);
            }

            struct FindPending tmpKey;
            bool flag = 0;
            while(true){
                for(int i = 0; i < tmp.num; ++i){
                    tmpKey = tmp.children[i].key;
                    if(tmpKey.PD == aimPD){
                        res.push_back(tmp.children[i].value);
                    }
                    else if(tmpKey.PD > aimPD){
                        flag = 1;
                        break;
                    }
                }
                if(tmp.next == -1 || flag) break;
                tree.read(tmp, tmp.next);
            }
        }

#ifdef _DEBUG

        bool check_non0_child_is_KeyValue(int index){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            if(tmp.is_leaf) return true;
            if(tmp.fa == -1){
                for(int i = 0; i < tmp.num; ++i)
                    if(!check_non0_child_is_KeyValue(tmp.chIndex[i])) return false;
                return true;
            }
            else{
                for(int i = 1; i < tmp.num; ++i){
                    if(tmp.children[i].value == int_min) return false;
                }
                return true;
            }
        }
        bool check(int index){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            if(tmp.is_leaf) return true;
            if(tmp.children[0] != KeyValue<KeyType, ValueType>()) return false;
            for(int i = 0; i < tmp.num; ++i){
                if(!check(tmp.chIndex[i])) return false;
            }
            return true;
        }
        bool order_check(int index){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            if(tmp.is_leaf) return true;
            for(int i = 0; i <= tmp.num - 2; ++i){
                if(tmp.children[i] >= tmp.children[i + 1])
                    return false;
            }
            for(int i = 0; i < tmp.num; ++i){
                if(!order_check(tmp.chIndex[i]))
                    return false;
            }
            return true;
        }
        bool is_zero_non_leaf(int index){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            if(tmp.is_leaf) return true;
            if(tmp.num == 0) return false;
            for(int i = 0; i < tmp.num; ++i){
                if(!is_zero_non_leaf(tmp.chIndex[i])) return false;
            }
            return true;
        }
        bool is_son_non_fa(int index){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, index);
            if(tmp.is_leaf) return true;
            Node<KeyType, ValueType> sonNode;
            for(int i = 0; i < tmp.num; ++i){
                tree.read(sonNode, tmp.chIndex[i]);
                if(sonNode.fa != tmp.index) return false;
            }
            for(int i = 0; i < tmp.num; ++i){
                if(!is_son_non_fa(tmp.chIndex[i]) ) return false;
            }
            return true;
        }
        bool check_root(){
            Node<KeyType, ValueType> tmp;
            tree.read(tmp, root_index);
            for(int i = 1; i < tmp.num; ++i){
                if(tmp.children[i].value == int_min) return false;
            }
            return true;
        }
        bool children_num(){
            Node<KeyType, ValueType> tmp;
            int cnt = 0;
            tree.read(tmp, head_index);
            while(true){
                cnt++;
                if(cnt <= 2){
                    if(tmp.next == -1) break;
                    tree.read(tmp, tmp.next);
                }
                else{
                    if(tmp.num < MIN_CHILDREN)
                        return false;
                    if(tmp.next == -1) break;
                    tree.read(tmp,tmp.next);
                }
            }
            return true;
        }
        int get_root_index(){
            return root_index;
        }

#endif
    };//class BPlusTree;
}//namespace B_PLUS_TREE;

#endif //TRAINTICKETSYSTEM_BPT_HPP
