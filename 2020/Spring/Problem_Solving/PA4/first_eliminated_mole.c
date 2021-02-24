#include <stdio.h>
#include <stdlib.h>

int first_eliminated_mole(int* m, int m_num) {
    int to_be_removed = 0;
    int removed_value = m[to_be_removed];

    int* count = (int*) calloc((m_num + 1), sizeof(int));
    for (int i = 0; i < m_num; i++) {
        count[m[i]]++;
    }

    while(1) {
        removed_value = m[to_be_removed];
        m[to_be_removed] = 0;
        if (!(--count[removed_value])) {
            break;
        }
        for (int cnt = 0; cnt <= removed_value; ) {
            if (++to_be_removed >= m_num)
                to_be_removed %= m_num;
            if (m[to_be_removed]) {
                cnt++;
            }
        }
    }

    return removed_value;
}

int main()
{
    int* m;
    int m_num = 0;

    scanf("%d", &m_num);

    m = (int*) malloc(sizeof(int) * m_num);

    for (int i = 0; i < m_num; i++) {
        scanf("%d", &m[i]);
    }

    printf("%d\n", first_eliminated_mole(m, m_num));

    free(m);

    return 0;
}