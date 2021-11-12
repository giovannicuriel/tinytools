#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <unistd.h>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

template <typename T>
struct DataChannel
{
    static DataChannel<T> *instance;
    static DataChannel<T> *getInstance();
    std::queue<T> queue;
    void send(T data) {
        queue.push(data);
    }
    T receive() {
        auto data = queue.front();
        queue.pop();
        return data;
    }
};
template <typename T>
DataChannel<T> *DataChannel<T>::instance;

template <typename T>
DataChannel<T> *DataChannel<T>::getInstance()
{
    if (DataChannel<T>::instance == nullptr)
    {
        DataChannel<T>::instance = new DataChannel<T>();
    }
    return DataChannel<T>::instance;
}

template <typename T>
struct Subscriber
{
    T receive()
    {
        auto dataChannel = DataChannel<T>::getInstance();
        return dataChannel->receive();
    };
};

template <typename T>
struct Publisher
{
    void notify(T data)
    {
        auto dataChannel = DataChannel<T>::getInstance();
        dataChannel->send(data);
    }
};

struct MouseEvent
{
    int x;
    int y;
};
struct GameEvent
{
    std::string event;
};

std::ostream &operator<<(std::ostream &out, const MouseEvent &ev)
{
    out << "(" << ev.x << ", " << ev.y << ")";
    return out;
}

int main(void)
{
    std::cout << "Oi\n";
    auto s = Subscriber<MouseEvent>();
    auto p = Publisher<MouseEvent>();
    auto ev1 = MouseEvent({x : 10, y : 11});
    p.notify(ev1);
    auto ev = s.receive();
    std::cout << ev << "\n";
    return 0;
}
