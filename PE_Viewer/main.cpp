#include <Windows.h>
#include <iostream>
#include <sstream>
//#include <exception>
#include <fstream>

int main(int argc, char* argv[])
{
    try
    {
        // if (argc != 2)
             //throw std::runtime_error("Wrong parameters count");

        std::ifstream in(/*"text.txt"*/"CertUtilTest.exe"/*argv[2]*/, std::ios::in | std::ios::binary);
        if (!in)
            throw std::runtime_error("Can't open file " + std::string(argv[2]));

        int fileSize = 0;
        in.seekg(0, in.end);
        fileSize = in.tellg();
        in.seekg(0, in.beg);

        if (fileSize < sizeof(_IMAGE_DOS_HEADER))
            throw std::runtime_error("File too small.");

        _IMAGE_DOS_HEADER dosHeader;

        char* p = reinterpret_cast<char*>(&dosHeader);
        in.read(p, sizeof(dosHeader));

        int MZWord = 23117;
        if (MZWord != dosHeader.e_magic)
            throw std::runtime_error("This is not PE format.");

        in.seekg(dosHeader.e_lfanew, in.beg); // move pointer to PE header

        _IMAGE_NT_HEADERS NTHeader;
        p = reinterpret_cast<char*>(&NTHeader);
        in.read(p, sizeof(NTHeader));

        p;


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