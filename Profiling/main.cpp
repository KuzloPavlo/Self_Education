#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

const char* const g_inputFileName = "text.txt";
const char* const g_outputFileName1 = "HardThreadFile1";
const char* const g_outputFileName2 = "HardThreadFile2";

const int g_simpleThreadCount = 7;

void Start();
void HardThread(const std::string& fileName);
void SimpleThread();

int main()
{
	Start();
	std::cin.get();
	return 0;
}

void ReadFile(const std::string& fileName, std::vector<char>* file)
{
	try
	{
		file->clear();
		std::ifstream in(fileName, std::ios::in | std::ios::binary);
		if (!in)
			throw std::runtime_error("Can't open file " + fileName);

		int fileSize = 0;
		in.seekg(0, in.end);
		fileSize = in.tellg();
		in.seekg(0, in.beg);

		std::vector<char> buff(fileSize);
		in.read(&buff[0], fileSize);

		file->swap(buff);
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << " " << ex.what() << std::endl;
	}
}

void WriteFile(const std::string& fileName, const std::vector<char>& file)
{
	try
	{
		std::ofstream out(fileName, std::ios::out | std::ios::binary);
		if (!out)
			throw std::runtime_error("Can't open file " + fileName);

		out.write(&file[0], file.size());
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << " " << ex.what() << std::endl;
	}
}

void HardThread(const std::string& fileName)
{
	while (true)
	{
		std::vector<char> file;
		ReadFile(g_inputFileName, &file);
		WriteFile(fileName, file);
        
        file.clear();
        ReadFile(fileName, &file);
		std::remove(fileName.c_str());
	}
}

void SimpleThread()
{
	while (true)
	{
		std::string str;
		str += "blah-blah-blah";
		const std::string str2(str);

		if (str2 == str)
		{
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void Start()
{
	for (size_t i = 0; i < g_simpleThreadCount; ++i)
	{
		std::thread simpleThread(SimpleThread);
		simpleThread.detach();
	}

	std::thread hardThread1(HardThread, g_outputFileName1);
	hardThread1.detach();
	std::thread hardThread2(HardThread, g_outputFileName2);
	hardThread2.detach();
}