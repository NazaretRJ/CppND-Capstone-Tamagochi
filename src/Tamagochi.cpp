#include <iostream>
#include <random>
#include "Tamagochi.h"

int getRandomNumber(int min = 0, int max = 30)
{
    std::random_device rd; // Obtain a random seed from hardware
    std::mt19937 generator(rd()); // Seed the generator
    std::uniform_int_distribution<int> distribution(min, max); // Create a uniform distribution

    return distribution(generator); // Generate and return a random number in the range [min, max]
}

/* Implementation of class "Tamagochi" */

Tamagochi::Tamagochi(std::shared_ptr<MessageQueue<Actions>> &msgQueue, std::mutex &accessMutex):
    _actionsQueue{msgQueue},
    _msgQueueAccessMutex{accessMutex}
{
}

Tamagochi::~Tamagochi()
{
    _readThread.join();
    _simulationThread.join();
}

void Tamagochi::start()
{
    _readThread = std::thread(&Tamagochi::waitForAction, this);
    _simulationThread = std::thread(&Tamagochi::simulate, this);
}

void Tamagochi::updateLevels()
{
    int prov = getRandomNumber(0,50);

    std::lock_guard<std::mutex> mlock(_playMutex);
    if(prov % 2 == 0)
    {
        _hungerLevel += 10;
        
        if(_hungerLevel > _MAX_LEVEL)
        {
            _hungerLevel = _MAX_LEVEL;
        }
    }
    else
    {
        _sleepLevel += 10;

        if(_sleepLevel > _MAX_LEVEL)
        {
            _sleepLevel = _MAX_LEVEL;
        }
    }
    std::cout << "Status of your tamagochi, hunger: " <<  _hungerLevel << ", sleep: " << _sleepLevel << "\n";
}


void Tamagochi::waitForAction()
{
    Actions act;
    bool userStops = !_canContinue;
    while(!userStops)
    {
        {
            std::lock_guard<std::mutex> mlock(_msgQueueAccessMutex);
            if(_actionsQueue == nullptr)
            {
                std::cout << "Error! The shared message queue is null \n";
                userStops = true;
                _canContinue.store(!userStops);
                continue;
            }
            else{
                act = _actionsQueue->receive();
            }
            
        }
        
        switch (act) {
            case Actions::eat:
            {
                feed();
                break;
            }
            case Actions::nap:
                nap();
                break;
            case Actions::play:
            {
                play();
                break;
            }
            case Actions::stop:
            {
                userStops = true;
                _canContinue.store(!userStops);
                break;
            }
        }
    }
}

void Tamagochi::simulate()
{
    int cycleDuration = getRandomNumber(2,6) * 1000; //convert to ms

    auto startTime {std::chrono::steady_clock::now()};
    auto endTime {startTime};
    int elapsedTime {0};
    Actions generatedAction;
    std::string msg{""};
    while(_canContinue)
    {
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count(); // in ms

        if(elapsedTime >= cycleDuration)
        {
            updateLevels();
            
            {
                std::lock_guard<std::mutex> mlock(_playMutex);

                if(_hungerLevel >= _MIN_HUNGER_LEVEL)
                {
                    msg = "Your Tamagochi is hungry.\n";
                }
                
                if(_sleepLevel >= _MIN_SLEEP_LEVEL)
                {
                    msg += "Your Tamagochi is sleepy.\n";
                }
            }

            //Warn the user
            std::cout << msg;

            startTime = endTime;
            cycleDuration = getRandomNumber(2,6) * 1000; //convert to ms
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        endTime = std::chrono::steady_clock::now();
    }
}


void Tamagochi::feed()
{
    {
        std::lock_guard<std::mutex> mlock(_playMutex);

        _hungerLevel -= _HUNGER_RESTORATION_POINTS; 
    }

    std::cout << "Eating...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    
    std::lock_guard<std::mutex> mlock(_playMutex);    
    if (_hungerLevel  < 0)
    {
        _hungerLevel = 0;
    }
    else if(_hungerLevel >= _MIN_HUNGER_LEVEL)
    {
        std::cout << "Your tamagochi is still hungry :( \n";
    }
}

void Tamagochi::play()
{
    std::cout << "Your tamagochi is very happy! :) \n";
}

void Tamagochi::nap()
{
    {
        std::lock_guard<std::mutex> mlock(_playMutex);

        _sleepLevel -= _SLEEP_RESTORATION_POINTS; 
    }
    std::cout << "sleeping... \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::lock_guard<std::mutex> mlock(_playMutex);
    if (_sleepLevel  < 0)
    {
        _sleepLevel = 0;
    }
    else if(_sleepLevel >= _MIN_SLEEP_LEVEL)
    {
        std::cout << "Your tamagochi is still sleepy :( sleepy: " << _sleepLevel << "\n";
    }
}

void Tamagochi::stop()
{
    _canContinue.store(false);
}