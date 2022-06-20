#ifndef TRAINTICKETSYSTEM_FILE_HPP
#define TRAINTICKETSYSTEM_FILE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <fstream>
#include <string>
#include "linked_hash_map.h"

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using sjtu::linked_hashmap;

template<class T>
struct pool_node{
    T data;
    bool isUpdate;
    pool_node():data(), isUpdate(0){}

    pool_node(const T &tmpdata, bool tmpflag){
        data = tmpdata;
        isUpdate = tmpflag;
    }
};

template<class T, int info_len = 2>
class MemoryRiver {//一个MemoryRiver 对应一个文件
private:
    /* your code here */
    const int MAX_POOL = 1000;
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    linked_hashmap<int, pool_node<T>> pool;

public:
    MemoryRiver() = default;

    ~MemoryRiver(){
        for(auto it = pool.begin(); it != pool.end(); ++it){
            if(it->second.isUpdate){
                inner_update(it->first, it->second.data);
            }
        }
    }

    void inner_update(int index, T &t){
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    MemoryRiver(const string& file_name) : file_name( file_name) { initialise();}

    void clear(){
        file.open(file_name, std::ofstream::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i){
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        file.close();
        initialise(file_name);
    }

    void initialise(string FN = "") {
        for(auto it = pool.begin(); it != pool.end(); ++it){
            if(it->second.isUpdate){
                inner_update(it->first, it->second.data);
            }
        }
        pool.clear();
        if (FN != "") file_name = FN;
        file.open(file_name, std::fstream::in | std::fstream::out);
        if(!file){//打开不成功说明文件不存在,并且初始化
            file.open(file_name, std::ofstream::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int& tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
        return;
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
        return;
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        /* your code here */
        int index;
        int num_T, del_head;
        get_info(num_T, 1);
        get_info(del_head, 2);
        if(!del_head)//在文件尾部加
            index = info_len * sizeof(int) + num_T * sizeofT;
        else {//在释放掉的空间加
            index = del_head;
            file.open(file_name);
            file.seekg(del_head);
            file.read(reinterpret_cast<char*>(&del_head), sizeof(int));
            file.close();
        }
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
        write_info(++num_T, 1);
        write_info(del_head, 2);//更新 del_head 的值
        if(pool.size() >= MAX_POOL){//内存池已满
            if(pool.begin()->second.isUpdate){
                inner_update(pool.begin()->first, pool.begin()->second.data);
            }
            pool.erase(pool.begin());
        }
        pool[index] = pool_node<T>(t, 0);
        return index;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        /* your code here */
        if(pool.count(index)){//如果在内存池中有,则只更新内存池
            pool[index] = pool_node<T>(t, 1);
        }
        else{
            inner_update(index, t);//没有则需要更新文件内容
        }
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        /* your code here */
        if(pool.find(index) != pool.end()){//内存池中有
            t = pool[index].data;
            return;
        }
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
        if(pool.size() >= MAX_POOL){
            if(pool.begin()->second.isUpdate){
                inner_update(pool.begin()->first, pool.begin()->second.data);
            }
            pool.erase(pool.begin());
        }
        pool[index] = pool_node<T>(t, 0);
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {//放入vector中
        if(pool.count(index)){
            pool.erase(pool.find(index));
        }
        int del_head;
        get_info(del_head, 2);
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&del_head), sizeof(int));
        del_head = index;
        file.close();
        write_info(del_head, 2);
    }
};

#endif //TRAINTICKETSYSTEM_FILE_HPP