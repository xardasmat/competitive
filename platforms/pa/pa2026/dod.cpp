
// #define GEN
// #define DEBUG
#ifdef GEN
#include <fstream>
#include <iomanip>
#include <sstream>
#endif

#include <iostream>

bool overflow(char a, char b, char c) {
  return (a - '0' + b - '0' + 1) % 10 == c - '0';
}

bool starter(char a, char b, char c) { return a - '0' + b - '0' < 10; }

bool is_starter(char a, char b, char c) {
  if (overflow(a, b, c))
    return a - '0' + b - '0' + 1 < 10;
  else
    return starter(a, b, c);
}

bool generate_overflow(char a, char b, char c) {
  if (overflow(a, b, c))
    return a - '0' + b - '0' + 1 >= 10;
  else
    return a - '0' + b - '0' >= 10;
}

bool finisher(char a, char b, char c) {
  return (a - '0' + b - '0') % 10 == c - '0';
}

int brute(std::string as, std::string bs, std::string cs) {
  int total = 0;
  int n = as.length();
  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      int a = std::atoi(as.substr(i, j - i + 1).c_str());
      int b = std::atoi(bs.substr(i, j - i + 1).c_str());
      int c = std::atoi(cs.substr(i, j - i + 1).c_str());
      if (a + b == c) ++total;
    }
  }
  return total;
}

#ifdef GEN
std::string gen() {
  std::ostringstream str;
  str << std::setfill('0') << std::setw(7) << std::rand() % 10000000;
  return str.str();
}
#endif

int main() {
  std::ios_base::sync_with_stdio(0);
  std::string a, b, c;
  int n;
#ifndef GEN
  std::cin >> a >> b >> c;
#endif
  n = a.length();
  long total = 0;

#ifdef GEN
  // For verification:
  for (int i = 0; i < 1000; ++i) {
    a = gen();
    b = gen();
    c = gen();
    int sol = brute(a, b, c);
    if (sol < 3) {
      --i;
      continue;
    }

    std::ofstream input("tests/dod/rand/" + std::to_string(i) + ".in");
    input << a << std::endl << b << std::endl << c << std::endl;

    std::ofstream output("tests/dod/rand/" + std::to_string(i) + ".out");
    output << sol << std::endl;
  }

  return 0;
#endif

  long starts = 0;
  bool prev_overflow = false;
  bool correct = false;
  for (int i = 0; i < n; ++i) {
#ifdef DEBUG
    std::clog << " ## " << i << " :: " << std::endl;
#endif
    if (!finisher(a[i], b[i], c[i]) && !overflow(a[i], b[i], c[i])) {
      correct = false;
      starts = 0;  // borken because does not add up
#ifdef DEBUG
      std::clog << a[i] << " + " << b[i] << " =?= " << c[i];
      std::clog << " not even adding up. Breaking completely" << std::endl;
#endif
      continue;
    }
    // verify if correctness continues
    if (correct) {
#ifdef DEBUG
      std::clog << "checking if overflow is passed correctly: " << prev_overflow
                << " & " << starter(a[i], b[i], c[i]) << std::endl;
#endif
      if (!prev_overflow)
        correct &= !generate_overflow(a[i], b[i], c[i]);
      else
        correct &= generate_overflow(a[i], b[i], c[i]);
    }

    if (!correct) {
#ifdef DEBUG
      std::clog << " broken because overflow is not correctly passed"
                << std::endl;
#endif
      starts = 0;  // broken because overflow is not correctly passed
      if (!is_starter(a[i], b[i], c[i])) {
#ifdef DEBUG
        std::clog << " and cannot be a starter of the proper sum" << std::endl;
#endif
        continue;
      }
    }
    correct = true;

    if (is_starter(a[i], b[i], c[i])) {
#ifdef DEBUG
      std::clog << "Found starter at " << i << std::endl;
#endif
      ++starts;
    }
    if (finisher(a[i], b[i], c[i])) {
#ifdef DEBUG
      std::clog << "Found finisher (" << starts << ") at " << i << std::endl;
#endif
      total += starts;
    }

    prev_overflow = overflow(a[i], b[i], c[i]);
  }
  std::cout << total << std::endl;
  return 0;
}
