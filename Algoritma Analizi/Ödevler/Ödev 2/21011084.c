#include <stdio.h>
#include <stdlib.h>

void matchKeysAndLocks(int** keys, int** lock, int low, int high);
int partition(int** arr, int low, int high, int pivotValue);
void swap(int* a, int* b);


int main() {
	
	// DE�ER TANIMLAMALARI VE D�Z�N�N KULLANICIDAN ALINMASI BA�LANGI�
	int *key, *lock;
	int N,i;
	printf("Anahtar/Kilit Sayisini Giriniz (N): ");
	scanf("%d",&N);
	key = (int*)malloc(N* sizeof(int));
	lock  =(int*)malloc(N* sizeof(int));
	for(i=0;i<N;i++){
		printf("Anahtar Dizisi %d. Elemani : ",i+1);
		scanf("%d",&key[i]);
	}
	for(i=0;i<N;i++){
		printf("Kilit Dizisi %d. Elemani : ",i+1);
		scanf("%d",&lock[i]);
	}
	system("cls");
	// DE�ER TANIMLAMALARI VE D�Z�N�N KULLANICIDAN ALINMASI B�T��
	
	// KULLANICIDAN ALINAN D�Z�N�N EKRANA YAZDIRILMASI BA�LANGI�
	printf("\n\n\tKeys And Locks Lists :\n\n");
	printf("\tKey : ");
	for(i=0;i<N;i++){
		printf("%d ",key[i]);
	}
	printf("\n\tLock: ");
	for(i=0;i<N;i++){
		printf("%d ",lock[i]);
	}
	printf("\n\n------------------------------------------------\n\n\tKey-Lock Match !! \n\n");
	// KULLANICIDAN ALINAN D�Z�N�N EKRANA YAZDIRILMASI B�T��
	
	// PROBLEMI COZEN FONKS�YON
	matchKeysAndLocks(&key,&lock,0,N-1);

	// E�LE�M�� ANAHTAR-K�L�T �K�L�LER�N� EKRANA YAZDIRMA
	printf("\n\n");
	printf("\tKey : ");
	for(i=0;i<N;i++){
		printf("%d ",key[i]);
	}
	printf("\n\tLock: ");
	for(i=0;i<N;i++){
		printf("%d ",lock[i]);
	}
	printf("\n\n");
	free(key);free(lock);
	
	return 0;
}


void matchKeysAndLocks(int** keys, int** locks, int low, int high){
	int pivotValue;
	if(low <high){
		
		// ANAHTAR D�Z�S�N� K�L�T D�Z�S�NDEN ALINAN P�VOTA G�RE SIRALAMA
		pivotValue = partition(keys,low,high,(*locks)[high]);
		// K�L�T D�Z�S�N� ANAHTAR D�Z�S�NDEN ALINAN P�VOTA G�RE SIRALAMA
		partition(locks,low,high,(*keys)[pivotValue]);
		
		//RECURS�VE OLARAK BU FONKS�YONU D�ND�RME
		matchKeysAndLocks(locks,keys,low,pivotValue-1);
		matchKeysAndLocks(locks,keys,pivotValue+1,high);
	}
}

int partition(int** array, int low, int high, int pivotValue){
	int i = low, j = high;int k;	
	int *arr = *array;
	// i D�Z�N�N SOLUNDAN j �SE D�Z�N�N SA�INDAN BA�LAYARAK ORTAYA DO�RU �LERLEN�R
	while(i<=j){
		// j �ND�S� SOLA DO�RUN �LERLET�L�R
		while(j>=0 && arr[j]>= pivotValue){
			// j P�VOTA E��TSE D�Z�N�N EN SA�INDAK� ELEMAN �LE SWAP ��LEM� YAPILIR
			if(arr[j] == pivotValue){
				swap(&arr[j],&arr[high]);
			}
			j--;
		}
		// i �ND�S� SA�A DO�RU �LERLET�L�R
		while(arr[i]<pivotValue){
			i++;
		}
		swap(&arr[i],&arr[j]);		
	}
	swap(&arr[i],&arr[j]);
	swap(&arr[i],&arr[high]);

	return(i);
}

void swap(int* a, int* b){
	int t= *a;
	*a = *b;
	*b = t;
}
