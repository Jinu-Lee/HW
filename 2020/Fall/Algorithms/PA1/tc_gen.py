import random
import string

random.seed(42)

N = [1000000]
#M = [1, 3, 5, 10, 15]
M = [4]
isRd = False
a, b = 5, 15

frac = 0.99

for n in N:
    for m in M:
        with open("./tc/{}e{}_{}_{}.txt".format(str(n)[0], len(str(n)) - 1, "rd_{}_{}".format(a, b) if isRd else m, frac), "w") as f:
            f.write("{}\n$\n".format(n))
            f.write("A(Key)\n$\n")

            words = list()
            for i in range(int(n * (1 - frac))):
                words.append(''.join(random.SystemRandom().choice(string.ascii_letters + string.digits + " ") for _ in range(m)))
            
            dup = [random.choice(words) for _ in range(n - int(n * (1 - frac)))]
            for i in range(len(dup)):
                a, b = [random.choice(range(len(dup[i]))) for _ in range(2)]
                if a > b:
                    a, b = b, a
                if random.uniform(0, 1) < 0.5:
                    dup[i] = dup[i][:a] + dup[i][a:b].upper() + dup[i][b:]
                else:
                    dup[i] = dup[i][:a] + dup[i][a:b].lower() + dup[i][b:]
            
            words += dup
            random.shuffle(words)
            for word in words:
                f.write(word + '\n')