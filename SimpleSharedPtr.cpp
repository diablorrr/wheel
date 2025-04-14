#include <iostream>
using namespace std;

struct ControlBlock {
  int ref_count;
  ControlBlock() : ref_count(1) {}
};

template <typename T> class SimpleSharedPtr {
public:
  SimpleSharedPtr() : ptr(nullptr), control(nullptr) {
    cout << "call SimpleSharedPtr default constructor" << endl;
  }

  explicit SimpleSharedPtr(T *p) : ptr(p) {
    if (p) {
      control = new ControlBlock();
      cout << "call SimpleSharedPtr constructor, ref_count = "
           << control->ref_count << endl;
    } else {
      control = nullptr;
    }
  }

  ~SimpleSharedPtr() { release(); }

  SimpleSharedPtr(const SimpleSharedPtr &other)
      : ptr(other.ptr), control(other.control) {
    if (control) {
      control->ref_count++;
      cout << "copied SimpleSharedPtr, ref_count = " << control->ref_count
           << endl;
    }
  }

  // 赋值运算符：左减右增
  SimpleSharedPtr &operator=(const SimpleSharedPtr &other) {
    if (this != &other) {
      release();
      ptr = other.ptr;
      control = other.control;
      if (control) {
        control->ref_count++;
        cout << "assigned SimpleSharedPtr, ref_count = " << control->ref_count
             << endl;
      }
    }
    return *this;
  }

  SimpleSharedPtr(SimpleSharedPtr &&other) noexcept
      : ptr(other.ptr), control(other.control) {
    other.ptr = nullptr;
    other.control = nullptr;
  }

  SimpleSharedPtr &operator=(SimpleSharedPtr &&other) noexcept {
    if (this != &other) {
      release();
      ptr = other.ptr;
      control = other.control;
      other.ptr = nullptr;
      other.control = nullptr;
    }
    return *this;
  }

  T &operator*() const { return *ptr; }

  T *operator->() const { return ptr; }

  int use_count() const { return control ? control->ref_count : 0; }

  T *get() const { return ptr; }

  void reset(T *p = nullptr) {
    release();
    ptr = p;
    if (p) {
      control = new ControlBlock();
      cout << "reset SimpleSharedPtr, ref_count = " << control->ref_count
           << endl;
    } else {
      control = nullptr;
    }
  }

private:
  T *ptr;
  ControlBlock *control;

  // 减少引用计数，脱离对资源的控制
  void release() {
    if (control) {
      control->ref_count--;
      cout << "decremented ref_count to " << control->ref_count << endl;
      if (control->ref_count == 0) {
        delete ptr;
        delete control;
        cout << "resource and controlblock destroyed" << endl;
      }
    }
    ptr = nullptr;
    control = nullptr;
  }
};

class Test {
public:
  Test(int val) : value(val) { cout << "Test construct: " << value << endl; }

  ~Test() { cout << "Test destroy: " << value << endl; }

  void show() const { cout << "Value: " << value << endl; }

private:
  int value;
};

int main() {
  {
    std::cout << "Creating default constructed shared_ptr..." << std::endl;
    SimpleSharedPtr<Test> ptr1; // 默认构造
    std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;

    std::cout << "\nCreating shared_ptr with resource..." << std::endl;
    SimpleSharedPtr<Test> ptr2(new Test(100)); // 非默认构造
    std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    ptr2->show();

    std::cout << "\nCopying ptr2 to ptr3..." << std::endl;
    SimpleSharedPtr<Test> ptr3 = ptr2; // 拷贝构造
    std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    std::cout << "ptr3 use_count: " << ptr3.use_count() << std::endl;
    ptr3->show();

    std::cout << "\nAssigning ptr3 to ptr1..." << std::endl;
    ptr1 = ptr3; // 拷贝赋值
    std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
    std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    std::cout << "ptr3 use_count: " << ptr3.use_count() << std::endl;

    std::cout << "\nResetting ptr2..." << std::endl;
    ptr2.reset(new Test(200)); // 重新指向新的对象
    std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    ptr2->show();
    std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
    std::cout << "ptr3 use_count: " << ptr3.use_count() << std::endl;

    std::cout << "\nExiting scope..." << std::endl;
  } // ptr2, ptr1, ptr3 离开作用域

  std::cout << "End of main." << std::endl;
  return 0;
}
