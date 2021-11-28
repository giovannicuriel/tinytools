#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <utility>
#include <sstream>

typedef size_t Index;

class StringKeySpec {
public:
    typedef std::string KeyType;
    typedef char KeyBit;

    static KeyType slice(const KeyType& data, Index ix) { return data.substr(ix); }
    static KeyType slice(const KeyType& data, Index start, Index end) { return data.substr(start, end); }
    static size_t length(const KeyType& data) { return data.length(); }
    static KeyBit at(const KeyType& data, Index index) { return data.at(index); }
    static KeyType getDefaultKey() { return "^"; }
};

////////////////////////////////////////////////////////////////////////////////
template<typename KeySpec = StringKeySpec>
class Key {
protected:
    KeySpec::KeyType m_Key;
public:
    typedef KeySpec Spec;
    typedef KeySpec::KeyBit KeyBit;

    Key(typename KeySpec::KeyType key) : m_Key(key) {};
    Index findFirstDifferenceIndex(const Key& k) const;
    std::pair<Key, Key> sliceAt(Index index) const;
    inline size_t length() const { return KeySpec::length(m_Key); }
    inline KeySpec::KeyBit firstBit() const { return KeySpec::at(m_Key, 0); };
    inline KeySpec::KeyBit at(Index index) { return KeySpec::at(m_Key, index); };

    template<typename T>
    friend std::ostream& operator<<(std::ostream& out, const Key<T>& obj);
};

template<typename KeySpec>
std::ostream& operator<<(std::ostream& out, const Key<KeySpec>& obj) {
    out << obj.m_Key;
    return out;
}

template<typename KeySpec>
Index Key<KeySpec>::findFirstDifferenceIndex(const Key<KeySpec>& other) const {
    size_t max = KeySpec::length(this->m_Key) < KeySpec::length(other.m_Key) ? KeySpec::length(this->m_Key) : KeySpec::length(other.m_Key);
    Index s = 0;
    for (; s < max; s++) {
        if (KeySpec::at(this->m_Key, s) != KeySpec::at(other.m_Key, s)) {
            break;
        }
    }
    return s;
}

template<typename KeySpec>
std::pair<Key<KeySpec>, Key<KeySpec>> Key<KeySpec>::sliceAt(Index index) const {
    return std::pair(
        Key(KeySpec::slice(m_Key, 0, index)),
        Key(KeySpec::slice(m_Key, index))
    );
}

////////////////////////////////////////////////////////////////////////////////
template<typename DataType, typename KeyType>
class Node {
protected:
    KeyType m_Key;
    DataType m_Data;
    std::map<typename KeyType::KeyBit, Node*> m_Children;
    const Index m_Index;

    KeyType sliceKeyAt(Index index);
public:
    typedef KeyType Key;
    typedef DataType Data;

    Node(KeyType key, Data data, Index index);
    Node* getChildAt(typename KeyType::KeyBit c) const;
    void addChild(typename KeyType::KeyBit b, Node* child);
    KeyType getKey() const;
    inline Index getIndex() const { return m_Index; }
    void splitNode(Index newIndex);
    void splitNode(Index newIndex, Data data);

    std::string toString() const;
    std::string toString(std::string preamble) const;

    template<typename A, typename K>
    friend std::ostream& operator<<(std::ostream& out, const Node<A, K>& obj);
};

template<typename DataType, typename KeyType>
Node<DataType, KeyType>::Node(Key key, DataType data, Index index) :
    m_Key(key),
    m_Data(data),
    m_Index(index) {
}

template<typename DataType, typename KeyType>
Node<DataType, KeyType>* Node<DataType, KeyType>::getChildAt(typename KeyType::KeyBit bit) const {
    return (m_Children.contains(bit) ? m_Children.at(bit) : nullptr );
}

template<typename DataType, typename KeyType>
void Node<DataType, KeyType>::addChild(typename KeyType::KeyBit bit, Node<DataType, KeyType>* child) {
    m_Children[bit] = child;
}

template<typename DataType, typename KeyType>
KeyType Node<DataType, KeyType>::getKey() const {
    return Key(m_Key);
}

template<typename DataType, typename KeyType>
KeyType Node<DataType, KeyType>::sliceKeyAt(Index index) {
    auto [k1, k2] = m_Key.sliceAt(index);
    m_Key = k1;
    return k2;
}

template<typename DataType, typename KeyType>
void Node<DataType, KeyType>::splitNode(Index newIndex) {
    KeyType newKey = sliceKeyAt(newIndex);
    auto newNode = new Node<DataType, KeyType>(newKey, m_Data, m_Index + newIndex);
    newNode->m_Children = m_Children;
    m_Children.clear();
    m_Data = DataType();
    m_Children[newKey.firstBit()] = newNode;
}

template<typename DataType, typename KeyType>
void Node<DataType, KeyType>::splitNode(Index newIndex, DataType data) {
    splitNode(newIndex);
    m_Data = data;
}


template<typename DataType, typename KeyType>
std::string Node<DataType, KeyType>::toString(std::string preamble) const {
    std::stringstream ss;
    ss << preamble << m_Key;
    ss << " (" << m_Data << ")\n";
    for (const auto& [key, value]: m_Children) {
        ss << value->toString(preamble + std::string("").assign(m_Key.length(), '.'));
    }
    return ss.str();
}

template<typename DataType, typename KeyType>
std::ostream& operator<<(std::ostream& out, const Node<DataType, KeyType>& obj) {
    out << obj.toString("");
    return out;
}


////////////////////////////////////////////////////////////////////////////////
template<typename Node, typename DataType = Node::Data, typename Key = Node::Key>
class PatriciaTree {
protected:
    Node* findNode(Node* currNode, Key key, bool relaxed) const;
    Node* findInChildren(Node* currNode, Key key, bool relaxed) const;

    Node m_Root;
public:
    PatriciaTree();
    Node* findNode(Key key, bool relaxed) const;
    void insertNode(Key key, DataType data);

    template<typename N, typename A, typename K>
    friend std::ostream& operator<<(std::ostream& out, const PatriciaTree<N, A, K>& obj);
};

template<typename Node, typename Data, typename Key>
PatriciaTree<Node, Data, Key>::PatriciaTree() : m_Root(Key(Key::Spec::getDefaultKey()), Data(), 0) { }

template<typename Node, typename Data, typename Key>
Node* PatriciaTree<Node, Data, Key>::findInChildren(
    Node* currNode,
    Key key,
    bool relaxed
) const {
    if (key.length() > 0) {
        Node* child = currNode->getChildAt(key.at(0));
        return findNode(child, key, relaxed);
    } else {
        return nullptr;
    }
}

template<typename Node, typename Data, typename Key>
Node* PatriciaTree<Node, Data, Key>::findNode(
    Node* currNode,
    Key key,
    bool relaxed
) const {
    if (currNode == nullptr) {
        return nullptr;
    }
    auto longestIndex = currNode->getKey().findFirstDifferenceIndex(key);
    if (longestIndex == currNode->getKey().length()) {
        Node* result = findInChildren(currNode, key.sliceAt(longestIndex).second, relaxed);
        return (result != nullptr ? result : currNode);
    } else if (longestIndex > 0) {
        return (relaxed ? currNode : nullptr);
    } else {
        return nullptr;
    }
}

template<typename Node, typename Data, typename Key>
Node* PatriciaTree<Node, Data, Key>::findNode(
    Key key,
    bool relaxed
) const {
    Node * child = m_Root.getChildAt(key.firstBit());
    if (child == nullptr) {
        return nullptr;
    }
    return findNode(child, key, relaxed);
}

template<typename Node, typename Data, typename Key>
void PatriciaTree<Node, Data, Key>::insertNode(
    Key key,
    Data data
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
    Node * newNode = new Node(newRemainder, data, newIndex);

    if (matchingIndex > 0 && matchingIndex != insertionNode->getKey().length()) {
        insertionNode->splitNode(matchingIndex);
    }
    insertionNode->addChild(newRemainder.firstBit(), newNode);
}

template<typename Node, typename Data, typename Key>
std::ostream& operator<<(std::ostream& out, const PatriciaTree<Node, Data, Key>& obj) {
    out << obj.m_Root;
    return out;
}

int main(void) {
    std::cout << "Add new command!\n";
    PatriciaTree<Node<std::string, Key<StringKeySpec>>> pt;

    std::string command = "command";
    std::string key;

    while(command != "exit") {
        getline(std::cin, key);
        pt.insertNode(key, command);
        std::cout << pt << "\n";
    }
    return 0;
}

