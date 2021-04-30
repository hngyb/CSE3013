#include "Header.h"

int main(int argc, char *argv[])
{
    int T;
    scanf("%d", &T); // 테스트 케이스의 개수 입력

    int *pageNum = getPageNum(T); // 각 테스트 케이스의 페이지 수 가져오기

    for (int i = 0; i < T; i++)
    {
        int *count = countDigits(pageNum, i); // 페이지를 구성하는 각 숫자들의 개수 계산

        for (int j = 0; j < 10; j++)
        { // 숫자들의 개수 출력
            printf("%d ", count[j]);
        }
        printf("\n");
        free(count);
    }
    return 0;
}