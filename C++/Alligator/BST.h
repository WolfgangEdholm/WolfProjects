//
//  BST.h
//  Alligator
//


#ifndef BST_h
#define BST_h

#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;
using std::endl;
using std::cout;
using std::endl;

template <typename T>
class BST {
private:
  struct Node {
    T _data;
    Node *_left, *_right;
    Node(const T &d, Node *l = nullptr, Node *r = nullptr)
      : _data(d), _left(l), _right(r) {}

    //copy constructor
    Node(const Node* that);
  };

  Node *_root;
  size_t _size;

  //helpers

  bool _insert(Node *&p, const T& elem);
  bool _remove(Node *&p, const T& elem);
  bool _recursive_delete(Node *&p);
  const Node *_find_min(Node *p) const;
  Node *_find(Node *p, const T &elem) const;
  string _to_string(const Node *p) const;

  static Node *_deep_copy(const Node *p);

public:
  BST() : _root(nullptr), _size(0) { }
  virtual ~BST() { _recursive_delete(_root); }

  virtual size_t get_size() const { return _size; }

  virtual bool insert(const T &elem) { return _insert(_root, elem); }
  virtual bool remove(const T &elem) { return _remove(_root, elem); }
  virtual bool clear() { return _recursive_delete(_root); }

  virtual bool contains(const T &elem) const { return _find(_root, elem) != nullptr; }

  virtual T &find(const T &elem) const;

  virtual string to_string() const;

  class Not_found_exception : public std::exception {
  public:
    string to_string() { return "Not found exception"; }
  };

  friend class Tx;
  friend class Tests;
};

template <typename T>
BST<T>::Node::Node(const BST<T>::Node* that) {
  _data = that->_data;
  if (that->_left != nullptr) {
    _left = new Node(that->_left);
    _left->_data = that->_left->_data;
  }
  if (that->_right != nullptr) {
    _right = new Node(that->_right);
    _right->_data = that->_right->_data;
  }
}

template <typename T>
typename BST<T>::Node* BST<T>::_deep_copy(const typename BST<T>::Node *p) {
  if (p == nullptr) {
    return nullptr;
  }

  typename BST<T>::Node* copyNode = new typename BST<T>::Node(p->_data);
  if (p->_right != nullptr) {
    copyNode->_right = new typename BST<T>::Node(p->_right);
    copyNode->_right->_data = p->_right->_data;
  }
  if (p->_left != nullptr) {
    copyNode->_left = new typename BST<T>::Node(p->_left);
    copyNode->_left->_data = p->_left->_data;
  }
  return copyNode;
}

template <typename T>
bool BST<T>::_insert(Node *&p, const T& elem) {
  if (p == nullptr) {
    _size += 1;
    p = new typename BST<T>::Node(elem);
    return true;
  }
  if (elem < p->_data) {
    return _insert(p->_left, elem);
  }
  if (p->_data < elem) {
    return _insert(p->_right, elem);
  }
  return false; // duplicate
}


template <typename T>
bool BST<T>::_remove(Node *&p, const T& elem) {
  if (p == nullptr) {
    return false;
  }
  if (elem < p->_data) {
    return _remove(p->_left, elem);
  }
  if (p->_data < elem) {
    return _remove(p->_right, elem);
  }

  //found the node
  if (p->_left != nullptr && p->_right != nullptr) {
    const Node* minNode = _find_min(p->_right);
    p->_data = minNode->_data;
    _remove(p->_right, minNode->_data);
  } else {
    Node* nodeToRemove = p;
    p = (p->_left != nullptr) ? p->_left : p->_right;
    delete nodeToRemove;
    nodeToRemove = nullptr;
    _size -= 1;
  }

  return true;
}

template <typename T>
bool BST<T>::_recursive_delete(Node *&p) {
  if (p == nullptr) {
    return false;
  }
  if (p->_left != nullptr) {
    auto q = p->_left;
    p->_left = nullptr;
    _recursive_delete(q);
  }
  if (p->_right != nullptr) {
    auto q = p->_left;
    p->_left = nullptr;
    _recursive_delete(q);
  }
  auto q = p;
  p = nullptr;
  delete q;
  return true;
}

template <typename T>
const typename BST<T>::Node* BST<T>::_find_min(Node *p) const {
  if (p == nullptr) {
    return nullptr;
  }
  while (p->_left != nullptr) {
    p = p->_left;
  }
  //Elem found
  return p;
}

template <typename T>
typename BST<T>::Node* BST<T>::_find(Node *p, const T &elem) const {
  if (p == nullptr) {
    return nullptr;
  }
  if (elem < p->_data) {
    return _find(p->_left, elem);
  }
  if (p->_data < elem) {
    return _find(p->_right, elem);
  }
  //Elem found
  return p;
}

template <typename T>
T& BST<T>::find(const T &elem) const {
  typename BST<T>::Node* n = _find(_root, elem);
  if (n == nullptr) {
    throw Not_found_exception();
  }
  return n->_data;;
}

template <typename T>
string BST<T>::_to_string(const Node *p) const {
  stringstream ss;

  if (!(p->_left == nullptr && p->_right == nullptr)) {
    ss << p->_data << " : ";
    if (p->_left != nullptr) {
      ss <<  p->_left->_data;
    } else {
      ss << "[null]";
    }
    ss << " ";
    if (p->_right != nullptr) {
      ss <<  p->_right->_data;
    } else {
      ss << "[null]";
    }
    ss << endl;
  }
  if (p->_left != nullptr) {
    ss << _to_string(p->_left);
  }
  if (p->_right != nullptr) {
    ss << _to_string(p->_right);
  }
  return ss.str();
}

template <typename T>
string BST<T>::to_string() const {
  stringstream ss;
  ss << "# Tree rooted at " << _root->_data << endl;
  ss << "# size = " << _size << endl;
  ss << _to_string(_root);
  ss << "# End of Tree\n";
  return ss.str();
}

#endif /* BST_h */
