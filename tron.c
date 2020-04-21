#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define NBLIN 20
#define NBCOL 20

void initArray(int nbLin, int nbCol, int iArray[nbLin][nbCol], int value){
	int k, l;
	for(k=0; k<nbLin; k++){
		for(l=0; l<nbCol; l++){
			iArray[k][l] = value;
		}
	}
}

void initVectPlayer(int nbLin, int nbCol, int board[nbLin][nbCol], int vectPlayer[], int numPlayer){
	if(nbCol<10){
		exit(-1);
	}
	vectPlayer[0] = (nbLin/2)-1;
	if(numPlayer==1){
		vectPlayer[1] = 2;
		vectPlayer[2] = 2;
	}else{
		vectPlayer[1] = nbCol-3;
		vectPlayer[2] = 0;
	}
	board[vectPlayer[0]][vectPlayer[1]] = numPlayer;
}

void clearScreen(){
	/* Animation - clear the previous display */
	printf("%c[2J", 0x1B);
	/* Animation - move the cursor top-left */
	printf("%c[%d;%dH", 0x1B, 1, 1);
}

void showBoard(int nbLin, int nbCol, int board[nbLin][nbCol]){
	int k, l;
	clearScreen();
	usleep(300000);
	printf(" ");
	for(k=0; k<nbCol; k++){
		printf(" +");
	}
	printf("\n");
	for(k=0; k<nbLin; k++){
		printf("|");
		for(l=0; l<nbCol; l++){
			if(board[k][l]==0){
				printf("  ");
			}else if(board[k][l]==1){
				printf(" \033[31;01mo\033[00m");
			}else if(board[k][l]==2){
				printf(" \033[36;01mo\033[00m");
			}else if(board[k][l]==-1){
				printf(" \033[33;01mx\033[00m");
			}
		}
		printf("|\n");
	}
	printf(" ");
	for(k=0; k<nbCol; k++){
		printf(" +");
	}
	printf("\n");
}

char key_pressed(){
	struct termios oldterm, newterm;
	int oldfd;
	char c, result = 0;
	tcgetattr (STDIN_FILENO, &oldterm);
	newterm = oldterm;
	newterm.c_lflag &= ~(ICANON | ECHO);
	tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
	oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
	c = getchar();
	tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
	fcntl (STDIN_FILENO, F_SETFL, oldfd);
	if (c != EOF) {
		ungetc(c, stdin);
		result = getchar();
	}
	return result;
}

void catchNewEvents(int vectPlayer1[], int vectPlayer2[]){
	char ordres[10];
	int k;
	for(k=0; k<10; k++){
		ordres[k] = key_pressed();
	}
	for(k=0; k<10; k++){
		if(ordres[k]=='q'){
			vectPlayer1[2] = (vectPlayer1[2]+3)%4;
		}else if(ordres[k]=='d'){
			vectPlayer1[2] = (vectPlayer1[2]+1)%4;
		}else if(ordres[k]=='k'){
			vectPlayer2[2] = (vectPlayer2[2]+3)%4;
		}else if(ordres[k]=='m'){
			vectPlayer2[2] = (vectPlayer2[2]+1)%4;
		}
	}
}

void calculateNextPosition(int vectPlayer[], int nextPosPlayer[]){
	if(vectPlayer[2]==0){
		nextPosPlayer[0] = vectPlayer[0];
		nextPosPlayer[1] = vectPlayer[1]-1;
	}else if(vectPlayer[2]==1){
		nextPosPlayer[0] = vectPlayer[0]-1;
		nextPosPlayer[1] = vectPlayer[1];
	}else if(vectPlayer[2]==2){
		nextPosPlayer[0] = vectPlayer[0];
		nextPosPlayer[1] = vectPlayer[1]+1;
	}else if(vectPlayer[2]==3){
		nextPosPlayer[0] = vectPlayer[0]+1;
		nextPosPlayer[1] = vectPlayer[1];
	}
}

int checkForWallCollision(int nbLin, int nbCol, int nextPosPlayer[]){
	int check = 0;
	if(nextPosPlayer[0]<0 || nextPosPlayer[0]>= nbLin || nextPosPlayer[1]<0 || nextPosPlayer[1]>=nbCol){
		check = 1;
	}
	return check;
}

int checkForBeamCollision(int nbLin, int nbCol, int board[nbLin][nbCol], int nextPosPlayer[]){
	int check = 0;
	if(board[nextPosPlayer[0]][nextPosPlayer[1]]!=0){
		check = 1;
	}
	return check;
}

void cpr(){
	printf("\n\n\n\nTron game\n\n     Copyright Mickael Almeida 2018\n\n");
}

void moveVehicle(int nbLin, int nbCol, int board[nbLin][nbCol], int numPlayer, int vectPlayer[], int nextPosPlayer[]){
	board[nextPosPlayer[0]][nextPosPlayer[1]] = numPlayer;
	vectPlayer[0] = nextPosPlayer[0];
	vectPlayer[1] = nextPosPlayer[1];
}

int runAStep(int nbLin, int nbCol, int board[nbLin][nbCol], int vectPlayer1[], int vectPlayer2[]){
	int res = -1;
	int nextPosP1[2];
	int nextPosP2[2];
	int collP1 = 0;
	int collP2 = 0;
	catchNewEvents(vectPlayer1, vectPlayer2);
	calculateNextPosition(vectPlayer1, nextPosP1);
	calculateNextPosition(vectPlayer2, nextPosP2);
	if(nextPosP1[0]==nextPosP2[0] && nextPosP1[1]==nextPosP2[1]){
		board[nextPosP1[0]][nextPosP1[1]] = -1;
		collP1 = 1;
		collP2 = 1;
	}
	if(checkForWallCollision(nbLin, nbCol, nextPosP1)==1){
		board[vectPlayer1[0]][vectPlayer1[1]] = -1;
		collP1 = 1;
	}
	if(checkForWallCollision(nbLin, nbCol, nextPosP2)==1){
		board[vectPlayer2[0]][vectPlayer2[1]] = -1;
		collP2 = 1;
	}
	if(checkForBeamCollision(nbLin, nbCol, board, nextPosP1)==1){
		board[nextPosP1[0]][nextPosP1[1]] = -1;
		collP1 = 1;
	}
    if(checkForBeamCollision(nbLin, nbCol, board, nextPosP2)==1){
		board[nextPosP2[0]][nextPosP2[1]] = -1;
		collP2 = 1;
	}
	if(collP1==0){
		moveVehicle(nbLin, nbCol, board, 1, vectPlayer1, nextPosP1);
	}
	if(collP2==0){
		moveVehicle(nbLin, nbCol, board, 2, vectPlayer2, nextPosP2);
	}
	if(collP1==1 && collP2==1){
		res = 0;
	}else if(collP1==1){
		res = 2;
	}else if(collP2==1){
		res = 1;
	}
	return res;
}

int runGame(int nbLin, int nbCol, int board[nbLin][nbCol]){
	int flag = 1;
	int vectP1[3];
	int vectP2[3];
	int win = -1;
	initVectPlayer(nbLin, nbCol, board, vectP1, 1);
	initVectPlayer(nbLin, nbCol, board, vectP2, 2);
	showBoard(nbLin, nbCol, board);
	do{
		win = runAStep(nbLin, nbCol, board, vectP1, vectP2);
		showBoard(nbLin, nbCol, board);
		if(win!=-1){
			flag = 0;
		}
	}while(flag==1);
	return win;
}

void endOfGame(int numWinner){
	printf("\n\n#### THE GAME IS OVER ####\n");
	if(numWinner==0){
		printf("\nIt's a draw!\n\n");
	}else{
		printf("\nAnd the winner is Player %d!\n\n", numWinner);
	}
}

int main(){
	int board[NBLIN][NBCOL];
	int win;
	initArray(NBLIN, NBCOL, board, 0);
	win = runGame(NBLIN, NBCOL, board);
	endOfGame(win);
	cpr();
	return 0;
}
