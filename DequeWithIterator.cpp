#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <iterator>

template<typename T>
class Deque {
private:
    T* buffer;
    size_t capacity;
    size_t front_idx;
    size_t back_idx;
    size_t count;

    void resize(size_t new_capacity) {
        T* new_buffer = new T[new_capacity];
        for (size_t i = 0; i < count; ++i) {
            new_buffer[i] = buffer[(front_idx + i) % capacity];
        }
        delete[] buffer;
        capacity = new_capacity;
        buffer = new_buffer;
        front_idx = 0;
        back_idx = count;
    }

public:
    Deque(size_t initial_capacity = 8)
        : capacity(initial_capacity), front_idx(0), back_idx(0), count(0)
    {
        buffer = new T[capacity];
    }

    ~Deque() {
        delete[] buffer;
    }

    bool empty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

    void push_front(const T& value) {
        if (count == capacity) {
            resize(capacity * 2);
        }
        front_idx = (front_idx == 0) ? capacity - 1 : front_idx - 1;
        buffer[front_idx] = value;
        ++count;
    }

    void push_back(const T& value) {
        if (count == capacity) {
            resize(capacity * 2);
        }
        buffer[back_idx] = value;
        back_idx = (back_idx + 1) % capacity;
        ++count;
    }

    void pop_front() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        front_idx = (front_idx + 1) % capacity;
        --count;
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        back_idx = (back_idx == 0) ? capacity - 1 : back_idx - 1;
        --count;
    }

    T& front() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return buffer[front_idx];
    }

    const T& front() const{
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return buffer[front_idx];
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        size_t last_idx = (back_idx == 0) ? capacity - 1 : back_idx - 1;
        return buffer[last_idx];
    }

    const T& back() const{
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        size_t last_idx = (back_idx == 0) ? capacity - 1 : back_idx - 1;
        return buffer[last_idx];
    }

    //容器内部实现迭代器
    class Iterator {
        private:
            Deque<T>* deque_ptr;
            size_t index;
            size_t pos;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            Iterator(Deque<T>* deque, size_t position):deque_ptr(deque), pos(position) {}

            reference operator*() const {
                size_t real_idx = (deque_ptr->front_idx + pos) % deque_ptr->capacity;
                return deque_ptr->buffer[real_idx];
            }

            pointer operator->() const {
                size_t real_idx = (deque_ptr->front_idx + pos) % deque_ptr->capacity;
                return &(deque_ptr->buffer[real_idx]);
            }

            Iterator& operator++() {
                ++pos;
                return *this;
            }
            //类内部定义的时候Iterator<T>可以省略
            Iterator operator++(int) {
                Iterator tmp = *this;
                ++pos;
                return tmp;
            }

            Iterator& operator--() {
                --pos;
                return *this;
            }

            Iterator operator--(int) {
                Iterator tmp = *this;
                --pos;
                return tmp;
            }

            bool operator==(const Iterator& other) const {
                return (deque_ptr == other.deque_ptr) && (pos == other.pos);
            }

            //调用operator==
            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }
    };

    //this是指调用这个方法的容器
    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, count);
    }
};




int main() {
    Deque<std::string> dq;

    dq.push_back("Apple");
    dq.push_back("Banana");
    dq.push_back("Cherry");

    dq.push_front("Date");
    dq.push_front("Elderberry");

    std::cout << "Deque 大小: " << dq.size() << std::endl;

    std::cout << "Deque 元素: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "前端元素: " << dq.front() << std::endl;
    std::cout << "后端元素: " << dq.back() << std::endl;


    dq.pop_front();
    dq.pop_back();


    std::cout << "删除元素后的 Deque: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
