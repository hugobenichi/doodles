// Convert this program to C++
//  change array to vector<>
//  inline any short function

#include <iostream>
#include <vector>

const int N = 40;

template <class S>
inline S sum3(std::vector<S> v, S accum) {
  for (S x : v) {
    accum = accum + x;
  }
  return accum;
}

inline int sum(int n, int d[]) {
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += d[i];
  }
  return sum;
}

int main() {
  std::vector<int> data;
  data.reserve(N);
  for (int i = 0; i < N; ++i) {
    data.push_back(i);
  }

  int accum = 0;
  accum = sum3(data, accum);
  std::cout << "sum is " << accum << std::endl;

  return 0;
}
