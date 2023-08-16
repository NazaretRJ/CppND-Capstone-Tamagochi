#include <iostream>
#include <thread>
#include <atomic>

#include "Tamagochi.h"

void getUserDecission(std::atomic<bool> &canContinue)
{
    unsigned int choice {0};
    Actions userAction{Actions::play};
    bool userStop {!canContinue};

    while(!userStop)
    {
        std::cout << "Choose an action: \n";
        std::cout << "1. Feed, 2.Sleep, 3.Play, 4.Quit \n"; 
        std::cin >> choice;

        switch (choice) {
            case 1:
                userAction = Actions::eat;
                break;
            case 2:
                userAction = Actions::nap;
                break;
            case 3:
            {
                userAction = Actions::play;
                break;
            }
            case 4:
            {
                userStop = true;
                canContinue.store(!userStop);
                break;
            }
            default:
                std::cout << "Invalid choice" << std::endl;
        }

        if(!userStop)
        {
            // send to Tamagochi
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "End THread\n";

}

int main() {

    std::atomic<bool> canContinue(true);

    std::thread userThread(getUserDecission, std::ref(canContinue));
    
    while(canContinue)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "End Main\n";

    userThread.join();
    return 0;
}