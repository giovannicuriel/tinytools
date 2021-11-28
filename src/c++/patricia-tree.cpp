#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <utility>
#include <sstream>

typedef size_t Index;

////////////////////////////////////////////////////////////////////////////////
class Key {
protected:
    std::string m_Key;
public:
    Key(std::string key);
    Index findFirstDifferenceIndex(const Key& k) const;
    std::pair<Key, Key> sliceAt(Index index) const;
    inline size_t length() const { return m_Key.length(); }
    inline char firstChar() const { return m_Key[0]; };
    friend std::ostream& operator<<(std::ostream& out, const Key& obj);
};

Key::Key(std::string key): m_Key(key) {}

std::ostream& operator<<(std::ostream& out, const Key& obj) {
    out << obj.m_Key;
    return out;
}

Index Key::findFirstDifferenceIndex(const Key& other) const {
    size_t max = this->m_Key.length() < other.m_Key.length() ? this->m_Key.length() : other.m_Key.length();
    Index s = 0;
    for (; s < max; s++) {
        if (this->m_Key[s] != other.m_Key[s]) {
            break;
        }
    }
    return s;
}

std::pair<Key, Key> Key::sliceAt(Index index) const {
    return std::pair(
        Key(m_Key.substr(0, index)),
        Key(m_Key.substr(index))
    );
}

////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Node {
protected:
    Key m_Key;
    T * m_Data;
    std::map<char, Node*> m_Children;
    const Index m_Index;

    Key sliceKeyAt(Index index);
public:
    Node(Key key, T * data, Index index);
    Node* getChildAt(char) const;
    void addChild(char c, Node<T> * child);
    Key getKey() const;
    inline Index getIndex() const { return m_Index; }
    void splitNode(Index newIndex);
    void splitNode(Index newIndex, T* data);

    std::string toString() const;
    std::string toString(std::string preamble) const;

    template<typename A>
    friend std::ostream& operator<<(std::ostream& out, const Node<A>& obj);
};

template<typename T>
Node<T>::Node(Key key, T * data, Index index) :
    m_Key(key),
    m_Data(data),
    m_Index(index) {
}

template<typename T>
Node<T>* Node<T>::getChildAt(char bit) const {
    return (m_Children.contains(bit) ? m_Children.at(bit) : nullptr );
}

template<typename T>
void Node<T>::addChild(char bit, Node<T> * child) {
    m_Children[bit] = child;
}

template<typename T>
Key Node<T>::getKey() const {
    return Key(m_Key);
}

template<typename T>
Key Node<T>::sliceKeyAt(Index index) {
    auto [k1, k2] = m_Key.sliceAt(index);
    m_Key = k1;
    return k2;
}

template<typename T>
void Node<T>::splitNode(Index newIndex) {
    Key newKey = sliceKeyAt(newIndex);
    auto newNode = new Node<T>(newKey, m_Data, m_Index + newIndex);
    newNode->m_Children = m_Children;
    m_Children.clear();
    m_Data = nullptr;
    m_Children[newKey.firstChar()] = newNode;
}

template<typename T>
void Node<T>::splitNode(Index newIndex, T* data) {
    splitNode(newIndex);
    m_Data = data;
}


template<typename T>
std::string Node<T>::toString(std::string preamble) const {
    std::stringstream ss;
    ss << preamble << m_Key;
    if (m_Data != nullptr) {
        ss << " (" << *m_Data << ")";
    } else {
        ss << " (no data)";
    }
    ss << "\n";
    for (const auto& [key, value]: m_Children) {
        ss << value->toString(preamble + std::string("").assign(m_Key.length(), '.'));
    }
    return ss.str();
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Node<T>& obj) {
    out << obj.toString("");
    return out;
}


////////////////////////////////////////////////////////////////////////////////
template<typename T>
class PatriciaTree {
protected:
    Node<T>* findNode(Node<T>* currNode, std::string key, bool relaxed) const;
    Node<T>* findInChildren(Node<T>* currNode, std::string key, bool relaxed) const;
    void splitNode(Node<T> node, Index newIndex);

    Node<T> m_Root;
public:
    PatriciaTree();
    Node<T>* findNode(std::string key, bool relaxed) const;
    void insertNode(std::string key, T * data);

    template<typename A>
    friend std::ostream& operator<<(std::ostream& out, const PatriciaTree<A>& obj);
};

template<typename T>
PatriciaTree<T>::PatriciaTree() : m_Root(Key("^"), nullptr, 0) { }

template<typename T>
Node<T>* PatriciaTree<T>::findInChildren(
    Node<T>* currNode,
    std::string key,
    bool relaxed
) const {
    if (key.length() > 0) {
        Node<T>* child = currNode->getChildAt(key[0]);
        return findNode(child, key, relaxed);
    } else {
        return nullptr;
    }
}

template<typename T>
Node<T>* PatriciaTree<T>::findNode(
    Node<T>* currNode,
    std::string key,
    bool relaxed
) const {
    if (currNode == nullptr) {
        return nullptr;
    }
    auto longestIndex = currNode->getKey().findFirstDifferenceIndex(key);
    if (longestIndex == currNode->getKey().length()) {
        Node<T>* result = findInChildren(currNode, key.substr(longestIndex), relaxed);
        return (result != nullptr ? result : currNode);
    } else if (longestIndex > 0) {
        return (relaxed ? currNode : nullptr);
    } else {
        return nullptr;
    }
}

template<typename T>
Node<T>* PatriciaTree<T>::findNode(
    std::string key,
    bool relaxed
) const {
    Node<T> * child = m_Root.getChildAt(key[0]);
    if (child == nullptr) {
        return nullptr;
    }
    return findNode(child, key, relaxed);
}

template<typename T>
void PatriciaTree<T>::insertNode(
    std::string key,
    T * data
) {
    auto currNode = findNode(key, true);
    auto insertionNode = currNode == nullptr ? &m_Root : currNode;

    Key newKey(key);
    auto [prefixKey, remainderKey] = newKey.sliceAt(insertionNode->getIndex());
    auto matchingIndex = insertionNode->getKey().findFirstDifferenceIndex(remainderKey);
    if ((insertionNode->getIndex() + matchingIndex == newKey.length()) &&
        (newKey.length() == insertionNode->getKey().length())
    ) {
        // Key already exists
        return;
    } else if (insertionNode->getIndex() + matchingIndex == newKey.length()) {
        insertionNode->splitNode(matchingIndex, data);
        return;
    }

    Index newIndex = insertionNode->getIndex() + matchingIndex;
    auto [newPrefixKey, newRemainder] = remainderKey.sliceAt(matchingIndex);
    Node<T> * newNode = new Node(newRemainder, data, newIndex);

    if (matchingIndex > 0 && matchingIndex != insertionNode->getKey().length()) {
        insertionNode->splitNode(matchingIndex);
    }
    insertionNode->addChild(newRemainder.firstChar(), newNode);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const PatriciaTree<T>& obj) {
    out << obj.m_Root;
    return out;
}

int main(void) {
    PatriciaTree<std::string> pt;

    std::string command = "command";
    std::string key;

    while(command != "exit") {
        getline(std::cin, key);
        pt.insertNode(key, &command);
        std::cout << pt << "\n";
    }
    return 0;
}

