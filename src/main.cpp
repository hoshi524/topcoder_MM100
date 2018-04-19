#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

const int MAX_C = 6;
const int S = 1 << 7;
int X[S][S];
int sum[MAX_C][S][S];
int H, W, C;

class SameColorPairs {
 public:
  vector<string> removePairs(vector<string> board) {
    vector<string> ret;
    H = board.size();
    W = board[0].size();
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
      for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
          if (X[i][j] < 0) continue;
          auto pair = [&]() {
            for (int a = 0; a < H; ++a) {
              for (int b = 0; b < W; ++b) {
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
                  ret.push_back(to_string(i) + " " + to_string(j) + " " +
                                to_string(a) + " " + to_string(b));
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
                  return;
                }
              }
            }
          };
          pair();
        }
      }
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
