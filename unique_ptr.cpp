#include <memory>
#include <iostream>
using namespace std;

class MyClass {
	public:
		MyClass() {}
		~MyClass() {}
		void doSomething() {
			std::cout << "do something" << endl;
		}
};

void process(std::unique_ptr<MyClass> ptr) {
	ptr->doSomething();
}

int main() {

	auto ptr = make_unique<MyClass>();
	process(ptr);


}
