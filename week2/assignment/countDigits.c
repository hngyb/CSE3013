#include "Header.h"

int *countDigits(int *pageNum, int i)
{
    int *count = calloc(10, sizeof(int));

    for (int j = 1; j <= pageNum[i]; j++)
    {
        int temp = j;
        while (temp != 0)
        {
            count[temp % 10]++;
            temp /= 10;
        }
    }
    return count;
}