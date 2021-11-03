#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <unistd.h>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

struct GenericDataChannel {
};

template<int N>
struct TemplateCounter {
    static const int index = N;
    static const int next = N + 1;
};

template<>
struct TemplateCounter<0> {
    static const int index = 1;
    static const int next = 1;
};

template<typename T>
struct DataChannel: public GenericDataChannel {
    typedef T DataType;
    static constexpr const int index = std::type_index(typeid(T));
    std::queue<T> queue;

    void send(const T& data) {
        queue.push(data);
    }

    void receive(T& data) {
        data = queue.top();
        queue.pop();
    }
};

struct DataBroker {
    std::unordered_map<std::type_index, GenericDataChannel *> channels;
    template<typename T>
    auto getDataChannel() {
        int index = DataChannel<T>::index;
        if (channels.find(index) == channels.end()) {
            channels[index] = new DataChannel<T>();
        }
        return channels[index];
    }
};

DataBroker databroker;

template<typename T>
struct Subscriber {
    virtual void receive(const T& data) {
        auto dataChannel = dataBroker.getDataChannel<T>();
        dataChannel.receive(data);
    };
};

template<typename T>
struct Publisher {
    virtual void notify(const T& data) {
        auto dataChannel = dataBroker.getDataChannel<T>();
        dataChannel.send(data);
    }
};



int main(void) {
    std::cout << "Oi\n";
    return 0;
}
