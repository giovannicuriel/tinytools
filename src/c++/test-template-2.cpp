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

#include <unistd.h>
#include <iostream>
#include <queue>

class Publisher;
class Subscriber;

template <typename T>
// class Message;


class DataBroker{

};

enum MessagePriorityType {
    LOW_PRIORITY_MESSAGE = 0,
    NORMAL_PRIORITY_MESSAGE = 1,
    HIGH_PRIORITY_MESSAGE = 2,
};

enum EventType {
    MOUSE_CLICKED
};

template<typename T>
class Message {
protected:
    T m_Data;
    MessagePriorityType m_Priority;
public:
    Message() = delete;
    Message(const T& data, MessagePriorityType priority):
        m_Data(data),
        m_Priority(priority)
    {
        // How about a default constructor? with = default
    };

    inline MessagePriorityType getPriority() const { return m_Priority; }
    inline const T & getData() const { return m_Data; }
    template<typename X>
    friend std::ostream &operator<<(std::ostream & out, const Message<X>& message);
};

template<typename T>
class MessageComparator {
protected:
    bool m_IsReverse;
public:
    MessageComparator(const bool &reverse = false): m_IsReverse(reverse) {}
    bool operator()(const Message<T>& lhs, const Message<T>& rhs) {
        if (m_IsReverse) {
            return lhs.getPriority() > rhs.getPriority();
        } else {
            return lhs.getPriority() < rhs.getPriority();
        }
    }
};

struct Point {
    u_int32_t x;
    u_int32_t y;
};
class MouseEvent {
protected:
    Point m_Point;
    EventType m_Type;
public:
    MouseEvent(const Point & point, EventType type) :
        m_Point({point.x, point.y}),
        m_Type(type) {

    }

    inline const Point& getPoint() const { return m_Point; }
    inline EventType getType() { return m_Type; }
    friend std::ostream& operator<<(std::ostream &out, const MouseEvent& obj);
};

template<typename T>
std::ostream &operator<<(std::ostream & out, const Message<T>& message) {
    out << "Message with priority " << message.m_Priority << ", data: " << message.m_Data;
    return out;
}
std::ostream& operator<<(std::ostream& out, const EventType& type) {
    switch (type) {
        case MOUSE_CLICKED:
            out << "MOUSE_CLICKED";
            break;
        default:
            out << "UNKNOWN";
    }
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
typedef Message<std::string> StringMessage;
typedef Message<const MouseEvent> MouseEventMessage;

int main(void) {

    StringMessage msg1 = StringMessage("This is a normal priority message", NORMAL_PRIORITY_MESSAGE);
    StringMessage msg2 = StringMessage("This is a low priority message", LOW_PRIORITY_MESSAGE);
    StringMessage msg3 = StringMessage("This is anoter low priority message", LOW_PRIORITY_MESSAGE);
    StringMessage msg4 = StringMessage("This is another normal priority message", NORMAL_PRIORITY_MESSAGE);
    StringMessage msg5 = StringMessage("This is a high priority message", HIGH_PRIORITY_MESSAGE);
    MouseEventMessage ev1 = MouseEventMessage(MouseEvent({10, 10}, MOUSE_CLICKED), HIGH_PRIORITY_MESSAGE);

    std::cout << "Messages: \n";
    std::cout << msg1 << "\n";
    std::cout << msg2 << "\n";
    std::cout << msg3 << "\n";
    std::cout << msg4 << "\n";
    std::cout << msg5 << "\n";
    std::cout << ev1 << "\n";

    std::priority_queue<StringMessage, std::vector<StringMessage>, MessageComparator<std::string>> q;
    q.push(msg1);
    q.push(msg2);
    q.push(msg3);
    q.push(msg4);
    q.push(msg5);

    auto& eventData = ev1.getData();
    auto& point = eventData.getPoint();
    std::cout << "New point: " << point << "\n";
    std::cout << "New event: " << ev1 << "\n";
    Point p({ev1.getData().getPoint().x, ev1.getData().getPoint().y});
    std::cout << "Point: " << p << "\n";

    while (!q.empty()) {
        std::cout << "Removing from queue: " << q.top() << "\n";
        // auto & data = q.top().getData();
        // data.
        q.pop();
    }

    return 0;
}
