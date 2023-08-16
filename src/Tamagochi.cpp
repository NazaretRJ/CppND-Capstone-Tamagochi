#include <iostream>
#include <random>
#include "Tamagochi.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> mlock(_mutex);   

    _cond.wait(mlock, [this]{return !_deque.empty();});

    Actions receivedMsg = std::move(_deque.front());
    _deque.pop_front();

    return receivedMsg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{ 
    std::lock_guard<std::mutex> mlock(_mutex);
    _deque.push_back(std::move(msg));
    _cond.notify_one();
}


/* Implementation of class "Tamagochi" */

Tamagochi::Tamagochi()
{
    //readStatus();
}

Tamagochi::~Tamagochi()
{
    //writeStatus();
}

void Tamagochi::waitForAction()
{

}

void Tamagochi::simulate()
{

}

void Tamagochi::feed()
{
    _hungerLevel -= _HUNGER_RESTORATION_POINTS; 
    
    std::cout << "Eating...";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));    
    if (_hungerLevel  < 0)
    {
        _hungerLevel = 0;
    }
    else if(_hungerLevel > _MIN_HUNGER_LEVEL)
    {
        std::cout << "Your tamagochi is still hungry :( ";
    }
}

void Tamagochi::feed()
{
    std::cout << "Your tamagochi is very happy! :) ";
}

void Tamagochi::nap()
{
    _sleepLevel -= _SLEEP_RESTORATION_POINTS; 
    
    std::cout << "sleeping...";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (_sleepLevel  < 0)
    {
        _sleepLevel = 0;
    }
    else if(_sleepLevel > _MIN_SLEEP_LEVEL)
    {
        std::cout << "Your tamagochi is still sleepy :( ";
    }
}