#include <iostream>
#include <atomic>
#include <memory>

#include "Tamagochi.h"


int main() {

    std::atomic<bool> canContinue(true);
    
    std::shared_ptr<MessageQueue<Actions>> actionsQueue =  std::make_shared<MessageQueue<Actions>>();
    //MessageQueue<Actions> actionsQueue;

    unsigned int choice {0};
    Actions userAction{Actions::play};
    bool userStop {!canContinue};
    bool valid {true};

    Tamagochi pet(actionsQueue);
    pet.start();

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
                userAction = Actions::stop;

                break;
            }
            default:
            {
                std::cout << "Invalid choice" << std::endl;
                valid = false;
            }
                
        }

        if(valid)
        {
            // send to tamagochi
            actionsQueue->send(std::move(userAction));
        }

        valid = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "End Main\n";

    return 0;
}