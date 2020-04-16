#include <stdio.h>
#include <stdlib.h>

int block_length = 0;
int wall_length = 0;

char *block;
char *wall;

int answer_num = 0;
int answer_height = 0;

char blocks[200][1000] = {
    0,
};

int put(int index, int height, int mode)
{
    int i;
    if (mode == 0)
    {
        for (i = 0; i < block_length; i++)
            blocks[height][i + index] = 0;
    }
    else
    {
        for (i = 0; i < block_length; i++)
        {
            if (blocks[height][i + index] == 0)
                blocks[height][i + index] = block[i];
            else
                return 0;
        }
    }
    return 1;
}

char read(int index)
{
    int i = 199;
    while (i >= 0)
    {
        if (blocks[i][index] != 0)
            break;
        i--;
    }
    return blocks[i][index];
}

int test(int index, int height, int mode)
{
    int res = 0;
    if (height + mode < 0 || height + mode > 199)
        return res;
    res = put(index, height + mode, 1);
    if (res)
    {
        if (read(index) == wall[index])
            res = 1;
        else
            res = 0;
    }
    put(index, height + mode, 0);
    return res;
}

int check_pos(char c)
{
    int i;
    for (i = 0; i < block_length; i++)
        if (c == block[i])
            return i;
}

void calculator()
{
    int current_height = 100, pos, lowest_height = 100, highest_height = 100;
    put(0, current_height, 1);
    answer_num++;
    for (int i = 1; i < wall_length; i++)
    {
        char r = read(i);
        pos = 0;
        if (r == wall[i])
            continue;
        else
        {
            if (r == 0)
                pos = check_pos(wall[i]);

            int case_1 = test(i - pos, current_height, 1);
            int case_2 = test(i - pos, current_height, -1);
            if (case_1)
                current_height++;
            if (case_2)
                current_height--;
            put(i - pos, current_height, 1);
            answer_num++;
        }
        if (current_height > highest_height)
        {
            highest_height = current_height;
        }
        if (current_height < lowest_height)
        {
            lowest_height = current_height;
        }
    }
    answer_height = highest_height - lowest_height + 1;
}

int main()
{
    scanf("%d", &block_length);
    scanf("%d", &wall_length);

    block = malloc(sizeof(char) * block_length);
    wall = malloc(sizeof(char) * wall_length);
    scanf("%s", block);
    scanf("%s", wall);

    calculator();

    printf("%d\n", answer_num);
    printf("%d\n", answer_height);

    return 0;
}