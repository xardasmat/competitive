#include <cmath>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

char S[3] = {'P', 'K', 'N'};

int to_tit(char c) {
  if (c == S[0])
    return 0;
  else if (c == S[1])
    return 1;
  else if (c == S[2])
    return 2;
  else
    return -1;
}

std::string xor_rand(std::string s, bool master) {
  std::vector<bool> B;
  int a = master ? 100003 : 200003;
  int c = master ? 499 : 601;
  int bits = a * a + c;
  while (B.size() < s.size()) {
    bits = a * bits + c;
    for (int j = 0; j < 8; ++j) {
      B.push_back(bits & 1);
      bits >>= 1;
    }
  }
  auto b = B.begin();
  for (char& c : s) c = char(bool(c - '0') ^ *(b++)) + '0';
  return s;
}

std::string to_bits(int pack, int bits = 19) {
  std::string msg;
  for (int i = 0; i < bits; ++i) {
    msg.push_back((pack & (1 << i)) ? '1' : '0');
  }
  return msg;
}

char winner(char looser) {
  if (looser == 'P')
    return 'N';
  else if (looser == 'K')
    return 'P';
  else if (looser == 'N')
    return 'K';
  else
    return -1;
}

int count_score(char me, char them) {
  if (me == them)
    return 0;
  else if (winner(/*looser=*/me) == them)
    return -1;
  else
    return 1;
}

int n;
std::vector<int> to_packs(const std::string& msg, int bits = 19) {
  std::vector<int> P;
  int b = 0;
  int v = 0;
  for (int i = 0; i < n; ++i) {
    if (msg[i] == '1') v |= 1 << b;
    ++b;
    if (b >= bits) {
      P.push_back(v);
      b = 0;
      v = 0;
    }
  }
  if (b > 0) P.push_back(v);
  return P;
}

int play(int output_pack, int tits = 12) {
  int input_pack = 0;
  int mul = 1;
  for (int i = 0; i < tits; ++i) {
    char output_c = S[output_pack % 3];
    output_pack /= 3;
    std::cout << output_c << std::endl;
    char input_c;
    std::cin >> input_c;
    input_pack += to_tit(input_c) * mul;
    mul *= 3;

    if (output_c == input_c) continue;  // tie
    // reverse
    std::cout << input_c << std::endl;
    std::cin >> input_c;
  }
  return input_pack;
}

class Game {
 public:
  Game(std::string bits) : bits(bits) {}

  std::string play() {
    while (i < bits.size() || received_msg.size() < bits.size()) {
      play_one();
    }
    received_msg.resize(bits.size());
    return received_msg;
  }
  void play_one() {
    if (score == 0) {
      char output_c = encode_next();
      std::cout << output_c << std::endl;
      char input_c;
      std::cin >> input_c;
      received_msg += decode(input_c);
      score += count_score(output_c, input_c);
    } else if (score == -1) {
      std::cout << 'K' << std::endl;
      char input_c;
      std::cin >> input_c;
      score = 0;
    } else if (score == 1) {
      std::cout << 'N' << std::endl;
      char input_c;
      std::cin >> input_c;
      score = 0;
    }
  }

  char encode_next() {
    if (i >= bits.size()) return S[i * 499 % 3];
    if (bits[i] == '0') {
      ++i;
      return 'K';
    } else {
      ++i;
      if (bits[i] == '0') {
        ++i;
        return 'N';
      } else {
        ++i;
        return 'P';
      }
    }
  }

  std::string decode(char c) {
    if (c == 'K')
      return "0";
    else if (c == 'N')
      return "10";
    else if (c == 'P')
      return "11";
    else
      return "";
  }

  std::string bits;
  std::string received_msg;
  int i = 0;
  int score = 0;
};

class BlocksGame {
 public:
  BlocksGame(std::string bits, bool master) : bits(bits), master(master) {}

  std::string play() {
    while (i < bits.size() || received_msg.size() < bits.size()) {
      play_one();
    }
    received_msg.resize(bits.size());
    return received_msg;
  }

  void play_one() {
    if (outl == outr) get_block();
    if (inl == inr) put_block();

    if (score == 0) {
      __int128_t out_diff = outr - outl;
      __int128_t outm1 = outl + out_diff / 3;
      __int128_t outm2 = outl + 2 * out_diff / 3;

      char outc;
      if (outm <= outm1) {
        outc = 'K';
        outl = outl;
        outr = outm1;
      } else if (outm <= outm2) {
        outc = 'N';
        outl = outm1 + 1;
        outr = outm2;
      } else {
        outc = 'P';
        outl = outm2 + 1;
        outr = outr;
      }
      std::cout << outc << std::endl;

      __int128_t in_diff = inr - inl;
      __int128_t inm1 = inl + in_diff / 3;
      __int128_t inm2 = inl + 2 * in_diff / 3;

      char inc;
      std::cin >> inc;
      if (inc == 'K') {
        inl = inl;
        inr = inm1;
      } else if (inc == 'N') {
        inl = inm1 + 1;
        inr = inm2;
      } else if (inc == 'P') {
        inl = inm2 + 1;
        inr = inr;
      }
      score += count_score(outc, inc);
    } else if (score == 1 || score == -1) {
      __int128_t in_diff = inr - inl;
      __int128_t out_diff = outr - outl;
      int out_bits = std::max(int(bits.size()) - i + outbitcount, 0);
      int in_bits =
          std::max(int(bits.size()) - int(received_msg.size()) + inbitcount, 0);

      bool non_master = !master;
      if (std::tie(out_bits, out_diff, master) >
          std::tie(in_bits, in_diff, non_master)) {
        __int128_t outm1 = outl + out_diff / 4;
        char outc;
        if (outm <= outm1) {
          outc = 'K';
          outl = outl;
          outr = outm1;
        } else {
          outc = score == 1 ? 'N' : 'P';
          outl = outm1 + 1;
          outr = outr;
        }
        std::cout << outc << std::endl;
        char inc;
        std::cin >> inc;
        score += count_score(outc, inc);
        if (inc != 'K') std::clog << "Why not K?" << std::endl;
      } else {
        char outc = 'K';
        std::cout << outc << std::endl;

        __int128_t inm1 = inl + in_diff / 4;
        char inc;
        std::cin >> inc;
        if (inc == 'K') {
          inl = inl;
          inr = inm1;
        } else {
          inl = inm1 + 1;
          inr = inr;
        }
        score += count_score(outc, inc);
      }

    } else
      std::cout << "xD" << std::endl;

    if (outl == outr) get_block();
    if (inl == inr) put_block();
  }

  void get_block() {
    outm = 0;
    outbitcount = 0;
    i = j;
    while (outbitcount < 120 && j < bits.size()) {
      outm <<= 1;
      outm |= bits[j] == '1' ? 1 : 0;
      ++outbitcount;
      ++j;
    }
    if (outbitcount == 0) outbitcount = 1;
    outl = 0;
    outr = __int128_t(1) << outbitcount;
    --outr;
  }

  void put_block() {
    __int128_t block = inl;
    int bitcount = inbitcount;
    while (bitcount > 0 && received_msg.size() < bits.size()) {
      --bitcount;
      if (block & (__int128_t(1) << bitcount))
        received_msg.push_back('1');
      else
        received_msg.push_back('0');
    }
    inbitcount =
        std::max(1, std::min(int(bits.size() - received_msg.size()), 120));
    inl = 0;
    inr = __int128_t(1) << inbitcount;
    --inr;
  }

  std::string bits;
  std::string received_msg;
  int i = 0;
  int j = 0;
  int score = 0;
  int outbitcount = 0;

  // input range
  __int128_t inl = 0;
  __int128_t inr = 0;
  int inbitcount = 0;
  // output range
  __int128_t outl = 0;
  __int128_t outr = 0;
  __int128_t outm = 0;

  bool master;
};

int main() {
  std::ios_base::sync_with_stdio(0);
  std::string role;
  std::cin >> role;
  bool master = role == "Algosia";
  int T;
  std::cin >> n >> T;

  for (int t = 0; t < T; ++t) {
    std::string msg;
    std::cin >> msg;
    std::string input_msg;
    int nn = n;
    // huffman
    // Game game(xor_rand(msg, master));
    // input_msg = game.play();

    // optimal
    BlocksGame game(xor_rand(msg, master), master);
    input_msg = game.play();

    // tricodes
    // for (int output_pack : to_packs(xor_rand(msg, master))) {
    //   int input_pack = play(output_pack);
    //   int bits = std::min(19, nn);
    //   nn -= bits;
    //   input_msg += to_bits(input_pack, bits);
    // }

    std::cout << "! " << xor_rand(input_msg, !master) << std::endl;
  }

  return 0;
}
