#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NBLIN 10
#define NBCOL 10

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

void showBoard(int nbLin, int nbCol, int board[nbLin][nbCol]){
	clearScreen();
	printf("   j");
	for(int k=0; k<nbCol; k++){
		printf("%3d ", k);
	}
	printf("\n i ");	
	for(int k=0; k<nbCol; k++){
		printf(" ---");
	}
	printf("\n");
	for(int k=0; k<nbLin; k++){
		printf("%2d |", k);
		for(int l=0; l<nbCol; l++){
			if(board[k][l]>0){
				printf("\033[36;01m%3d\033[00m ", board[k][l]);
			}else if(board[k][l]<0){
				printf("\033[31;01m%3d\033[00m ", -1*board[k][l]);
			}else{
				printf("%3d ", board[k][l]);
			}
		}
		printf("\n");
	}
}

int getNextUnitForce(){
	int a=rand()%20+1;
	return a;
}

void cpr(){
	printf("\n\n\n\nProximity game\n\n     Copyright Mickael Almeida 2018\n\n");
}

void placeUnitForce(int nbLin, int nbCol, int board[nbLin][nbCol], int unitForce, int ufCoord[]){
	int i, j;
	do{
		printf("Enter the coordinates i and j of the case you want to put your UF on: ");
		if(scanf("%d %d", &i, &j)!=2){
			printf("Input error");
			exit(-1);
		}
		if(i<0 || i>=nbLin || j<0 || j>=nbCol){
			printf("Wrong coordinates (outside of the board), please re-try.\n");
		}else if(board[i][j]!=0){
			printf("Cell already full, please re-try.\n");
		}
	}while(i<0 || i>=nbLin || j<0 || j>=nbCol || board[i][j]!=0);
	board[i][j]=unitForce;
	ufCoord[0]=i;
	ufCoord[1]=j;
}

void affectNeighborhood(int nbLin, int nbCol, int board[nbLin][nbCol], int groundLin, int groundCol){
	int a, x=board[groundLin][groundCol];
	if(board[groundLin][groundCol]<0){
		a=-1;
	}else{
		a=1;
	}
	//sqrt((x-y)^2)-sqrt(x^2)-sqrt(y^2)=0 -> x & y different sign/ <0 -> x & y same sign
	//up
	if(groundLin!=0 && board[groundLin-1][groundCol]!=0){
		int y=board[groundLin-1][groundCol];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin-1][groundCol]*=-1;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin-1][groundCol]+=a;
		}
	}
	//down
	if(groundLin!=nbLin-1 && board[groundLin+1][groundCol]!=0){
		int y=board[groundLin+1][groundCol];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin+1][groundCol]*=-1;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin+1][groundCol]+=a;
		}
	}
	//left
	if(groundCol!=0 && board[groundLin][groundCol-1]!=0){
		int y=board[groundLin][groundCol-1];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin][groundCol-1]*=-1;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin][groundCol-1]+=a;
		}
	}
	//right
	if(groundCol!=nbCol-1 && board[groundLin][groundCol+1]!=0){
		int y=board[groundLin][groundCol+1];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin][groundCol+1]*=-1;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin][groundCol+1]+=a;
		}
	}
	//upper-left
	if(groundCol!=0 && groundLin!=0 && board[groundLin-1][groundCol-1]!=0){
		int y=board[groundLin-1][groundCol-1];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin-1][groundCol-1]=-1*y;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin-1][groundCol-1]+=a;
		}
	}
	//upper-right
	if(groundCol!=nbCol-1 && groundLin!=0 && board[groundLin-1][groundCol+1]!=0){
		int y=board[groundLin-1][groundCol+1];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin-1][groundCol+1]*=-1;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin-1][groundCol+1]+=a;
		}
	}
	//lower-right
	if(groundCol!=nbCol-1 && groundLin!=nbLin-1 && board[groundLin+1][groundCol+1]!=0){
		int y=board[groundLin+1][groundCol+1];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin+1][groundCol+1]*=-1;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin+1][groundCol+1]+=a;
		}
	}
	//lower-left
	if(groundCol!=0 && groundLin!=nbLin-1 && board[groundLin+1][groundCol-1]!=0){
		int y=board[groundLin+1][groundCol-1];
		if(abs(x-y)-abs(x)-abs(y)==0 && abs(y)<abs(x)){
			board[groundLin+1][groundCol-1]*=-1;
		}else if(abs(x-y)-abs(x)-abs(y)<0){
			board[groundLin+1][groundCol-1]+=a;
		}
	}
}


void runAStep(int nbLin, int nbCol, int board[nbLin][nbCol], int numPlayer){
	int uf=getNextUnitForce();
	int ufCoord[2];
	printf("\n#### Player %d ####\nYour next Unit Force is %d\n", numPlayer, uf);
	if(numPlayer==1){
		uf*=-1;
	}
	placeUnitForce(nbLin, nbCol, board, uf, ufCoord);
	affectNeighborhood(nbLin, nbCol, board, ufCoord[0], ufCoord[1]);
}


int getNumberOfTerritoriesForPlayer(int nbLin, int nbCol, int board[nbLin][nbCol], int numPlayer){
	int p1=0, p2=0, terter;
	for(int k=0; k<nbLin; k++){
		for(int l=0; l<nbCol; l++){
			if(board[k][l]>0){
				p2++;
			}else if(board[k][l]<0){
				p1++;
			}
		}
	}
	if(numPlayer==1){
		terter=p1;
	}else{
		terter=p2;
	}
	return terter;
}

int runGame(int nbLin, int nbCol, int board[nbLin][nbCol]){
	int coups=nbLin*nbCol, numPlayer=1, terter1, terter2, win;
	showBoard(nbLin, nbCol, board);
	for(int k=0; k<coups; k++){
		runAStep(nbLin, nbCol, board, numPlayer);
		showBoard(nbLin, nbCol, board);
		terter1=getNumberOfTerritoriesForPlayer(nbLin, nbCol, board, 1);
		terter2=getNumberOfTerritoriesForPlayer(nbLin, nbCol, board, 2);
		printf("\nTerritories Player 1: %d - Territories Player 2: %d\n", terter1, terter2);
		if(numPlayer==1){
			numPlayer=2;
		}else{
			numPlayer=1;
		}
	}
	if(terter1==terter2){
		win=0;
	}else if(terter1>terter2){
		win=1;
	}else{
		win=2;
	}
	return win;
}

void endOfGame(int numWinner){
	printf("\n\n#### THE GAME IS OVER ####\n");
	if(numWinner==0){
		printf("\nIt's a draw!\n");
	}else{
		printf("\nAnd the winner is Player %d!\n", numWinner);
	}
}

int main (){
	int game;
	srand(time(NULL));
	int board[NBLIN][NBCOL];
	initArray(NBLIN, NBCOL, board, 0);
	game=runGame(NBLIN, NBCOL, board);
	endOfGame(game);
	cpr();
	return 0;
}