#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <cctype> 
#include <cmath>
#define RESET   "\033[0m"
#define DARK_BG "\033[48;5;240m"
#define LIGHT_BG "\033[48;5;250m"


using namespace std;

char board[8][8]={
    {-4, -2, -3, -5, -6, -3, -2, -4},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 4,  2,  3,  5,  6,  3,  2,  4},

};
string getSymbol(int piece) {
    switch (piece) {
        case  1: return "♙";
        case  2: return "♘";
        case  3: return "♗";
        case  4: return "♖";
        case  5: return "♕";
        case  6: return "♔";
        case -1: return "♟";
        case -2: return "♞";
        case -3: return "♝";
        case -4: return "♜";
        case -5: return "♛";
        case -6: return "♚";
        default: return " ";
    }
}
int pawnTable[8][8] = {
    {  0,  0,  0,   0,   0,  0,  0,  0 },
    { 50, 50, 50,  50,  50, 50, 50, 50 },
    { 10, 10, 20,  30,  30, 20, 10, 10 },
    {  5,  5, 10,  25,  25, 10,  5,  5 },
    {  0,  0,  0,  20,  20,  0,  0,  0 },
    {  5, -5,-10,   0,   0,-10, -5,  5 },
    {  5, 10, 10, -20, -20, 10, 10,  5 },
    {  0,  0,  0,   0,   0,  0,  0,  0 }
};
int knightTable[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};
int bishopTable[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};
int rookTable[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0},
    {  5, 10, 10, 10, 10, 10, 10,  5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    {  0,  0,  0,  5,  5,  0,  0,  0}
};
int queenTable[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    {  0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};
int kingMidTable[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20}
};
int kingEndTable[8][8] = {
    {-50,-40,-30,-20,-20,-30,-40,-50},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-30,  0,  0,  0,  0,-30,-30},
    {-50,-30,-30,-30,-30,-30,-30,-50}
};
struct Move {
    int fromX, fromY, toX, toY;
};
int EvaluateBoard(char board[8][8]) {
    int score = 0;
    int pieceValues[7] = { 0, 100, 320, 330, 500, 900, 10000 };

    int whiteQueens = 0, blackQueens = 0;
    int whiteMaterial = 0, blackMaterial = 0;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int piece = board[y][x];
            int absPiece = abs(piece);

            if (absPiece == 0) continue;

            if (absPiece != 1 && absPiece != 6) {
                if (piece > 0)
                    whiteMaterial += pieceValues[absPiece];
                else
                    blackMaterial += pieceValues[absPiece];
            }

            if (absPiece == 5) {
                if (piece > 0) whiteQueens++;
                else blackQueens++;
            }
        }
    }

    bool isEndgame = (whiteQueens == 0 || blackQueens == 0) &&
                     (whiteMaterial <= 1300 && blackMaterial <= 1300);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int piece = board[y][x];
            int absPiece = abs(piece);
            if (absPiece == 0) continue;

            int value = pieceValues[absPiece];
            int bonus = 0;

            int mirroredY = (piece > 0) ? y : 7 - y;

            switch (absPiece) {
                case 1: bonus = pawnTable[mirroredY][x]; break;
                case 2: bonus = knightTable[mirroredY][x]; break;
                case 3: bonus = bishopTable[mirroredY][x]; break;
                case 4: bonus = rookTable[mirroredY][x]; break;
                case 5: bonus = queenTable[mirroredY][x]; break;
                case 6: bonus = isEndgame ? kingEndTable[mirroredY][x]
                                          : kingMidTable[mirroredY][x]; break;
            }

            if (piece > 0)
                score += value + bonus;
            else
                score -= value + bonus;
        }
    }

    return score;
}
bool ValidMove(int x, int y,int x1, int y1,char b[8][8])
{
    int piece = b[y][x];
    int piece1 =b[y1][x1];
    switch(piece)
    {
        case 1:
        {
            if(piece1==0)
            {
                if(x1==x&&y1==y-1)
                {
                    return false;
                }
                if(y == 6 && x1 == x && y1 == y - 2 && b[y - 1][x] == 0)
                {
                    return false;
                } 
                else
                {
                    return true;
                }

            }
            if (piece1<0)
            {
                if((x1==x+1||x1==x-1)&&(y1==y-1))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;
        }
        case -1:
        {
            if(piece1==0)
            {
                if(x1==x&&y1==y+1)
                {
                    return false;
                }
                if(y == 1 && x1 == x && y1 == y + 2 && b[y + 1][x] == 0)
                {
                    return false;
                } 
                else
                {
                    return true;
                }
            }
            if (piece1>0)
            {
                if((x1==x+1||x1==x-1)&&(y1==y+1))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;

        }
        case 2:
        {
            if(piece1==0||(piece1<0))
            {
                if (((x1==x+1||x1==x-1)&&(y1==y+2||y1==y-2))||((x1==x+2||x1==x-2)&&(y1==y+1||y1==y-1)))
                {
                    return false;
                }
                else
                {
                    return true;
                }

            }
            break;
        }
        case -2:
        {
            if (piece1==0||piece1>0)
            {
                if (((x1==x+1||x1==x-1)&&(y1==y+2||y1==y-2))||((x1==x+2||x1==x-2)&&(y1==y+1||y1==y-1)))
                {
                    return false;
                }
                else
                {
                    return true;
                }

            }
            break;
        }
        case 3:
        {
            if(piece1 == 0 || piece1 < 0) {
                if(abs(x1 - x) == abs(y1 - y)) {
                    int dx = (x1 > x) ? 1 : -1;
                    int dy = (y1 > y) ? 1 : -1;
                    int steps = abs(x1 - x) - 1;
                    
                    for(int s = 1; s <= steps; s++) {
                        if(b[y + s*dy][x + s*dx] != 0) {
                            return true;
                        }
                    }
                    return false;
                }
                return true;
            }
            break;
        }
        case -3:
        {
            if(piece1 == 0 || piece1 > 0) {
                if(abs(x1 - x) == abs(y1 - y)) {
                    int dx = (x1 > x) ? 1 : -1;
                    int dy = (y1 > y) ? 1 : -1;
                    int steps = abs(x1 - x) - 1;
                    
                    for(int s = 1; s <= steps; s++) {
                        if(b[y + s*dy][x + s*dx] != 0) {
                            return true;
                        }
                    }
                    return false;
                }
                return true;
            }
            break;
        }
        case 4:
        {
            if(piece1==0||piece1<0)
            {
                if (x1==x||y1==y)
                {
                    bool path=true;
                    if(x1==x)
                    {
                        int miny=min(y,y1);
                        int maxy=max(y,y1);
                        for(int i=miny+1;i<maxy;i++)
                        {
                            if(b[i][x]!=0)
                            {
                                path=false;
                                break;
                            }
                        }
                    }
                    else if(y1==y)
                    {
                        int minx=min(x,x1);
                        int maxx=max(x,x1);
                        for(int i=minx+1;i<maxx;i++)
                        {
                            if(b[y][i]!=0)
                            {
                                path=false;
                                break;
                            }
                        }
                    }
                    if(path==true)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
            }
            break;
        }
        case -4:
        {
            if(piece1==0||piece1>0)
            {
                if (x1==x||y1==y)
                {
                    bool path=true;
                    if(x1==x)
                    {
                        int miny=min(y,y1);
                        int maxy=max(y,y1);
                        for(int i=miny+1;i<maxy;i++)
                        {
                            if(b[i][x]!=0)
                            {
                                path=false;
                                break;
                            }
                        }
                    }
                    else if(y1==y)
                    {
                        int minx=min(x,x1);
                        int maxx=max(x,x1);
                        for(int i=minx+1;i<maxx;i++)
                        {
                            if(b[y][i]!=0)
                            {
                                path=false;
                                break;
                            }
                        }
                    }
                    if(path==true)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
            }
            break;
        }
        case 5:
        {
            if(piece1 == 0 || piece1 < 0) {
                
                if(abs(x1 - x) == abs(y1 - y)) {
                    int dx = (x1 > x) ? 1 : -1;
                    int dy = (y1 > y) ? 1 : -1;
                    int steps = abs(x1 - x) - 1;
                    
                    for(int s = 1; s <= steps; s++) {
                        if(b[y + s*dy][x + s*dx] != 0) {
                            return true;
                        }
                    }
                    return false; 
                }
                
                else if(x1 == x || y1 == y) {
                    if(x1 == x) {
                        int dy = (y1 > y) ? 1 : -1;
                        for(int i = y + dy; i != y1; i += dy) {
                            if(b[i][x] != 0) return true;
                        }
                    } 
                    else { 
                        int dx = (x1 > x) ? 1 : -1;
                        for(int i = x + dx; i != x1; i += dx) {
                            if(b[y][i] != 0) return true;
                        }
                    }
                    return false;
                }
                return true;
            }
            break;
        }
        case -5:
        {
            if(piece1 == 0 || piece1 > 0) {
                
                if(abs(x1 - x) == abs(y1 - y)) {
                    int dx = (x1 > x) ? 1 : -1;
                    int dy = (y1 > y) ? 1 : -1;
                    int steps = abs(x1 - x) - 1;
                    
                    for(int s = 1; s <= steps; s++) {
                        if(b[y + s*dy][x + s*dx] != 0) {
                            return true;
                        }
                    }
                    return false;
                }
                
                else if(x1 == x || y1 == y) {
                    if(x1 == x) {
                        int dy = (y1 > y) ? 1 : -1;
                        for(int i = y + dy; i != y1; i += dy) {
                            if(b[i][x] != 0) return true;
                        }
                    } 
                    else {
                        int dx = (x1 > x) ? 1 : -1;
                        for(int i = x + dx; i != x1; i += dx) {
                            if(b[y][i] != 0) return true;
                        }
                    }
                    return false; 
                }
                return true; 
            }
            break;
        }
        case 6:
        {
            if(piece1==0||piece1<0)
            {
                if(abs(x1-x)<=1&&abs(y1-y)<=1)
                {
                    return false;
                }
                else
                {
                    return true;
                }
                
            }
            else
            {
                return true;
            }
            break;
        }
        case -6:
        {
            if(piece1==0||piece1>0)
            {
                if(abs(x1-x)<=1&&abs(y1-y)<=1)
                {
                    return false;
                }
                else
                {
                    return true;
                }

            }
            else
            {
                return true;
            }
            break;
        }
        default:
        {
            return true;
            break;
        }
    }
    return true;
}
vector<Move> GenerateMoves(char board[8][8], bool isWhiteTurn) {
    vector<Move> moves;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char piece = board[y][x];
            if ((isWhiteTurn && piece <= 0) || (!isWhiteTurn && piece >= 0)) continue;

            for (int y1 = 0; y1 < 8; y1++) {
                for (int x1 = 0; x1 < 8; x1++) {
                    if (!ValidMove(x, y, x1, y1, board)) {
                        Move m = {x, y, x1, y1};
                        moves.push_back(m);
                    }
                }
            }
        }
    }

    return moves;
}
void MakeMove(char src[8][8], char dst[8][8], Move m) {
    memcpy(dst, src, 64);
    dst[m.toY][m.toX] = dst[m.fromY][m.fromX];
    dst[m.fromY][m.fromX] = 0;
}
int Minimax(char board[8][8], int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0) {
        return EvaluateBoard(board);
    }

    vector<Move> moves = GenerateMoves(board, isMaximizing);
    if (moves.empty()) return EvaluateBoard(board);

    int bestScore = isMaximizing ? -100000 : 100000;

    for (const Move& m : moves) {
        char newBoard[8][8];
        MakeMove(board, newBoard, m);
        int score = Minimax(newBoard, depth - 1, !isMaximizing, alpha, beta);

        if (isMaximizing) {
            bestScore = max(bestScore, score);
            alpha = max(alpha, score);
        } else {
            bestScore = min(bestScore, score);
            beta = min(beta, score);
        }

        if (beta <= alpha) break;
    }

    return bestScore;
}
Move FindBestMove(char board[8][8], bool isWhiteTurn, int depth) {
    vector<Move> moves = GenerateMoves(board, isWhiteTurn);
    int bestScore = isWhiteTurn ? -100000 : 100000;
    Move bestMove;

    for (const Move& m : moves) {
        char newBoard[8][8];
        MakeMove(board, newBoard, m);
        int score = Minimax(newBoard, depth - 1, !isWhiteTurn, -100000, 100000);

        if ((isWhiteTurn && score > bestScore) || (!isWhiteTurn && score < bestScore)) {
            bestScore = score;
            bestMove = m;
        }
    }

    return bestMove;
}
void AIMove(char board[8][8], bool& isWhiteTurn) {
    Move m = FindBestMove(board, isWhiteTurn, 3); // 3-ply depth
    cout << "AI moves from " << char('A'+m.fromX) << 8-(m.fromY) << " to " 
         << char('A'+m.toX) << 8-(m.toY) << endl;

    board[m.toY][m.toX] = board[m.fromY][m.fromX];
    board[m.fromY][m.fromX] = 0;

    isWhiteTurn = !isWhiteTurn;
}
void printBoard(char b[8][8]) {
    cout << endl;
    for (int i = 0; i < 8; ++i) {
        cout << 8 - i << " "; // row label

        for (int j = 0; j < 8; ++j) {
            bool isDark = (i + j) % 2;
            string bgColor = isDark ? DARK_BG : LIGHT_BG;

            string piece = getSymbol(static_cast<int>(b[i][j]));
            cout << bgColor << " " << piece << "  " << RESET;
        }
        cout << endl;
    }   
        // Column labels (make spaced too)
    cout << "  ";
    for (char col = 'a'; col <= 'h'; ++col) {
        cout << " " << col << "  ";
    }
    cout << endl;
}
bool checkBlack(char b[8][8])

{
    int x;
    int y;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b[i][j] == -6) {
                y=i;
                x=j;
                char letter= 'A' + x; 
            }
        }
    }
    for (int i= 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (b[i][j] > 0) { // looks for any white piece
                    int y1=i;
                    int x1=j;
                    char letter2 = 'A' + x1;
                    //here we will want to check if the white piece can attack the black king usinf a function that is not ready yet.
                    bool valid= ValidMove(x1, y1, x, y, b);
                    if (valid == false) {

                        return true; // Found a white piece that can attack the black king
                    }
                }
            }
        }

    return false; // No black pieces found
}
bool checkWhite(char b[8][8])
{
    int x;
    int y;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b[i][j] == 6) { // looks for the white king
                y=i;
                x=j;
                char letter = 'A' + x; 
            }
        }
    }
    for (int i= 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (b[i][j] < 0) { // looks for any black piece
                    int y1=i;
                    int x1=j;
                    char letter2 ='A'+x1;
                    //here we will want to check if the black piece can attack the white king usinf a function that is not ready yet.
                    bool valid= ValidMove(x1, y1, x, y, b);
                    if (valid == false) {

                        return true; // Found a black piece that can attack the white king
                    }
                }
            }
        }
    return false; // No black pieces found
}
bool RoquePEsquerda(char b[8][8])
{
    if(b[0][0]!=-4||b[0][4]!=-6)
    {
        cout << "perdeu o Roque esquerdaP." << endl;
        return false;
    }
    return true;
}
bool RoquePDireita(char b[8][8])
{
    if(b[0][7]!=-4||b[0][4]!=-6)
    {
        cout << "perdeu o Roque direitaP." << endl;
        return false;
    }
    return true;
}
bool RoqueBEsquerda(char b[8][8])
{
    if(b[7][0]!=4||b[7][4]!=6)
    {
        cout << "perdeu o Roque esquerdaB." << endl;
        return false;
    }
    return true;
}
bool RoqueBDireita(char b[8][8])
{
    if(b[7][7]!=4||b[7][4]!=6)
    {
        cout << "perdeu o Roque direitaB." << endl;
        return false;
    }
    return true;
}
bool realizarRoquePEsquerda(char b[8][8],bool certificar)
{
    if (!certificar)
    {
        return false;
    }
    else
    {
        if(b[0][1]==0&&b[0][2]==0&&b[0][3]==0)
        {
            for (int i=0;i<8;i++)
            {
                for (int j=0;j<8;j++)
                {
                    if(b[i][j]>0)
                    {
                        if(!ValidMove(j, i, 2, 0, b)||!ValidMove(j,i,3,0,b)||!ValidMove(j,i,1,0,b))
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
}
bool realizarRoquePDireita(char b[8][8],bool certificar)
{
    if (!certificar)
    {
        return false;
    }
    else
    {
        if(b[0][5]==0&&b[0][6]==0)
        {
            for (int i=0;i<8;i++)
            {
                for (int j=0;j<8;j++)
                {
                    if(b[i][j]>0)
                    {
                        if(!ValidMove(j, i, 5, 0, b)||!ValidMove(j,i,6,0,b)||!ValidMove(j,i,7,0,b))
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
}
bool realizarRoqueBEsquerda(char b[8][8],bool certificar) 
{
    if (!certificar)
    {
        return false;
    }
    else
    {
        if(b[7][1]==0&&b[7][2]==0&&b[7][3]==0)
        {
            for (int i=0;i<8;i++)
            {
                for (int j=0;j<8;j++)
                {
                    if(b[i][j]<0)
                    {
                        if(!ValidMove(j, i, 2, 7, b)||!ValidMove(j,i,3,7,b)||!ValidMove(j,i,1,7,b))
                        {
                            return false;
                        }
                        else 
                        {
                            return true;
                        }
                    }
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
}
bool realizarRoqueBDireita(char b[8][8],bool certificar)
{
    if (!certificar)
{
        return false;
    }
    else
    {
        if(b[7][5]==0&&b[7][6]==0)
        {
            for (int i=0;i<8;i++)
            {
                for (int j=0;j<8;j++)
                {
                    if(b[i][j]<0)
                    {
                        if(!ValidMove(j, i, 5, 7, b)||!ValidMove(j,i,6,7,b)||!ValidMove(j,i,7,7,b))
                        {
                            return false;
                        }
                        else 
                        {
                            return true;
                        }
                    }
                }
            }
            return true;    
        }
        else
        {
            return false;
        }
    }
}
bool isCheckmateWhite(char b[8][8]) {
    if (!checkWhite(b)) 
    {
        return false;
    }
    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            if (b[y1][x1] > 0) {
                for (int y2 = 0; y2 < 8; ++y2) {
                    for (int x2 = 0; x2 < 8; ++x2) {
                        if (ValidMove(x1, y1, x2, y2, b) == false) {
                            char temp = b[y2][x2];
                            b[y2][x2] = b[y1][x1];
                            b[y1][x1] = 0;

                            bool stillInCheck = checkWhite(b);

                            b[y1][x1] = b[y2][x2];
                            b[y2][x2] = temp;

                            if (!stillInCheck) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
    
}
bool isCheckmateBlack(char b[8][8]) {
    if (!checkBlack(b)) {
        return false;
    }
    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            if (b[y1][x1] < 0) {
                for (int y2 = 0; y2 < 8; ++y2) {
                    for (int x2 = 0; x2 < 8; ++x2) {
                        if (ValidMove(x1, y1, x2, y2, b) == false) {
                            char temp = b[y2][x2];
                            b[y2][x2] = b[y1][x1];
                            b[y1][x1] = 0;

                            bool stillInCheck = checkBlack(b);
                            b[y1][x1] = b[y2][x2];
                            b[y2][x2] = temp;

                            if (!stillInCheck) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }    return true;
}
string move(char b[8][8],bool rockWL, bool rockWR, bool rockBL, bool rockBR)
{
    bool ply=true;
    cout<<"THE MAN"<<endl;
    bool t=true;

    string loc;
    cout << "what move would you like to do?: "<< endl;
    cout << "Example: E2-E4" <<endl;

    while(ply)
    {

        while(t)
    {

        cout << "Your move: ";
        cin >>loc;
        loc[0]=toupper(loc[0]);
        loc[3]=toupper(loc[3]);
        if(loc.length() != 5) 
        {
            cout << "Invalid input. Please enter a move in the format 'E2-E4'." << endl;
            continue;
        }
        if(loc[2] != '-') 
        {
            cout << "Invalid input. Please enter a move in the format 'E2-E4'." << endl;
            continue;
        }
        if((loc[0] < 'A' || loc[0] > 'H')||(loc[3] < 'A' || loc[3] > 'H')) 
        {
            cout << "Invalid input. Please enter a move in the format 'E2-E4'." << endl;
            continue;
        }
        if((loc[1] < '1' || loc[1] > '8')||(loc[4] < '1' || loc[4] > '8')) 
        {
            cout << "Invalid input. Please enter a move in the format 'E2-E4'." << endl;
            continue;
        }
        t=false;
    }
        int x= (loc[0] - 'A');
        int y= 8-(loc[1] - '0');
        int x1= (loc[3] - 'A');
        int y1= 8-(loc[4] - '0');
        if(realizarRoquePEsquerda(b,rockBL))
        {
            cout<<"Roque PE"<<endl;
            if((x==4&&y==0&&x1==2&&y1==0))
            {
                b[0][3]=b[0][0];
                b[0][0]=0;
                return loc;
            }
        }
        if(realizarRoquePDireita(b,rockBR))
        {
            cout<<"Roque PD"<<endl;
            if((x==4&&y==0&&x1==6&&y1==0))
            {
                b[0][5]=b[0][7];
                b[0][7]=0;
                return loc;
            }
        }
        if(realizarRoqueBEsquerda(b,rockWL))
        {
            cout<<"Roque BE"<<endl;
            if((x==4&&y==7&&x1==2&&y1==7))
            {
                b[7][3]=b[7][0];
                b[7][0]=0;
                return loc;
            }
                
        }
        if(realizarRoqueBDireita(b,rockWR))
        {
            cout<<"Roque BD"<<endl;
            if((x==4&&y==7&&x1==6&&y1==7))
            {
                b[7][5]=b[7][7];
                b[7][7]=0;
                return loc;
            }            
        }
    
        if(b[y1][x1]!=6&&b[y1][x1]!=-6)
        {
          t=ValidMove(x, y,x1,y1,b);
        }
        else
        {
            cout<<"Ganhar o jogo não é tão fácil!"<<endl;
            t=true;
        }


        if (t==true)
        {               
             continue;
        }
        else{
            ply=false;
            continue;
        }
    }
        return loc;

}       
void where(char x,char y,char x1, char y1,char b[8][8],bool& turn)
{

    int col1= (x - 'A');
    int line1= 8-(y - '0');
    int col2= (x1 - 'A');
    int line2= 8-(y1 - '0');
        if(!turn&&b[line1][col1]>0)
    {
        cout << "It's not your turn!" << endl;
        return;
    }
    if(turn&&b[line1][col1]<0)
    {
        cout << "It's not your turn!" << endl;
        return;
    }
    
    if((b[line1][col1]==1||b[line1][col1]==-1)&&(line2==7||line2==0))
    {
        cout << "Pawn promotion!" << endl;
        cout << "Choose a piece to promote to (2: Knight, 3: Bishop, 4: Rook, 5: Queen): ";
        int promotionChoice;
        cin >> promotionChoice;
        bool promo=true;
        while(promo)
        {
            switch(promotionChoice)
        {
            case 2:
            {
                b[line1][col1] = 2*b[line1][col1]; // Knight
                promo=false;
                break;
            }
            case 3:
            {
                b[line1][col1] = 3*b[line1][col1];; // Bishop
                promo=false;
                break;
            }
            case 4:
            {
                b[line1][col1] = 4*b[line1][col1];; // Rook
                promo=false;
                break;
            }
            case 5:
            {
                b[line1][col1] = 5*b[line1][col1];; // Queen
                promo=false;
                break;
            }
            default:
            {
                cout << "Invalid choice. Pawn remains unchanged." << endl;
                break;
            }
        }
    }
}
        
    b[line2][col2]=b[line1][col1];
    b[line1][col1] = 0;
    cout << "Moving piece from " << x << y << " to " << x1 << y1 << endl;
        turn = !turn;
}

int main() 
{   
    bool turn=true;
    bool rockBL=true;
    bool rockBR=true;
    bool rockWL=true;
    bool rockWR=true;
    bool ply=true;
    printBoard(board);
    while(ply)
    {
        if(turn)
        {
            cout<<"your turn"<<endl;
            string l=move(board,rockWL,rockWR,rockBL,rockBR);
            where (l[0],l[1],l[3],l[4], board,turn);
            printBoard(board);
        if(rockBL)
        {
            rockBL=RoquePEsquerda(board);
        }
        if(rockBR)
        {
            rockBR=RoquePDireita(board);
        }
        if(rockWL)
        {
            rockWL=RoqueBEsquerda(board);
        }
        if(rockWR)
        {
            rockWR=RoqueBDireita(board);
        }
        if(checkWhite(board))
        {
            cout << "CHECK!" << endl;
            if(isCheckmateWhite(board))
            {
                cout << "Black wins!" << endl;
                ply=!ply;
                break;
            }
        }
        if(checkBlack(board))
        {
            cout << "CHECK!" << endl;
            if(isCheckmateBlack(board))
            {
                cout << "White wins!" << endl;
                ply=!ply;
                break;
            }
        }
    }
        if(!turn)
        {
            cout<<"AI turn"<<endl;
            AIMove(board, turn);
            printBoard(board);
            if(rockBL)
            {
                rockBL=RoquePEsquerda(board);
            }
            if(rockBR)
            {
                rockBR=RoquePDireita(board);
            }
            if(rockWL)
            {
                rockWL=RoqueBEsquerda(board);
            }
            if(rockWR)
            {
                rockWR=RoqueBDireita(board);
            }
            if(checkWhite(board))
            {
                cout << "CHECK!" << endl;
                if(isCheckmateWhite(board))
                {
                    cout << "Black wins!" << endl;
                    ply=!ply;
                    break;
                }
            }
            if(checkBlack(board))
            {
                cout << "CHECK!" << endl;
                if(isCheckmateBlack(board))
                {
                    cout << "White wins!" << endl;
                    ply=!ply;
                    break;
                }
            }
        }
    }
}
