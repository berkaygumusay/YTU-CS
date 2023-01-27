#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main() {
	int j,i,num,sira,arr1[500],arr2[500];
	srand(time(NULL));
	printf("Dizinin eleman sayisini giriniz: ");
	scanf("%d",&num);
	for(i=0;i<num;i++){
		printf("%d. eleman = ",i+1);
		scanf("%d",&arr1[i]);
	}
	for(i=0;i<num;i++){
		printf("%d ",arr1[i]);
	}
	
	for(i=0;i<num;i++){
		sira=0;
		for(j=0;j<num;j++){
			if(arr1[i]>arr2[j]){
				sira++;
			}
		}
		arr2[sira]=arr1[i];
	}
	for(i=0;i<num;i++){
		printf("%d ",arr2[i]);
	}
	return 0;
}
