#pragma once

#include <iterator>
#include <iostream>
#include <vector>

using namespace std;

template<typename T, const int MAXLEVEL=32>
class skip_set {
public:
  typedef int size_type;

  template<typename T>
  class node {
  public:
    node(const T value, int level)
      : value(value), express(level, nullptr) {}

    T value;
    vector<node<T>*> express;
  };

  class skip_set_iterator {
  public:
    skip_set_iterator() : curr{ nullptr }, set{ nullptr } {}
    explicit skip_set_iterator(node<T>* pos, skip_set<T>& set): curr(pos), set(set) {}
    skip_set_iterator& operator=(const skip_set_iterator& other) {
      curr = other.curr;
      return *this;
    }
    skip_set_iterator& operator++() {
      curr = curr->express[0];
      return *this;
    }
    skip_set_iterator operator+(const int offset) {
      skip_set_iterator ssit = *this;
      ssit += offset;
      return ssit;
    }
    skip_set_iterator& operator += (const int offset) {
      if (offset <= 0) {
        return *this;
      }
      for (int i = 0; i < offset; ++i) {
        ++* this;
      }
      return *this;
    }
    skip_set_iterator& operator--() {
      node<T>* node = set.head;
      for (unsigned int i = node_level(set.head); i-- > 0;) {
        while (node->express[i]->value < curr->value) {
          node = node->express[i];
        }
      }
      curr = node;

      return *this;
    }


    node<T>* operator -> () { 
      if (curr == set.head) {
        curr = curr->express[0];
      } else if (curr == set.NIL) {
        node<T>* node = set.head;
        for (unsigned int i = node_level(set.head); i-- > 0;) {
          while (node->express[i]->value < curr->value) {
            node = node->express[i];
          }
        }
        curr = node;
      }
      return curr;
    }
    node<T>& operator *() { 
      if (curr == set.head) {
        curr = curr->express[0];
      } else if (curr == set.NIL) {
        node<T>* node = set.head;
        for (unsigned int i = node_level(set.head); i-- > 0;) {
          while (node->express[i]->value < curr->value) {
            node = node->express[i];
          }
        }
        curr = node;
      }
      return curr->value;
    }

    bool operator == (const skip_set_iterator& b) const { return curr == b.curr; }
    bool operator != (const skip_set_iterator& b) const { 
      return curr != b.curr; }
    
  private:
    node<T>* curr;
    skip_set<T>& set;
    friend class skip_set;
  };

  using iterator = skip_set_iterator;

  skip_set(T head_val, T nil_val)
    : probability(0.5), max_level(MAXLEVEL),
      head_val(head_val), nil_val(nil_val) {
    head = new node<T>(head_val, max_level);
    NIL = new node<T>(nil_val, max_level);

    std::fill(head->express.begin(), head->express.end(), NIL);
  }

  ~skip_set() {
    auto node = head;
    while (node->express[0]) {
      auto temp = node;
      node = node->express[0];
      delete temp;
    }
    delete node;
  }

  int size() const {
    int count = 0;
    node<T>* node = head;
    while (node->express[0] && node->express[0] != NIL) {
      count++;
      node = node->express[0];
    }
    return count;
  }

  bool find_bool(T value) {
    return find_node(value) != nullptr;
  }

  iterator find(T value) {
    node<T>* temp = find_node(value);
    return temp != nullptr ? skip_set_iterator(temp) : nullptr;
  }

  void insert(T value) {
    vector<node<T>*> preds = get_predecessors(value);

    // It's a set so if it already exists do nothing
    auto next = preds[0]->express[0];
    if (next->value == value && next != NIL) {
      return;
    }

    // create new node
    const int newNodeLevel = random_level();
    node<T>* newNodePtr = create_node(value, newNodeLevel);

    // connect pointers of predecessors and new node to respective successors
    for (int i = 0; i < newNodeLevel; ++i) {
      newNodePtr->express[i] = preds[i]->express[i];
      preds[i]->express[i] = newNodePtr;
    }
  }

  bool erase(T value) {
    vector<node<T>*> preds = get_predecessors(value);

    // check if the node exists
    node<T>* node = preds[0]->express[0];
    if (node->value != value || node == NIL) {
      return false;
    }

    // update pointers and delete node 
    for (size_t i = 0; i < node_level(node); ++i) {
      preds[i]->express[i] = node->express[i];
    }
    delete node;
    return true;
  }

  void print() {
    node<T>* list = head->express[0];

    cout << "{";

    while (list != NIL) {
      std::cout << "value: " << list->value
        << ", level: " << node_level(list);

      list = list->express[0];

      if (list != NIL) cout << " : ";
      if (list != NIL && node_level(list) != node_level(list->express[0])) {
        cout << endl;
      }
    }
    cout << "}\n";
  }

  iterator begin() {
    return skip_set_iterator(head, *this);
  }

  iterator end() {
    return skip_set_iterator(NIL, *this);
  }

private:

  int random_level() {
    int l = 1;
    while (((double)std::rand() / RAND_MAX) < probability &&
      l < max_level) {
      l++;
    }
    return l;
  }

  static size_t node_level(const node<T>* v) {
    return v->express.size();
  }

  static node<T>* create_node(T val, int level) {
    return new node<T>(val, level);
  }

  node<T>* find_node(T value) const {
    if (node<T>* temp = lower_bound(value)) {
      if (temp->value == value && temp != NIL) {
        return temp;
      }
    }
    return nullptr;
  }

  node<T>* lower_bound(T value) const {
    node<T>* node = head;
    for (unsigned int i = node_level(head); i-- > 0;) {
      while (node->express[i]->value < value) {
        node = node->express[i];
      }
    }
    return node->express[0];
  }

  vector<node<T>*> get_predecessors(T value) const {
    vector<node<T>*> result(node_level(head), nullptr);
    node<T>* temp = head;

    for (unsigned int i = node_level(head); i-- > 0;) {
      while (temp->express[i]->value < value) {
        temp = temp->express[i];
      }
      result[i] = temp;
    }
    return result;
  }

  node<T>* head; // first node
  node<T>* NIL; // last node

  T head_val;
  T nil_val;

  float probability;
  int max_level;
};
