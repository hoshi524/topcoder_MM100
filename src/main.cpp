#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

const int MAX_C = 6;
const int S = 1 << 7;
int X[S][S];
int sum[MAX_C][S][S];
int P1[S * S];
int P2[S * S];
int H, W, C, P;
int R1, R2;
int RESULT1[S * S / 2][2];
int RESULT2[S * S / 2][2];

class SameColorPairs {
 public:
  vector<string> removePairs(vector<string> board) {
    H = board.size();
    W = board[0].size();
    R2 = 0;
    for (int test = 0; test < 6; ++test) {
      {
        P = 0;
        for (int i = 0; i < H; ++i) {
          for (int j = 0; j < W; ++j) {
            P1[P] = (i << 8) | j;
            P2[P] = (i << 8) | j;
            ++P;
          }
        }
        auto compare = [&](int a, int b) {
          auto value = [](int x) {
            int i = x >> 8;
            int j = x & 0xff;
            return abs(H / 2 - i) + abs(W / 2 - j);
          };
          int va = value(a);
          int vb = value(b);
          return test & 1 ? va > vb : va < vb;
        };
        sort(P1, P1 + P, compare);
        sort(P2, P2 + P, compare);
        for (int i = 0; i < P; ++i) {
          auto swap = [&](int* x) {
            int j = i + get_random() % 10;
            if (j < P) {
              int t = x[i];
              x[i] = x[j];
              x[j] = t;
            }
          };
          swap(P1);
          swap(P2);
        }
      }
      R1 = 0;
      C = 0;
      memset(sum, 0, sizeof(sum));
      for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
          int c = board[i][j] - '0';
          X[i][j] = c;
          sum[c][i + 1][j + 1] = 1;
          if (C < c + 1) C = c + 1;
        }
      }
      for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
          for (int c = 0; c < C; ++c) {
            sum[c][i + 1][j + 1] +=
                sum[c][i][j + 1] + sum[c][i + 1][j] - sum[c][i][j];
          }
        }
      }
      bool ok = true;
      while (ok) {
        ok = false;
        for (int p1 = 0; p1 < P; ++p1) {
          int i = P1[p1] >> 8;
          int j = P1[p1] & 0xff;
          if (X[i][j] < 0) continue;
          auto pair = [&]() {
            for (int p2 = 0; p2 < P; ++p2) {
              int a = P2[p2] >> 8;
              int b = P2[p2] & 0xff;
              if (X[a][b] < 0) continue;
              if (i == a && j == b) continue;
              if (X[i][j] != X[a][b]) continue;
              auto check = [&]() {
                int minh = min(i, a);
                int maxh = max(i, a) + 1;
                int minw = min(j, b);
                int maxw = max(j, b) + 1;
                for (int c = 0; c < C; ++c) {
                  if (X[i][j] == c) continue;
                  if ((sum[c][maxh][maxw] - sum[c][maxh][minw] -
                       sum[c][minh][maxw] + sum[c][minh][minw]) > 0)
                    return false;
                }
                return true;
              };
              if (check()) {
                auto remove = [&](int i_, int j_) {
                  int c = X[i_][j_];
                  X[i_][j_] = -1;
                  for (int h = i_ + 1; h <= H; ++h) {
                    for (int w = j_ + 1; w <= W; ++w) {
                      sum[c][h][w]--;
                    }
                  }
                };
                remove(i, j);
                remove(a, b);
                ok = true;
                RESULT1[R1][0] = P1[p1];
                RESULT1[R1][1] = P2[p2];
                ++R1;
                return;
              }
            }
          };
          pair();
        }
      }
      if (R2 < R1) {
        R2 = R1;
        memcpy(RESULT2, RESULT1, sizeof(RESULT1));
      }
    }
    vector<string> ret;
    for (int i = 0; i < R2; ++i) {
      int a = RESULT2[i][0] >> 8;
      int b = RESULT2[i][0] & 0xff;
      int c = RESULT2[i][1] >> 8;
      int d = RESULT2[i][1] & 0xff;
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
