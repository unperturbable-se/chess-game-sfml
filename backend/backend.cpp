#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include "../crossplatform.h"



using namespace std;
class Board;
void refresh();

void renderWindow()
{
    ofstream("render").close();
}

void closeWindow()
{
    filesystem::remove("render");
}

class Piece{
protected:
    string color;
    bool hasMoved;
public:
    Piece(string color) : color(color), hasMoved(false){}
    virtual ~Piece() = default;
    virtual vector<pair<int, int>> getValidMoves(Board& board, int row, int col) = 0;

    string getColor() const{ return color; }
    void setMoved(){ hasMoved = true; }
    bool getMoved() const{ return hasMoved; }
    virtual string toString() const = 0;
};

class Pawn: public Piece{
public:

    Pawn(string color): Piece(color){}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override{ return color[0] + string("P"); }
};

class Rook: public Piece{
public:

    Rook(string color): Piece(color){}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override{ return color[0] + string("R"); }
};

class Knight: public Piece{
public:

    Knight(string color): Piece(color){}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override{ return color[0] + string("N"); }
};

class Bishop: public Piece{
public:

    Bishop(string color) : Piece(color){}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override{ return color[0] + string("B"); }
};

class Queen : public Piece{
public:
    Queen(string color) : Piece(color){}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override{ return color[0] + string("Q"); }
};

class King: public Piece{
public:
    King(string color) : Piece(color){}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override{ return color[0] + string("K"); }
};

class Board{
private:
    shared_ptr<Piece> board[8][8];
public:

    Board(){ setup(); }

    void setup(){
        for (int i = 0; i < 8; ++i){
            board[1][i] = make_shared<Pawn>("black");
            board[6][i] = make_shared<Pawn>("white");
        }
        board[0][0] = make_shared<Rook>("black");
        board[0][7] = make_shared<Rook>("black");
        board[7][0] = make_shared<Rook>("white");
        board[7][7] = make_shared<Rook>("white");
        board[0][1] = make_shared<Knight>("black");
        board[0][6] = make_shared<Knight>("black");
        board[7][1] = make_shared<Knight>("white");
        board[7][6] = make_shared<Knight>("white");
        board[0][2] = make_shared<Bishop>("black");
        board[0][5] = make_shared<Bishop>("black");
        board[7][2] = make_shared<Bishop>("white");
        board[7][5] = make_shared<Bishop>("white");
        board[0][3] = make_shared<Queen>("black");
        board[7][3] = make_shared<Queen>("white");
        board[0][4] = make_shared<King>("black");
        board[7][4] = make_shared<King>("white");
    }

    bool isValid(int row, int col) const{
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool isEmpty(int row, int col) const{
        return isValid(row, col) && !board[row][col];
    }

    bool isEnemy(int row, int col, string color) const{
        return isValid(row, col) && board[row][col] && board[row][col]->getColor() != color;
    }

    shared_ptr<Piece> getPiece(int row, int col) const{
        if (isValid(row, col)) return board[row][col];
        return nullptr;
    }

    bool movePiece(int sr, int sc, int er, int ec){
        auto piece = getPiece(sr, sc);
        if (piece){
            auto validMoves = piece->getValidMoves(*this, sr, sc);
            for (auto& move : validMoves){
                if (move.first == er && move.second == ec){
                    board[er][ec] = board[sr][sc];
                    board[sr][sc].reset();
                    board[er][ec]->setMoved();
                    return true;
                }
            }
        }
        return false;
    }

    void print() const{
        closeWindow();
        ofstream outfile("data.txt", ios::out | ios::trunc);
        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c){
                if (board[r][c]) outfile << board[r][c]->toString() << " ";
                else outfile << ".. ";
            }
            outfile<< endl;
        }
        outfile.close();
        renderWindow();
    }

    vector<pair<int, int>> getLinearMoves(int row, int col, string color, vector<pair<int, int>> directions){
        vector<pair<int, int>> moves;
        for (auto dir : directions){
            int dr = dir.first, dc = dir.second;
            int r = row + dr, c = col + dc;
            while (isValid(r, c)){
                if (isEmpty(r, c)){
                    moves.push_back({r, c});
                } else if (isEnemy(r, c, color)){
                    moves.push_back({r, c});
                    break;
                } else break;
                r += dr;
                c += dc;
            }
        }
        return moves;
    }
};

// Piece move implementations (same as yours)
vector<pair<int, int>> Pawn::getValidMoves(Board& board, int row, int col){
    vector<pair<int, int>> moves;
    int dir = (color == "white") ? -1 : 1;
    int startRow = (color == "white") ? 6 : 1;
    if (board.isEmpty(row + dir, col)){
        moves.push_back({row + dir, col});
        if (row == startRow && board.isEmpty(row + 2 * dir, col))
            moves.push_back({row + 2 * dir, col});
    }
    for (int dc = -1; dc <= 1; dc += 2){
        if (board.isEnemy(row + dir, col + dc, color))
            moves.push_back({row + dir, col + dc});
    }
    return moves;
}

vector<pair<int, int>> Rook::getValidMoves(Board& board, int row, int col){
    return board.getLinearMoves(row, col, color, {{1,0}, {-1,0}, {0,1}, {0,-1}});
}

vector<pair<int, int>> Bishop::getValidMoves(Board& board, int row, int col){
    return board.getLinearMoves(row, col, color, {{1,1}, {1,-1}, {-1,1}, {-1,-1}});
}

vector<pair<int, int>> Queen::getValidMoves(Board& board, int row, int col){
    return board.getLinearMoves(row, col, color, {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}});
}

vector<pair<int, int>> Knight::getValidMoves(Board& board, int row, int col){
    vector<pair<int, int>> moves;
    int drc[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    for (int i = 0; i < 8; ++i) {
        int r = row + drc[i][0], c = col + drc[i][1];
        if (board.isValid(r, c) && (board.isEmpty(r, c) || board.isEnemy(r, c, color)))
            moves.push_back({r, c});
    }

    return moves;
}

vector<pair<int, int>> King::getValidMoves(Board& board, int row, int col){
    vector<pair<int, int>> moves;
    int drc[8][2] = {{-1,0}, {1,0}, {0,1}, {0,-1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    for (int i = 0; i < 8; ++i){
        int r = row + drc[i][0], c = col + drc[i][1];
        if (board.isValid(r, c) && (board.isEmpty(r, c) || board.isEnemy(r, c, color)))
            moves.push_back({r, c});
    }

    return moves;
}

class Game{
private:
    Board board;
    string currentTurn;

    pair<int, int> parsePosition(string pos){
        if (pos.length() != 2) return {-1, -1};
        char file = tolower(pos[0]);
        char rank = pos[1];
        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') return {-1, -1};
        int col = file - 'a';
        int row = 8 - (rank - '0');
        return {row, col};
    }

public:

    Game() : currentTurn("white") {
        
    }

    ~Game() 
    {

    }

    void switchTurn(){
        currentTurn = (currentTurn == "white") ? "black" : "white";
    }

    void play(){
        while (filesystem::exists("run")){
            board.print();
            cout << currentTurn << "'s move (format: e2 e4 or type 'exit'): ";
            string startPos, endPos;
            
            while(startPos.empty()&&filesystem::exists("run")){refresh();cin>>startPos;}
            while(endPos.empty()&&filesystem::exists("run")){refresh();cin>>endPos;}
            

            cout<<"Moves:"<<startPos<<' '<<endPos<<endl;
            auto [sr, sc] = parsePosition(startPos);
            auto [er, ec] = parsePosition(endPos);

            if (sr == -1 || sc == -1 || er == -1 || ec == -1){
                cout << "Invalid input format!\n";
                continue;
            }

            auto piece = board.getPiece(sr, sc);
            if (piece && piece->getColor() == currentTurn){
                if (board.movePiece(sr, sc, er, ec)){
                    cout << "Move successful!\n";
                    switchTurn();
                }
                else{
                    cout << "Invalid move.\n";
                }
            }
            else{
                cout << "Invalid piece selected.\n";
            }
        }
    }
};

int main()
{
    MAKE_FIFO("data.txt");
    try
    {
        Game game;
        game.play();
        cout << "Thanks for playing!\n";
        closeWindow();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    UNLINK_FIFO("data.txt");
    
    return 0;
}

void refresh() 
{
    while(!std::filesystem::exists("../mouseOutput.txt")){};
    static ifstream in("../mouseOutput.txt");
    cin.rdbuf(in.rdbuf());

    // This allows reading more if new data has been appended
    in.clear();
    in.seekg(0, std::ios::cur);
}
