#ifndef TAMAGOCHI_H
#define TAMAGOCHI_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <thread>
#include <memory>
#include <atomic>

enum Actions
{
    eat,
    nap,
    play,
    stop
};

template <class T>
class MessageQueue
{
public:
    MessageQueue(){};
    
    void send(T&& msg)
    {
        std::lock_guard<std::mutex> mlock(_mutex);
        _deque.push_back(std::move(msg));
        _cond.notify_one();
    }
    
    T receive()
    {
        std::unique_lock<std::mutex> mlock(_mutex);   

        _cond.wait(mlock, [this]{return !_deque.empty();});

        Actions receivedMsg = std::move(_deque.front());
        _deque.pop_front();

        return receivedMsg;
    }

    bool isEmpty()
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        return _deque.empty();
    }

private:
    std::deque<Actions> _deque;
    std::condition_variable _cond;
    std::mutex _mutex;

};

class Tamagochi
{
public:
    // constructor / destructor
    Tamagochi(std::shared_ptr<MessageQueue<Actions>> &msgQueue, std::mutex &accessMutex);
    ~Tamagochi();

    void start();

private:

    void updateLevels();

    void waitForAction();
    void simulate();

    void feed();
    void play();
    void nap();
    void stop();

    std::condition_variable _condition;
    std::mutex _playMutex;
    std::mutex& _msgQueueAccessMutex; // lock access to _actionsQueue;

    int _hungerLevel {0};
    int _sleepLevel {0}; 

    // levels to consider that the pet is hungry or sleepy;
    const unsigned int _MIN_HUNGER_LEVEL {60};
    const unsigned int _MIN_SLEEP_LEVEL {70};
    const unsigned int _MAX_LEVEL {100};

    const unsigned int _HUNGER_RESTORATION_POINTS {40};
    const unsigned int _SLEEP_RESTORATION_POINTS {30};

    std::thread _readThread;
    std::thread _simulationThread;
    std::atomic<bool> _canContinue{true};

    std::shared_ptr<MessageQueue<Actions>> _actionsQueue;
};

#endif