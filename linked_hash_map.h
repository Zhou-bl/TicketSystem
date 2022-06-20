#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include "utility.h"
#include "exceptions.h"

namespace sjtu {

    template<
            class Key,
            class T,
            class Hash = std::hash<Key>,
            class Equal = std::equal_to<Key>
    > class linked_hashmap {
    public:
        typedef pair<const Key, T> value_type;
        struct Node{
            value_type *data = nullptr;
            int hash_value = -1; //hash_code + rehash 运算后的值;
            Node *nxtData = nullptr; //单链表存储
            Node *preIns = nullptr, *nxtIns = nullptr; //提供双链表的遍历循序
            Node(){}
            Node(const Node &other){
                data = new value_type (*(other.data));
                hash_value = other.hash_value;
            }
            Node(const value_type &val, const int &hash){
                data = new value_type (val);
                hash_value = hash;
            }
            Node &operator = (const Node &other){
                if(this == &other)
                    return *this;
                delete data;
                data = new value_type (*(other.data));
                hash_value = other.hash_value;
            }
            ~Node(){
                delete data;
            }
        };
        size_t capacity = 16, num = 0;
        double LoadFactor = 0.75;
        Node **array = nullptr;
        Node *head = nullptr, *tail = nullptr;
        class const_iterator;
        class iterator {
            friend class linked_hashmap;
            friend class const_iterator;
        private:
            Node *ptr = nullptr;
            const linked_hashmap *from = nullptr;
        public:
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::output_iterator_tag;


            iterator() {}
            iterator(Node *p, const linked_hashmap *f){
                ptr = p;
                from = f;
            }
            iterator(const iterator &other) {
                ptr = other.ptr;
                from = other.from;
            }
            iterator operator++(int) {
                if(ptr == from->tail || ptr == nullptr)
                    throw invalid_iterator();
                iterator res = *this;
                ptr = ptr->nxtIns;
                return res;
            }
            iterator & operator++() {
                if(ptr == from->tail || ptr == nullptr)
                    throw invalid_iterator();
                ptr = ptr->nxtIns;
                return *this;
            }
            iterator operator--(int) {
                if(ptr == from->head->nxtIns || ptr == nullptr)
                    throw invalid_iterator();
                iterator res = *this;
                ptr = ptr->preIns;
                return res;
            }
            iterator & operator--() {
                if(ptr == from->head->nxtIns || ptr == nullptr)
                    throw invalid_iterator();
                ptr = ptr->preIns;
                return *this;
            }
            linked_hashmap::value_type & operator*() const {
                if(ptr == nullptr || ptr == from->tail || ptr == from->head)
                    throw invalid_iterator();
                return *(ptr->data);
            }
            linked_hashmap::value_type* operator->() const noexcept {
                if(ptr == nullptr || ptr == from->tail || ptr == from->head)
                    throw invalid_iterator();
                return ptr->data;
            }
            bool operator==(const iterator &rhs) const {
                return from == rhs.from && ptr == rhs.ptr;
            }
            bool operator==(const const_iterator &rhs) const {
                return from == rhs.from && ptr == rhs.ptr;
            }
            bool operator!=(const iterator &rhs) const {
                return ptr != rhs.ptr || from != rhs.from;
            }
            bool operator!=(const const_iterator &rhs) const {
                return ptr != rhs.ptr || from != rhs.from;
            }
        };

        class const_iterator {
            friend class linked_hashmap;
            friend class iterator;
        private:
            Node *ptr = nullptr;
            const linked_hashmap *from = nullptr;
        public:
            const_iterator() {}
            const_iterator(Node *p, const linked_hashmap *f){
                ptr = p;
                from = f;
            }
            const_iterator(const const_iterator &other) {
                ptr = other.ptr;
                from = other.from;
            }
            const_iterator(const iterator &other) {
                ptr = other.ptr;
                from = other.from;
            }
            const_iterator operator++(int){
                if(ptr == from->tail || ptr == nullptr)
                    throw invalid_iterator();
                const_iterator res = *this;
                ptr = ptr->nxtIns;
                return res;
            }
            const_iterator &operator++(){
                if(ptr == from->tail || ptr == nullptr)
                    throw invalid_iterator();
                ptr = ptr->nxtIns;
                return *this;
            };
            const_iterator operator--(int){
                if(ptr == from->head->nxtIns || ptr == nullptr)
                    throw invalid_iterator();
                const_iterator res = *this;
                ptr = ptr->preIns;
                return res;
            }
            const_iterator &operator--(){
                if(ptr == from->head->nxtIns || ptr == nullptr)
                    throw invalid_iterator();
                ptr = ptr->preIns;
                return *this;
            }
            const linked_hashmap::value_type & operator*() const {
                if(ptr == nullptr || ptr == from->tail || ptr == from->head)
                    throw invalid_iterator();
                return *(ptr->data);
            }
            const linked_hashmap::value_type* operator->() const noexcept{
                if(ptr == nullptr || ptr == from->tail || ptr == from->head)
                    throw invalid_iterator();
                return ptr->data;
            }
            bool operator==(const iterator &rhs) const {
                return from == rhs.from && ptr == rhs.ptr;
            }
            bool operator==(const const_iterator &rhs) const {
                return from == rhs.from && ptr == rhs.ptr;
            }
            bool operator!=(const iterator &rhs) const {
                return ptr != rhs.ptr || from != rhs.from;
            }
            bool operator!=(const const_iterator &rhs) const {
                return ptr != rhs.ptr || from != rhs.from;
            }
        };

        linked_hashmap() {
            array = new Node *[capacity];
            head = new Node;
            tail = new Node;
            head->nxtIns = tail;
            tail->preIns = head;
            for(int i = 0; i < capacity; ++i)
                array[i] = nullptr;
        }
        linked_hashmap(const linked_hashmap &other) {
            capacity = other.capacity;
            array = new Node *[capacity];
            head = new Node;
            tail = new Node;
            head->nxtIns = tail;
            tail->preIns = head;
            for(int i = 0; i < capacity; ++i)
                array[i] = nullptr;
            for(Node *p = other.head->nxtIns; p != other.tail; p = p->nxtIns)
                insert(*(p->data));
        }

        linked_hashmap & operator=(const linked_hashmap &other) {
            if(this == &other) return *this;
            clear();
            for(Node *p = other.head->nxtIns; p != other.tail; p = p->nxtIns)
                insert(*(p->data));
            return *this;
        }

        ~linked_hashmap() {
            clear();
            delete []array;
            delete head;
            delete tail;
        }
        T & at(const Key &key) {
            int pos = GetIndex(MyHash(key));
            Node *p = array[pos];
            bool flag = 0;
            while(p){
                if(Equal()(p->data->first, key)){
                    flag = 1;
                    break;
                }
                p = p->nxtData;
            }
            if(!flag)
                throw index_out_of_bound();
            return p->data->second;
        }
        const T & at(const Key &key) const {
            int pos = GetIndex(MyHash(key));
            Node *p = array[pos];
            bool flag = 0;
            while(p){
                if(Equal()(p->data->first, key)){
                    flag = 1;
                    break;
                }
                p = p->nxtData;
            }
            if(!flag)
                throw index_out_of_bound();
            return p->data->second;
        }
        T & operator[](const Key &key) {
            const_iterator res = find(key);
            if(res == cend())
                return insert(value_type(key, T())).first.ptr->data->second;
            return res.ptr->data->second;
        }
        const T & operator[](const Key &key) const {
            const_iterator res = find(key);
            if(res == cend())
                throw index_out_of_bound();
            return res.ptr->data->second;
        }
        iterator begin() {
            return iterator(head->nxtIns, this);
        }
        const_iterator cbegin() const {
            return const_iterator(head->nxtIns, this);
        }
        iterator end() {
            return iterator(tail, this);
        }
        const_iterator cend() const {
            return const_iterator(tail, this);
        }
        bool empty() const {
            return !num;
        }
        size_t size() const {
            return num;
        }
        void clear() {
            for(int i = 0; i < capacity; ++i)
                if(array[i] != nullptr){
                    Node *p = array[i], *tmp;
                    while(p){
                        tmp = p->nxtData;
                        delete p;
                        p = tmp;
                    }
                    array[i] = nullptr;
                }
            num = 0;
            head->nxtIns = tail;
            tail->preIns = head;
        }
        int MyHash(const Key &o) const {
            int res = Hash()(o);
            res ^= (res >> 20) ^ (res >> 12);
            return res ^ (res >> 7) ^ (res >> 4);
        }
        int GetIndex(const int &h) const {
            return h & (capacity - 1);
        }
        void DoubleSpace(){
            delete[] array;
            capacity <<= 1;
            array = new Node *[capacity];
            std::fill(array, array + capacity, nullptr);
            for(Node *cur = head->nxtIns; cur != tail; cur = cur->nxtIns){
                cur->nxtData = nullptr;
                int pos = GetIndex(cur->hash_value);
                Node *p = array[pos], *las = nullptr;
                if(p == nullptr){
                    array[pos] = cur;
                }
                else{
                    while(p->nxtData)
                        p = p->nxtData;
                    p->nxtData = cur;
                }
            }
        }
        pair<iterator, bool> insert(const value_type &value) {
            iterator res = find(value.first);
            if(res.ptr != tail)
                return {res, false};
            if(num >= capacity * LoadFactor)
                DoubleSpace();
            int h = MyHash(value.first), pos;
            pos = GetIndex(h);
            Node *tmp, *las;
            if(array[pos] == nullptr){
                array[pos] = new Node(value, h);
                tmp = array[pos];
            }
            else{
                Node *p = array[pos];
                while(p->nxtData)
                    p = p->nxtData;
                p->nxtData = new Node(value, h);
                tmp = p->nxtData;
            }
            num++;
            las = tail->preIns;
            tail->preIns = las->nxtIns = tmp;
            tmp->preIns = las;
            tmp->nxtIns = tail;
            return {iterator(tmp, this), true};
        }
        void erase(iterator pos) {
            if(pos.from != this || pos.ptr == tail || pos.ptr == head || pos.ptr == nullptr)
                throw invalid_iterator();
            int index = GetIndex(pos.ptr->hash_value);
            Node *p = array[index], *las = nullptr;
            while(p != pos.ptr){
                las = p;
                p = p->nxtData;
            }
            p->preIns->nxtIns = p->nxtIns;
            p->nxtIns->preIns = p->preIns;
            if(las == nullptr){
                array[index] = p->nxtData;
                delete p;
            }
            else{
                las->nxtData = p->nxtData;
                delete p;
            }
            num--;
        }
        size_t count(const Key &key) const {
            if(find(key) == cend()) return 0;
            else return 1;
        }
        iterator find(const Key &key) {
            int pos = GetIndex(MyHash(key));
            Node *p = array[pos];
            while(p){
                if(Equal()(p->data->first, key))
                    return iterator(p, this);
                p = p->nxtData;
            }
            return end();
        }
        const_iterator find(const Key &key) const{
            int pos = GetIndex((MyHash(key)));
            Node *p = array[pos];
            while(p){
                if(Equal()(p->data->first, key))
                    return const_iterator(p, this);
                p = p->nxtData;
            }
            return cend();
        }
    };

}

#endif