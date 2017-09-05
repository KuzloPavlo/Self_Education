#include <Windows.h>
#include <iostream>
//#include <exception>
#include <fstream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
            throw std::runtime_error("Wrong parameters count");

        std::ifstream in(argv[2], std::ios::in | std::ios::binary);
        if (!in)
            throw std::runtime_error("Can't open file " + std::string(argv[2]));


    }
    catch (std::exception& ex)
    {
        const char* what = ex.what();
        std::cout << "Exception in main: " << what << "\n";
    }

    if (::IsDebuggerPresent()) // run from VS
    {
        std::cout << std::endl;
        std::system("pause");
    }
}