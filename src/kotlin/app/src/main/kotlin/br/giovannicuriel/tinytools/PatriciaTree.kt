package br.giovannicuriel.tinytools


class PatriciaTree {

    val rootNode = Node("^", 0.0, mutableListOf(), 0)

    private fun findLongestMatchingIndex(s1: String, s2: String): Int = s1.commonPrefixWith(s2).length

    fun findNode(subKey: String, currNode: Node, relaxed: Boolean = false): Node? {
        return findLongestMatchingIndex(subKey, currNode.key).let { matchingIndex ->
            when  {
                // Could be currNode itself or one of its children
                matchingIndex == currNode.key.length -> currNode.children.firstNotNullOfOrNull {
                    findNode(subKey.substring(matchingIndex), it, relaxed)
                } ?: currNode
                // Incomplete key - no such key was found
                matchingIndex == subKey.length -> if (relaxed) currNode else null
                matchingIndex > 0 -> if (relaxed) currNode else null
                else -> null
            }
        }
    }

    fun findNode(key: String, relaxed: Boolean = false) = rootNode.children.firstNotNullOfOrNull { findNode(key, it, relaxed) }

    fun splitNode(node: Node, newIndex: Int) = node.apply {
        val nodeChildren = mutableListOf<Node>().apply { addAll(children) }
        children.clear()
        children.addAll(
            mutableListOf(
                Node(
                    node.key.substring(newIndex, node.key.length),
                    node.data,
                    nodeChildren,
                    index + newIndex
                )
            )
        )
        key = key.substring(0, newIndex)
    }


    fun insertNode(key: String, data: Double) {
        (findNode(key, true) ?: rootNode).let { currNode ->
            val subKey = key.substring(currNode.index)
            val matchingIndex = findLongestMatchingIndex(currNode.key, subKey)
            val newNode = Node(
                key.substring(currNode.index + matchingIndex, key.length),
                data,
                mutableListOf(),
                currNode.index + matchingIndex
            )
            if (matchingIndex != 0 && matchingIndex != currNode.key.length) {
                splitNode(currNode, matchingIndex)
            }
            currNode.children.add(newNode)
        }
    }
}