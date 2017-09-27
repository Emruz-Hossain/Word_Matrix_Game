#include<GL/freeglut.h>
#include<GL/glut.h>

#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;
#define GRID_WIDTH 25
#define GRID_HEIGHT 25

#define blocked glColor4f(.53,.08,.32,0.1)
#define selected glColor4f(.53,.08,.32,0.3)
#define writing glColor4f(.22,.28,.82,0.3)
#define black glColor3f(0, 0, 0)
#define keyboardColor glColor4f(.22,.28,.82,.01)

vector<string> dictionary;

char Alphabet[][4] = {
    {'A', 'B', 'C', 'D'},
    {'E', 'F', 'G', 'H'},
    {'I', 'J', 'K', 'L'},
    {'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T'},
    {'U', 'V', 'W', 'X'},
    {'Y', 'Z', '*', '#'}};

char keyboardMap[1205][605];
char BoardMatrix[GRID_WIDTH][GRID_HEIGHT];
int cellWidth = 42;
int cellHeight = 30;
struct point{
    int x, y;
};
point selectedCell;
char selectedCharacter;

vector<point> chosenCell;

int playerOneScore = 0, playerTwoScore = 0;
int optionForSelection = 0;
int optionForWriting = 0;

struct sp{
    int X, Y;
} MapToBoardMatrix[1201][601];
char GameText[GRID_WIDTH][GRID_HEIGHT];
int cntX[GRID_WIDTH], cntY[GRID_HEIGHT];
using namespace std;

void drawBitmapText(string text, int x,int y)
{
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    for (int i = 0; i < text.size();i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
}
int leftToRight = 0, topToBottom = 0,moveId=0;
bool validSelection(int cx,int cy)
{
    if (chosenCell.size() == 0)
        return true;
    else if(leftToRight||topToBottom)
    {
        point aa;
        aa = chosenCell.back();
        if(leftToRight==1)
        {
            if(aa.x+1==cx&&aa.y==cy)
            {
                leftToRight = 1;
                topToBottom = 0;
                return true;
            }
            else
                return false;
        }
        else
        {
            if(aa.x==cx&&aa.y-1==cy)
            {
                leftToRight = 0;
                topToBottom = 1;
                return true;
            }
            else
                return false;
        }
    }
    else if(chosenCell.size()==1)
    {
        point aa;
        aa = chosenCell.back();
        if(aa.x==cx&&aa.y-1==cy)
        {
            topToBottom = 1;
            leftToRight = 0;
            return true;
        }
        else if(aa.x+1==cx&&aa.y==cy)
        {
            leftToRight = 1;
            topToBottom = 0;
            return true;
        }
        else
            return false;
    }
    else 
    {
        return false;
    }
}
void showSelected(void)
{
    //-----clear existing text-------
    glColor3f(1, 1, 1);
    glBegin(GL_POINTS);
    for (int i = 900; i <= 1150;i++)
    {
        for (int j = 395; j <= 420;j++)
        {
            glVertex2f(i, j);
        }
    }
    glEnd();
    glFlush();
    //----------set new text-----------
    glColor3f(0, 0, 0);
    string ss;
    for (int i = 0; i < chosenCell.size();i++)
    {
        ss += GameText[chosenCell[i].x][chosenCell[i].y];
    }
    drawBitmapText(ss, 900, 400);
    glFlush();
}
void showMove(void)
{
    //-----clear existing text-------
    glColor3f(1, 1, 1);
    glBegin(GL_POINTS);
    for (int i = 900; i <= 1150;i++)
    {
        for (int j = 395; j <= 420;j++)
        {
            glVertex2f(i, j);
        }
    }
    glEnd();
    glFlush();
    //----------set new text-----------
    glColor3f(0, 0, 0);
    string ss="Player "+to_string(moveId+1)+"'s Move";
    drawBitmapText(ss, 900, 400);
    glFlush();
}
void showText(string ss)
{
    //-----clear existing text-------
    glColor3f(1, 1, 1);
    glBegin(GL_POINTS);
    for (int i = 900; i <= 1150;i++)
    {
        for (int j = 395; j <= 420;j++)
        {
            glVertex2f(i, j);
        }
    }
    glEnd();
    glFlush();
    //----------set new text-----------
    glColor3f(0, 0, 0);
    drawBitmapText(ss, 900, 400);
    glFlush();
}
void blockCell()
{
    for (int k = 0; k < chosenCell.size();k++)
    {
            // printf("failed in = %d %d\n", cntX[k], cntY[l]);

            int x = cntX[chosenCell[k].x];
            int y = cntY[chosenCell[k].y];
            //---------clear color to original------
            glColor3f(1, 1, 1);
            glBegin(GL_POINTS);
            for (int i = x + 1; i < x + cellWidth; i++)
            {
                for (int j = y + 1; j < y + cellHeight;j++)
                {
                    glVertex2f(i,j);
                }
            }
            glEnd();
            glFlush();
          //------------------set new color----------------
            glColor4f(.13, .08, .32, 0.3);
            glBegin(GL_POINTS);
            for (int i = x + 1; i < x + cellWidth; i++)
            {
                for (int j = y + 1; j < y + cellHeight;j++)
                {
                    glVertex2f(i,j);
                }
            }
            glEnd();
            glFlush();
            //--------------set text------------------
            string ss;
      
            ss+= GameText[chosenCell[k].x][chosenCell[k].y];
            drawBitmapText(ss, x+cellWidth/2-10, y+5);
    }
}

void selectCell(int cx,int cy)
{
    if(validSelection(cx,cy)==true)
    {
        int x = cntX[cx];
        int y = cntY[cy];
        selected;
        glBegin(GL_POINTS);
        for (int i = x + 1; i < x + cellWidth; i++)
        {
            for (int j = y + 1; j < y + cellHeight;j++)
            {
                glVertex2f(i,j);
            }
        }
        glEnd();
        glFlush();
        point aa;
        aa.x=cx;
        aa.y = cy;
        chosenCell.push_back(aa);
        showSelected();
    }
}
void selectWritingCell(int x,int y)
{

    //------add a background to indicate writing on this cell-----------
    writing;
    glBegin(GL_POINTS);
    for (int i = x + 1; i < x + cellWidth; i++)
    {
        for (int j = y + 1; j < y + cellHeight;j++)
        {
            glVertex2f(i,j);
        }
    }
    glEnd();
    glFlush();
}
void writeOnCell(void)
{
      //------set text--------
    black;
    string ss = "";
    ss += selectedCharacter;
    if(selectedCell.x!=-1&&GameText[selectedCell.x][selectedCell.y]=='*')
    {
        drawBitmapText(ss, cntX[selectedCell.x] + cellWidth / 2 - 10, cntY[selectedCell.y] + 5);
        GameText[selectedCell.x][selectedCell.y] = selectedCharacter;
        optionForSelection = 1;
        optionForWriting = 0;
    }
    glFlush();
}
bool searchInDatabase(string sp)
{
    string ss="";
    for (int i = 0; i < sp.size();i++)
    {
        if(sp[i]>='A'&&sp[i]<='Z')
            ss += sp[i] - ('Z' - 'z');
    }
    int l = 0, h = dictionary.size() - 1, m;
    while(l<=h)
    {
        m = (l + h) / 2;
        if(dictionary[m]==ss)
            return true;
        else if(dictionary[m]<ss)
            l = m + 1;
        else
            h = m - 1;
    }
    return false;
}
bool existInDatabase(void)
{
    string ss = "";
    for (int i = 0; i < chosenCell.size();i++)
        ss += GameText[chosenCell[i].x][chosenCell[i].y];
    if(searchInDatabase(ss)==true)
        return true;
    else
        return false;
}
void setPlayerOneScore(void)
{
    //---------clear previous score----------
    glColor3f(1, 1, 1);
    glBegin(GL_POINTS);
    for (int i = 990; i <= 1030; i++)
    {
        for (int j = 535; j <= 560; j++)
        {
            glVertex2f(i, j);
        }
    }
    glEnd();

    //-------------set new score----------------
    glColor3f(0, 0, 0);
    string score = to_string(playerOneScore);
    drawBitmapText(score, 990, 535);
    glFlush();
 }
 void setPlayerTwoScore(void)
 {
    //---------clear previous score----------
     glColor3f(1, 1, 1);
     glBegin(GL_POINTS);
     for (int i = 1140; i <= 1170; i++)
     {
         for (int j = 535; j <= 560;j++)
         {
             glVertex2f(i, j);
         }
     }
     glEnd();

     //-------------set new score----------------
     glColor3f(0, 0, 0);
     string score = to_string(playerTwoScore);
     drawBitmapText(score, 1140, 535);
     glFlush();
 }
bool isSelectedNewCell(void)
{
    int flag = 0;
    for (int i = 0; i < chosenCell.size();i++)
    {
        if(chosenCell[i].x==selectedCell.x&&chosenCell[i].y==selectedCell.y)
            flag = 1;
    }
    return flag;
}
void Verify(void)
{
    blockCell();
    if(existInDatabase()==true&&isSelectedNewCell()==true)
    {
        if(moveId==0)
        {
            playerOneScore += chosenCell.size();
            setPlayerOneScore();
        }
        else
        {
            playerTwoScore += chosenCell.size();
            setPlayerTwoScore();
        }
        moveId^=1;
        showMove();
    }
    else
    {
        showText("Word Not Found");
        sleep(2);
        moveId^=1;
        showMove();
    }
    chosenCell.clear();
    topToBottom = 0;
    leftToRight = 0;
    optionForWriting = 1;
}
void MouseListener(int button,int state,int x,int y)
{
    int cx = MapToBoardMatrix[x][y].X;
    int cy = MapToBoardMatrix[x][y].Y;
    // printf("%d %d\n",x,600-y);
    if(x>=890&&x<=1170&&600-y>=50&&600-y<=360)
    {
       selectedCharacter=keyboardMap[x][600-y];
       writeOnCell();
    }
    else if (cntX[cx] != -1 && cntY[cy] != -1)
    {

        if (button == GLUT_LEFT_BUTTON&&state==GLUT_UP)
        {
            if(optionForSelection==1&&GameText[cx][cy]!='*')
            {
                selectCell(cx,cy);
            }
        }
        if(button==GLUT_RIGHT_BUTTON&&state==GLUT_UP)
        {
            selectedCell.x = cx;
            selectedCell.y = cy;
            if(GameText[cx][cy]=='*'&&optionForWriting==1)
            {
                selectWritingCell(cntX[cx], cntY[cy]);
            }
            else
            {
                selectedCell.x = -1;
                selectedCell.y = -1;
            }
        }
    }
    else
    {
       if(x>=900&&x<=1150&&y>=120&&y<=160&&state==GLUT_UP)
       {
           Verify();
       }
    }
}
void drawSidePanel(void)
{
    glColor3f(0, 0, 0);
    //--------------Player 1 info-------------------
    drawBitmapText("Player 1:", 890, 535);
    setPlayerOneScore();

    //-------------Player 2 info--------------------
    drawBitmapText("Player 2:", 1040, 535); //Player 2: text
    setPlayerTwoScore();

    //-------------Verify Button--------------------
    drawBitmapText("Verify", 990, 450);

    glFlush();
}
void drawKeyboardLayout(void)
{
    int rowY, initX, keyCellSizeH,keyCellSizeV, i, j, k, l, gap, x, y, rowSize;
    initX = 900;
    rowY = 320;
    keyCellSizeH = 60;
    keyCellSizeV = 35;
    gap = 10;
    for (i = 0; i < 7; i++)
    {
        x = initX;
        y = rowY;
        rowY -= keyCellSizeV + gap;
        rowSize = 4;
        if(i==6)
            rowSize = 2;
        for (j = 0; j < rowSize;j++)
        {
            glColor4f(.53,.08,.32,0.3);
            glBegin(GL_POINTS);
            for (k = x; k <= x + keyCellSizeH;k++)
            {
                for (l = y; l <= y + keyCellSizeV;l++)
                {
                    glVertex2f(k, l);
                    keyboardMap[k][l] = Alphabet[i][j];
                }
            }
            glEnd();
            glFlush();
            string ss = "";
            ss += Alphabet[i][j];
            drawBitmapText(ss, x + 20, y + 10);
            glFlush();
            x += keyCellSizeH + gap;
        }
    }
}
void DrawBoard(void)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glOrtho(0, 1200, 0, 600, 0, 1);
    //-------------------------Draw Vertical Diveder Line on the left -----------
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2f(850, 0);
    glVertex2f(850, 600);
    glEnd();

    glColor4f(.53, .08, .32, 0.3);
    //-----------------------Draw Rectangle for Player Score -----------------
    glBegin(GL_LINE_LOOP);
    glVertex2f(870, 580);
    glVertex2f(1180, 580);
    glVertex2f(1180, 500);
    glVertex2f(870, 500);
    glEnd();

     //-----------------------Draw Rectangle for Verification -----------------
    glBegin(GL_POLYGON);
    glVertex2f(900, 480);
    glVertex2f(1150, 480);
    glVertex2f(1150, 440);
    glVertex2f(900, 440);
    glEnd();
     //-----------------------Draw Rectangle for  details -----------------
    glBegin(GL_LINE_LOOP);
    glVertex2f(870, 430);
    glVertex2f(1180, 430);
    glVertex2f(1180, 380);
    glVertex2f(870, 380);
    glEnd();

    //----------------------Draw Grid for Game Text---------------------------
    int x = 0,y=0;
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0);
    int cnt = 0;
    for (int i = 0; i < 1200;i++)
    {
        for (int j = 0; j < 600;j++)
        {
            MapToBoardMatrix[i][j].X =MapToBoardMatrix[i][j].Y= 24;
        }
    }
        while (x + cellWidth < 850)
        {
            glBegin(GL_LINES);
            glVertex2f(x, 0);
            glVertex2f(x, 600);
            glEnd();
            if (cnt < 19)
                cntX[cnt] = x;
            for (int i = x; i < x + cellWidth; i++)
            {
                for (int j = 0; j <= 600; j++)
                {
                    MapToBoardMatrix[i][j].X = cnt;
                }
            }
            x += cellWidth;
            cnt++;
        }
    cnt = 0;
    while (y < 600)
    {
        glBegin(GL_LINES);
        glVertex2f(0, y);
        glVertex2f(800, y);
        glEnd();
        cntY[cnt] = y;
        for (int i = y; i < y + cellHeight; i++)
        {
            for (int j = 0; j <= 850;j++)
            {
                MapToBoardMatrix[j][i].Y = 19-cnt;
            }
        }
        y += cellHeight;
        cnt++;
    }
    //---------border around the grid-------------
    glColor4f(0,0,0,0.5);
    glLineWidth(5.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(2, 3);
    glVertex2f(800, 3);
    glVertex2f(800, 597);
    glVertex2f(2, 597);
    glEnd();

    glFlush();
    drawSidePanel();
    drawKeyboardLayout();
    showMove();
    optionForWriting = 1;
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            GameText[i][j] = '*';
        }
        }
}
int main(int argc, char **argv)
{
    //----------------Creating dictionary------
    freopen("words_alpha.txt","r",stdin);
    string ss;
    while (cin>>ss)
    {
        dictionary.push_back(ss);
    }
    sort(dictionary.begin(), dictionary.end());

    glutInit(&argc,argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1200, 600);
    glutCreateWindow("Word Matrix");
    memset(cntX, -1, sizeof(cntX));
    memset(cntY,-1,sizeof(cntY));
    glutDisplayFunc(DrawBoard);
    glutMouseFunc(MouseListener);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glutMainLoop();
}
