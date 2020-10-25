for name in ['final']:
    for k in [1, 2, 3, 4, 5, 10, 15]:
        for j in [6]:
            for i in [1]:
                if i == 5 and j == 6:
                    break
                for l in ['0', '0.2', '0.5', '0.75', '0.99']:
                    print('time ./{0} ./tc/{1}e{2}_{3}_{4}.txt ./tc/ans_{0}_{1}e{2}_{3}_{4}.txt\n'.format(name, i, j, k, l))