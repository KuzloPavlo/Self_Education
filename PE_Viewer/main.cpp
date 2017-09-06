#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
            throw std::runtime_error("\nApplication: program_name <file_name>");

        std::ifstream in(argv[1], std::ios::in | std::ios::binary);
        if (!in)
            throw std::runtime_error("Can't open file " + std::string(argv[1]));

        int fileSize = 0;
        in.seekg(0, in.end);
        fileSize = in.tellg();
        in.seekg(0, in.beg);

        if (fileSize < sizeof(_IMAGE_DOS_HEADER))
            throw std::runtime_error("File too small.");

        _IMAGE_DOS_HEADER dosHeader;
        char* p = reinterpret_cast<char*>(&dosHeader);
        in.read(p, sizeof(dosHeader));

        if (IMAGE_DOS_SIGNATURE != dosHeader.e_magic) // check MZ
            throw std::runtime_error("This is not PE format.");

        in.seekg(dosHeader.e_lfanew, in.beg); // move inside file pointer to PE header

        _IMAGE_NT_HEADERS NTHeader;
        p = reinterpret_cast<char*>(&NTHeader);
        in.read(p, sizeof(NTHeader));

        if (IMAGE_NT_SIGNATURE != NTHeader.Signature) // check PE
            throw std::runtime_error("Can't find Signature.");

        std::cout << "NumberOfSections " << NTHeader.FileHeader.NumberOfSections << std::endl;

        IMAGE_SECTION_HEADER sectionHeader;
        p = reinterpret_cast<char*>(&sectionHeader);

        for (int i = 0; i < NTHeader.FileHeader.NumberOfSections; ++i)
        {
            in.read(p, sizeof(sectionHeader));

            std::stringstream ss;
            ss
                << "Section name: " << sectionHeader.Name << std::endl
                << "Virtual Size: " << std::hex << sectionHeader.Misc.VirtualSize << std::endl
                << "Virtual Address: " << std::hex << sectionHeader.VirtualAddress << std::endl
                << "Raw Size: " << std::hex << sectionHeader.SizeOfRawData << std::endl
                << "Raw Address: " << std::hex << sectionHeader.PointerToRawData << std::endl
                << std::endl;

            std::cout << ss.str();
        }
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