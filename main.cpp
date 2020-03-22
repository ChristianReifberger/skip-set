#include "skip_set.h"

#include <iostream>
#include <string>

#define RUN(X) std::cout << "================================" << std::endl \
  << "---- " << (#X) << " ----" << std::endl; \
  (X); \
  std::cout << "================================" << std::endl << std::endl

#define LOG(X) std::cout << (#X) << std::endl; (X)
#define DEBUG(X) std::cout << (#X) << " -> " << (X) << std::endl

using namespace std;

void test_string_set() {
  skip_set<string> s("a", "z");

  LOG(s.insert("b"));
  LOG(s.insert("g"));
  LOG(s.insert("e"));
  LOG(s.insert("u"));

  cout << endl;
  DEBUG(s.size());

  cout << endl;
  DEBUG(s.erase("g"));
  DEBUG(s.erase("x"));
  DEBUG(s.size());

  cout << endl;
  DEBUG(s.find_bool("g"));
  DEBUG(s.find_bool("x"));
  DEBUG(s.find_bool("b"));
}

void test_int_set() {
  skip_set<int> s(0, 100);

  LOG(s.insert(5));
  LOG(s.insert(10));
  LOG(s.insert(8));
  LOG(s.insert(3));
  LOG(s.insert(20));

  cout << endl;
  DEBUG(s.size());

  cout << endl;
  DEBUG(s.erase(5));
  DEBUG(s.erase(60));
  DEBUG(s.size());

  cout << endl;
  DEBUG(s.find_bool(5));
  DEBUG(s.find_bool(60));
  DEBUG(s.find_bool(20));
}

void test_forward_iterator() {
  skip_set<int> s(0, 100);

  LOG(s.insert(5));
  LOG(s.insert(10));
  LOG(s.insert(8));
  LOG(s.insert(3));
  LOG(s.insert(20));
  
  for (skip_set<int>::iterator it = s.begin(); it != s.end(); ++it) {
    DEBUG(it->value);
  }
}

void test_bidirectional_iterator() {
  skip_set<int> s(0, 100);

  LOG(s.insert(1));
  LOG(s.insert(10));
  LOG(s.insert(3));
  LOG(s.insert(50));
  LOG(s.insert(7));

  for (skip_set<int>::iterator it = s.end(); it != s.begin(); --it) {
    DEBUG(it->value);
  }
}


int main() {
  cout << std::boolalpha;

  RUN(test_int_set());
  RUN(test_string_set());
  RUN(test_forward_iterator());
  RUN(test_bidirectional_iterator());

  return 0;
}