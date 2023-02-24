
/*
gcc -g -Wall -o my_QS my_QS.c -lpthread
./my_QS

*/
#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 10


typedef struct qsort_starter
{
  int *arr;
  int low;
  int high;
} quickSort_parameters;

void swap(int* a, int* b) {
   int t = *a;
   *a = *b;
   *b = t;
}

int partition (int arr[], int low, int high, int piv) {
   int piv_VAL = arr[piv];
   swap(&arr[piv], &arr[high]);
   int i = low-1;

   for (int j = low; j <= high-1; j++){
      if (arr[j] <= piv_VAL){
         i++;
         swap(&arr[i], &arr[j]);
      }
   }
   swap(&arr[i+1], &arr[high]);
   return i+1;
}

void quickSort(int *arr, int low, int high);

void *aux_qs(void * values){
   quickSort_parameters* p = values;
   quickSort(p->arr, p-> low, p->high);
   return NULL;
}
void quickSort(int *arr, int low, int high){
   if (low < high){
      int piv = low + (high - low) / 2;

      piv = partition(arr, low, high, piv);
      pthread_t thread;
      int res;

      if (THREAD_COUNT > 0){
         quickSort_parameters thread_param = {arr, low, piv - 1};
         res = pthread_create(&thread, NULL, aux_qs, &thread_param);
         if (res == 0){
            printf("Thread %lu created\n", pthread_self());
         }
         quickSort(arr, piv + 1, high);
         pthread_join(thread, NULL);
      }
      else
      {      
         quickSort(arr, low, piv - 1);
         quickSort(arr, piv + 1, high);
      }  
   }
}

void printArray(int arr[], int size){
   int i;
   for (i=0; i < size; i++)
      printf("%d ", arr[i]);
   printf("\n");
}

int main(){
   long int n = 5;
   int *arr = (int*)malloc(n * sizeof(int));
   srand(0);
   long int i;
   for (i = 0; i < n; i++) {
      arr[i] = rand() % n;
   }
   
   printf("The unsorted array is: \n");
   printArray(arr, n);
   quickSort(arr, 0, n-1);

   printf("The sorted array is: \n");
   printArray(arr, n);
   return 0;
}
