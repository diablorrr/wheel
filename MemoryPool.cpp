#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <new>
#include <stack>
using namespace std;

class MemoryPool {
public:
  MemoryPool(size_t osize, size_t psize):
    objSize(osize), poolSize(psize), pool((char*)malloc(objSize * poolSize)){
    if (pool == nullptr) {
      throw std::bad_alloc();
    }
    // 将char*指针压入stack<void*>
    for (size_t i = 0; i < poolSize; ++i) {
      freeList.push(pool + i * objSize);
    }
  }

  ~MemoryPool() {
    free(pool);
  }

  void* allocate() {
    if (freeList.empty()) {
      throw std::bad_alloc();
    }
    void* ptr = freeList.top();
    freeList.pop();
    return ptr;
  }
  // stack<void*>中存放的都是char*指针
  void deallocate(void* ptr) {
    freeList.push((char*)ptr);
  }
private:
  size_t objSize;
  size_t poolSize;
  // char*表示字节级内存
  char* pool;
  stack<void*> freeList;
};


class MyClass{
public:
  MyClass(int val):
    value(val) {
    cout << "MyClass constructor: " << value << endl;
  }

  ~MyClass() {
    cout << "MyClass deconstructor: " << value << endl;
  }

  int value;
};


int main(){
  try {
    MemoryPool pool(sizeof(MyClass), 3);
    void* mem1 = pool.allocate();
    void* mem2 = pool.allocate();
    //使用定位new
    MyClass* obj1 = new(mem1) MyClass(100);
    MyClass* obj2 = new(mem2) MyClass(200);
    cout << "obj1 value: " << obj1->value << endl;
    cout << "obj2 value: " << obj2->value << endl;
    //显示调用析构函数
    obj1->~MyClass();
    obj2->~MyClass();
    //释放内存
    pool.deallocate(mem1);
    pool.deallocate(mem2);
  } catch (const std::bad_alloc& e) {
    cerr << "Memory pool allocation error: " << e.what() <<endl;
    return 1;
  }
  return 0;
}
