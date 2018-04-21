import subprocess
import json
import threading
import queue

subprocess.call('g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/main.out src/main.cpp', shell=True)
subprocess.call('javac -d out src/SameColorPairsVis.java', shell=True)

def solve(seed):
    return (1.0 - float(subprocess.check_output('java -cp out SameColorPairsVis -exec out/main.out -seed {}'.format(seed), shell=True))) * 10

class State:
    count = 0
    rate = 0
    lock = threading.Lock()

    def add(self, seed, score):
        with self.lock:
            self.count = self.count + 1
            self.rate = (self.rate * (self.count - 1) + score) / self.count
            print('{}\t{:.6f}\t{:.6f}'.format(seed, score, self.rate))


scores = State()
q = queue.Queue()

def worker():
    while True:
        seed = q.get()
        if seed is None:
            break
        score = solve(seed)
        scores.add(seed, score)
        q.task_done()

num_worker_threads = 4
threads = []
for i in range(num_worker_threads):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)

for seed in range(1, 1000):
    q.put(seed)

q.join()

for i in range(num_worker_threads):
    q.put(None)
for t in threads:
    t.join()
