/**
 * @file test-template-2.cpp
 * @author Giovanni Curiel dos Santos
 * @brief A simple exercise on template programming
 * @version 0.1
 * @date 2021-10-24
 *
 * @copyright Copyright (c) 2021
 */

/**
 * As an exercie (yet for by battleship little game), I'll try to build a
 * publisher/subscriber framework, based on internal prioritized queues. I'll
 * add messages with arbitrary payload objects and I'll try not to use void*
 * to set them (this is where I intend to use templates). Let's see where this
 * goes.
 */

#include <utility>
#include <unistd.h>
#include <iostream>
#include <queue>

class Publisher;
class Subscriber;

template <typename T>
// class Message;


class DataBroker{ };

struct Point {
    u_int32_t x;
    u_int32_t y;
};

enum MouseEventType {
    MOUSE_CLICKED,
    MOUSE_ENTERED,
    MOUSE_EXITED
};
std::ostream& operator<<(std::ostream& out, const MouseEventType& type) {
    switch (type) {
        case MOUSE_CLICKED:
            out << "MOUSE_CLICKED";
            break;
        case MOUSE_ENTERED:
            out << "MOUSE_ENTERED";
            break;
        case MOUSE_EXITED:
            out << "MOUSE_EXITED";
            break;
        default:
            out << "UNKNOWN";
    }
    return out;
}

class MouseEvent {
protected:
    Point m_Point;
    MouseEventType m_Type;
public:
    MouseEvent(const Point & point, MouseEventType type) :
        m_Point({point.x, point.y}),
        m_Type(type) {
    }
    inline const Point& getPoint() const { return m_Point; }
    inline MouseEventType getType() { return m_Type; }
    friend std::ostream& operator<<(std::ostream &out, const MouseEvent& obj);
};

class GameEvent {
protected:
    std::string m_Event;
public:
    GameEvent(std::string event) : m_Event(event) {}
    friend std::ostream& operator<<(std::ostream &out, const GameEvent& obj);
};

enum EventType {
    MOUSE_EVENT,
    GAME_EVENT
};

class GenericEventData {
protected:
    EventType m_EventType;
public:
    GenericEventData(EventType eventType) : m_EventType(eventType) {}
    virtual const EventType getEventType() { return m_EventType; }
};

template<EventType eventType>
class EventData final : public GenericEventData {
public:
    std::string m_Data;
    EventData(const std::string& data):m_Data(data), GenericEventData(eventType) {}
    const std::string& getData() const { return m_Data; }
};

template<>
class EventData<MOUSE_EVENT> final : public GenericEventData {
protected:
    MouseEvent m_Data;
public:
    EventData(const Point & point, MouseEventType type):
        GenericEventData(MOUSE_EVENT),
        m_Data(MouseEvent(point, type))
    { }
    const MouseEvent& getData() const { return m_Data; }
};

template<>
class EventData<GAME_EVENT> final : public GenericEventData {
protected:
    GameEvent m_Data;
public:
    EventData(std::string event):
        GenericEventData(GAME_EVENT),
        m_Data(event)
    { }
    const GameEvent& getData() const { return m_Data; }
};

enum MessagePriorityType {
    LOW_PRIORITY_MESSAGE = 0,
    NORMAL_PRIORITY_MESSAGE = 1,
    HIGH_PRIORITY_MESSAGE = 2,
};

class Message {
protected:
    GenericEventData * m_Data; // this should be unique_ptr or smth.
    MessagePriorityType m_Priority;
public:
    Message(GenericEventData * data, MessagePriorityType priority):
        m_Data(data),
        m_Priority(priority)
    { };

    inline MessagePriorityType getPriority() const { return m_Priority; }
    template<EventType T>
    const auto getData() {
        // Indeed I did some checks beforehand to certify that EventData<T>
        // is indeed base of GenericEventData
        // As stated here: https://en.cppreference.com/w/cpp/language/dynamic_cast
        return static_cast<EventData<T>*>(m_Data)->getData();
    }

    inline EventType getEventType() { return m_Data->getEventType(); }
    friend std::ostream &operator<<(std::ostream & out, const Message& message);
};

class MessageComparator {
protected:
    bool m_IsReverse;
public:
    MessageComparator(const bool &reverse = false): m_IsReverse(reverse) {}
    bool operator()(const Message& lhs, const Message& rhs) {
        if (m_IsReverse) {
            return lhs.getPriority() > rhs.getPriority();
        } else {
            return lhs.getPriority() < rhs.getPriority();
        }
    }
};

std::ostream &operator<<(std::ostream & out, const Message& message) {
    out << "Message with priority " << message.m_Priority;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Point& obj) {
    out << "(" << obj.x << ", " << obj.y << ")";
    return out;
}

std::ostream& operator<<(std::ostream& out, const MouseEvent& obj) {
    out << "[" << obj.m_Type << "@" << obj.m_Point << ")]";
    return out;
};

std::ostream& operator<<(std::ostream& out, const GameEvent& obj) {
    out << "[" << obj.m_Event << "]";
    return out;
};

int main(void) {
    Message msg1 = Message(new EventData<GAME_EVENT>("This is a normal priority message"), NORMAL_PRIORITY_MESSAGE);
    Message msg2 = Message(new EventData<GAME_EVENT>("This is a low priority message"), LOW_PRIORITY_MESSAGE);
    Message msg3 = Message(new EventData<GAME_EVENT>("This is anoter low priority message"), LOW_PRIORITY_MESSAGE);
    Message msg4 = Message(new EventData<GAME_EVENT>("This is another normal priority message"), NORMAL_PRIORITY_MESSAGE);
    Message msg5 = Message(new EventData<GAME_EVENT>("This is a high priority message"), HIGH_PRIORITY_MESSAGE);
    Message ev1 = Message(new EventData<MOUSE_EVENT>({10, 10}, MOUSE_CLICKED), HIGH_PRIORITY_MESSAGE);

    std::cout << "Messages: \n";
    std::cout << msg1 << "\n";
    std::cout << msg2 << "\n";
    std::cout << msg3 << "\n";
    std::cout << msg4 << "\n";
    std::cout << msg5 << "\n";
    std::cout << ev1 << "\n";

    std::priority_queue<Message, std::vector<Message>, MessageComparator> q;
    q.push(msg1);
    q.push(msg2);
    q.push(msg3);
    q.push(msg4);
    q.push(msg5);
    q.push(ev1);

    while (!q.empty()) {
        auto event = q.top();
        std::cout << "Removing from queue: " << q.top() << "\n";
        switch (event.getEventType()) {
            case MOUSE_EVENT:
                std::cout << "Mouse event: " << event.getData<MOUSE_EVENT>() << "\n";
                break;
            case GAME_EVENT:
                std::cout << "Game event: " << event.getData<GAME_EVENT>() << "\n";
                break;
        }
        q.pop();
    }
    return 0;
}
