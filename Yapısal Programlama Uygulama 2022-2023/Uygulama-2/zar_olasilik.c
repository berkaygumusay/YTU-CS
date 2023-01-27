#include<stdio.h>
#include<stdlib.h>

float** initialize_mat(int nrow, int ncolumn) //D�NAM�K 2 BOYUTLU MATR�S i�in haf�zada yer ay�r�yor
{
	int i;
	float **mat;
	mat=(float**) malloc(nrow*sizeof(float*)); //ya haf�zada yer ay�racak (adres verecek) ya da NULL d�necek
	//matrisi olu�turduk
	if(mat==NULL){ // NULL olup olmad���n�n kontrol� yap�l�yor
		printf("Bellek Ayrilamadi");
		exit(-1);
	}
	for(i=0;i<nrow;i++){
		mat[i]=(float*)calloc(ncolumn,sizeof(float)); // Bellek alan�n� ay�rd�ktan sonra sat�r say�s� kadar alan� s�f�rlayarak doldurduk. Matrisin ilk adresini mat[i]. adresinde sakl�yoruz.
		mat[i][0]=i+3;
	}

	return mat;
}

void calc_prob(float **mat)  //tek tek olas�l�klar� hesapl�y�p matrise yaz�yor
{
	int i,j,k,top;
	for(i=1;i<=6;i++){
		for(j=1;j<=6;j++){
			for(k=1;k<=6;k++){
				top=i+j+k;
				mat[top-3][1] += (float) 1/(6*6*6);
			}
		}
	}
	// return mat;
}

void print_probs(float **mat, int nrow) //matrisi yazd�r�yor
{
	int i;
	printf("Toplam Olasilik: \n");
	for(i=0;i<nrow;i++){
		printf("%f\t%f\t%f\n",mat[i][0],mat[i][1]);
	}
}

void free_mat(float **mat, int nrow)// Belle�in gereksiz kullan�m�n� engellemek i�in haf�zadaki yeri bo�alt�yor
{
	int i;
	for(i=0;i<nrow;i++){ //sat�r sat�r belle�i bo�altt�k (�nce ald���m�z dizileri serbest b�rak�yoruz)
		free(mat[i]); //matrisin i. de�erini free yap
		
	}
	free(mat);// hepsini tutan pointer� bo�altt�k
}

int main() {
	float **mat;
	int nrow = 16, ncolumn = 2;
	mat = initialize_mat(nrow, ncolumn);
	print_probs(mat,nrow);
	calc_prob(mat);
	print_probs(mat,nrow);
	free_mat(mat, nrow);
	return 0;
}
