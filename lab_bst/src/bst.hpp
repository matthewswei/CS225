/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    return find(root, key)->value;
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    if(subtree == nullptr || subtree->key == key)
    {
        return subtree;
    }
    else if (subtree->key > key)  //may need to check for same values
    {
        return find(subtree->left, key);
    }
    else
    {
        return find(subtree->right, key);
    }
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    Node*& insertHere = find(root, key);
    if (insertHere == NULL)
    {
        insertHere = new Node(key, value);
    }
    
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    Node *& temp = second;
    second = first;
    first = temp;
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    if(this->height() == -1)    //empty case, do nothing.
    {
        return;
    }
    if(this->height() == 0 && root->key == key)
    {
        root = NULL;
        return;
    }
    //TODO: remove only node.
    remove(root, key);

}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    Node *node = find(subtree, key);
    if (node->left==NULL and node->right==NULL) {
        delete node;
    } else if (node->left==NULL) {
        node->key = node->right->key;
        node->value = node->right->value;
        node->left = node->right->left;
        node->right = node->right->right;
        delete node->right;
    } else if (node->right==NULL) {
        node->key = node->left->key;
        node->value = node->left->value;
        node->left = node->left->left;
        node->right = node->left->right;
        delete node->left;
    } else {
        Node *inorder = node->right;
        while (inorder->left!=NULL) {
            inorder = inorder->left;
        }
        node->key = inorder->key;
        node->value = inorder->value;
        node->left = inorder->left;
        node->right = inorder->right;
        delete inorder;
    }
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    return BST<K, V>(); //change 
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    return std::vector<int>(); //change
}