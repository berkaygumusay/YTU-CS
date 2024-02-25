#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
typedef struct HashTableNode{
	char *userName;
	int deleted;
}hashTableNode;

long int hornersMethod(char* string);
int isPrime(int n);
int findPrime(int n);
long int power(long int x, int y);
int insertUser(char* userName, hashTableNode* hashTable, int tableSize, int mode);
void searchUser(char* userName, hashTableNode* hashTable, int tableSize, int mode);
int deleteUser(char* userName, hashTableNode* hashTable, int tableSize, int mode);
void printUserList(hashTableNode* hashTable, int tableSize);
int searchUserNoMsg(char* userName, hashTableNode* hashTable, int table);
void printUserStatus(hashTableNode* hashTable, int tableSize, int userNum);
hashTableNode* rehashTable(hashTableNode* oldHashTable, int* oldTableSize, int newTableSize);


int main() {
	int mode=0,menu=0,tableSize,i;
	int N, key, userNum=0, newTableSize;
	float loadFactor;
	char string[20];
	printf("N: ");
	scanf("%d",&N);
	printf("Load Factor: ");
	scanf("%f",&loadFactor);
	tableSize = findPrime((int)(N/loadFactor));
	system("cls");
	hashTableNode* hashTable = (hashTableNode*)calloc(tableSize,sizeof(hashTableNode));
		printf("1- Normal Mode\n2- Debug Mode\n3- Exit\n   Choice: ");
		scanf("%d",&mode);
		if(mode != 3){ 
			system("cls");
			menu = 0;
			while(menu != 6){  
				if(userNum == tableSize){	// TABLO DOLULU�U KONTROL ED�L�R
					printf("!! Tablo Dolu !! Duzenleme Yapmaniz Onerilir !!\n\n");
				}
				printf("1- Yeni Kullanici Ekle\n2- Kullanici Ara\n3- Kullanici Sil\n4- Tabloyu Yazdir\n5- Tabloyu Duzenle\n6- Geri\n   Secim: ");
				scanf("%d",&menu);
				system("cls");
				switch(menu){	// MEN� SE��M� YAPILIR
					case(1):{ 
						printf("Kullanici Adini Giriniz (Buyuk Harflerle) : ");
						scanf("%s",string);
						if(userNum == tableSize){
							printf("Tablo Dolu !");	// TABLO DOLULU�U KONTROL ED�L�R
						}
						else{
							if(insertUser(string,hashTable,tableSize,mode) == 1){
								userNum++;	// TABLO DE��LSE KULLANICI EKLEME ��LEM� YAPILIP KULLANICI SAYISI 1 ARTTIRILIR
							}
						}
						
						printf("\nPress Enter");	// ANA MEN�YE D�N�� YAPILIR
						getch();
						system("cls");
						break;
					}
					case(2):{	// ARAMA ��LEM�
						printf("Aramak Istediginiz Kullanici Adini Giriniz (Buyuk Harflerle) : ");  
						scanf("%s",string);
						searchUser(string,hashTable,tableSize,mode);	// ARAMA FONKS�YONU
						printf("\nPress Enter");
						getch();
						system("cls");
						break;
					}
					case(3):{
						printf("Silmek Istediginiz Kullanicinin Adini Giriniz (Buyuk Harflerle) : ");
						scanf("%s",string);
						if(deleteUser(string,hashTable,tableSize,mode) == 1){	// S�LME ��LEM�
							userNum--;	// KULLANICI SAYISINI AZALTMA
						}
						printf("\nPress Enter");
						getch();
						system("cls");
						break;
					}
					case(4):{
						printUserList(hashTable,tableSize);	// KULLANICI L�STES�N� YAZDIRMA
						printf("\n\nPress Enter");
						getch();
						system("cls");
						break;
					}
					case(5):{
						if(mode == 2){
							printUserStatus(hashTable,tableSize,userNum);	// KULLANICILARIN S�L�N�P S�L�NMED�G�N� EKRANA YAZDIRMA
							printf("\nPress Enter");
							getch();
							system("cls");
						}
						printf("Yeni Tablo Boyutunu Giriniz (Asal Sayi) : ");
						scanf("%d",&newTableSize);
						hashTable = rehashTable(hashTable,&tableSize,newTableSize);	// YEN�DEN ORGAN�ZE ETME ��LEM� (REHASH)
						printf("\n\nTablo Duzenleme Islemi Bitmistir");
						printf("\nPress Enter");
						getch();
						system("cls");
						break;
					}
					case(6):{
						system("cls");	// �IKI� ��LEM�
						break;
					}
				}
			}
		}
		system("cls");	
		return 0;
	}
	

int isPrime(int n){	// ASALLI�I KONTROL EDEN FONKS�YON
	int i;
    if (n < 2) {
        return 0;
    }
    else {
        for(i = 2; i <= sqrt(n); i++){
            if (n % i == 0)
                return 0;
        }
    }
    return 1;
}
int findPrime(int num){	// num SAYISINDAN B�Y�K EN K���K ASAL SAYIYI BULAN FONKS�YON
	int i = 0;
	while(!isPrime(num)){
		num++;
	}
	return num;
}
long int power(long int x, int y){	// �S ALMA FONKS�YONU
    if(y == 0){
        return 1;
    }
    int i;
    long int result = 1;
    
    for(i=0; i<y; i++){
        result *= x;
    }
    return result;
}
long int hornersMethod(char* string){ // Horner Y�NTEM�
	int i;
	int length;
	long int key=0;
	long int R = 17;
	length = strlen(string);	// A = 65
	for(i=0 ; i<length ; i++){
		key += power(R,(length-1-i)) * ((long int)((string[i] - 65 +1)));
	}
	return key;
}
int h1(long int key, int m){ // �DEVDE BEL�RT�LEN h1() FONSK�YONU
	return (key%m);
}
int h2(long int key, int m){ // �DEVDE BEL�RT�LEN h2() FONSK�YONU
	return (1+ (key%(m-2)));
}
int doubleHashing(long int key, int i, int m){ // Double Hashing METODU
	int tmp;
	tmp = (h1(key,m) + i*h2(key,m) ) % m;
	return tmp;
}

int insertUser(char* userName, hashTableNode* hashTable, int tableSize, int mode){	// KULLANICI EKLEME FONKS�YONU
	int key = hornersMethod(userName);	//	HORNER Y�NTEM� �LE KEY �RET�M�
	int index = h1(key,tableSize);	//	D�V�S�ON METHOD �LE �LK �NDEX�N �RET�M�
	int i = 0;
	if(searchUserNoMsg(userName, hashTable, tableSize) == 1){	//	EKLENECEK KULLANICININ TABLODA MEVCUT OLUP OLMADI�ININ KONTROL�
		printf("Eleman tabloda mevcut oldugu icin ekleme islemi yapilmadi\n");	
		return 0;
	}
	if(mode == 2){	// DEBUG MOD KONTROL�
		printf("Debug Message : h1('%s') = %d\n",userName,index);
		printf("Debug Message : h2('%s') = %d\n",userName,h2(key,tableSize));
	}
	if((hashTable[index].userName == NULL) || (hashTable[index].deleted == 1)){	// �LK �NDEX�N M�SA�T OLUP OLMADI�ININ KONTROL�
		hashTable[index].deleted = 0;
		hashTable[index].userName = strdup(userName);
		printf("%s elemani %d. adrese yerlestirildi\n",userName,index);
		return 1;
	}
	
	if(mode == 2){	// DEBUG MOD KONTROL�
		printf("Debug Message : %s kelimesi %d. adres dolu oldugu icin yerlestirilemedi\n",userName,index);
	}
	i++;
	index = doubleHashing(key,i,tableSize);	//	DOUBLE HASH�NG �LE ADIM B�Y�KL���N�N �RET�M�
	while((hashTable[index].userName != NULL) && (index<=tableSize)){	//	HASHTABLE DA NULL ELEMAN G�RENE KADAR D�N�L�R
		if(hashTable[index].deleted == 1){	//	�NCEDEN S�L�NEN B�R ELEMANA DENK GEL�N�RSE �ZER�NE EKLEME YAPILIR
			hashTable[index].deleted = 0;
			hashTable[index].userName = strdup(userName);
			printf("%s elemani %d. adrese yerlestirildi\n",userName,index);
			return 1;
		}
		if(mode == 2){	// DEBUG MOD KONTROL�
			printf("Debug Message : %s kelimesi %d. adres dolu oldugu icin yerlestirilemedi\n",userName,index);
		}
		i++;
		index = doubleHashing(key,i,tableSize);	//	BO� YER BULUNAMADIK�A DOUBLE HASH�NG YAPILARAK ADIM SAYISI KADAR �LERLEN�R
	}
	hashTable[index].deleted = 0;
	hashTable[index].userName = strdup(userName);
	printf("%s elemani %d. adrese yerlestirildi\n",userName,index);
	return 1;
}

void searchUser(char* userName, hashTableNode* hashTable, int tableSize, int mode){	//	KULLANICI ARAMA FONKS�YONU
	int temp = 0;
	long int key = hornersMethod(userName);
	int index = h1(key,tableSize);
	int i = 0;
	if(mode == 2){	// DEBUG MOD KONTROL�
		printf("Debug Message : h1('%s') = %d\n",userName,index);
		printf("Debug Message : h2('%s') = %d\n",userName,h2(key,tableSize));
	}
	if(hashTable[index].userName == NULL){	// E�ER �LK �NDEX NULL �SE BU ELEMAN L�STEYE H�� EKLENMEM��T�R VE DOLAYISIYLA L�STEDE YOKTUR
		printf("%s isimli kullanici tabloda bulunamadi",userName);
		return;
	}
	else{
		if((strcmp(hashTable[index].userName,userName) == 0) && (hashTable[index].deleted == 0)){	//	�LK �NDEX�N KONTROL�
			printf("%s isimli kullanici %d. adreste bulunmaktadir",userName,index);
			return;
		}
		if(mode == 2){	// DEBUG MOD KONTROL�
			printf("Debug Message : %s isimli kullanici %d. adreste bulunamadi\n",userName,index);
		}
		temp++;	//	TEMP DE���KEN� L�STEDE KONTROL ED�LEN �NDEX SAYISINI TUTAR
		i++;
		index = doubleHashing(key,i,tableSize);	//	ELEMAN BULUNAMAZSA ADIM SAYISI KADAR �LERLEN�R
		while((index<=tableSize)&&(hashTable[index].userName != NULL)){	//	HASHTABLE DA NULL ELEMAN G�RENE KADAR D�N�L�R
			if((strcmp(hashTable[index].userName,userName) == 0) && (hashTable[index].deleted == 0)){	//	ARANAN ELAMAN BULUNURSA EKRANA YAZDIRILIP D�NG�DEN �IKILIR
				printf("%s isimli kullanici %d. adreste bulunmaktadir",userName,index);
				return;
			}
			if(mode == 2){	// DEBUG MOD KONTROL�
				printf("Debug Message : %s isimli kullanici %d. adreste bulunamadi\n",userName,index);
			}
			
			temp++;
			i++;
			index = doubleHashing(key,i,tableSize);	//	BULUNAMAZ �SE DOUBLE HASH�NG YAPILARAK ADIM SAYISI KADAR �LERLEN�R
			if(temp == tableSize){	// E�ER KONTROL ED�LEN �ND�S SAYISI TABLO BOYUTUNA E��TSE BU ELEMAN TABLODA YOKTUR
				printf("%s isimli kullanici tabloda bulunamadi",userName);
				return;
			}
		}
	}
	printf("%s isimli kullanici tabloda bulunamadi",userName);
	return;
}

int deleteUser(char* userName, hashTableNode* hashTable, int tableSize, int mode){	//	KULLANICI S�LME FONKS�YONU
	long int key = hornersMethod(userName);
	int index = h1(key,tableSize);
	int i = 0;
	int temp = 0;
	if(mode == 2){	// DEBUG MOD KONTROL�
		printf("Debug Message : h1('%s') = %d\n",userName,index);
		printf("Debug Message : h2('%s') = %d\n",userName,h2(key,tableSize));
	}
	if(hashTable[index].userName == NULL){	// E�ER �LK �NDEX NULL �SE BU ELEMAN L�STEYE H�� EKLENMEM��T�R VE DOLAYISIYLA L�STEDE YOKTUR
		printf("%s elemani tabloda bulunmuyor",userName);
		return 0;
	}
	else{
		if((strcmp(hashTable[index].userName,userName) == 0) && (hashTable[index].deleted == 0)){	//	�LK �NDEX�N KONTROL�	
			printf("%d adresindeki %s elemani silindi",index,userName);
			hashTable[index].deleted = 1;
			return 1;
		}
		if(mode == 2){	// DEBUG MOD KONTROL�
			printf("Debug Message : %s kelimesi %d. adreste bulunamadi\n",userName,index);
		}
		temp++;	//	TEMP DE���KEN� L�STEDE KONTROL ED�LEN �NDEX SAYISINI TUTA
		i++;	
		index = doubleHashing(key,i,tableSize);	//	ELEMAN BULUNAMAZSA ADIM SAYISI KADAR �LERLEN�R
		while((index<=tableSize)&&(hashTable[index].userName != NULL)){	//	HASHTABLE DA NULL ELEMAN G�RENE KADAR D�N�L�R
			if((strcmp(hashTable[index].userName,userName) == 0) && (hashTable[index].deleted == 0)){//	ARANAN ELAMAN BULUNURSA deleted = 1 YAPILIP D�NG�DEN �IKILIR
				hashTable[index].deleted = 1;
				printf("%d adresindeki %s elemani silindi",index,userName);
				return 1;
			}
			if(mode == 2){	// DEBUG MOD KONTROL�
				printf("Debug Message : %s kelimesi %d. adreste bulunamadi\n",userName,index);
			}
			temp++;
			i++;
			index = doubleHashing(key,i,tableSize);	//	BULUNAMAZ �SE DOUBLE HASH�NG YAPILARAK ADIM SAYISI KADAR �LERLEN�R
			if(temp == tableSize){
				printf("%s elemani tabloda bulunamadi",userName);	// E�ER KONTROL ED�LEN �ND�S SAYISI TABLO BOYUTUNA E��TSE BU ELEMAN TABLODA YOKTUR
				return;
			}
		}
	}
	printf("%s elemani tabloda bulunmuyor",userName);
	return 0;	
}

void printUserList(hashTableNode* hashTable, int tableSize){
	int i;
	for(i=0;i<tableSize;i++){
		printf("%d- ",i);
		if((hashTable[i].userName != NULL) && (hashTable[i].deleted != 1)){	// E�ER �NDEXTE B�R KULLANICI ADI VARSA EKRANA YAZDIRILIR
			printf(" %s\n",hashTable[i].userName);
		} 
		else{
			printf(" Empty\n");	//	�NDEXTE NULL DE�ER VEYA S�L�NM�� B�R KULLANICI VARSA EKRANA Empty YAZDIRILIR
		}
	}
}

void printUserStatus(hashTableNode* hashTable, int tableSize, int userNum){
	int i = 0;
	int deleted=0;
	for(i=0;i<tableSize;i++){
		if(hashTable[i].userName == NULL){
			printf("%d - NULL",i);	//	E�ER �NDEX NULL �SE EKRANA NULL YAZDIRILIR
		}
		else{
			if(hashTable[i].deleted == 1){
				printf("%d - %s (Silinmis)",i,hashTable[i].userName);	//	E�ER �NDEXTEK� ELEMAN S�L�NM�� �SE EKRANA Silinmis YAZDIRILIR
			}
			else{
				printf("%d - %s (Silinmemis)",i,hashTable[i].userName);	//	E�ER �NDEXTEK� ELEMAN S�L�NMEM�� �SE EKRANA Silinmemis YAZDIRILIR
			}
		}
		printf("\n");
	}
	
}

hashTableNode* rehashTable(hashTableNode* oldHashTable, int *oldTableSize, int newTableSize){	// REHASH�NG ��LEM�
	hashTableNode* newHashTable = (hashTableNode*)calloc(newTableSize,sizeof(hashTableNode));	// KULLANICININ G�RD��� TABLO BOYUTUNDA YEN� B�R HASHTABLE OLU�TURULUR
	int i;
	
	for(i=0;i<(*oldTableSize);i++){	
		if((oldHashTable[i].deleted == 0) && (oldHashTable[i].userName != NULL)){	//	TABLODA OLAN S�L�NMEM�� KULLANICI ADLARI YEN� HASHTABLE A HASH�NG YAPILARAK EKLEN�R
			insertUser(oldHashTable[i].userName, newHashTable, newTableSize, 1);
		}
	}
	free(oldHashTable);	//	ESK� TABLO SERBEST BIRAKILIR
	*oldTableSize = newTableSize;	// TABLO BOYUTU G�NCELLEN�R
	return newHashTable;
}

int searchUserNoMsg(char* userName, hashTableNode* hashTable, int tableSize){	//	ARAMA FONKS�YONUNUN �IKTI OLARAK MESAJ VER�LMEYEN HAL�
	int temp = 0;
	long int key = hornersMethod(userName);
	int index = h1(key,tableSize);
	int i = 0;
	if(hashTable[index].userName == NULL){
		return 0;
	}
	if((strcmp(hashTable[index].userName,userName) == 0) && (hashTable[index].deleted == 0)){
		return 1;
	}
	temp++;
	i++;
	index = doubleHashing(key, i, tableSize);
	while((index<=tableSize)&&(hashTable[index].userName != NULL)){
		if((strcmp(hashTable[index].userName,userName) == 0) && (hashTable[index].deleted == 0)){
			return 1;
		}
		temp++;
		i++;
		index = doubleHashing(key,i,tableSize);
		if(temp == tableSize){
			return 0;
		}
	}
	return 0;
}









