#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.h"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
    private:
        T *data;
        int num, max_size;
    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */


        class const_iterator;

        class iterator {
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            T *it;
            const vector<T> *vec;
            friend vector<T>;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            //iterator构造函数
            iterator() = default;

            iterator(const iterator &n) = default;

            iterator(T *t, const vector<T> *v) : it(t), vec(v) {};

            iterator operator+(const int &n) const {
                return iterator(it + n, vec);
                //TODO
            }

            iterator operator-(const int &n) const {
                //TODO
                return iterator(it - n, vec);
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                if (vec == rhs.vec)
                    return it - rhs.it;
                else throw invalid_iterator();
            }

            iterator &operator+=(const int &n) {
                //TODO
                it += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                //TODO
                it -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp(*this);
                it++;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                ++it;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp(*this);
                it--;
                return tmp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                --it;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return *it;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return it == rhs.it;
            }

            bool operator==(const const_iterator &rhs) const {
                return it == rhs.it;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return it != rhs.it;
            }

            bool operator!=(const const_iterator &rhs) const {
                return it != rhs.it;
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

            //const
            const_iterator() = default;

            const_iterator(const const_iterator &n) = default;

            const_iterator(T *t, const vector<T> *v) : it(t), vec(v) {};

            const_iterator operator+(const int &n) const {
                return iterator(it + n, vec);
                //TODO
            }

            const_iterator operator-(const int &n) const {
                //TODO
                return iterator(it - n, vec);
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                if (vec == rhs.vec)
                    return it - rhs.it;
                else throw invalid_iterator();
            }

            const_iterator &operator+=(const int &n) {
                //TODO
                it += n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                //TODO
                it -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                iterator tmp(*this);
                it++;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                ++it;
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                iterator tmp(*this);
                it--;
                return tmp;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                --it;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return *it;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return it == rhs.it;
            }

            bool operator==(const const_iterator &rhs) const {
                return it == rhs.it;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return it != rhs.it;
            }

            bool operator!=(const const_iterator &rhs) const {
                return it != rhs.it;
            }

        private:
            /*TODO*/
            T *it;
            const vector<T> *vec;
            friend vector<T>;
        };

        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
        vector() {
            num = 0;
            max_size = 10;
            data = (T *) (new char[max_size * sizeof(T)]);
        }

        vector(const vector &other) : num(other.num), max_size(other.max_size) {
            data = (T *) (new char[max_size * sizeof(T)]);
            for (int i = 0; i < num; ++i) {
//                data[i] = other.data[i];
                new(data + i) T(other.data[i]);
            }
        }

        /**
         * TODO Destructor
         */
        ~vector() {
            clear();
            delete[] (char*)data;
        }

        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if (this == &other) return *this;
            clear();
            delete[] (char*)data;
            max_size = other.max_size;
            num = other.num;
            data = (T *) (new char[max_size * sizeof(T)]);
            for (int i = 0; i < num; ++i) {
                new(data + i) T(other.data[i]);
            }
            return *this;
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, max_size)
         */
        T &at(const size_t &pos) {
            if (pos >= num || pos < 0) throw index_out_of_bound();
            else return data[pos];
        }

        const T &at(const size_t &pos) const {
            if (pos >= num || pos < 0) throw index_out_of_bound();
            else return data[pos];
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, max_size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos) {
            return at(pos);
        }

        const T &operator[](const size_t &pos) const {
            return at(pos);
        }

        /**
         * access the first element.
         * throw container_is_empty if max_size == 0
         */
        const T &front() const {
            if (!num) throw container_is_empty();
            else return data[0];
        }

        /**
         * access the last element.
         * throw container_is_empty if max_size == 0
         */
        const T &back() const {
            if (!num) throw container_is_empty();
            else return data[num - 1];
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(data, this);
        }

        const_iterator cbegin() const {
            return const_iterator(data, this);
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return iterator(data + num, this);
        }

        const_iterator cend() const {
            return const_iterator(data + num, this);
        }

        /**
         * checks whether the container is empty
         */
        bool empty() const {
            if (!num) return true;
            return false;
        }

        /**
         * returns the number of elements
         */
        size_t size() const {
            return num;
        }

        /**
         * clears the contents
         */
        void clear() {
            while (num) {
                --num;
                data[num].~T();
            }
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            int ind = pos.it - data;
            if (num >= max_size) double_space();
            if (pos.it - data == num) {
                new(data + num) T(value);
            } else {
                if (num > 0) new(data + num) T(data[num - 1]);
                for (int i = num - 1; i > ind; --i)
                    data[i] = data[i - 1];
                data[ind] = value;
            }
            ++num;
            return pos;
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > max_size (in this situation ind can be max_size because after inserting the max_size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > num) throw index_out_of_bound();
            if (num >= max_size) double_space();
            // TODO：对最后一个的后面一块T内存要调用构造函数
            if (ind == num) {
                new(data + num) T(value);
            } else {
                if (num > 0) new(data + num) T(data[num - 1]);
                for (int i = num - 1; i > ind; --i)
                    data[i] = data[i - 1];
                data[ind] = value;
            }
            ++num;
            return iterator(ind, this);
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            for (int i = pos.it - data + 1; i < num; ++i)
                data[i - 1] = data[i];
            data[--num].~T();
            return pos;
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= max_size
         */
        iterator erase(const size_t &ind) {
            if (ind >= num) throw index_out_of_bound();
            for (int i = ind + 1; i < num; --i)
                data[i - 1] = data[i];
            data[--num].~T();
            return iterator(ind, this);
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if (num >= max_size) double_space();
            new(data + num) T(value);
            ++num;
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if max_size() == 0
         */
        void pop_back() {
            if (!num) throw container_is_empty();
            data[--num].~T();
        }

        void double_space() {
            max_size *= 2;
            T *new_data = (T *) (new char[max_size * sizeof(T)]);
            for (int i = 0; i < num; ++i) {
                new (new_data + i) T(data[i]);
                data[i].~T();
            }
            delete [] (char*)data;
            data = new_data;
        }
    };


}

#endif