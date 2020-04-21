#include <stdio.h>
#include <stdlib.h>
#define NBLIN 6
#define NBCOL 7

void initArray(int nbLin, int nbCol, int iArray[nbLin][nbCol], int value){ //this function fills the Array with 0
	for(int k=0; k<nbLin; k++){ //for each line,
		for(int l=0; l<nbCol; l++){ //for each column,
			iArray[k][l]=value; //we give 0 as value of the box
		}
	}
}

void clearScreen(){
	//Animation - clear the previous display
	printf("%c[2J", 0x1B);
	//Animation - move the cursor top-left
	printf("%c[%d;%dH", 0x1B, 1, 1);
}

void cpr(){
	printf("\n\n\n\nPower 4 game\n\n     Copyright Mickael Almeida & Roman Didelet 2018\n\n");
}

void showBoard(int nbLin, int nbCol, int board[nbLin][nbCol]){ //in this function, we use the "for"s to place the board at the same time than the pawns
	clearScreen();
	for(int k=0; k<nbLin; k++){
		printf("|");
		for(int l=0; l<nbCol; l++){
			if(board[k][l]==0){
				printf("   |");
			}else if(board[k][l]==1){
				printf(" \033[31;01mo\033[00m |");
			}else if(board[k][l]==2){
				printf(" \033[36;01mo\033[00m |");
			}
		}
		printf("\n");
	}
	for(int m=0; m<nbCol; m++){
		printf(" ---");
	}
	printf("\n");
	for(int i=1; i<=nbCol; i++){
		printf("%3d ", i);
	}
	printf("\n");
}

int getColumnForPawn(int nbLin, int nbCol, int board[nbLin][nbCol]){ //this function returns the column for the new pawn
	int pos;
	do{
		printf("Enter the coordinate of the column you want to put your pawn on: ");
		if(scanf("%d", &pos)!=1){ //we wait for the column chosen by the player
			printf("Input error\n"); //if it is not a number, the program breaks
			exit(-1);
		}
		pos--; //we decrement the value because we count from 1 to nbCol but the computer counts from 0 to nbCol-1
		//if there is an error, we display one of the following messages and then we ask the player his column again
		if(pos<0 || pos>=nbCol){ //if the column chosen is outside the limits
			printf("\nWrong number of column (outside the board), please re-try.\n");
		}else if(board[0][pos]!=0){ //if the last box is full i.e. the column is full
			printf("\nThis column is full so you can't add any more pawn, please re-try.\n");
		}
		printf("\n");
	}while(pos<0 || pos>=nbCol || board[0][pos]!=0);
	return pos;
}

int placePawn(int nbLin, int nbCol, int board[nbLin][nbCol], int pawnCol, int pawn){ //in this function we place the pawn on the board
	int k;
	for(k=nbLin-1; k>=0; k--){ //for each line starting from the bottom
		if(board[k][pawnCol]==0){ //if the box is empty
			board[k][pawnCol]=pawn; //we place the pawn and we break the loop
			break;
		}
	}
	return k;
}

int checkFourInLine(int nbLin, int nbCol, int board[nbLin][nbCol], int pawnLin, int pawnCol){ //in this function, we will check if there are 4 consecutive pawns with the one that just has been placed
	int a=board[pawnLin][pawnCol], win=0, nbPion1=1, nbPion2=1, nbPion3=1, nbPion4=1, k1=0, k2=0, i=0, x1=0, y1=0, x2=0, y2=0, x3=0, y3=0, x4=0, y4=0;
	//we have to see in the 4 dimensions if there are 4 consecutive pawns, so, we check independantly the 7 possibilities starting from the pawn (there will never be a pawn on this pawn) and we use the same variable for those who are in the same dimension

	//horizontal left
	do{
		k1++;
		if(pawnCol-k1>=0 && board[pawnLin][pawnCol-k1]==a){
			nbPion1++;
		}
	}while(board[pawnLin][pawnCol-k1]==a && pawnCol-k1>=0);

	//horizontal right
	do{
		k2++;
		if(pawnCol+k2<=nbCol && board[pawnLin][pawnCol+k2]==a){
			nbPion1++;
		}
	}while(board[pawnLin][pawnCol+k2]==a && pawnCol+k2<nbCol);

	//vertical down
	do{
		i++;
		if(pawnLin+i<nbLin && board[pawnLin+i][pawnCol]==a){
			nbPion2++;
		}
	}while(board[pawnLin+i][pawnCol]==a &&pawnLin+i<nbLin);

	//diagonal "Nord-West"
	do{
		x1++;
		y1++;
		if(pawnCol-x1>=0 && pawnLin-y1>=0 && board[pawnLin-y1][pawnCol-x1]==a){
			nbPion3++;
		}
	}while(pawnCol-x1>=0 && pawnLin-y1>=0 && board[pawnLin-y1][pawnCol-x1]==a);

	//diagonal "South-East"
	do{
		x2++;
		y2++;
		if(pawnCol+x2<nbCol && pawnLin+y2<nbLin && board[pawnLin+y2][pawnCol+x2]==a){
			nbPion3++;
		}
	}while(pawnCol+x2<nbCol && pawnLin+y2<nbLin && board[pawnLin+y2][pawnCol+x2]==a);

	//diagonal "Nord-East"
	do{
		x3++;
		y3++;
		if(pawnCol+x3<nbCol && pawnLin-y3>=0 && board[pawnLin-y3][pawnCol+x3]==a){
			nbPion4++;
		}
	}while(pawnCol+x3<nbCol && pawnLin-y3>=0 && board[pawnLin-y3][pawnCol+x3]==a);

	//diagonal "South-West"
	do{
		x4++;
		y4++;
		if(pawnCol-x4>=0 && pawnLin+y4<nbLin && board[pawnLin+y4][pawnCol-x4]==a){
			nbPion4++;
		}
	}while(pawnCol-x4>=0 && pawnLin+y4<nbLin && board[pawnLin+y4][pawnCol-x4]==a);

	//finally, we check if one of the cases has been completed, if so, we change the return value from 0 to 1
	if(nbPion1>=4 || nbPion2>=4 || nbPion3>=4 || nbPion4>=4){
		win=1;
	}
	return win;
}

int runAStep(int nbLin, int nbCol, int board[nbLin][nbCol], int numPlayer){ //this function does only one step
	int col, lin, win;
	printf("\n#### Player %d, your turn ####\n", numPlayer);
	col=getColumnForPawn(nbLin, nbCol, board); //we recieve the column chosen by the player and we save the column "col"
	lin=placePawn(nbLin, nbCol, board, col, numPlayer); //then we place the pawn and we save the line "lin"
	win=checkFourInLine(nbLin, nbCol, board, lin, col); //using the coordinates of the pawn, we search for the consecutive ones
	return win; //if there are at least 4 consecutive pawns, the function will return 1, otherwise it will return 0
}

int runGame(int nbLin, int nbCol, int board[nbLin][nbCol]){ //this function manages the most part of the game
	int flag=1, coups=nbLin*nbCol, numPlayer=1, step, win=0;
	showBoard(nbLin, nbCol, board); //first, we show the board
	do{ //the do/while loop is used to play until someone loses
		step=runAStep(nbLin, nbCol, board, numPlayer); //then we do a step of the game
		showBoard(nbLin, nbCol, board); //we print the board again
		if(step==1){ //if the function runAStep returns 1, it means that the player currently playing won
			flag=0; //so we tell the function that someone lost
			win=numPlayer; //and we return the number of the player
		}
		if(numPlayer==1){ //we change the player
			numPlayer=2;
		}else{
			numPlayer=1;
		}
		coups--;
		if(coups==0){ //if we can not play anymore
			flag=0; //we tell the function that someone lost
		}
	}while(flag==1);
	return win;
}

void endOfGame(int numWinner){ //this function only writes the result of the game with the information returned by runGame
	printf("\n\n#### THE GAME IS OVER ####\n");
	if(numWinner==0){
		printf("\nIt's a draw!\n");
	}else{
		printf("\nAnd the winner is Player %d!\n", numWinner);
	}
}

int main(){ //the main is the function that will be executed by the computer
	int board[NBLIN][NBCOL]; //we define the board as this 2D array
	initArray(NBLIN, NBCOL, board, 0); //we initialise it
	int winner=runGame(NBLIN, NBCOL, board); //we play the game
	endOfGame(winner); //and we end it
	cpr();
	return 0;
}

//This "Puissance 4" game has been thought and written by ALMEIDA Mickael and DIDELET Roman
