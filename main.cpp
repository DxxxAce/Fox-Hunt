#include <iostream>
#include <cmath>
#include <graphics.h>
#include <winbgim.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>

using namespace std;
ifstream fin("english.txt");

#define MAX 9
#define BACKGROUND WHITE

int ChessBoard[MAX][MAX];

int left_edge, top_edge, width, height, edge, n, win;

char text[17][50];

struct position{
    int line, column;
};


void drawFox(int line, int column, int color)
{
    int x1,y1,x2,y2;
    setcolor(RED); setfillstyle(SOLID_FILL,RED);
    x1=left_edge+edge*(column-1); y1=top_edge+edge*(line-1);
    x2=x1+edge/2; y2=y1+edge/2;
    fillellipse(x2,y2,18,18);
    setcolor(color);
    setlinestyle(SOLID_LINE,BLACK,2);
    circle(x2,y2,18);
    setlinestyle(SOLID_LINE,BLACK,1);
}

void drawDog(int line, int column, int color)
{
    int x1,y1,x2,y2;
    setcolor(GREEN); setfillstyle(SOLID_FILL,GREEN);
    x1=left_edge+edge*(column-1); y1=top_edge+edge*(line-1);
    x2=x1+edge/2; y2=y1+edge/2;
    fillellipse(x2,y2,18,18);
    setcolor(color);
    setlinestyle(SOLID_LINE,BLACK,2);
    circle(x2,y2,18);
    setlinestyle(SOLID_LINE,BLACK,1);
}

void initPositions(position &F, position D[])
{
    //Initialising the fox's position:
    drawFox(1,4,BLACK); ChessBoard[1][4]=RED;
    F.line=1; F.column=4;

    //Initialising the dogs' positions:
    for (int i=1, k=0; i<=7; i+=2, k++)
    {
        drawDog(8,i,BLACK);
        ChessBoard[8][i]=GREEN;
        D[k].line=8; D[k].column=i;
    }
}

void drawBoard(position &F, position D[])
{
    int i,j; n=8;
    width=400; height=400; edge=width/n;
    top_edge=(getmaxy()-width)/2; left_edge=(getmaxx()-height)/2;
    setbkcolor(BACKGROUND); clearviewport(); setcolor(BLACK);

    //Drawing the chess board:
    for (i=1; i<=n; i++)
        for (j=1; j<=n; j++)
            if (i%2==j%2)
            {
                setfillstyle(SOLID_FILL,WHITE);
                bar(left_edge+edge*(i-1),top_edge+edge*(j-1),left_edge+edge*i,top_edge+edge*j);
                ChessBoard[i][j]=WHITE;
                rectangle(left_edge+edge*(i-1),top_edge+edge*(j-1),left_edge+edge*i,top_edge+edge*j);
            }
            else
            {
                setfillstyle(SOLID_FILL,DARKGRAY);
                bar(left_edge+edge*(i-1),top_edge+edge*(j-1),left_edge+edge*i,top_edge+edge*j);
                ChessBoard[i][j]=DARKGRAY;
                rectangle(left_edge+edge*(i-1),top_edge+edge*(j-1),left_edge+edge*i,top_edge+edge*j);
            }

    setlinestyle(SOLID_LINE,BLACK,5);

    rectangle(left_edge-5,top_edge-5,left_edge+edge*n+5,top_edge+edge*n+5);

    //Displays game title:
    settextstyle(BOLD_FONT, HORIZ_DIR, 5);
    outtextxy(getmaxx()/2-106,getmaxy()/15,text[0]);

    setlinestyle(SOLID_LINE,BLACK,1);   settextstyle(BOLD_FONT, HORIZ_DIR, 3);

    //Draws Restart button:
    rectangle(getmaxx()*5/6-100,getmaxy()*2/3+50,getmaxx()*5/6+100,getmaxy()*2/3+110);
    rectangle(getmaxx()*5/6-105,getmaxy()*2/3+45,getmaxx()*5/6+105,getmaxy()*2/3+115);
    outtextxy(getmaxx()*5/6-46,getmaxy()*2/3+70,text[4]);

    //Draws Main Menu button:
    rectangle(getmaxx()*5/6-100,getmaxy()*2/3+130,getmaxx()*5/6+100,getmaxy()*2/3+190);
    rectangle(getmaxx()*5/6-105,getmaxy()*2/3+125,getmaxx()*5/6+105,getmaxy()*2/3+195);
    outtextxy(getmaxx()*5/6-60,getmaxy()*2/3+150,text[5]);

    initPositions(F,D);
}

void allMoves(int l, int c, int color)
{
    setfillstyle(SOLID_FILL,color);     setcolor(BLACK);

    if (ChessBoard[l-1][c-1]==DARKGRAY)
    {
        bar(left_edge+edge*(c-2),top_edge+edge*(l-2),left_edge+edge*(c-1),top_edge+edge*(l-1));
        rectangle(left_edge+edge*(c-2),top_edge+edge*(l-2),left_edge+edge*(c-1),top_edge+edge*(l-1));
    }

    if (ChessBoard[l-1][c+1]==DARKGRAY)
    {
        bar(left_edge+edge*(c),top_edge+edge*(l-2),left_edge+edge*(c+1),top_edge+edge*(l-1));
        rectangle(left_edge+edge*(c),top_edge+edge*(l-2),left_edge+edge*(c+1),top_edge+edge*(l-1));
    }

    //Checking whether the piece can move backwards or not:
    if (ChessBoard[l][c]==RED)
    {
        if (ChessBoard[l+1][c-1]==DARKGRAY)
        {
            bar(left_edge+edge*(c-2),top_edge+edge*(l),left_edge+edge*(c-1),top_edge+edge*(l+1));
            rectangle(left_edge+edge*(c-2),top_edge+edge*(l),left_edge+edge*(c-1),top_edge+edge*(l+1));
        }

        if (ChessBoard[l+1][c+1]==DARKGRAY)
        {
            bar(left_edge+edge*(c),top_edge+edge*(l),left_edge+edge*(c+1),top_edge+edge*(l+1));
            rectangle(left_edge+edge*(c),top_edge+edge*(l),left_edge+edge*(c+1),top_edge+edge*(l+1));
        }
    }
}

void moveFox(position &F, bool &restart, bool &menu)
{
    int l1,c1,l2,c2,x,y;
    bool click=false;

    //Selecting the fox:
    do
        if (ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            click=true;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex(); y=mousey();

            if (x>getmaxx()*5/6-105 && x<getmaxx()*5/6+105)
            {
                if (y>getmaxy()*2/3+45 && y<getmaxy()*2/3+115)
                {
                    restart=true;
                    break;
                }
                if (y>getmaxy()*2/3+125 && y<getmaxy()*2/3+195)
                {
                    menu=true;
                    break;
                }
            }
            l1=(y-top_edge)/edge+1; c1=(x-left_edge)/edge+1;
            if(ChessBoard[l1][c1]!=RED) click=false;
        }
    while (!click);

    if (restart || menu) return;

    //Highlighting the fox and its possible new positions:
    drawFox(l1,c1,YELLOW);
    allMoves(l1,c1,YELLOW);
    click=false;

    //Choosing a new position for the fox:
    do
        if (ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            click=true;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex(); y=mousey();
            if (x>getmaxx()*5/6-105 && x<getmaxx()*5/6+105)
            {
                if (y>getmaxy()*2/3+45 && y<getmaxy()*2/3+115)
                {
                    restart=true;
                    break;
                }
                if (y>getmaxy()*2/3+125 && y<getmaxy()*2/3+195)
                {
                    menu=true;
                    break;
                }
            }

            l2=(y-top_edge)/edge+1; c2=(x-left_edge)/edge+1;
            if (ChessBoard[l2][c2]!=DARKGRAY) click=false;
            if (abs(l1-l2)!=1 || abs(c1-c2)!=1) click=false;
        }
    while (!click);

    if (restart || menu) return;

    //Moving the fox to new position:
    allMoves(l1,c1,DARKGRAY);
    ChessBoard[l2][c2]=RED;
    F.line=l2; F.column=c2;
    drawFox(l2,c2,BLACK);
    ChessBoard[l1][c1]=DARKGRAY;
    setfillstyle(SOLID_FILL,DARKGRAY);
    bar(left_edge+edge*(c1-1),top_edge+edge*(l1-1),left_edge+edge*c1,top_edge+edge*l1);
    setcolor(BLACK);
    rectangle(left_edge+edge*(c1-1),top_edge+edge*(l1-1),left_edge+edge*c1,top_edge+edge*l1);
}

void moveDog(position D[], bool &restart, bool &menu)
{
    int l1,c1,l2,c2,x,y,k;
    bool unselect, click=false;

    //Selecting the dog:
    do
    {
        unselect=false;

        do
            if (ismouseclick(WM_LBUTTONDOWN) && !click)
            {
                click=true;
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex(); y=mousey();
                if (x>getmaxx()*5/6-105 && x<getmaxx()*5/6+105)
            {
                if (y>getmaxy()*2/3+45 && y<getmaxy()*2/3+115)
                {
                    restart=true;
                    break;
                }
                if (y>getmaxy()*2/3+125 && y<getmaxy()*2/3+195)
                {
                    menu=true;
                    break;
                }
            }
                l1=(y-top_edge)/edge+1; c1=(x-left_edge)/edge+1;
                if(ChessBoard[l1][c1]!=GREEN) click=false;
            }
        while (!click);

        if (restart || menu) break;

        k=0;
        while (D[k].line!=l1 || D[k].column!=c1)
            k++;

        //Highlighting the dog and its possible new positions:
        drawDog(l1,c1,YELLOW);
        allMoves(l1,c1,YELLOW);
        click=false;

        //Choosing a new position for the dog:
        do
            if (ismouseclick(WM_LBUTTONDOWN) && !click)
            {
                click=true;
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex(); y=mousey();

                if (x>getmaxx()*5/6-105 && x<getmaxx()*5/6+105)
                {
                    if (y>getmaxy()*2/3+45 && y<getmaxy()*2/3+115)
                    {
                        restart=true;
                        break;
                    }
                    if (y>getmaxy()*2/3+125 && y<getmaxy()*2/3+195)
                    {
                        menu=true;
                        break;
                    }
                }

                l2=(y-top_edge)/edge+1; c2=(x-left_edge)/edge+1;
                if (ChessBoard[l2][c2]!=DARKGRAY) click=false;
                if (abs(l1-l2)>1 || abs(c1-c2)>1) click=false;
                if (l2>l1) click=false;

                //Checking if the dog was unselected:
                if (l1==l2 && c1==c2)
                {
                    unselect=true;
                    drawDog(l1,c1,BLACK);
                    allMoves(l1,c1,DARKGRAY);
                    break;
                }
            }
        while (!click);

        if (restart || menu) break;
    }
    while (unselect);

    if (restart || menu) return;

    //Moving the dog to new position:
    allMoves(l1,c1,DARKGRAY);
    ChessBoard[l2][c2]=GREEN;
    D[k].line=l2; D[k].column=c2;
    drawDog(l2,c2,BLACK);
    ChessBoard[l1][c1]=DARKGRAY;
    setfillstyle(SOLID_FILL,DARKGRAY);
    bar(left_edge+edge*(c1-1),top_edge+edge*(l1-1),left_edge+edge*c1,top_edge+edge*l1);
    setcolor(BLACK);
    rectangle(left_edge+edge*(c1-1),top_edge+edge*(l1-1),left_edge+edge*c1,top_edge+edge*l1);
}

void drawMove(position D[], int k, int l, int c)
{
    drawDog(D[k].line,D[k].column,YELLOW);
    allMoves(D[k].line,D[k].column,YELLOW);
    delay(1000);

    allMoves(D[k].line,D[k].column,DARKGRAY);
    ChessBoard[l][c]=GREEN;
    drawDog(l,c,BLACK);
    ChessBoard[D[k].line][D[k].column]=DARKGRAY;

    setfillstyle(SOLID_FILL,DARKGRAY);
    bar(left_edge+edge*(D[k].column-1),top_edge+edge*(D[k].line-1),left_edge+edge*D[k].column,top_edge+edge*D[k].line);
    setcolor(BLACK);
    rectangle(left_edge+edge*(D[k].column-1),top_edge+edge*(D[k].line-1),left_edge+edge*D[k].column,top_edge+edge*D[k].line);
}

void moveComputer(position F, position D[])
{
    int k,low,high,l,c;
    bool ok;

    //Search for the upmost and downmost dogs in the array:
    low=high=0;
    for (int i=1; i<4; i++)
    {
        if (D[i].line > D[low].line) low=i;
        if (D[i].line < D[high].line) high=i;
    }

    if (D[low].line > F.line+1)
    {
        do
        {
            ok=true;
            k=rand()%4;
            if (D[k].line%2 == 0 && ChessBoard[D[k].line-1][D[k].column+1] != DARKGRAY) ok=false;
            if (D[k].line%2 == 1 && ChessBoard[D[k].line-1][D[k].column-1] != DARKGRAY) ok=false;
        }
        while (D[k].line != D[low].line && !ok);

        l=D[k].line;
        if ((l--)%2 == 0) c=D[k].column+1;
        else c=D[k].column-1;
    }
    else
    {
        if (D[high].line > F.line)
        {
            if (D[low].line%2 == 1)
            {
                k=3;
                if (ChessBoard[D[k].line-1][D[k].column-1] != DARKGRAY)
                {
                    k=0;
                    c=D[k].column+1;
                }
                else c=D[k].column-1;
            }
            else
            {
            k=0;
                if (ChessBoard[D[k].line-1][D[k].column+1] != DARKGRAY)
                {
                    k=3;
                    c=D[k].column-1;
                }
            }
            l=D[k].line-1;
            if (ChessBoard[l][c] != DARKGRAY)
            {
                if (D[k].line%2 == 0) c-=2;
                else c+=2;
            }
        }
        else
        {
            k=2;
            if (D[k].column-2 == F.column)
            {
                l=D[k].line-1;
                c=D[k].column-1;
            }
            else
            {
                l=D[k].line-1;
                c=D[k].column+1;
            }
        }

    }

    drawMove(D,k,l,c);

    D[k].line=l; D[k].column=c;
}

bool winFox(position &F, position D[])
{
    //Fox reaches line 8:
    if (F.line==8)
    {
        outtextxy(getmaxx()/20,getmaxy()*7/8,text[10]);
        return 1;
    }

    //Fox gets past all dogs:
    for (int k=0; k<4; k++)
        if (D[k].line>F.line) return 0;

    outtextxy(getmaxx()/20,getmaxy()*7/8,text[11]);
    return 1;
}

bool winDogs(position &F)
{
    //Checking if the fox has any moves left:
    if (ChessBoard[F.line-1][F.column-1]==DARKGRAY) return 0;
    if (ChessBoard[F.line-1][F.column+1]==DARKGRAY) return 0;
    if (ChessBoard[F.line+1][F.column-1]==DARKGRAY) return 0;
    if (ChessBoard[F.line+1][F.column+1]==DARKGRAY) return 0;

    outtextxy(getmaxx()/20,getmaxy()*7/8,text[12]);
    return 1;
}

void PvC(bool &menu)
{
    position F,C[4];
    drawBoard(F,C); win=0;
    settextstyle(BOLD_FONT, HORIZ_DIR, 1);

    bool restart=false;

    //Play until win condition is satisfied:
    do
    {
        outtextxy(getmaxx()/20,getmaxy()*7/8,text[8]);
        moveFox(F,restart,menu);
        if (restart)
        {
            drawBoard(F,C);
            break;
        }
        if (menu) return;
        if (winFox(F,C))
        {
            outtextxy(getmaxx()/20,getmaxy()*7/8+20,text[13]);
            win=1;
        }
        else
        {
            outtextxy(getmaxx()/20,getmaxy()*7/8,text[9]);
            moveComputer(F,C);
            if (winDogs(F))
            {
                outtextxy(getmaxx()/20,getmaxy()*7/8+20,text[14]);
                win=2;
            }
            else
            {
                if (winFox(F,C))
                {
                    outtextxy(getmaxx()/20,getmaxy()*7/8+20,text[13]);
                    win=1;
                }
            }
        }
    }
    while (!win);

    //Checking if the Restart or Main Menu buttons are pressed:
    while (!restart)
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int x,y;
            x=mousex(); y=mousey();

            if (x>getmaxx()*5/6-105 && x<getmaxx()*5/6+105)
            {
                if (y>getmaxy()*2/3+45 && y<getmaxy()*2/3+115)
                {
                    drawBoard(F,C);
                    restart=true;
                }
                if (y>getmaxy()*2/3+125 && y<getmaxy()*2/3+195)
                {
                    menu=true;
                    return;
                }
            }
        }
}

void PvP(bool &menu)
{
    position F,D[4];
    drawBoard(F,D); win=0;
    settextstyle(BOLD_FONT, HORIZ_DIR, 1);

    bool restart=false;

    //Play until win condition is satisfied:
    do
    {
        outtextxy(getmaxx()/20,getmaxy()*7/8,text[6]);
        moveFox(F,restart,menu);
        if (restart)
        {
            drawBoard(F,D);
            break;
        }
        if (menu) return;
        if (winFox(F,D))
        {
            outtextxy(getmaxx()/20,getmaxy()*7/8+20,text[15]);
            win=1;
        }
        else
        {
            outtextxy(getmaxx()/20,getmaxy()*7/8,text[7]);
            moveDog(D,restart,menu);
            if (restart)
            {
                drawBoard(F,D);
                break;
            }
            if (menu) return;
            if (winDogs(F))
            {
                outtextxy(getmaxx()/20,getmaxy()*7/8+20,text[16]);
                win=2;
            }
            else
            {
                if (winFox(F,D))
                {
                    outtextxy(getmaxx()/20,getmaxy()*7/8+20,text[15]);
                    win=1;
                }
            }
        }
    }
    while (!win);

    //Checking if the Restart or Main Menu buttons are pressed:
    while (!restart)
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int x,y;
            x=mousex(); y=mousey();

            if (x>getmaxx()*5/6-105 && x<getmaxx()*5/6+105)
            {
                if (y>getmaxy()*2/3+45 && y<getmaxy()*2/3+115)
                {
                    drawBoard(F,D);
                    restart=true;
                }
                if (y>getmaxy()*2/3+125 && y<getmaxy()*2/3+195)
                {
                    menu=true;
                    return;
                }
            }
        }
}

void drawMenu()
{
    setcolor(BLACK); setbkcolor(BACKGROUND); clearviewport();
    readimagefile("bkimg.jpg",0,0,getmaxx(),getmaxy());

    //Display game title:
    settextstyle(BOLD_FONT, HORIZ_DIR, 5);
    outtextxy(getmaxx()/2-106,getmaxy()/15,text[0]);

    settextstyle(BOLD_FONT, HORIZ_DIR, 3);

    //Draw PvC button:
    rectangle(getmaxx()/2-135,getmaxy()*2/3-30,getmaxx()/2+135,getmaxy()*2/3+30);
    rectangle(getmaxx()/2-140,getmaxy()*2/3-35,getmaxx()/2+140,getmaxy()*2/3+35);
    outtextxy(getmaxx()/2-132,getmaxy()*2/3-10,text[1]);

    //Draw PvP button:
    rectangle(getmaxx()/2-135,getmaxy()*2/3+50,getmaxx()/2+135,getmaxy()*2/3+110);
    rectangle(getmaxx()/2-140,getmaxy()*2/3+45,getmaxx()/2+140,getmaxy()*2/3+115);
    outtextxy(getmaxx()/2-118,getmaxy()*2/3+70,text[2]);

    //Draw Exit button:
    rectangle(getmaxx()/2-135,getmaxy()*2/3+130,getmaxx()/2+135,getmaxy()*2/3+190);
    rectangle(getmaxx()/2-140,getmaxy()*2/3+125,getmaxx()/2+140,getmaxy()*2/3+195);
    outtextxy(getmaxx()/2-26,getmaxy()*2/3+150,text[3]);
}

void Game()
{
    drawMenu();
    int select=0;   int x,y;

    //Button select:
    do
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex(); y=mousey();

            if(x>getmaxx()/2-140 && x<getmaxx()/2+140)
            {
                if (y>getmaxy()*2/3-35 && y<getmaxy()*2/3+35)
                {
                    select=1;
                    break;
                }

                if (y>getmaxy()*2/3+45 && y<getmaxy()*2/3+115)
                {
                    select=2;
                    break;
                }

                if (y>getmaxy()*2/3+125 && y<getmaxy()*2/3+195)
                {
                    select=3;
                    break;
                }
            }
        }
    while (true);

    if (select == 1)
    {
        //Calling on the function for the Player vs. Player button:
        bool menu=false;
        while (!menu)
            PvC(menu);
        Game();
    }
    else
    {
        if (select == 2)
        {
            //Calling on the function for the Player vs. Player button:
            bool menu=false;
            while (!menu)
                PvP(menu);
            Game();
        }
        else
            if (select == 3)
            {
                //Calling on the function for the Exit button:
                closegraph();
            }
    }
}

int main()
{
    for (int i=0; i<17; i++)
        fin.getline(text[i],50);

    initwindow(1000,600);
    Game();
    return 0;
}
