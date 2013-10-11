#include <stdio.h>

//The number of column
#define COL_NUM 9
//Square root of the number of columns
#define COL_NUM_SQR 3

//It is rowXcolumnXnumber array initialize to zero by being global.
//It counts the number of neighboars with the same number.  
int flags[COL_NUM][COL_NUM][COL_NUM];
//Counts the number of neighboars
int counts[COL_NUM][COL_NUM];

//The soduko problem.
int problem[COL_NUM][COL_NUM]=
  {{0,0,4, 8,0,0, 0,1,7},
   {6,7,0, 9,0,0, 0,0,0},
   {5,0,8, 0,3,0, 0,0,4},

   {3,0,0, 7,4,0, 1,0,0},
   {0,6,9, 0,0,0, 7,8,0},
   {0,0,1, 0,6,9, 0,0,5},

   {1,0,0, 0,8,0, 3,0,6},
   {0,0,0, 0,0,6, 0,9,1},
   {2,4,0, 0,0,1, 5,0,0}};

void printBoard() 
{
  int col,row;
  int val;
  for (row=0;row<COL_NUM;row++) {
    if (row % COL_NUM_SQR==0 && row>0) {
      for (col=0;col<COL_NUM*2-1;col++) {
	printf("-");
      }
    }
    printf("\n");
    for (col=0;col<COL_NUM;col++) {
      
      val=problem[row][col];
      if (val==0) {
	printf(".");
      } else {
	printf("%d",problem[row][col]);
      }
      if ((col+1)%COL_NUM_SQR==0 && col<COL_NUM-1) {
	printf("|");
      } else {
	printf(" ");
      }
    }
    printf("\n");
  }
}

//Mark num as a neighbour of the element in row r and column c.
void setNeighbour(int r,int c,int num) 
{
  if (!flags[r][c][num]++) {
    counts[r][c]--;
  }    
}

//Unmark num as a neighbour of the element in row r and column c.
void unsetNeighbour(int r,int c,int num)
{
  if (!--flags[r][c][num]) {
    counts[r][c]++;
  }
}

//Set the value of the element in row r and column c to num
void setElement(int r,int c,int num) 
{
  int i,j;
  int rowOrig=(r / COL_NUM_SQR)*COL_NUM_SQR,
    colOrig=(c / COL_NUM_SQR)*COL_NUM_SQR;
  problem[r][c]=num;
  //The flags are index by 0
  num--;
  //Mark the row and column
  for (i=0;i<COL_NUM;i++) {
    setNeighbour(i,c,num);
    setNeighbour(r,i,num);
  }
  //Mark squares
  for (i=0;i<COL_NUM_SQR;i++) {
    for (j=0;j<COL_NUM_SQR;j++) {
      setNeighbour(rowOrig+i,colOrig+j,num);
    }
  }
}

//Undo the action done by the set function
void undoSetElement(int r,int c,int num) 
{
  int i,j;
  int rowOrig=(r / COL_NUM_SQR)*COL_NUM_SQR,
    colOrig=(c / COL_NUM_SQR)*COL_NUM_SQR;
  problem[r][c]=0;
  //The flags are index by 0
  num--;
  //Mark the row and column
  for (i=0;i<COL_NUM;i++) {
    unsetNeighbour(i,c,num);
    unsetNeighbour(r,i,num);
  }
  //Mark squares
  for (i=0;i<COL_NUM_SQR;i++) {
    for (j=0;j<COL_NUM_SQR;j++) {
      unsetNeighbour(rowOrig+i,colOrig+j,num);
    }
  }
}

void init() 
{
  int i,j,num;
  for (i=0;i<COL_NUM;i++) {
    for (j=0;j<COL_NUM;j++) {
      counts[i][j]=9;
    }
  }
  for (i=0;i<COL_NUM;i++) {
    for (j=0;j<COL_NUM;j++) {
      num=problem[i][j];
      if (num) {
	setElement(i,j,num);
      }
    }
  }
}

//Find unsolved element with a minimal number of options to choose from.
//It return r as the row, c as the column of the element.
//It return 1 if it found an element 
//and 0 If the board is solved. 
int findMin(int *r,int *c) 
{
  int row,col;
  int min=COL_NUM+1;
  int minr,minc;
  int val;
  for (row=0;row<COL_NUM;row++) {
    for (col=0;col<COL_NUM;col++) {
      if (!problem[row][col]) {
	val=counts[row][col];
	if (val<min) {
	  minr=row;
	  minc=col;
	  min=val;
	}
      }
    }
  }
  if (min<COL_NUM+1) {
    *r=minr;
    *c=minc;
    return 1;
  }
  return 0;
}

//Let the user choose if he want to continue.
//Return 1 to continue and 0 to stop.
int continueOrNot() {
  char c[2];
  printBoard();
  printf("\npress ';' to continue or any other key to abort ");
  scanf("%1s",c);
  if (c[0]==';') {
    return 1;
  }
  return 0;
}

//Solve the soduko puzzle.
//It return 1 to continue and 0 to stop.
int solve()
{
  int r,c;
  int found=findMin(&r,&c);
  if (found) {
    int min=counts[r][c];
    if (min==0) {
      return 1;
    }
    int num;
    for (num=0;num<COL_NUM;num++) {
      if (!flags[r][c][num]) {
	setElement(r,c,num+1);
	if (!solve()) return 0;
	undoSetElement(r,c,num+1);
      }
    }
    return 1;
  }
  return continueOrNot(); 
}

int main(int argc, const char *argv[]) 
{
  init();
  solve();
}
