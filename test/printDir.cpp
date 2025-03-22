// MIT License

// Copyright (c) 2021-2025 kadirlua

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

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <concurrency/WorkerQueue.h>
#include <string>
#include <thread>

using namespace sdk::concurrency;

namespace{
	constexpr auto const DEFAULT_MS = 100;
	//  print the current directory given
	void printDir(const std::string& strDir)
	{
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
	}
}

int main(int argc, const char * argv[])
{
	if (argc != 2) {
		std::cout << "Missing argument.\r\nUsage <exe_name> <directory path>";
		return EXIT_FAILURE;
	}

	const char* dirPath = argv[1];
	if (!std::filesystem::exists(dirPath)) {
		std::cout << "Directory does not exist.\r\n";
		return EXIT_FAILURE;
	}
	WorkerQueue wQueue;
	wQueue.push(printDir, dirPath);
	// wait until worker queue has finished
	while (!wQueue.empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(DEFAULT_MS));
	}
	return EXIT_SUCCESS;
}
