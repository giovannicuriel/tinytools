package br.giovannicuriel.tinytools


class PatriciaTree {
    val rootNode = Node("^", 0.0, mutableMapOf(), 0)

    private fun findLongestMatchingIndex(s1: String, s2: String): Int = s1.commonPrefixWith(s2).length

    private fun findInChildren(currNode: Node, subKey: String, relaxed: Boolean) =
        if (subKey.length > 0) currNode.children[subKey[0]]?.let {
            findNode(subKey, it, relaxed)
        } else null

    private fun findNode(subKey: String, currNode: Node, relaxed: Boolean = false): Node? {
        return findLongestMatchingIndex(subKey, currNode.key).let { matchingIndex ->
            when {
                // Could be currNode itself or one of its children
                matchingIndex == currNode.key.length -> findInChildren(currNode, subKey.substring(matchingIndex), relaxed) ?: currNode
                // Incomplete key - no such key was found
                matchingIndex > 0 -> if (relaxed) currNode else null
                else -> null
            }
        }
    }

    fun findNode(key: String, relaxed: Boolean = false) =
        rootNode.children[key[0]]?.let { findNode(key, it, relaxed) }

    private fun splitNode(node: Node, newIndex: Int) = node.apply {
        val nodeChildren = mutableMapOf<Char, Node>().apply { putAll(children) }
        val prefixKey = key.substring(newIndex, key.length)
        children.clear()
        children.putAll(
            mutableMapOf(
                prefixKey[0] to Node(prefixKey, node.data, nodeChildren, index + newIndex)
            )
        )
        key = key.substring(0, newIndex)
    }


    fun insertNode(key: String, data: Double) {
        (findNode(key, true) ?: rootNode).let { currNode ->
            val subKey = key.substring(currNode.index)
            val matchingIndex = findLongestMatchingIndex(currNode.key, subKey)
            if (currNode.index + matchingIndex == key.length) {
                // key already exists
                return
            }
            val newNode = Node(
                key.substring(currNode.index + matchingIndex, key.length),
                data,
                mutableMapOf(),
                currNode.index + matchingIndex
            )
            if (matchingIndex != 0 && matchingIndex != currNode.key.length) {
                splitNode(currNode, matchingIndex)
            }
            currNode.children.put(newNode.key[0], newNode)
        }
    }
}