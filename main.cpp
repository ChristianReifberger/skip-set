#include "skip_set.h"

#include <iostream>
#include <string>

using namespace std;

void test_string_set() {
  skip_set<string> s("a", "z");

  s.insert("A");
  s.insert("B");
  s.insert("D");
  s.insert("Z");
  s.insert("G");

  cout << std::boolalpha;
  cout << "SIZE: " << s.size() << endl;

  s.print();

  bool erase5 = s.erase("A");
  bool erase60 = s.erase("Z");

  cout << "SIZE: " << s.size() << endl;
  cout << "erase 5 = " << erase5 << endl;
  cout << "erase 60 = " << erase60 << endl;
  s.print();
}

void test_int_set() {
  skip_set<int> s(0, 100);

  s.insert(5);
  s.insert(10);
  s.insert(8);
  s.insert(3);
  s.insert(20);

  cout << std::boolalpha;
  cout << "SIZE: " << s.size() << endl;

  s.print();

  bool erase5 = s.erase(5);
  bool erase60 = s.erase(60);

  cout << "SIZE: " << s.size() << endl;
  cout << "erase 5 = " << erase5 << endl;
  cout << "erase 60 = " << erase60 << endl;
  s.print();
}


int main() {
  test_string_set();
  
  return 0;
}