#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NBLIN 15
#define NBCOL 50
#define NBSTEPS 500

void initArray(int nbLin, int nbCol, int board[nbLin][nbCol], int value){
	for(int k=0; k<nbLin; k++){
		for(int l=0; l<nbCol; l++){
			board[k][l]=value;
		}
	}
}

void clearScreen(){
	printf("%c[2J", 0x1B);
	printf("%c[%d;%dH", 0x1B, 1, 1);
}

void cpr(){
	printf("\n\n\n\nGame of life game\n\n     Copyright Mickael Almeida 2018\n\n");
}

void showBoard(int nbLin, int nbCol, int board[nbLin][nbCol]){
	clearScreen();
	usleep(300000);
	for(int k=0; k<nbLin; k++){
		for(int l=0; l<nbCol; l++){
			if(board[k][l]==1){
				printf(" 0");
			}else{
				printf("  ");
			}
		}printf("\n");
	}
}

void seedRandomCells(int nbLin, int nbCol, int board[nbLin][nbCol]){
	for(int k=0; k<nbLin; k++){
		for(int l=0; l<nbCol; l++){
			if(rand()%5==0){
				board[k][l]=1;
			}
		}
	}
}

int getNumberOfNeighboursAlive(int nbLin, int nbCol, int board[nbLin][nbCol], int cellLin, int cellCol){
	int n=0;
	if(cellLin!=0){
		n+=board[cellLin-1][cellCol];
	}
	if(cellLin!=nbLin-1){
		n+=board[cellLin+1][cellCol];
	}
	if(cellCol!=0){
		n+=board[cellLin][cellCol-1];
	}
	if(cellCol!=nbCol-1){
		n+=board[cellLin][cellCol+1];
	}
	if(cellLin!=0 && cellCol!=0){
		n+=board[cellLin-1][cellCol-1];
	}
	if(cellLin!=0 && cellCol!=nbCol-1){
		n+=board[cellLin-1][cellCol+1];
	}
	if(cellLin!=nbLin-1 && cellCol!=0){
		n+=board[cellLin+1][cellCol-1];
	}
	if(cellLin!=nbLin-1 && cellCol!=nbCol-1){
		n+=board[cellLin+1][cellCol+1];
	}
	return n;
}

int isCellDeadOrAlive(int nbLin, int nbCol, int board[nbLin][nbCol], int cellLin, int cellCol){
	int res=0;
	int n=getNumberOfNeighboursAlive(nbLin, nbCol, board, cellLin, cellCol);
	if(board[cellLin][cellCol]==1 && n==2){
		res=1;
	}else if(n==3){
		res=1;
	}
	return res;
}

void copyArray(int nbLin, int nbCol, int array1[nbLin][nbCol], int array2[nbLin][nbCol]){
	for(int k=0; k<nbLin; k++){
		for(int l=0; l<nbCol; l++){
			array2[k][l]=array1[k][l];
		}
	}
}

void runAStep(int nbLin, int nbCol, int board[nbLin][nbCol]){
	int tempo[nbLin][nbCol];
	for(int k=0; k<nbLin; k++){
		for(int l=0; l<nbCol; l++){
			tempo[k][l]=isCellDeadOrAlive(nbLin, nbCol, board, k, l);
		}
	}
	copyArray(nbLin, nbCol, tempo, board);
}

int getNumberOfLivingCells(int nbLin, int nbCol, int board[nbLin][nbCol]){
	int n=0;
	for(int k=0; k<nbLin; k++){
		for(int l=0; l<nbCol; l++){
			if(board[k][l]==1){
				n++;
			}
		}
	}
	return n;
}

int runGame(int nbLin, int nbCol, int board[nbLin][nbCol]){
	int livingCells;
	seedRandomCells(nbLin, nbCol, board);
	showBoard(nbLin, nbCol, board);
	for(int k=0; k<NBSTEPS; k++){
		runAStep(nbLin, nbCol, board);
		showBoard(nbLin, nbCol, board);
		livingCells=getNumberOfLivingCells(nbLin, nbCol, board);
		printf("\n\nThere is %d living cells.\n", livingCells);
	}
	return 0;
}

int main(){
	srand(time(NULL));
	int board[NBLIN][NBCOL];
	initArray(NBLIN, NBCOL, board, 0);
	runGame(NBLIN, NBCOL, board);
	cpr();	
	return 0;
}
