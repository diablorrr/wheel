#include <iostream>
#include <iterator>

template<typename T>
class List;

template<typename T>
struct Node {
    T data;
    Node* prev;
    Node* next;
    Node(const T& value = T()): data(value), prev(nullptr), next(nullptr){}
};

template<typename T>
class Iterator {
public:
    using value_type = T;
    using referecne = T&;
    using pointer = T*;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;

    Iterator(Node<T>* ptr = nullptr): node_ptr(ptr) {}

    referecne operator*() const{
        return node_ptr->data;
    }

    pointer operator->() const {
        return &(node_ptr->data);
    }

    Iterator& operator++() {
        if (node_ptr) node_ptr = node_ptr->next;
        return *this;
    }

    //复用前置++
    Iterator operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    Iterator& operator--() {
        if (node_ptr) node_ptr = node_ptr->prev;
        return *this;
    }

    Iterator operator--(int) {
        Iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const Iterator& other) const {
        return node_ptr == other.node_ptr;
    }

    bool operator!=(const Iterator& other) const {
        return node_ptr != other.node_ptr;
    }

private:
    Node<T>* node_ptr;
    friend class List<T>;
};


template<typename T>
class List {
public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<T>;

    List() {
        head = new Node<T>();
        tail = new Node<T>();
        head->next = tail;
        tail->prev = head;
    }

    ~List() {
        clear();
        delete head;
        delete tail;
    }

    List(const List& other) = delete;
    List& operator=(const List& other) = delete;

    iterator insert(iterator pos, const T& value) {
        Node<T>* cur = pos.node_ptr;
        Node<T>* pre = cur->prev;
        Node<T>* new_node = new Node<T>(value);

        new_node->next = cur;
        new_node->prev = pre;
        cur->prev = new_node;
        pre->next = new_node;

        return iterator(new_node);
    }

    iterator erase(iterator pos) {
        Node<T>* cur = pos.node_ptr;
        if (cur == head || cur == tail) return pos;

        Node<T>* pre = cur->prev;
        Node<T>* nxt = cur->next;

        pre->next = nxt;
        nxt->prev = pre;

        delete cur;

        return iterator(nxt);
    }

    void push_front(const T& value) {
        insert(begin(), value);
    }

    void push_back(const T& value) {
        insert(end(), value);
    }

    void pop_front() {
        if (!empty()) {
            erase(begin());
        }
    }

    void pop_back() {
        if (!empty()) {
            iterator tmp = end();
            --tmp;
            erase(tmp);
        }
    }

    T& front() {
        return head->next->data;
    }

    T& back() {
        return tail->prev->data;
    }

    bool empty() const {
        return head->next == tail;
    }

    size_t size() const {
        size_t count = 0;
        for (auto it = begin(); it != end(); ++it) {
            ++count;
        }
        return count;
    }

    //error:for循环中删除元素
    void clear() {
        if (empty()) return;
        for (auto it = begin(); it != end(); ++it) {
            erase(it);
        }
    }

    iterator begin() {
        return iterator(head->next);
    }

    iterator end() {
        return iterator(tail);
    }

    void print() {
        if (empty()) {
            std::cout << "empty";
            return;
        }
        for (auto it = begin(); it != end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::flush;
    }

private:
    Node<T>* head;
    Node<T>* tail;
};


int main() {
    List<int> lst;

    // 插入元素
    lst.push_back(10);    // 链表: 10
    lst.push_front(5);    // 链表: 5, 10
    lst.push_back(15);    // 链表: 5, 10, 15
    lst.insert(++lst.begin(), 7); // 链表: 5, 7, 10, 15

    // 打印链表
    std::cout << "链表内容: ";
    lst.print(); // 输出: 5 7 10 15

    // 删除元素
    lst.pop_front(); // 链表: 7, 10, 15
    lst.pop_back();  // 链表: 7, 10

    // 打印链表
    std::cout << "删除头尾后链表内容: ";
    lst.print(); // 输出: 7 10

    // 插入和删除
    auto it = lst.begin();
    lst.insert(it, 3);  // 链表: 3, 7, 10
    lst.erase(++it);    // 链表: 3, 10

    // 打印链表
    std::cout << "插入和删除后链表内容: ";
    lst.print(); // 输出: 3 7

    // 清空链表
    lst.clear();
    std::cout << "清空后，链表是否为空: " << (lst.empty() ? "是" : "否") << std::endl;

    return 0;
}
