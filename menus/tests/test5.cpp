
#include <iostream>
class A {
	public:
	    static	void do_something(){
		    std::cout << "A is done!";
		}
};

class B {
	public:
          static	void do_something(){
			std::cout << "B is done!";
		}
};

class C{
	public:
	int x = 0;
};

template <typename WORKER>
class manager{
	public:
	static	void do_something(){
		   WORKER::do_something();
		}
	static C A[2][10];
};

template <typename WORKER>
C manager<WORKER>::A[2][10];


int main(){

	manager<A>::do_something();
	manager<B>::do_something();
        manager<A>::A[0][0].x = 100;
	
        std::cout <<  manager<A>::A[0][0].x << "\n";
	return 0;
}
