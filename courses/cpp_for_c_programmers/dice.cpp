#include <iostream>
#include <cstdlib>
#include <ctime>

//using namespace std;

const int sides = 6;

inline int r_sides() {
  return rand() % sides + 1;
}

template <class summable>
summable sum(const summable data[], int size, summable s=0) {
  for (int i = 0; i < size; i++) {
    s += data[i];
  }
  return s;
}

template <class T>
void swap(T& a, T&b) {
  T t = a;
  a = b;
  b = t;
}

int main(void) {

  const int n_dice = 2;
  srand(clock());
  std::cout << "\nEnter number of trials";
  int trials;
  std::cin >> trials;
  int* outcomes = new int[n_dice * sides + 1];

  for (int j = 0; j < trials; j++) {
    int d1 = r_sides();
    int d2 = r_sides();
    outcomes[d1 + d2]++;
  }

  std::cout << "\nprobability\n";
}
