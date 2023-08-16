#ifndef TAMAGOCHI_H
#define TAMAGOCHI_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <thread>

enum Actions
{
    eat,
    nap,
    play
};

template <class T>
class MessageQueue
{
public:
    void send(T&& phase);
    T receive();
private:
    std::deque<Actions> _deque;
    std::condition_variable _cond;
    std::mutex _mutex;

};

class Tamagochi
{
public:
    // constructor / destructor
    Tamagochi();
    ~Tamagochi();

    void waitForAction();
    void simulate();

    void feed();
    void play();
    void nap();

private:

    //void readStatus();
    //void writeStatus();

    //void getStatus();

    std::condition_variable _condition;
    std::mutex _mutex;
    unsigned int _hungerLevel {0};
    unsigned int _sleepLevel {0}; 
    MessageQueue<Actions> _actionsQueue;

    // levels to consider that the pet is hungry or sleepy;
    const unsigned int _MIN_HUNGER_LEVEL {50};
    const unsigned int _MIN_SLEEP_LEVEL {70};

    const unsigned int _HUNGER_RESTORATION_POINTS {40};
    const unsigned int _SLEEP_RESTORATION_POINTS {30};

};

#endif