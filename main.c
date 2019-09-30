#include <stdio.h>
#include <time.h>
#include <stdlib.h> 

int Table[49];  //สร้างอาเรย์เพื่อเก็บตารางขนาด 49

int Miss[49];   //สร้างอาเรย์เพื่อเก็บตำแหน่ง miss ขนาด 49
int Treasure[49];   //สร้างอาเรย์เพื่อเก็บตำแหน่งขุ่มทรัพย์ขนาด 49
int Bomb[49];   //สร้างอาเรย์เพื่อเก็บตำแหน่งระเบิดขนาด 49

int Score[2];   //สร้างอาเรย์เพื่อเก็บคะแนนของผู้เล่น

int Entered[49];
int MEntered[49];
int TEntered[49];
int BEntered;

int TimeLeft;   //สร้างตัวแปรเก็บเวลาชีวิต

void ClearScreen(){     //ฟังกฺ์ชั่นล้างหน้าจอ
    system("@cls||clear");
}

void RanMiss(int MissNum){  //ฟังก์ชั่นสุ่มตำแหน่ง miss ตามจำนวนที่รับมา
    int i;
    for (i = 0; i < MissNum; i++){      //วนลูปตามจำนวนที่รับมา
        int num = (rand() % 49) + 1;    //สุ่มตำแหน่งตั้งแต่ 0-49
        Miss[i] = num;                  //เก็บตำแหน่งลงในอาเรย์ Miss
        int j;
        for (j = 0; j < i; j++){        //ลูปเพื่อลบตำแหน่งที่เก็บออก
            if (Miss[j] == num)
                i -= 1;
        }
    }
}

void RanTreasure(int TreasureNum, int MissNum){     //ฟังก์ชั่นสุ่มตำแหน่ง treasure ตามจำนวนที่รับมา
    int i;
    for (i = 0; i < TreasureNum; i++){              //วนลูปตามจำนวนที่รับมา
        int num = (rand() % 49) + 1;                //สุ่มตำแหน่งตั้งแต่ 0-49
        Treasure[i] = num;                          //เก็บตำแหน่งลงในอาเรย์ treasure
        int j;
        for (j = 0; j < i; j++){                    //ลูปเพื่อลบตำแหน่งที่เก็บออก
            if (Treasure[j] == num)
                i -= 1;
        }
        for (j = 0; j < MissNum; j++)               //ลูปเพื่อเช็คต่ำแหนงที่ของ miss ออก
            if (Miss[j] == num)
                i -= 1;
    }
}

void RanBomb(int BombNum, int TreasureNum, int MissNum){    //ฟังก์ชั่นสุ่มตำแหน่ง bomb ตามจำนวนที่รับมา
    int i;
    for (i = 0; i < BombNum; i++){                          //วนลูปตามจำนวนที่รับมา
        int num = (rand() % 49) + 1;                        //สุ่มตำแหน่งตั้งแต่ 0-49
        Bomb[i] = num;                                      //เก็บตำแหน่งลงในอาเรย์ bomb
        int j;
        for (j = 0; j < i; j++){                            //ลูปเพื่อลบตำแหน่งที่เก็บออก
            if (Bomb[j] == num)
                i -= 1;
        }
        for (j = 0; j < TreasureNum; j++)                   //ลูปเพื่อเช็คต่ำแหนงที่ของ traesure ออก
            if (Treasure[j] == num)
                i -= 1;
        for (j = 0; j < MissNum; j++)                       //ลูปเพื่อเช็คต่ำแหนงที่ของ miss ออก
            if (Miss[j] == num)
                i -= 1;
    }
}

//ฟังก์ชั่นเช็คคะแนนโดยรับ ตาของผู้เล่น ตำแหน่ง จำนวน bomb จำนวน treasure จำนวน miss
int CheckScore(int Player,int Pos, int BombNum, int TreasureNum, int MissNum){
    int i;

    for (i = 0; i < MissNum; i++){      //ลูปเพื่อเช็คว่าตำแหน่งที่รับมานั้นตรงกับตำแหน้ง miss หรือไม่
        if (Pos == Miss[i]){            //ถ้าตำแหน่งตรงกัน
            Score[Player - 1] += 1;     //คะแนนของ player +1
            MEntered[Pos - 1] = Pos;    //เก็บตำแหน่งที่กรอกมาไปไว้ในอาเรย์ MEntered
            return 1;
        }
    }
    for (i = 0; i < TreasureNum; i++){  //ลูปเพื่อเช็คว่าตำแหน่งที่รับมานั้นตรงกับตำแหน้ง miss หรือไม่
        if (Pos == Treasure[i]){        //ถ้าตำแหน่งตรงกัน
            Score[Player - 1] += 3;     //คะแนนของ player +3
            TEntered[Pos - 1] = Pos;    //เก็บตำแหน่งที่กรอกมาไปไว้ในอาเรย์ TEntered
            return 2;
        }
    }
    for (i = 0; i < BombNum; i++){      //ลูปเพื่อเช็คว่าตำแหน่งที่รับมานั้นตรงกับตำแหน้ง miss หรือไม่
        if (Pos == Bomb[i]){            //ถ้าตำแหน่งตรงกัน
            BEntered = Pos;             //เก็บตำแหน่งที่กรอกมาไปไว้ในอาเรย์ BEntered
            return 3;
        }
    }
    return 0;   //ถ้าไม่ตรงอะไรเลย
}

//เช็คตำแหน่งทั้งหมด
int CheckIsEntered(int Pos){
    if (Entered[Pos - 1] == Pos)    //ถ้าตำแหน่งที่รับมามีอยู่ใน Entered
        return 0;                   //return ค่า 0 ออกไป
    return 1;                       //ถ้าไม่ return 1
}

//เช็คตำแหน่งที่รับมานั้นตรงกับตำแหน่งอะไร
int CheckWhatInPos(int Pos){
    if (MEntered[Pos - 1] == Pos)   //ถ้าตำแหน่งที่รับมามีอยู่ใน Entered
        return 1;                   //return ค่า 1 ออกไป
    else if (TEntered[Pos - 1] == Pos)  //ถ้าตำแหน่งที่รับมามีอยู่ใน TEntered
        return 2;                   //return ค่า 2 ออกไป
    else if (BEntered == Pos)       //ถ้าตำแหน่งที่รับมามีอยู่ใน BEntered
        return 3;                   //return ค่า 3 ออกไป
    return 0;       //ถ้าไม่ return ค่า 0 ออกไป
}

//ฟังก์ชั่นแสดงตาราง
void Show(){
    ClearScreen();      //เรียกใช้ฟังก์ชั่นล้างหน้าจอ
    printf("\t\t\t  About %d minute left\n", (int)(TimeLeft/CLOCKS_PER_SEC)/60);    //แสดงเวลาที่เหลือเป็นนาที
    printf("\t\t\t    About %d Seconds\n", (int)(TimeLeft/CLOCKS_PER_SEC));         //แสดงเวลาที่เหลือเป็นวินาที
    printf("----------------------------------------------------------------------\n");
    printf("Player 1 : %d Point(s)\t\t\t\tPlayer 2 : %d Point(s)\n", Score[0], Score[1]);   //แสดงคะแนนของ player 1 and 2
    printf("----------------------------------------------------------------------\n");
    int i;
    for (i = 0; i < 49; i++){       //ลูปเพื่อเช็คสร้างตารางออกมา
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

//ฟังก์ชั่นเริ่มเกม
void Start(){
    int i;
    for (i = 0; i < 49; i++){   //ลูปเพื่อตังค่าต่างๆ
        Table[i] = i+1;         //แสดงตัวเลขในตาราง
        Entered[i] = 0;         //ตำแหน่งทั้งหมดเท่ากับ 0
        MEntered[i] = 0;        //ตำแหน่ง miss เท่ากับ 0
        TEntered[i] = 0;        //ตำแหน่ง treasure เท่ากับ 0
    }
    BEntered = 0;               //ระเบิดเท่ากับ 0

    Score[0] = 0;       //คะแนน player 1 = 0
    Score[1] = 0;       //คะแนน player 2 = 0 

    TimeLeft = 180000;  //เวลาเท่ากับ 180000 มิลลิวินาที = 3 นาที

    srand ( time(NULL) );   //สุุ่มค่าตามเวลาเพิ้อให้สามารถสุ่มค่าใหม่ได้
}

int main(){

    Start();    //เรียกฟังกฺชั่นเริ่มเกม

    int Choice = 49;    //ตำเลือก 49
    
    //ลูปเพื่อให้กรอกจำนวน miss หากไม่ถูกต้องจำไม่หลุดลูป
    int MissNum;       
    do{ 
        ClearScreen();
        printf("Enter Number of Miss [0 to %d] : ", Choice);
        scanf("%d", &MissNum);
    }while (MissNum < 0 || MissNum > Choice);

    RanMiss(MissNum);   //เรียกฟังก์ชั่นสุ่ม miss จากจำนวนที่กรอก
    Choice -= MissNum;  //จำนวนตำแหน่งที่เหลือทั้งหมดลบด้วยจำนวน miss

    //ลูปเพื่อให้กรอกจำนวน treasure หากไม่ถูกต้องจำไม่หลุดลูป
    int TreasureNum;
    do{
        printf("\nEnter Number of Treasure [0 to %d] : ", Choice);
        scanf("%d", &TreasureNum);
    }while (TreasureNum < 0 || TreasureNum > Choice);

    RanTreasure(TreasureNum, MissNum);  //เรียกฟังก์ชั่นสุ่ม treusure จากจำนวน treasure และ miss ที่กรอก
    Choice -= TreasureNum;              //จำนวนตำแหน่งที่เหลือทั้งหมดลบด้วยจำนวน treusure

    //ลูปเพื่อให้กรอกจำนวน bomb หากไม่ถูกต้องจำไม่หลุดลูป
    int BombNum;
    do{
        printf("\nEnter Number of Bomb [0 to %d] : ", Choice);
        scanf("%d", &BombNum);
    }while (BombNum < 0 || BombNum > Choice);

    RanBomb(BombNum, TreasureNum, MissNum); //เรียกฟังก์ชั่นสุ่ม bomb จากจำนวน bomb treasure และ miss ที่กรอก
    Choice -= BombNum;          //จำนวนตำแหน่งที่เหลือทั้งหมดลบด้วยจำนวน bomb

    int Player = 1, Pos = 0, Time, WhichItem = 0;   //เริม player 1 จำแหน่ง 0 จำนวน 0 
    
    while (1){  // ลูปเพื่อเริ่มเกม
        clock_t begin = clock();    //เริ้มนับเวลา
        do {            //ลูปเพื่อกรอกตำแหน่งหากไม่ถูกต้องหรือกรอกซ้ำจะไม่หลุดลูป
            Show();     //เรียกฟังฟังกชั่นแสดงตาราง
            
            printf("Player %d Choose a Number : ", Player);     //แสดงว่าถึงเรินของใคร
            scanf("%d", &Pos);      //รับตำแหน่งมาเก็บไว้ที่ตัวแปร Pos

            if (CheckIsEntered(Pos) == 1){      //เรียกฟังชั่นเช็คตาราง
                WhichItem = CheckScore(Player, Pos, BombNum, TreasureNum, MissNum);     //เช็คคะแนน
            }

        }while (Pos < 1 || Pos > 49 || CheckIsEntered(Pos) == 0);

        Entered[Pos - 1] = Pos;     //บันทึกตำแหน่งที่กรอกลงใน index ของ Entered
        
        if (TimeLeft < 0)           //ถ้าเวลาน้อยกว่า 0 ให้เวล่า = 0
            TimeLeft = 0;

        //ถ้าคะแนนใครมากกว่าหรือเจอระเบิดจะหยุดลูปเกม
        if (Score[0] >= 12 || Score[1] >= 12 || TimeLeft <= 0 || CheckWhatInPos(Pos) == 3)
            break;

        Player += 1;        //เทรินของ player +1
        if (Player > 2)     //ถ้ามากกว่า 2 ให่เป็น player 1
            Player = 1;

        clock_t end = clock();  //เวลาทีใช้
        TimeLeft -= end - begin;    //เวลาที่เหลือ เท่ากับ ลบด้วยเวลาที่เริ่มต้น
    }
    Show();     //เรียกฟังชั่นแสดงตาราง
    if (Score[0] > Score[1] && CheckWhatInPos(Pos) != 3)    //ถ้าคะแนน player 1 > player 2 ไม่เจอระเบิด
        printf("\033[1;33m===Player 1 WIN===\033[0m\n");    //แสดงข้อความ player 1 win
    else if (Score[1] > Score[0] && CheckWhatInPos(Pos) != 3)   //ถ้าคะแนน player 2 > player 1 ไม่เจอระเบิด
        printf("\033[1;33m===Player 2 WIN===\033[0m\n");    //แสดงข้อความ player 2 win
    else if (Score[1] == Score[0] && CheckWhatInPos(Pos) != 3)  //ถ้าคะแนนเท่ากันไม่เจอระเบิด
        printf("\033[1;33m===DRAW===\033[0m\n");        //แสดงข้อความเสมอ
    
    if (CheckWhatInPos(Pos) == 3){      //ถ้าเจอระเบิด
        if (Player == 1)        // player 1 เจอ
            printf("\033[1;33m===Player 2 WIN===\033[0m\n");    //แสดงข้อความ player 2 win
        else                    // player 2 เจอ
            printf("\033[1;33m===Player 1 WIN===\033[0m\n");    //แสดงข้อความ player 1 win
    }

    printf("Press any key to continue...");
    getchar();
    return 0;
}