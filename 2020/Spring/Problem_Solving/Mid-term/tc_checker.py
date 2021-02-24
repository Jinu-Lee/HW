f1 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_100/t9_n1.txt")
f2 = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_100/t9_n2.txt")

n1 = set(map(int, f1.readline().split()))
n2 = set(map(int, f2.readline().split()))

f = open("2020/Spring/Problem Solving/Mid-term/testcase/ratio_100/t9_answer.txt", 'w')
f.write(str(len(n2))+'\n')
f.write(str(len(n1))+'\n')
f.write(str(len(n1.intersection(n2))))