#include <stdio.h>
#include <stdlib.h>

#define THRESHOLD 10
void insertionSort(int a[], int left, int right);
void merge(int a[], int left, int mid, int right);
void mergeInsertionSort(int a[], int left, int right);

void swap(int *a, int *b);
int medianOfThree(int a[], int left, int right);
int partition(int a[], int left, int right);
void randomizedQuickSort(int a[], int left, int right);

void maxHeapify(int a[], int i, int heapSize);
void buildMaxHeap(int a[], int n);
void heapSort(int a[], int n);


int main(int argc, char *argv[]){
    FILE *ptr_input = fopen(argv[1], "r");
    
    //input 파일 열기
    if (ptr_input == NULL) {
        printf("Fail open input file\n");
        return 1;
    }

    //입력되는 정수 개수
    int n;
    if(fscanf(ptr_input, "%d", &n)!= 1){
        printf("Wrong Input numbers\n");
        fclose(ptr_input);
        return 1;
    }

    //입력 문자열 받기 (동적할당)
    int *arr = (int*)malloc(n*sizeof(int));
    if(arr == NULL){
        printf("Memory Error\n");
        fclose(ptr_input);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(ptr_input, "%d", &arr[i]) != 1) {
            printf("Error: Failed to read element %d.\n", i);
            free(arr);
            fclose(ptr_input);
            return 1;
        }
    }

    //입력파일 처리 끝 -> 닫기
    fclose(ptr_input);

    //arr들 생성. 내용 복제
    int *arr_mergeI = (int*)malloc(n*sizeof(int));
    int *arr_randQ = (int*)malloc(n*sizeof(int));
    int *arr_heap = (int*)malloc(n*sizeof(int));

    for(int i=0; i<n; i++){
        arr_mergeI[i] = arr[i];
        arr_randQ[i] = arr[i];
        arr_heap[i] = arr[i];
    }

    //메인 실행부

    // Merge + Insertion sort
    mergeInsertionSort(arr_mergeI, 0, n - 1);
    // Randomized Quick sort
    randomizedQuickSort(arr_randQ, 0, n - 1);
    // heap sort
    heapSort(arr_heap, n);

    // output 파일 열기
    FILE *ptr_output = fopen(argv[2], "w");
    if (ptr_output == NULL)
    {
        printf("Fail open output file\n");
        free(arr);
        free(arr_heap);
        free(arr_mergeI);
        free(arr_randQ);
        fclose(ptr_output);
        return 1;
    }

    //배열 요소 파일에 출력
    for(int i=0; i<n; i++) {
        fprintf(ptr_output, "%d", arr_mergeI[i]);
        if (i < n-1) {
        fprintf(ptr_output, " ");
        }
    }

    fprintf(ptr_output,"\n");

    for(int i=0; i<n; i++) {
        fprintf(ptr_output, "%d", arr_randQ[i]);
        if (i < n-1) {
        fprintf(ptr_output, " ");
        }
    }

    fprintf(ptr_output,"\n");

    for(int i=0; i<n; i++) {
        fprintf(ptr_output, "%d", arr_heap[i]);
        if (i < n-1) {
        fprintf(ptr_output, " ");
        }
    }


    fclose(ptr_output);
    free(arr);
    free(arr_heap);
    free(arr_mergeI);
    free(arr_randQ);
}

// 작은 구간에 대해 Insertion Sort 수행
void insertionSort(int a[], int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int key = a[i];
        int j = i - 1;
        while (j >= left && a[j] > key)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

// 두 개의 정렬된 부분 배열을 병합
void merge(int a[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++)
    {
        L[i] = a[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = a[mid + 1 + j];
    }
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            a[k++] = L[i++];
        else
            a[k++] = R[j++];
    }
    while (i < n1)
    {
        a[k++] = L[i++];
    }
    while (j < n2)
    {
        a[k++] = R[j++];
    }
    free(L);
    free(R);
}

// Merge sort에 구간 크기가 THRESHOLD 이하이면 Insertion sort를 사용하는 함수
void mergeInsertionSort(int a[], int left, int right)
{
    if (right - left + 1 <= THRESHOLD)
    {
        insertionSort(a, left, right);
        return;
    }
    int mid = left + (right - left) / 2;
    mergeInsertionSort(a, left, mid);
    mergeInsertionSort(a, mid + 1, right);
    merge(a, left, mid, right);
}













// 두 변수의 값을 교환하는 함수
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// median-of-three를 이용해 left, mid, right 값 중 중앙값의 인덱스를 반환
int medianOfThree(int a[], int left, int right)
{
    int mid = left + (right - left) / 2;
    if (a[left] > a[mid])
        swap(&a[left], &a[mid]);
    if (a[left] > a[right])
        swap(&a[left], &a[right]);
    if (a[mid] > a[right])
        swap(&a[mid], &a[right]);
    return mid; // a[mid]가 중앙값
}

// 파티션 함수: median-of-three로 선택한 피벗을 right-1 위치로 옮기고 분할 진행
int partition(int a[], int left, int right)
{
    int medianIndex = medianOfThree(a, left, right); // 피벗을 right-1 위치로 이동
    swap(&a[medianIndex], &a[right - 1]);
    int pivot = a[right - 1];
    int i = left, j = right - 1;
    while (1)
    {
        while (a[++i] < pivot)
            ;
        while (a[--j] > pivot)
            ;
        if (i >= j)
            break;
        swap(&a[i], &a[j]);
    } // 피벗을 최종 위치로 이동
    swap(&a[i], &a[right - 1]);
    return i;
}

// tail recursion 제거 기법을 적용한 randomized quick sort
void randomizedQuickSort(int a[], int left, int right)
{
    while (left < right)
    { // 작은 구간인 경우 Insertion sort로 정렬 (구간 크기가 10 미만인 경우)
        if (right - left + 1 < 10)
        {
            int i, j, key;
            for (i = left + 1; i <= right; i++)
            {
                key = a[i];
                j = i - 1;
                while (j >= left && a[j] > key)
                {
                    a[j + 1] = a[j];
                    j--;
                }
                a[j + 1] = key;
            }
            break;
        }
        else
        {
            int pivotIndex = partition(a, left, right); // 더 작은 부분 배열을 먼저 재귀 호출하여 호출 깊이를 줄임
            if (pivotIndex - left < right - pivotIndex)
            {
                randomizedQuickSort(a, left, pivotIndex - 1);
                left = pivotIndex + 1; // tail recursion 제거: 반복문으로 처리
            }
            else
            {
                randomizedQuickSort(a, pivotIndex + 1, right);
                right = pivotIndex - 1;
            }
        }
    }
}























// 부모, 왼쪽 자식, 오른쪽 자식 인덱스를 계산하는 매크로
#define LEFT(i) (2 * (i) + 1)
#define RIGHT(i) (2 * (i) + 2)

// maxHeapify: index i를 루트로 하는 서브트리에서 최대 힙 성질을 유지하도록 조정한다.
void maxHeapify(int a[], int i, int heapSize)
{
    int l = LEFT(i);
    int r = RIGHT(i);
    int largest = i;
    if (l < heapSize && a[l] > a[largest])
    {
        largest = l;
    }
    if (r < heapSize && a[r] > a[largest])
    {
        largest = r;
    }
    if (largest != i)
    {
        // swap a[i]와 a[largest] 교환
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        // 재귀 호출로 하위 트리도 정리
        maxHeapify(a, largest, heapSize);
    }
}

// buildMaxHeap: 배열을 최대 힙 구조로 만든다.
void buildMaxHeap(int a[], int n)
{
    int i;
    for (i = n / 2 - 1; i >= 0; i--)
    {
        maxHeapify(a, i, n);
    }
}

// heapSort: 최대 힙을 이용하여 배열 a를 오름차순 정렬한다.
void heapSort(int a[], int n)
{
    int heapSize = n;
    buildMaxHeap(a, n);
    // 배열의 맨 뒤에서부터 정렬 수행
    for (int i = n - 1; i > 0; i--)
    {
        // a[0] (최대값)과 a[i] 교환
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;

        heapSize--; // 힙 크기 감소
        maxHeapify(a, 0, heapSize);
    }
}