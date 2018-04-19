#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

const int S = 1 << 7;
int X[S][S];
int H, W;

class SameColorPairs {
 public:
  vector<string> removePairs(vector<string> board) {
    vector<string> ret;
    H = board.size();
    W = board[0].size();
    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        X[i][j] = board[i][j] - '0' + 1;
      }
    }
    bool ok = true;
    while (ok) {
      ok = false;
      for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
          if (X[i][j] == 0) continue;
          for (int a = i; a < H; ++a) {
            for (int b = j; b < W; ++b) {
              if (X[a][b] == 0) continue;
              if (i == a && j == b) continue;
              if (X[i][j] != X[a][b]) continue;
              auto check = [&]() {
                for (int h = i; h <= a; ++h) {
                  for (int w = j; w <= b; ++w) {
                    if (X[h][w] != X[i][j] && X[h][w] != 0) return false;
                  }
                }
                return true;
              };
              if (check()) {
                ret.push_back(to_string(i) + " " + to_string(j) + " " +
                              to_string(a) + " " + to_string(b));
                X[i][j] = 0;
                X[a][b] = 0;
                ok = true;
              }
            }
          }
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
