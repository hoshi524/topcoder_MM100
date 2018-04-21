#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;
typedef long long ll;
typedef int16_t int16;

class Timer {
 public:
  void restart();
  double getElapsed();

  Timer();

 private:
  static double rdtsc_per_sec_inv;

  double getTimeOfDay();
  unsigned long long int getCycle();

  double start_time;
  unsigned long long int start_clock;
};
double Timer::rdtsc_per_sec_inv = -1;

inline double Timer::getElapsed() {
  if (rdtsc_per_sec_inv != -1)
    return (double)(getCycle() - start_clock) * rdtsc_per_sec_inv;

  const double RDTSC_MEASUREMENT_INTERVAL = 0.1;
  double res = getTimeOfDay() - start_time;
  if (res <= RDTSC_MEASUREMENT_INTERVAL) return res;

  rdtsc_per_sec_inv = 1.0 / (getCycle() - start_clock);
  rdtsc_per_sec_inv *= getTimeOfDay() - start_time;
  return getElapsed();
}

inline void Timer::restart() {
  start_time = getTimeOfDay();
  start_clock = getCycle();
}

Timer::Timer() { restart(); }

inline double Timer::getTimeOfDay() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 0.000001;
}

inline unsigned long long int Timer::getCycle() {
  unsigned int low, high;
  __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
  return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
}

Timer timer;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

constexpr float TIME_LIMIT = 2.5;
constexpr int MAX_C = 6;
constexpr int S = 1 << 7;

int16 X[S][S];
int16 CP[MAX_C][S * S];
int16 P1[S * S];
ll SUM[S][S];
int H, W, C, P, B;
int R1, R2, R3;
int bit;
int16 RESULT1[S * S / 2][2];
int16 RESULT2[S * S / 2][2];
int16 RESULT3[S * S / 2][2];

class SameColorPairs {
 public:
  vector<string> removePairs(vector<string> board) {
    H = board.size();
    W = board[0].size();
    B = H * W / 2;
    C = 0;
    R1 = 0, R2 = 0, R3 = 0;
    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        int c = board[i][j] - '0' + 1;
        if (C < c) C = c;
      }
    }
    bit = 60 / C;
    for (int test = 0, back = 0; timer.getElapsed() < TIME_LIMIT; ++test) {
      memset(X, 0, sizeof(X));
      memset(SUM, 0, sizeof(SUM));
      for (int i = 0; i < C; ++i) CP[i][0] = 1;
      for (int i = 0; i < R1; ++i) {
        auto off = [&](int p) { X[p >> 8][p & 0xff] = -1; };
        off(RESULT2[i][0]);
        off(RESULT2[i][1]);
      }
      P = 0;
      for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
          if (X[i][j] == 0) {
            int t = (i << 8) | j;
            int c = board[i][j] - '0';
            P1[P++] = t;
            X[i][j] = c;
            SUM[i + 1][j + 1] = 1LL << (bit * c);
            CP[c][CP[c][0]++] = t;
          }
          SUM[i + 1][j + 1] += SUM[i][j + 1] + SUM[i + 1][j] - SUM[i][j];
        }
      }
      sort(P1, P1 + P, [&](int a, int b) {
        auto value = [](int x) {
          int i = x >> 8;
          int j = x & 0xff;
          return abs(H / 2 - i) + abs(W / 2 - j);
        };
        int va = value(a);
        int vb = value(b);
        return test & 1 ? va > vb : va < vb;
      });
      {
        static int16 tmp[S * S];
        memcpy(tmp, P1, sizeof(int16) * P);
        for (int i = 0; i < P; ++i) {
          int t = get_random() % min(10, P - i);
          P1[i] = tmp[t];
          memcpy(tmp + t, tmp + t + 1, sizeof(int16) * (P - i - 1));
        }
      }
      bool ok = true;
      while (ok) {
        ok = false;
        for (int p1_ = 0; p1_ < P; ++p1_) {
          int p1 = P1[p1_];
          int i = p1 >> 8;
          int j = p1 & 0xff;
          int c = X[i][j];
          if (c < 0) continue;
          for (int p2_ = 1; p2_ < CP[c][0]; ++p2_) {
            int p2 = CP[c][p2_];
            if (p1 == p2) continue;
            int a = p2 >> 8;
            int b = p2 & 0xff;
            int minh, maxh, minw, maxw;
            if (i < a) {
              minh = i, maxh = a + 1;
            } else {
              minh = a, maxh = i + 1;
            }
            if (j < b) {
              minw = j, maxw = b + 1;
            } else {
              minw = b, maxw = j + 1;
            }
            ll t = SUM[maxh][maxw] + SUM[minh][minw] - SUM[maxh][minw] -
                   SUM[minh][maxw];
            ll m = ~(((1LL << bit) - 1LL) << (bit * c));
            if ((t & m) == 0) {
              auto remove = [&](int p) {
                for (int k = 1; k < CP[c][0]; ++k) {
                  if (CP[c][k] == p) {
                    CP[c][k] = CP[c][--CP[c][0]];
                    break;
                  }
                }
                int i_ = p >> 8;
                int j_ = p & 0xff;
                X[i_][j_] = -1;
                ll t = 1LL << (bit * c);
                for (int h = i_ + 1; h <= H; ++h) {
                  for (int w = j_ + 1; w <= W; ++w) {
                    SUM[h][w] -= t;
                  }
                }
              };
              remove(p1);
              remove(p2);
              ok = true;
              RESULT1[R1][0] = p1;
              RESULT1[R1][1] = p2;
              ++R1;
              break;
            }
          }
        }
      }
      auto backsize = [&]() {
        int x = R2 - back;
        return x * 20 < R2 ? 0 : x;
      };
      if (R2 < R1 || backsize() == 0) {
        int t = backsize();
        memcpy(RESULT2 + t, RESULT1 + t, sizeof(int16) * 2 * (R1 - t));
        R2 = R1;
        back = 0;
      }
      back += 2;
      R1 = backsize();
      if (R3 < R2) {
        R3 = R2;
        memcpy(RESULT3, RESULT2, sizeof(int16) * 2 * R3);
        if (R3 == B) break;
      }
    }
    vector<string> ret;
    for (int i = 0; i < R3; ++i) {
      int a = RESULT3[i][0] >> 8;
      int b = RESULT3[i][0] & 0xff;
      int c = RESULT3[i][1] >> 8;
      int d = RESULT3[i][1] & 0xff;
      ret.push_back(to_string(a) + " " + to_string(b) + " " + to_string(c) +
                    " " + to_string(d));
    }
    return ret;
  }
};

// -------8<------- end of solution submitted to the website -------8<-------
int main() {
  SameColorPairs scp;
  int H;
  cin >> H;
  vector<string> board(H);
  for (int i = 0; i < H; ++i) cin >> board[i];

  vector<string> ret = scp.removePairs(board);
  cout << ret.size() << endl;
  for (int i = 0; i < (int)ret.size(); ++i) cout << ret[i] << endl;
  cout.flush();
}
