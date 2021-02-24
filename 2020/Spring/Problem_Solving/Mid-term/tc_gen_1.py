import random

n1 = set()
n2 = set()

n2_len = 12000000
n1_len = n2_len - random.randint(0, 700000)

while len(n1) <= n1_len:
    k = random.randint(0, 2**31)
    n1.add(k)
    if random.randint(0, 100) % 6 <= 4:
        n2.add(k)

while len(n2) <= n2_len:
    n2.add(random.randint(0, 2**31))

f1 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_1/t5_n1.txt", 'w')
f2 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_1/t5_n2.txt", 'w')

for i in sorted(n1):
    f1.write('{0} '.format(i))
for i in sorted(n2):
    f2.write('{0} '.format(i))