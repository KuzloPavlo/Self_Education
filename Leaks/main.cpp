#include <iostream>
#include <thread>
#include <vector>

void LeakImpl(const int buffSize, const int sleepForMS)
{
    try
    {
        std::vector<int> memoryBuffer(1024 * 1024); // will free memory

        while (true)
        {
            char* emptiness = new char[buffSize];
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepForMS));
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << std::this_thread::get_id() << ' ' << ex.what() << std::endl;
    }
}

int main()
{
    std::thread SmallLeak(LeakImpl, 16, 10);
    SmallLeak.detach();

    std::thread MiddlelLeak(LeakImpl, 512, 100);
    MiddlelLeak.detach();

    std::thread BigLeak(LeakImpl, 2048, 500);
    BigLeak.detach();

    std::cin.get();
    return 0;
}