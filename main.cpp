#include <iostream>
#include <fstream>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

void selectSort(int *arr, int posI, int tam)
{
	int pos_min,temp;
	for (int i=posI; i < tam-1; i++)
	{
	    pos_min = i;
		for (int j=i+1; j < tam; j++)
		{
			if (arr[j] < arr[pos_min])	pos_min=j;
		}
            if (pos_min != i)
            {
                 temp = arr[i];
                 arr[i] = arr[pos_min];
                 arr[pos_min] = temp;
            }
	}
}

int *merge(int *arr, int **auxV, int tam, int nT)
{
	//cria um vetor auxiliar com o tamanho de numeros do arquivo que vai receber o resultado.
  	//menor recebe arr[auxV[0][1]]
  	/*for (int l = 0; l < tam; ++l)
    {
    	for (int i = 0; i < nT-1; ++i)
    	{
    		if (arr[auxV[i+1][1]] < menor) 
        	{
        		menor = arr[auxV[i+1][1]];
            	auxV[i+1][1] += 1;
        	}
    	}
        vResult[l] = menor; //adiciona "menor" ao vetor
    }
    */
  	int menor;
  	int indice;
	int *a = new int [nT];
	int *vetorOrd = new int [tam];
  	for(int i =0;i<nT;i++)
    {
      a[i]=auxV[i][0];
    }
	for(int i=0;i<tam-1;i++){
		menor = arr[a[0]];
		indice = 0;
		for(int j=0;j<nT;j++)
		{
			if(arr[a[j]]<menor){
				menor=arr[a[j]];
				indice = j;
				cout<<"menor:"<<menor<<endl;
			}
		}
   		a[indice]++;
   		vetorOrd[i] = menor;
      	//a[0] tem o menor valor
      	//adciona a[0] no vetor auxiliar
      	//a[0]++;
	}
	vetorOrd[tam-1] = arr[tam-1]; 
	return vetorOrd;
}

int main (int argc, char **argv)
{
	
	int nThreads = atoi(argv[2]); 	
	int tam, *vArquivo;
	int **auxV = new int*[nThreads];
	int marcaUltimo = 0;
	ifstream ifs;

	for (int i = 0; i < nThreads; ++i)
	{
		auxV[i] = new int[2];
	}

	ifs.open(argv[1]);
	if(ifs.good())
	{
		ifs>>tam;			
		vArquivo = new int[tam];	
		int fatia = tam/nThreads;
		for(int i = 0; i < tam; i++)
		{
			ifs>>vArquivo[i];
		}
	}

	int fatia = tam/nThreads;
	int sobra = tam-(nThreads*fatia);
	int t = time(0);
	#pragma omp parallel num_threads(nThreads)
	{
		//int *sub = new int[fatia];
		int indice = omp_get_thread_num();
		printf("\nEntrou na thread %d.", indice);

		//for (int i = indice*fatia; i < (indice+1)*fatia; ++i)
		//{
			//sub[j] = vArquivo[i]; //inicio da atribuicao
          	auxV[indice][0] = indice*fatia;
			//++j;
			if (indice == (nThreads-1))
			{
          		auxV[indice][1] = fatia+sobra;
				/*int *aux = new int[fatia+sobra];
				for(int k = 0; k < fatia; k++)
				{
					aux[k] = sub[k];
				}
				for (int l = indice*fatia; l < tam; l++)
				{
					aux[l] = vArquivo[l];
				}
				//delete []sub;
				sub = aux;*/
				//delete []aux;
			}
      		else auxV[indice][1] = fatia;
		//}
		//printf("\nO Tamanho do subvetor da thread %d eh %d\n", indice, auxV[indice][1]);
      	
      	printf(" %d %d\n", auxV[indice][0], auxV[indice][1]);
		selectSort(vArquivo, auxV[indice][0], (auxV[indice][0]+auxV[indice][1]));
		vArquivo = merge(vArquivo,auxV,tam,nThreads);
		cout << endl << "tempo: " << time(0)-t << endl;
/*
		for(int i = auxV[indice][0]; i < (auxV[indice][0]+auxV[indice][1]); ++i)
		{
			printf("%d ", vArquivo[i]);
		}
	}
	


	cout << endl << endl;
	for(int i = 0; i < tam; ++i)
	{
		printf("%d ", vArquivo[i]);
	}
	*/
	return 0;
}
