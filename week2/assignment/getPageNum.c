#include "Header.h"

int *getPageNum(int T)
{

    int *pageNum = malloc(sizeof(int) * T); // 테스트 케이스의 개수 만큼 동적 메모리 할당

    for (int i = 0; i < T; i++)
    {
        scanf("%d", &pageNum[i]); // 각 테스트 케이스의 페이지 수 입력
    }

    return pageNum;
}