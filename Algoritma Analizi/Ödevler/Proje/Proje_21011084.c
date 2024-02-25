#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

typedef struct EDGE{
	int node2;
	int node1;
	double value;
	int visited;
}edge;

typedef struct QUEUE{
    int items[100];
    int front;
    int rear;
} Queue;

void addPointToEdge(int numOfEdges, edge* allEdges, int node , double point, int* numShortestPaths, double tmpArr[MAX_NODES]);
edge* countEdges(int n, int matrix[MAX_NODES][MAX_NODES], int* numOfEdges);
void findAndInc(edge* edges, int node1, int node2, int edgeLength);
double findOtherEdges(edge* allEdges, int numOfEdges, int node);
void bfs2(int adjMatrix[MAX_NODES][MAX_NODES], int start, int vertices, int* numShortestPaths, int numOfEdges, edge* allEdges);
void resetEdges(edge* allEdges, int numOfEdges);
void BFS(int graph[MAX_NODES][MAX_NODES], int V, int source, int distance[MAX_NODES], int *numShortestPaths, int numOfEdges, edge* allEdges, double tmpArr[MAX_NODES]);
void oneIteration(int adjacencyMatrix[MAX_NODES][MAX_NODES], int V, int distance[MAX_NODES], int *numShortestPaths, int numOfEdges, edge* allEdges, double tmpArr[MAX_NODES]);
int cutTheEdges(edge* allEdges, int adjacencyMatrix[MAX_NODES][MAX_NODES], int numOfEdges);
int isCuttable(edge* allEdges, int numOfEdges, int max);
int findEdgeNum(edge* allEdges, int numOfEdges);
void BFSForDisconnectedGraphs(int graph[MAX_NODES][MAX_NODES], int V, int numOfEdges, edge* allEdges, double tmpArr[MAX_NODES]);
void resetTmpArr(double tmpArr[MAX_NODES], int numOfEdges);
int findMinimumCommunitySize(int graph[MAX_NODES][MAX_NODES], int V);
void oneCommunity(int graph[MAX_NODES][MAX_NODES], int V, int source, int visited[MAX_NODES], int *communitySize);
void printEdges(edge* allEdges, int numOfEdges);
void bfsWithoutOperations(int graph[MAX_NODES][MAX_NODES], int V, int source, int visited[MAX_NODES], int *communitySize);
int findNumberOfCommunities(int graph[MAX_NODES][MAX_NODES], int V);
void readMatrixFromFile(int matrix[MAX_NODES][MAX_NODES], int *V);


int main(){
	int i, j, k, t;
	int numOfEdges=0;
	int adjacencyMatrix[MAX_NODES][MAX_NODES];
	int V;
	readMatrixFromFile(adjacencyMatrix, &V);
	printf("(Tur Sayisi)k: ");
    scanf("%d",&k);
    printf("(Minimum Uye Sayisi)t:");
    scanf("%d",&t);
	edge* allEdges = countEdges(V, adjacencyMatrix, &numOfEdges);
	int nodes[MAX_NODES] = {0};
	int numShortestPaths[MAX_NODES] = {0};
	double tmpArr[MAX_NODES] = {0};
	int minCommunitySize;
	int flag = 0;
	int k_tmp = 0;
	i = 0;
	
	while(flag == 0){
		i++;
		printf("\n-------%d. Adim-------\n",i);
		minCommunitySize = findMinimumCommunitySize(adjacencyMatrix, V);
		if(minCommunitySize <= t){
			printf("!! Sonuc !!\n\n");
		}
		printf("Guncel Minimum Topluluk Boyutu : %d\n",minCommunitySize);
		resetTmpArr(tmpArr, numOfEdges);
		findNumberOfCommunities(adjacencyMatrix, V);
		if(minCommunitySize <= t){
			flag = 1;
		}
		else{
			oneIteration(adjacencyMatrix, V, nodes, numShortestPaths, numOfEdges, allEdges, tmpArr);
			if(cutTheEdges(allEdges, adjacencyMatrix, numOfEdges) == 0){
				k_tmp++;
			}
			numOfEdges = findEdgeNum(allEdges, numOfEdges);
			if(k_tmp == k){
				printf("!! Sonuc !!\n\n");
				findNumberOfCommunities(adjacencyMatrix, V);
				flag = 1;
			}
			printEdges(allEdges, numOfEdges);
		}
	}
	
	return 0;
}



/*
@brief						Bir dosyadan kom�uluk matrisini okuyan fonksiyon
		
@param	matrix				MAX_NODES x MAX_NODES boyutunda olan bir kom�uluk matrisi
@param	V					Dosyadan okunan d���m say�s�n� tutan de�i�ken

@return						void
*/
void readMatrixFromFile(int matrix[MAX_NODES][MAX_NODES], int *V) {
    FILE *file = fopen("info.txt", "r");
    int i, j;
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", "info.txt");
    }

    if (fscanf(file, "%d", V) != 1) {
        fprintf(stderr, "Error reading the number of vertices.\n");
        fclose(file);
        exit(1);
    }
    
    for (i = 0; i < *V; i++) {
        for (j = 0; j < *V; j++) {
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error reading matrix element at row %d, column %d\n", i + 1, j + 1);
                fclose(file);
                exit(1);
            }
        }
    }

    fclose(file);
}



/*
@brief						T�m kenarlar� i�eren bir diziyi gruplara ay�ran ve grup say�s�n� bulan fonksiyon

@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	numOfEdges			Kesilmi� kenarlar dahil toplam kenar say�s�

@return						Verilen graf�n g�ncel kenar say�s�
*/
int findEdgeNum(edge* allEdges, int numOfEdges){
	int i, groupNum = 0;
	for(i=0 ; i<numOfEdges ; i++){
		if(allEdges[i].value != -1){
			allEdges[groupNum].node1 = allEdges[i].node1;
			allEdges[groupNum].node2 = allEdges[i].node2;
			allEdges[groupNum].value = 0.0;
			allEdges[groupNum].visited = 0;
			groupNum++;
		}
	}
	return groupNum++;
}



/*
@brief						T�m kenarlar�n de�erlerini kontrol ederek t�m kenarlar�n birbirine e�itli�ini kontrol eden fonksiyon

@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	numOfEdges			Toplam kenar say�s�
@param	max					Maksimum de�erdeki kenar�n de�eri

@return						T�m kenar�n maksimum de�erde olup olmad���n� kontrol eder, t�m kenarlar birbirine e�itse 0 de�ilse de�ilse 1 d�nd�r�r
*/
int isCuttable(edge* allEdges, int numOfEdges, int max){
	double epsilon = 1e-9;
	int i;
	for(i=0 ; i<numOfEdges ; i++){
		if(fabs(allEdges[i].value - max) >= epsilon){
			return 1;
		}
	}
	return 0;
}



/*
@brief						T�m kenarlar� i�eren bir diziyi kullanarak maksimum de�ere sahip kenarlar� keserek kom�uluk matrisini g�ncelleyen fonksiyon
	
@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	adjacencyMatrix		Kom�uluk Matrisi
@param	numOfEdges			Toplam kenar say�s�

@return						Maksimum de�ere sahip kenarlar� keserek kom�uluk matrisini g�nceller ve herhangi bir kenar kesildiyse 1, aksi takdirde 0 d�nd�r�r
*/
int cutTheEdges(edge* allEdges, int adjacencyMatrix[MAX_NODES][MAX_NODES], int numOfEdges){
	double max = 0;
	double epsilon = 1e-9;
	int i;
	for(i=0 ; i<numOfEdges ; i++){
		if(max < allEdges[i].value){
			max = allEdges[i].value;
		}
	}
	if(isCuttable(allEdges, numOfEdges, max) == 0){
		return 0;
	}
	for(i=0 ; i<numOfEdges ; i++){
		if(fabs(allEdges[i].value-max) < epsilon){
			printf("%c - %c Kenari Kesildi\n", 'A'+allEdges[i].node1, 'A'+allEdges[i].node2);
			adjacencyMatrix[allEdges[i].node1][allEdges[i].node2] = 0;
			adjacencyMatrix[allEdges[i].node2][allEdges[i].node1] = 0;
			allEdges[i].value = -1;
		}
	}
	return 1;
}



/*
@brief						Algoritmada bir iterasyon ger�ekle�tiren fonksiyon. Kom�uluk matrisini, BFS algoritmas� kullanarak g�nceller ve kenar de�erlerine eklemeler yapar

@param	adjacencyMatrix		Kom�uluk Matrisi
@param	V					D���m say�s�
@param	distance			D���mlerin di�er d���mlere olan uzakl�klar�n�n tutulaca�� dizi
@param	numShortestPaths	Bir d���mden di�er d���me olan k�sa yollar�n say�s�n� tutan dizi
@param	numOfEdges			G�ncel toplam kenar say�s�
@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	tmpArr				Kenar de�erlerini tutan ge�ici bir dizi

@return						void
*/
void oneIteration(int adjacencyMatrix[MAX_NODES][MAX_NODES], int V, int distance[MAX_NODES], int *numShortestPaths, int numOfEdges, edge* allEdges, double tmpArr[MAX_NODES]){
	int i;
	double epsilon = 1e-9;
	for(i=0 ; i<numOfEdges ; i++){
		if(fabs(allEdges[i].value - (-1.0)) < epsilon){
			allEdges[i].value = 0.0;
		}
	}
	BFSForDisconnectedGraphs(adjacencyMatrix, V, numOfEdges, allEdges, tmpArr);
	for(i=0 ; i<numOfEdges ; i++){
		allEdges[i].value = tmpArr[i];
	}
	
}



/*
@brief						Queue'dan eleman ekleyen fonksiyon

@param	queue				Queue veri yap�s�
@param	item				S�raya eklenmek istenen eleman

@return						void
*/
void enqueue( Queue *queue, int item) {
    if (queue->rear == MAX_NODES - 1) {
        // Queue is full
        return;
    }
    queue->items[++(queue->rear)] = item;
}



/*
@brief						Queue'dan eleman ��karan fonksiyon

@param	queue				Queue veri yap�s�

@return						Queue'dan bir eleman ��kar�r, s�ra bo�sa -1 d�nd�r�r
*/
int dequeue(Queue *queue) {
    if (queue->front > queue->rear) {
        return -1;
    }
    return queue->items[(queue->front)++];
}



/*
@brief						Bir s�ran�n bo� olup olmad���n� kontrol eden fonksiyon

@param	queue				Queue veri yap�s�

@return						S�ran�n bo� olup olmad���n� kontrol eder, bo�sa 1, de�ilse 0 d�nd�r�r
*/
int isEmpty(Queue *queue) {
    return queue->front > queue->rear;
}



/*
@brief						Bir kenar�n ziyaret edilip edilmedi�ini kontrol eden fonksiyon

@param	node1				Birinci d���m
@param	node2				Ikinci d���m
@param	nodeNum				Toplam d���m say�s�
@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi

@return						Kenar ziyaret edilmi�se 0, edilmemi�se 1 d�nd�r�r
*/
int controlNode(int node1, int node2, int nodeNum, edge* allEdges){
	int i;
	double epsilon = 1e-9;
	for(i=0 ; i<nodeNum ; i++){
		if((allEdges[i].node1 == node1 && allEdges[i].node2 == node2) || (allEdges[i].node1 == node2 && allEdges[i].node2 == node1)){
			if(allEdges[i].visited == 1){
				return 0;
			}
		}
	}
	return 1;	
}



/*
@brief						Bir d���m�n sahip oldu�u kenar say�s�n� bulan fonksiyon

@param	graph				Kom�uluk matrisi
@param	V					D���m say�s�
@param	dugum				Kenar say�s� bulunmak istenen d���m
@param	numShortestPaths	D���me ula�an k�sa yol say�s�n� tutan dizi
@param	edgeNum				G�ncel toplam kenar say�s�
@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi

@return						Bir d���m�n kenar say�s�n� d�nd�r�r
*/
int findEdgeNumber(int graph[MAX_NODES][MAX_NODES], int V, int node, int numShortestPaths[MAX_NODES], int edgeNum, edge* allEdges){
	int edgeNumber = 0;
	int i;
	for(i=0 ; i<V ; i++){
		if ((graph[node][i] == 1) && controlNode(node, i, 9, allEdges)) {
			edgeNumber = edgeNumber + numShortestPaths[i];
        }
	}
	return edgeNumber;
}



/*
@brief						Ba�l� olmayan grafikler i�in BFS algoritmas�

@param	graph				Kom�uluk matrisi
@param	V					D���m say�s�
@param	numOfEdges			G�ncel toplam kenar say�s�
@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	tmpArr				Ge�ici bir dizi, kenar de�erlerini tutar

@return 					void
*/
void BFSForDisconnectedGraphs(int graph[MAX_NODES][MAX_NODES], int V, int numOfEdges, edge* allEdges, double tmpArr[MAX_NODES]) {
    int distance[MAX_NODES];
    int numShortestPaths[MAX_NODES];
    int visited[MAX_NODES];
	int i;
    for (i = 0; i < V; i++) {
        visited[i] = 0;
        distance[i] = -1; // -1 indicates infinity
        numShortestPaths[i] = 0;
    }

    for (i = 0; i < V; i++) {
        if (!visited[i]) {
            BFS(graph, V, i, distance, numShortestPaths, numOfEdges, allEdges, tmpArr);
        }
    }
}



/*
@brief						S�ras�yla:
							1- T�m d���mlere olan en k�sa yol say�s�n� bulur
							2- Daha sonra Queue veri yap�s�n� kullanarak kenarlara puan verme i�lemini yapar

@param	graph				Bir tamsay� matrisi, kom�uluk ili�kilerini temsil eder
@param	V					D���m say�s�
@param	distance			D���mlerin di�er d���mlere olan uzakl�klar�n�n tutulaca�� dizi
@param	numOfEdges			G�ncel toplam kenar say�s�
@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	tmpArr				Ge�ici bir dizi, kenar de�erlerini tutar

@return						void
*/
void BFS(int graph[MAX_NODES][MAX_NODES], int V, int source, int distance[MAX_NODES], int *numShortestPaths, int numOfEdges, edge* allEdges, double tmpArr[MAX_NODES]){
    Queue queue;
    queue.front = 0;
    queue.rear = -1;
    int i;

    int visited[MAX_NODES];
    for (i = 0; i < V; i++) {
        visited[i] = 0;
        distance[i] = -1;
        numShortestPaths[i] = 0;
    }
    visited[source] = 1;
    distance[source] = 0;
    numShortestPaths[source] = 1;

    enqueue(&queue, source);

    while (!isEmpty(&queue)) {
        int current = dequeue(&queue);
        for (i = 0; i < V; i++) {
            if (graph[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                distance[i] = distance[current] + 1;
                numShortestPaths[i] += numShortestPaths[current];
                enqueue(&queue, i);
            } else if (graph[current][i] == 1 && distance[i] == distance[current] + 1) {
                numShortestPaths[i] += numShortestPaths[current];
            }
        }
    }
    int edgeNumber;
    double point;
    for(i=0 ; i<V ; i++){
    	if(queue.rear >= 0){ // g�stermeye �al��iyim
    		edgeNumber = findEdgeNumber(graph, V, queue.items[queue.rear], numShortestPaths, numOfEdges, allEdges);
	    	if(edgeNumber != 0){
		    	point = 1.0;
		    	point = point + findOtherEdges(allEdges, numOfEdges, queue.items[queue.rear]);
		    	point = point/edgeNumber;
	    		addPointToEdge(numOfEdges, allEdges, queue.items[queue.rear], point, numShortestPaths, tmpArr);
			}
		}
		queue.rear--;
	}
	resetEdges(allEdges, numOfEdges);
}



/*
@brief						Bir d���me ba�l� olan di�er kenarlar�n toplam de�erini bulan fonksiyon

@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	numOfEdges			G�ncel toplam kenar say�s�
@param	node				Kenarlar�n�n puanlar�n�n toplam� bulunmak istenen d���m

@return						Kenarlar�n toplam de�erini bulur ve d�nd�r�r
*/
double findOtherEdges(edge* allEdges, int numOfEdges, int node){
	int i;
	double allValues = 0.0;
	for(i=0 ; i<numOfEdges ; i++){
		if((allEdges[i].node1 == node || allEdges[i].node2 == node)){
			if(allEdges[i].visited == 1){
				allValues += allEdges[i].value;
			}
		}
	}
	return allValues;
}



/*
@brief						Bir d���me verilen puan ekleyen fonksiyon

@param	numOfEdges			G�ncel toplam kenar say�s�
@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	node				Kenar�na puan eklenecek d���m
@param	point				Eklenecek puan miktar�
@param	numShortestPaths	D���me ula�an k�sa yol say�s�n� tutan dizi
@param	tmpArr				Ge�ici bir dizi, kenar de�erlerini tutar

@return 					void
*/
void addPointToEdge(int numOfEdges, edge* allEdges, int node , double point, int* numShortestPaths, double tmpArr[MAX_NODES]){
	int i;
	for(i=0 ; i<numOfEdges ; i++){
		if((allEdges[i].node1 == node) && (allEdges[i].visited == 0)){
			allEdges[i].value = allEdges[i].value + (point * numShortestPaths[allEdges[i].node2]);
			allEdges[i].visited = 1;
			tmpArr[i] += allEdges[i].value;
		}
		else if((allEdges[i].node2 == node) && (allEdges[i].visited == 0)){
			allEdges[i].value = allEdges[i].value + (point * numShortestPaths[allEdges[i].node1]);
			allEdges[i].visited = 1;
			tmpArr[i] += allEdges[i].value;
		}
	}
}



/*
@brief						Program�n ba��nda graf�n t�m kenarlar�n� ve graf�n toplam kenar say�s�n� bulan fonksiyon

@param	n					Graftaki d���m say�s�
@param	matrix				Kom�uluk matrisi
@param	numOfEdges			Toplam kenar say�s�n�n yerle�tirilece�i de�i�ken

@return						T�m kenarlar� i�eren edge tipinde bir struct dizisi
*/
edge* countEdges(int n, int matrix[MAX_NODES][MAX_NODES], int *numOfEdges){
    int i, j;
    for (i = 0; i < n; ++i) {
        for (j = i + 1; j < n; ++j) {
            if (matrix[i][j] == 1) {
                (*numOfEdges)++;
            }
        }
    }
    edge *allEdges = (edge *)malloc((*numOfEdges) * sizeof(edge));
    int numEdges = 0;
	for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
        	if (matrix[i][j] == 1) {
        		allEdges[numEdges].node1 = i;
	           	allEdges[numEdges].node2 = j;
	           	allEdges[numEdges].value = 0.0;
	           	allEdges[numEdges].visited = 0;
	           	numEdges++;
            }
        }
    }
    return allEdges;
}



/*
@brief						T�m kenarlar�n ziyaret durumunu ve de�erlerini s�f�rlayan fonksiyon

@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	numOfEdges			Toplam kenar say�s�

@return 					void
*/
void resetEdges(edge* allEdges, int numOfEdges){
	int i;
	for(i=0 ; i<numOfEdges ; i++){
		allEdges[i].visited = 0; allEdges[i].value = 0.0;
	}
}



/*
@brief						Bir grafikteki en k���k toplulu�un boyutunu bulan fonksiyon

@param	graph				Kom�uluk matrisi
@param	V					D���m say�s�

@return						En k���k toplulu�un boyutunu d�nd�r�r
*/
int findMinimumCommunitySize(int graph[MAX_NODES][MAX_NODES], int V) {
    int visited[MAX_NODES] = {0};
    int minimumCommunitySize = V; // Initialize with the maximum possible size
	int i;
    for (i = 0; i < V; i++) {
        if (!visited[i]) {
            int communitySize = 0;
            oneCommunity(graph, V, i, visited, &communitySize);

            if (communitySize < minimumCommunitySize) {
                minimumCommunitySize = communitySize;
            }
        }
    }
    return minimumCommunitySize;
}



/*
@brief						Bir graftaki bir toplulu�un boyutunu bulan fonksiyon

@param	graph				Kom�uluk matrisi
@param	V					D���m say�s�
@param	source				Bir toplulu�un ba�lang�� d���m�
@param	visited				Ziyaret edilen d���mleri tutan dizi
@param	communitySize		Toplulu�un boyutunun yerle�tirilece�i de�i�ken

@return						void
*/
void oneCommunity(int graph[MAX_NODES][MAX_NODES], int V, int source, int visited[MAX_NODES], int *communitySize) {
    Queue queue;
    queue.front = 0;
    queue.rear = -1;

    visited[source] = 1;
    enqueue(&queue, source);
	int i;
    while (!isEmpty(&queue)) {
        int current = dequeue(&queue);
        communitySize[0]++; // Increase the size of the current community

        for (i = 0; i < V; i++) {
            if (graph[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(&queue, i);
            }
        }
    }
}



/*
@brief						Ge�ici dizinin elemanlar�n� s�f�rlayan fonksiyon

@param	tmpArr				Ge�ici dizi
@param	numOfEdges			Toplam kenar say�s�

@return						void
*/
void resetTmpArr(double tmpArr[MAX_NODES], int numOfEdges){
	int i;
	for(i=0 ; i<numOfEdges ; i++){
		tmpArr[i] = 0.0;
	}
}



/*
@brief						Graf �zerindeki topluluk say�s�n� bulan ve ekrana yazan fonksiyon

@param	graph				Kom�uluk matrisi
@param	V					D���m say�s�

@return						Toplam topluluk say�s�n� d�nd�r�r
*/
int findNumberOfCommunities(int graph[MAX_NODES][MAX_NODES], int V) {
	int communitySize[MAX_NODES];
    int visited[MAX_NODES] = {0};
    int communityCount = 0;
	int i, j;
    for (i = 0; i < V; i++) {
        if (visited[i] == 0) {
            bfsWithoutOperations(graph, V, i, visited, communitySize);
            printf("%d. Topluluk : \n",communityCount+1);
            for (j = i; j < V; j++) {
                if (visited[j] == 1) {
                    printf("%c ", j+'A');
                    visited[j] = -1;
                }
            }
            printf("\n");
            communityCount++;
        }
        
    }
    printf("\nMevcut Topluluk Sayisi : %d\n----------------------------------------\n",communityCount);

    return communityCount;
}



/*
@brief						Graf �zerinde BFS algoritmas� uygulayan ve topluluk boyutunu hesaplayan fonksiyon (��lemler olmadan)

@param	graph				Kom�uluk matrisi
@param	V					D���m say�s�
@param	source				BFS'nin ba�lad��� d���m
@param	visited				Ziyaret edilen d���mleri tutan dizi
@param	communitySize		Toplulu�un boyutunu tutan de�i�ken

@return						void
*/
void bfsWithoutOperations(int graph[MAX_NODES][MAX_NODES], int V, int source, int visited[MAX_NODES], int *communitySize) {
    Queue queue;
    queue.front = 0;
    queue.rear = -1;

    visited[source] = 1;
    enqueue(&queue, source);
	int i, current;
    while (!isEmpty(&queue)) {
        current = dequeue(&queue);
        communitySize[0]++;
        for ( i = 0; i < V; i++) {
            if (graph[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(&queue, i);
            }
        }
    }
}



/*
@brief						G�ncel olarak kopmam�� t�m kenarlar� ekrana yazd�ran fonksiyon

@param	allEdges			T�m kenarlar� i�eren edge tipinde bir struct dizisi
@param	numOfEdges			Toplam kenar say�s�

@return 					void
*/
void printEdges(edge* allEdges, int numOfEdges){
	int i;
	printf("\n\nKalan Kenarlar : \n\n");
	for(i=0 ; i<numOfEdges ; i++){
		printf("%c - %c\n", 'A'+allEdges[i].node1, 'A'+allEdges[i].node2);
	}
}

