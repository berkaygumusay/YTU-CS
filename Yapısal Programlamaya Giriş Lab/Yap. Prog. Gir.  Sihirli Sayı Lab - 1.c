#include <stdio.h>
#include <stdlib.h>
#define SIZE 1000
int main() {
	int i,n,arr[SIZE]={0},temp,count,j=0,mgccount=0,k;
    int mgc[SIZE] = {0};
    printf("Dizinin Boyutunu Giriniz : ");  // D�Z� BOYUTUNU ALMA
    scanf("%d",&n);
    for(i=0;i<n;i++){
        printf("Dizinin %d. elemaninin giriniz: ",i+1); // D�Z� ELEMANLARINI ALMA
        scanf("%d",&arr[i]);
    }
    for(i=0;i<n;i++){
        temp = arr[i];  // D�Z�N�N ELEMANINI GE��C� DE���KENE ATAMA
        count=0;        // SAYA� DE���KEN
        for(k=0;k<n;k++){
            if(arr[k] == temp){     //AYNI ELEMANLARI KONTROL ETME
                    count++;        //SAYACI ARTTIRMA
            }
        }
        if(count == arr[i]){        //SAYAC �LE SAYININ KEND�S� E��T �SE SAYIYI D�Z�YE ATMA
            mgc[j] = temp;
            mgccount++;
            j++;
            arr[i] = 0;                //TEKRARDAN SAYMAMAK ���N SAYIYI 0 A �EV�RME
        }
    }
    printf("\n\n");
    if(mgccount != 0){
        printf("Bu Dizide Bulunan Sihirli Sayilar :  ");
        for(i=0;i<mgccount;i++){
            printf("%d  ",mgc[i]);
        }
    }
    else{
        printf("Bu Dizide Sihirli Sayi Yoktur");
    }
	return 0;
}
