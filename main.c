#include <stdio.h>
#include <time.h>
#include <stdlib.h> 

int Table[49];

int Miss[49];
int Treasure[49];
int Bomb[49];

int Score[2];

int Entered[49];
int MEntered[49];
int TEntered[49];
int BEntered;

int TimeLeft;

void ClearScreen(){
    system("@cls||clear");
}

void RanMiss(int MissNum){
    int i;
    for (i = 0; i < MissNum; i++){
        int num = (rand() % 49) + 1;
        Miss[i] = num;
        int j;
        for (j = 0; j < i; j++){
            if (Miss[j] == num)
                i -= 1;
        }
    }
}

void RanTreasure(int TreasureNum, int MissNum){
    int i;
    for (i = 0; i < TreasureNum; i++){
        int num = (rand() % 49) + 1;
        Treasure[i] = num;
        int j;
        for (j = 0; j < i; j++){
            if (Treasure[j] == num)
                i -= 1;
        }
        for (j = 0; j < MissNum; j++)
            if (Miss[j] == num)
                i -= 1;
    }
}

void RanBomb(int BombNum, int TreasureNum, int MissNum){
    int i;
    for (i = 0; i < BombNum; i++){
        int num = (rand() % 49) + 1;
        Bomb[i] = num;
        int j;
        for (j = 0; j < i; j++){
            if (Bomb[j] == num)
                i -= 1;
        }
        for (j = 0; j < TreasureNum; j++)
            if (Treasure[j] == num)
                i -= 1;
        for (j = 0; j < MissNum; j++)
            if (Miss[j] == num)
                i -= 1;
    }
}

int CheckScore(int Player,int Pos, int BombNum, int TreasureNum, int MissNum){
    int i;

    for (i = 0; i < MissNum; i++){
        if (Pos == Miss[i]){
            Score[Player - 1] += 1;
            MEntered[Pos - 1] = Pos;
            return 1;
        }
    }
    for (i = 0; i < TreasureNum; i++){
        if (Pos == Treasure[i]){
            Score[Player - 1] += 3;
            TEntered[Pos - 1] = Pos;
            return 2;
        }
    }
    for (i = 0; i < BombNum; i++){
        if (Pos == Bomb[i]){
            BEntered = Pos;
            return 3;
        }
    }
    return 0;
}

int CheckIsEntered(int Pos){
    if (Entered[Pos - 1] == Pos)
        return 0;
    return 1;
}

int CheckWhatInPos(int Pos){
    if (MEntered[Pos - 1] == Pos)
        return 1;
    else if (TEntered[Pos - 1] == Pos)
        return 2;
    else if (BEntered == Pos)
        return 3;
    return 0;
}

void Show(){
    ClearScreen();
    printf("\t\t\t  About %d minute left\n", (int)(TimeLeft/CLOCKS_PER_SEC)/60);
    printf("\t\t\t    About %d Seconds\n", (int)(TimeLeft/CLOCKS_PER_SEC));
    printf("----------------------------------------------------------------------\n");
    printf("Player 1 : %d Point(s)\t\t\t\tPlayer 2 : %d Point(s)\n", Score[0], Score[1]);
    printf("----------------------------------------------------------------------\n");
    int i;
    for (i = 0; i < 49; i++){
        if (i+1 < 10 && CheckIsEntered(Table[i]) == 1)
            printf("|    %d   |", Table[i]);
        else if (CheckIsEntered(Table[i]) == 0 && CheckWhatInPos(Table[i]) == 0)
            printf("|\033[1;36m  XXXX  \033[0m|");
        else if (CheckIsEntered(Table[i]) == 0 && CheckWhatInPos(Table[i]) == 1)
            printf("|\033[1;32m  Miss  \033[0m|");
        else if (CheckIsEntered(Table[i]) == 0 && CheckWhatInPos(Table[i]) == 2)
            printf("|\033[1;33mTreasure\033[0m|");
        else if (CheckIsEntered(Table[i]) == 0 && CheckWhatInPos(Table[i]) == 3)
            printf("|\033[1;31m  BOOM  \033[0m|");
        else
            printf("|   %d   |", Table[i]);
        if ((i+1) % 7 == 0)
            printf("\n");
    }
    printf("----------------------------------------------------------------------\n");
}

void Start(){
    int i;
    for (i = 0; i < 49; i++){
        Table[i] = i+1;
        Entered[i] = 0;
        MEntered[i] = 0;
        TEntered[i] = 0;
    }
    BEntered = 0;

    Score[0] = 0;
    Score[1] = 0;

    TimeLeft = 180000;

    srand ( time(NULL) );
}

int main(){

    Start();

    int Choice = 49;
    int MissNum;
    do{
        ClearScreen();
        printf("Enter Number of Miss [0 to %d] : ", Choice);
        scanf("%d", &MissNum);
    }while (MissNum < 0 || MissNum > Choice);

    RanMiss(MissNum);
    Choice -= MissNum;

    int TreasureNum;
    do{
        printf("\nEnter Number of Treasure [0 to %d] : ", Choice);
        scanf("%d", &TreasureNum);
    }while (TreasureNum < 0 || TreasureNum > Choice);

    RanTreasure(TreasureNum, MissNum);
    Choice -= TreasureNum;

    int BombNum;
    do{
        printf("\nEnter Number of Bomb [0 to %d] : ", Choice);
        scanf("%d", &BombNum);
    }while (BombNum < 0 || BombNum > Choice);

    RanBomb(BombNum, TreasureNum, MissNum);
    Choice -= BombNum;

    int Player = 1, Pos = 0, Time, WhichItem = 0;
    
    while (1){
        clock_t begin = clock();
        do {
            Show();
            
            printf("Player %d Choose a Number : ", Player);
            scanf("%d", &Pos);

            if (CheckIsEntered(Pos) == 1){
                WhichItem = CheckScore(Player, Pos, BombNum, TreasureNum, MissNum);
            }

        }while (Pos < 1 || Pos > 49 || CheckIsEntered(Pos) == 0);

        Entered[Pos - 1] = Pos;
        
        if (TimeLeft < 0)
            TimeLeft = 0;

        if (Score[0] >= 12 || Score[1] >= 12 || TimeLeft <= 0 || CheckWhatInPos(Pos) == 3)
            break;

        Player += 1;
        if (Player > 2)
            Player = 1;

        clock_t end = clock();
        TimeLeft -= end - begin;
    }
    Show();
    if (Score[0] > Score[1] && CheckWhatInPos(Pos) != 3)
        printf("\033[1;33m===Player 1 WIN===\033[0m\n");
    else if (Score[1] > Score[0] && CheckWhatInPos(Pos) != 3)
        printf("\033[1;33m===Player 2 WIN===\033[0m\n");
    else if (Score[1] == Score[0] && CheckWhatInPos(Pos) != 3)
        printf("\033[1;33m===DRAW===\033[0m\n");
    
    if (CheckWhatInPos(Pos) == 3){
        if (Player == 1)
            printf("\033[1;33m===Player 2 WIN===\033[0m\n");
        else
            printf("\033[1;33m===Player 1 WIN===\033[0m\n");
    }

    printf("Press any key to continue...");
    getchar();
    return 0;
}