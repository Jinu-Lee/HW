import random
import string

random.seed(42)

n = 1000000
t = 1
M = [1, 5, 10, 15]
isRd = False
a, b = 5, 15


for m in M:
    with open("./tc/{}e{}_{}.txt".format(str(n)[0], len(str(n)) - 1, "rd_{}_{}".format(a, b) if isRd else str(m)), "r") as f:
        words = f.readlines()[4:]
    with open("./tc/ans_{}e{}_{}.txt".format(str(n)[0], len(str(n)) - 1, "rd_{}_{}".format(a, b) if isRd else str(m)), "w") as f:
        for word in sorted(words):
            f.write(word)