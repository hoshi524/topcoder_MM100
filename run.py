import subprocess
import json

subprocess.call('g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/main.out src/main.cpp', shell=True)
subprocess.call('javac -d out src/SameColorPairsVis.java', shell=True)

def solve(seed):
    return 1.0 - float(subprocess.check_output('java -cp out SameColorPairsVis -exec out/main.out -seed {}'.format(seed), shell=True))

testcase = 100
rate = 0

for seed in range(1, testcase):
    score = solve(seed)
    rate = (rate * (seed - 1) + score) / seed
    print('{}\t{:.6f}\t{:.6f}'.format(seed, score, rate))
