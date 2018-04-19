import subprocess
import json

subprocess.call('g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/main.out src/main.cpp', shell=True)
subprocess.call('javac -d out src/SameColorPairsVis.java', shell=True)

def solve(seed):
    return float(subprocess.check_output('java -cp out SameColorPairsVis -exec out/main.out -seed {}'.format(seed), shell=True))

testcase = 100
filename = 'max.json'
rate = 0

file = open(filename, 'r')
max = json.loads(file.read())
file.close()

for seed in range(1, testcase):
    score = solve(seed)
    if max[seed] < score:
        max[seed] = score
    rate = (rate * (seed - 1) + score) / seed
    print('{}\t{:.6f}\t{:.6f}'.format(seed, score, rate))

file = open(filename, 'w')
file.write(json.dumps(max))
file.close()