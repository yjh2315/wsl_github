#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    int n = 10;
    
    // input.txt 파일을 쓰기 모드로 연다.
    FILE *fp = fopen("input.txt", "w");
    if (fp == NULL) {
        printf("Cannot open input.txt for writing.\n");
        return 1;
    }
    
    // 첫 줄에 n을 출력
    fprintf(fp, "%d\n", n);
    
    // 난수 생성 초기화 (현재 시간을 시드로 사용)
    srand((unsigned)time(NULL));
    
    // 두 번째 줄에 n개의 무작위 정수(예: 1~1000 범위)를 공백으로 구분하여 출력
    for (int i = 0; i < n; i++) {
        int num = rand() % 1000 + 1;
        fprintf(fp, "%d", num);
        if (i < n - 1)
            fprintf(fp, " ");
    }
    
    fclose(fp);
    printf("input.txt file has been created with %d random numbers.\n", n);
    return 0;
}