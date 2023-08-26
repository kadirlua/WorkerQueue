
#include <iostream>
#if __cplusplus >= 201703L
#include <filesystem>
#endif
#include <concurrency/WorkerQueue.h>

using namespace sdk::concurrency;

void foo()
{
	std::cout << "foo\n";
}

int func()
{
	std::cout << "ret\n";
	return 12;
}

int mySum(int first, int sec)
{
	std::cout << "mySum\n";
	std::cout << "param1 \t: " << first << "\t"
										   "param2 \t:"
			  << sec << "\n";

	return first + sec;
}

double mySum2(double first, double sec)
{
	std::cout << "mySum\n";
	std::cout << "param1 \t: " << first << "\t"
										   "param2 \t:"
			  << sec << "\n";

	return first + sec;
}

//  print the current directory given
void printDir(const std::string& strDir)
{
#if __cplusplus >= 201703L
	namespace fs = std::filesystem;
	try {
		const fs::path currentDir{ strDir };
		for (const auto& dir : fs::directory_iterator(currentDir)) {
			std::cout << "found: " << dir.path() << "\n";
		}
	}
	catch (const std::filesystem::filesystem_error& err) {
		std::cout << err.what() << "\n";
	}
#endif
}

class DummyClass {
public:
	DummyClass() = default;
	~DummyClass() = default;

	static void doSomeFunc(int x)
	{
		std::cout << x << "\n";
	}

	void Foo(double d) const
	{
		std::cout << d << "\n";
	}

	int Assign(int a)
	{
		mx = a;
		std::cout << "mx : " << mx << "\n";
		return mx;
	}

private:
	int mx = 0;
};

// an example of callback function
void myCallback(int par, const std::function<void(int x)>& myFn, int param)
{
	std::cout << "myCallback: " << par << "\n";
	if (myFn) {
		myFn(param);
	}
}

int main()
{
	//  Create an instances of WorkerQueue class
	WorkerQueue wQueue;

	std::cout << "version number: " << WorkerQueue::getVersionStr() << "\n";

	std::cout << "number of threads available: " << wQueue.size() << "\n";

	std::cout << "getQueueSize: " << wQueue.getQueueSize() << "\n";

	// push our functions into the worker queue.
	wQueue.push(foo);
	wQueue.push(func);
	wQueue.push([](int a, int c) {
		std::cout << "HELLO WORLD!!!\n"
				  << a << " : " << c << "\n";
	},
		14, 15);

	wQueue.push([](int a) mutable {
		std::cout << "MUTABLE FUNCTION!!!\n"
				  << a << "\n";
	},
		888);

	wQueue.push(&mySum, 3, 5);

	wQueue.push(&mySum2, 5454.1, 78.3);

	wQueue.push(&DummyClass::doSomeFunc, 23);

	wQueue.push(&DummyClass::Foo, DummyClass{}, 75);

	wQueue.push(&DummyClass::Assign, DummyClass{}, 15);

	wQueue.push(printDir, "c:\\");

	wQueue.push(
		myCallback, 5, [](int param) {
			std::cout << "callback is called!: " << param << "\n";
		}, 15);

	std::cout << "getQueueSize: " << wQueue.getQueueSize() << "\n";

	system("PAUSE"); // wait for user input
}
