#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    // 난수 생성 시드 초기화 
    srand((unsigned)time(NULL));

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

    //정렬된 왼쪽 파트와 오른쪽 파트를 동적할당한 추가 공간에 배치
    for (int i = 0; i < n1; i++)
    {
        L[i] = a[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = a[mid + 1 + j];
    }

    //두 파트에서 각각 index를 i, j로 관리하며 값이 작은 것을 먼저 작성하고 index를 증가시킨다. 
    int i = 0, j = 0, k = left;
    //index가 각 파트의 크기 안일경우
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            a[k++] = L[i++];
        else
            a[k++] = R[j++];
    }

    //아래는 한 파트가 다른 파트보다 클 경우에 비교연산 없이 남은 요소들은 가장 큰 요소들이기에 a에 차례대로 쌓는다.
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
    //THRESHOLD와 크기를 비교해서 이보다 작을때는 insertion sort를 사용용
    if (right - left + 1 <= THRESHOLD)
    {
        insertionSort(a, left, right);
        return;
    }
    
    //실질적인 mergesort 부분. mid를 기준으로 merge 두개로 문제를 나눔
    int mid = left + (right - left) / 2;
    mergeInsertionSort(a, left, mid);
    mergeInsertionSort(a, mid + 1, right);
    //mergesort에서 문제가 모두 나뉘고 난 뒤 insertionsort로 각 정렬된 part를 merge한다.
    merge(a, left, mid, right);
}













// 두 변수의 값을 교환하는 함수
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int randomMedianPivot(int a[], int left, int right)
{
    int indices[3];
    int range = right - left + 1;
    indices[0] = left + rand() % range;
    indices[1] = left + rand() % range;
    indices[2] = left + rand() % range;

    // 세 인덱스를 a 배열의 값에 따라 오름차순 정렬
    if (a[indices[0]] > a[indices[1]])
    {
        swap(&indices[0], &indices[1]);
    }
    if (a[indices[0]] > a[indices[2]])
    {
        swap(&indices[0], &indices[2]);
    }
    if (a[indices[1]] > a[indices[2]])
    {
        swap(&indices[1], &indices[2]);
    }
    // 정렬 후 중앙값은 indices[1]
    return indices[1];
}

// 파티션 함수: median-of-three로 선택한 피벗을 right-1 위치로 옮기고 분할 진행
int partition(int a[], int left, int right)
{
    int medianIndex = randomMedianPivot(a, left, right); // 피벗을 median of three의 결과값으로 선택
    swap(&a[medianIndex], &a[right]);               // 피벗을 가장 오른쪽에 위치
    int pivot = a[right];                           // 피벗값을 저장하고
    int i = left-1;                                 // i 초기값 설정 (모두 pivot보다 작다면 return i+1로 0번째 자리에 피벗이 들어감)
    
    for(int j=left; j<right; j++){                  // 왼쪽부터 피봇 전까지
        if(a[j]<=pivot){                            // 피봇보다 작은 갯수를 count하고 count한 자리와 자리 바꾸기(해당 조건을 만족x -> pivot보다 큰 값이기에 i값 증가x)
            i=i+1;
            swap(&a[i],&a[j]);
        }
    }
    swap(&a[i+1], &a[right]);
    return i+1;
}

// tail recursion 제거 기법을 적용한 randomized quick sort
void randomizedQuickSort(int a[], int left, int right)
{
    while (left < right)
    {
        int pivotIndex = partition(a, left, right); // 더 작은 부분 배열을 먼저 재귀 호출하여 호출 깊이를 줄임
        if (pivotIndex - left < right - pivotIndex)
        {
            randomizedQuickSort(a, left, pivotIndex - 1);
            left = pivotIndex + 1;                  // tail recursion 제거: 긴 부분은 반복문으로 처리 -> 다시 partition 이후 작은 값은 재귀/큰 값은 반복문.
        }
        else
        {
            randomizedQuickSort(a, pivotIndex + 1, right);
            right = pivotIndex - 1;
        }
    }
}




















// 부모, 왼쪽 자식, 오른쪽 자식 인덱스를 계산하는 매크로
#define LEFT(i) (2 * (i) + 1)
#define RIGHT(i) (2 * (i) + 2)

// maxHeapify: index i를 루트로 하는 서브트리에서 최대 힙 성질을 유지하도록 조정한다.
void maxHeapify(int a[], int i, int heapSize)
{
    int l = LEFT(i);        //자식 인덱스 계산
    int r = RIGHT(i);
    int largest = i;        //루트
    if (l < heapSize && a[l] > a[largest])
    {
        largest = l;
    }
    if (r < heapSize && a[r] > a[largest])
    {
        largest = r;        //루트 인덱스 갱신
    }
    if (largest != i)
    {
        // a[i]와 a[largest] 교환
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        // 재귀 호출로 하위 트리도 정리
        maxHeapify(a, largest, heapSize);       //이전 root가 현재 위치한 자리 기준으로 다시 heapify -> 아래도 maxheap 성질 만족
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