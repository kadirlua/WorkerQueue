// MIT License

// Copyright (c) 2021-2024 kadirlua

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#if (__cplusplus >= 201703L)
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

	wQueue.push(
		myCallback, 5, [](int param) {
			std::cout << "callback is called!: " << param << "\n";
		}, 15);

	std::cout << "getQueueSize: " << wQueue.getQueueSize() << "\n";

	system("PAUSE"); // wait for user input
}
