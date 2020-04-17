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

int lowest_height = 100, highest_height = 100;

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

int test(int index, int length)
{
    int res1 = 0, res2, i, j;
    for (i = lowest_height - 1; i <= highest_height + 1; i++)
    {
        if (i > 0 && i < 199)
        {
            if (!blocks[i + 1][index] && !blocks[i - 1][index])
                continue;
        }
        else if (i == 0)
        {
            if (!blocks[i + 1][index])
                continue;
        }
        else if (i == 199)
        {
            if (!blocks[i - 1][index])
                continue;
        }
        else
        {
            continue;
        }
        res1 = put(index, i, 1);
        if (res1)
        {
            res2 = 1;
            for (j = 0; j < length; j++)
            {
                char c = read(index + j);
                if (c != wall[index + j])
                {
                    res2 = 0;
                    break;
                }
            }
            put(index, i, 0);
        }
        if (res2)
        {
            return i;
        }
    }
    return 0;
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
    int current_height = 100, pos, temp, i, j;
    put(0, current_height, 1);
    answer_num++;
    for (i = 1; i < wall_length; i++)
    {
        char r = read(i);
        pos = 0;
        if (r == wall[i])
            continue;
        else
        {
            if (r == 0)
                pos = check_pos(wall[i]);
            temp = test(i - pos, pos + 1);
            if (temp)
                current_height = temp;
            else
            {
                for(j = 1; j < block_length; j++)
                {
                    int t = test(i - pos - j, j);
                    if(t) {
                        put(i - pos - j, t, 1);
                        answer_num++;
                        if(t < lowest_height)
                            lowest_height = t;
                        if(t > highest_height)
                            highest_height = t;
                        break;
                    } 
                }
                i--;
                continue;
            }
            put(i - pos, current_height, 1);
            answer_num++;
        }
        if (current_height > highest_height)
            highest_height = current_height;
        if (current_height < lowest_height)
            lowest_height = current_height;
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