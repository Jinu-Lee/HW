import random

n1 = set()

n1_len = 25000000

while len(n1) <= n1_len:
    k = random.randint(0, 2**31)
    n1.add(k)


f1 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_1/t6_n1.txt", 'w')
f2 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_1/t6_n2.txt", 'w')

temp = list(sorted(n1))

for i in range(0, n1_len - 2, 2):
    f1.write(str(temp[i]) + ' ')
    f2.write(str(temp[i+1]) + ' ')