#include <iostream>


template<typename T>
class SimpleUniquePtr {
public:
  SimpleUniquePtr():ptr(nullptr) {}

  explicit SimpleUniquePtr(T* p):ptr(p) {}

  ~SimpleUniquePtr() {
    delete ptr;
  }

  SimpleUniquePtr(const SimpleUniquePtr&) = delete;
  SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;

  SimpleUniquePtr(SimpleUniquePtr&& other) noexcept
    :ptr(other.ptr) {
    other.ptr = nullptr;
  }

  SimpleUniquePtr& operator=(SimpleUniquePtr&& other) noexcept {
    if (this != &other) {
      delete ptr;
      ptr = other.ptr;
      other.ptr = nullptr;
    }
    return *this;
  }

  T& operator*() const {
    return *ptr;
  }

  T* operator->() const {
    return ptr;
  }

  T* get() const {
    return ptr;
  }

  T* release() {
    T* tmp = ptr;
    ptr = nullptr;
    return tmp;
  }

  void reset(T* p = nullptr) {
    delete ptr;
    ptr = p;
  }

private:
  T* ptr;
};


class Test {
public:
    Test(int val) : value(val) {
        std::cout << "Test Constructor: " << value << std::endl;
    }
    ~Test() {
        std::cout << "Test Destructor: " << value << std::endl;
    }
    void show() const {
        std::cout << "Value: " << value << std::endl;
    }

private:
    int value;
};


int main() {
    // 创建一个 SimpleUniquePtr
    SimpleUniquePtr<Test> ptr1(new Test(1));
    ptr1->show();
    (*ptr1).show();

    // 移动所有权到 ptr2
    SimpleUniquePtr<Test> ptr2 = std::move(ptr1);
    if (ptr1.get() == nullptr) {
        std::cout << "ptr1 is now nullptr after move." << std::endl;
    }
    ptr2->show();

    // 释放所有权
    Test* rawPtr = ptr2.release();
    if (ptr2.get() == nullptr) {
        std::cout << "ptr2 is now nullptr after release." << std::endl;
    }
    rawPtr->show();
    delete rawPtr; // 手动删除

    // 使用 reset
    ptr2.reset(new Test(2));
    ptr2->show();

    ptr2.reset(); // 自动删除
    if (ptr2.get() == nullptr) {
        std::cout << "ptr2 is now nullptr after reset." << std::endl;
    }

    return 0;
}
