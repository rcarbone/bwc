import random

random.seed()
words = [w.strip() for w in open('dictionary.txt')]
scrambled = []
while len(words) > 0:
    i = random.randint(0, len(words) - 1)
    scrambled.append(words[i])
    del words[i]

for i in xrange(1000000):
    f = random.random()
    f = f ** 2
    print scrambled[int(f * len(scrambled))]
