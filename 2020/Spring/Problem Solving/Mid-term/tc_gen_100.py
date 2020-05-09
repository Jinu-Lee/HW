import random

n1 = list()
n2 = set()

n2_len = 100000000-1
n1_len = n2_len // random.randint(100, 110) - random.randint(0, 10000)

while len(n2) <= n2_len:
    n2.add(random.randint(0, 2**31))

idx = set()
for i in range(n1_len // 500):
    if random.randint(0, 10) % 7 <= 4:
        idx.add(random.randint(0, n2_len - 500))

n = list(sorted(n2))
for i in idx:
    n1 += n[i : i + 500]
n1 = set(n1)
while len(n1) <= n1_len:
    n1.add(random.randint(0, 2**31))

f1 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_100/t8_n1.txt", 'w')
f2 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_100/t8_n2.txt", 'w')

for i in sorted(n1):
    f1.write('{0} '.format(i))
for i in sorted(n2):
    f2.write('{0} '.format(i))