#include <bits/stdc++.h>
#include <conio.h>
using namespace std;
#define FILL '$'
#define CROSS '.'
#define UNFILL ' '


enum state {square,cross,uncertain,wrong};

vector<vector<char>> board;
vector<vector<int>> rows;
vector<vector<int>> cols;
bool foundSolution = false;
bool isStepByStep = false;

vector<int> parseInt(string &line){
    vector<int> res;
    int num = 0;
    for(int i = 0;i < line.size();i++){
        if(line[i] == ','){
            res.push_back(num);
            num = 0;
        }
        else{
            num = num*10 + (line[i]-'0');
        }
    }
    res.push_back(num);
    return res;
}

int nonogram_sum(vector<int> &v){
    int sum = 0;
    for(int i = 0;i < v.size();i++){
        sum += v[i];
    }
    return sum + v.size() - 1;
}

void nonogram_row_place(vector<vector<bool>> &visited,int index){
    int current_col = 0;
    for (int i = 0; i < rows[index].size(); i++)
    {
        for (int j = 0; j < rows[index][i]; j++)
        {
            board[index][current_col++] = FILL;
        }
        if(current_col != board[index].size()){
            board[index][current_col++] = CROSS;
        }
    }
    for (int i = 0; i < board[index].size(); i++)
    {
        visited[index][i] = true;
    }
}

void nonogram_col_place(vector<vector<bool>> &visited,int index){
    int current_row = 0;
    for (int i = 0; i < cols[index].size(); i++)
    {
        for (int j = 0; j < cols[index][i]; j++)
        {
            board[current_row++][index] = FILL;
        }
        if(current_row != board.size()){
            board[current_row++][index] = CROSS;
        }
    }
    for (int i = 0; i < board.size(); i++)
    {
        visited[i][index] = true;
    }
}



void nonogram_quick_place(vector<vector<bool>> &visited){
    for (int i = 0; i < rows.size(); i++)
    {
        if(nonogram_sum(rows[i]) == board[i].size()){
            nonogram_row_place(visited,i);
        }
    }
    for (int i = 0; i < cols.size(); i++)
    {
        if(nonogram_sum(cols[i]) == board.size()){
            nonogram_col_place(visited,i);
        }
    }

}

state nonogram_row_heuristic(int row, int col){
    int current_filled_square = 0;
    int total_filled_square = 0;
    int rows_index = 0;
    bool isComplete = false;
    for(int i = 0; i < col; i++){
        if(board[row][i] == FILL){
            if (isComplete){
                return wrong;
            }
            current_filled_square++;
            total_filled_square++;
        }
        else if(board[row][i] == CROSS && current_filled_square != 0){
            if (rows_index == rows[row].size()){
                isComplete = true;
            }
            if (current_filled_square != rows[row][rows_index]){
                return wrong;
            }
            rows_index++;
            current_filled_square = 0;
        }
    }
    int total_sum = 0;
    for (int i = 0; i < rows[row].size(); i++)
    {
        total_sum += rows[row][i];
    }
    total_sum -= total_filled_square;
    if (board[row].size() - col < total_sum ||(col == board[row].size() - 1 && rows_index < rows[row].size() - 1)){
        return wrong;
    }
    if (isComplete){
        return cross;
    }
    if (current_filled_square == 0){
        return uncertain;
    }
    if (current_filled_square <= rows[row][rows_index] - 1){
        return square;
    }
    if (current_filled_square == rows[row][rows_index]){
        return cross;
    }
    return wrong;
}
state nonogram_col_heuristic(int row, int col){
    int current_filled_square = 0;
    int total_filled_square = 0;
    int cols_index = 0;
    bool isComplete = false;
    for(int i = 0; i < row; i++){
        if(board[i][col] == FILL){
            if (isComplete){
                return wrong;
            }
            current_filled_square++;
            total_filled_square++;
        }
        else if(board[i][col] == CROSS && current_filled_square != 0){
            if (cols_index == cols[col].size()){
                isComplete = true;
            }
            if (current_filled_square != cols[col][cols_index]){
                return wrong;
            }
            cols_index++;
            current_filled_square = 0;
        }
    }
    int total_sum = 0;
    for (int i = 0; i < cols[col].size(); i++)
    {
        total_sum += cols[col][i];
    }
    total_sum -= total_filled_square;
    if (board.size() - row < total_sum || (row == board.size() - 1 && cols_index < cols[col].size() - 1)){
        return wrong;
    }
    if (isComplete){
        return cross;
    }
    if (current_filled_square == 0){
        return uncertain;
    }
    if (current_filled_square <= cols[col][cols_index] - 1){
        return square;
    }
    if (current_filled_square == cols[col][cols_index]){
        return cross;
    }
    return wrong;
}

void print_board(){
    //Set cursor position to top left corner
    cout << "\033[0;0H";
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[i].size(); j++){
            cout << board[i][j];
        }
        cout << endl;
    }
}
string print_state(state s){
    switch (s)
    {
    case square:
        return "square";
    case cross:
        return "cross";
    case uncertain:
        return "uncertain";
    case wrong:
        return "wrong";
    default:
        return "wrong";
    }
}


bool isNonogramComplete(){
    //TODO: check if the nonogram is complete
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[i].size(); j++){
            if(board[i][j] == UNFILL){
                return false;
            }
        }
    }
    for(int current_row = 0; current_row < board.size(); current_row++){
        int current_filled_square = 0;
        int rows_index = 0;
        bool isComplete = false;
        for(int i = 0; i < board[current_row].size(); i++){
            if(board[current_row][i] == FILL){
                if (isComplete){
                    return false;
                }
                current_filled_square++;
            }
            else if(board[current_row][i] == CROSS && current_filled_square != 0){
                if (current_filled_square != rows[current_row][rows_index]){
                    return false;
                }
                rows_index++;
                if (rows_index == rows[current_row].size()){
                    isComplete = true;
                }
                current_filled_square = 0;
            }
        }
        if(!isComplete){
            if (current_filled_square != rows[current_row][rows_index]){
                return false;
            }
        }
    }
    for (int current_col = 0; current_col < board[0].size(); current_col++){
        int current_filled_square = 0;
        int cols_index = 0;
        bool isComplete = false;
        for(int i = 0; i < board.size(); i++){
            if(board[i][current_col] == FILL){
                if (isComplete){
                    return false;
                }
                current_filled_square++;
            }
            else if(board[i][current_col] == CROSS && current_filled_square != 0){
                if (current_filled_square != cols[current_col][cols_index]){
                    return false;
                }
                cols_index++;
                if (cols_index == cols[current_col].size()){
                    isComplete = true;
                }
                current_filled_square = 0;
            }
        }
        if(!isComplete){
            if (current_filled_square != cols[current_col][cols_index]){
                return false;
            }
        }
    }
    return true;
}
bool checkNonogramRowSoFar(int row, int col){
    int current_filled_square = 0;
    int rows_index = 0;
    bool isComplete = false;
    for(int i = 0; i <= col; i++){
        if(board[row][i] == FILL){
            if (isComplete){
                return false;
            }
            current_filled_square++;
        }
        else if(board[row][i] == CROSS && current_filled_square != 0){
            if (current_filled_square != rows[row][rows_index]){
                return false;
            }
            rows_index++;
            if (rows_index == rows[row].size()){
                isComplete = true;
            }
            current_filled_square = 0;
        }
    }
    if(!isComplete){
        if (col == board[row].size() - 1){
            if (rows_index != rows[row].size() - 1){
                return false;
            }
            else{
                return current_filled_square == rows[row][rows_index];
            }
        }
        else{
            return current_filled_square <= rows[row][rows_index];
        }
    }
    return true;
}

bool checkNonogramColSoFar(int row, int col){
    int current_filled_square = 0;
    int cols_index = 0;
    bool isComplete = false;
    for(int i = 0; i <= row; i++){
        if(board[i][col] == FILL){
            if (isComplete){
                return false;
            }
            current_filled_square++;
        }
        else if(board[i][col] == CROSS && current_filled_square != 0){
            if (current_filled_square != cols[col][cols_index]){
                return false;
            }
            cols_index++;
            if (cols_index == cols[col].size()){
                isComplete = true;
            }
            current_filled_square = 0;
        }
    }
    if(!isComplete){
        if (row == board.size() - 1){
            if (cols_index != cols[col].size() - 1){
                return false;
            }
            else{
                return current_filled_square == cols[col][cols_index];
            }
        }
        else{
            return current_filled_square <= cols[col][cols_index];
        }
    }
    return true;
}


void backtrack(vector<vector<bool>> &visited, int row, int col){
    if (foundSolution){
        return;
    }
    if (isStepByStep){
        print_board();
    }
    if (row == board.size()){
        if (isNonogramComplete()){
            print_board();
            cout << "Found solution" << endl;
            foundSolution = true;
        }
        return;
    }
    int next_row,next_col;
    if (col == board[0].size() - 1){
        next_row = row + 1;
        next_col = 0;
    }
    else{
        next_row = row;
        next_col = col + 1;
    }
    if (visited[row][col]){
        backtrack(visited,next_row,next_col);
        return; 
    }
    state row_state = nonogram_row_heuristic(row,col);
    state col_state = nonogram_col_heuristic(row,col);
    switch (row_state){
        case wrong:
            return;
        case cross:
            switch (col_state){
                case cross:
                case uncertain:
                    visited[row][col] = true;
                    board[row][col] = CROSS;
                    if (checkNonogramRowSoFar(row,col) && checkNonogramColSoFar(row,col)){
                        backtrack(visited,next_row,next_col);
                    }
                    visited[row][col] = false;
                    board[row][col] = UNFILL;
                    break;
                case square:
                case wrong:
                    return;
                    break;
            }
            break;
        case uncertain:
            switch (col_state){
                case wrong:
                    return;
                case cross:
                    visited[row][col] = true;
                    board[row][col] = CROSS;
                    if (checkNonogramRowSoFar(row,col) && checkNonogramColSoFar(row,col)){
                        backtrack(visited,next_row,next_col);
                    }
                    visited[row][col] = false;
                    board[row][col] = UNFILL;
                    break;
                case square:
                    visited[row][col] = true;
                    board[row][col] = FILL;
                    if (checkNonogramRowSoFar(row,col) && checkNonogramColSoFar(row,col)){
                        backtrack(visited,next_row,next_col);
                    }
                    visited[row][col] = false;
                    board[row][col] = UNFILL;
                    break;
                case uncertain:
                    visited[row][col] = true;
                    board[row][col] = CROSS;
                    if (checkNonogramRowSoFar(row,col) && checkNonogramColSoFar(row,col)){
                        backtrack(visited,next_row,next_col);
                    }
                    board[row][col] = FILL;
                    if (checkNonogramRowSoFar(row,col) && checkNonogramColSoFar(row,col)){
                        backtrack(visited,next_row,next_col);
                    }
                    visited[row][col] = false;
                    board[row][col] = UNFILL;
                    break;
            }
            break;
        case square:
            switch (col_state){
                case wrong:
                case cross:
                    return;
                case uncertain:
                case square:
                    visited[row][col] = true;
                    board[row][col] = FILL;
                    backtrack(visited,next_row,next_col);
                    visited[row][col] = false;
                    board[row][col] = UNFILL;
                    break;
            }
            break;
    }
}


int main(int argc, const char * argv[]) {
    if (argc != 3){
        cout << "Usage: ./nonogram <input_file> <0 for only show solution, 1 for showing the steps>" << endl;
        return 0;
    }
    system("cls");
    //Hide cursor
    cout << "\033[?25l";
    fstream file;
    file.open(argv[1]);
    isStepByStep = argv[2][0] == '1';
    string line;
    int row_size,col_size;
    line = "";
    getline(file,line);
    row_size = parseInt(line)[1];
    col_size = parseInt(line)[0];
    board.resize(row_size);
    for (int i = 0; i < row_size; i++)
    {
        board[i] = vector<char>(col_size,UNFILL);
    }
    rows.resize(row_size);
    cols.resize(col_size);
    getline(file,line);
    for (int i = 0; i < col_size; i++)
    {
        getline(file,line);
        cols[i] = parseInt(line);
    }
    getline(file,line);
    for (int i = 0; i < row_size; i++)
    {
        getline(file,line);
        rows[i] = parseInt(line);
    }
    file.close();
    vector<vector<bool>> visited(row_size,vector<bool>(col_size,false));
    auto start = chrono::high_resolution_clock::now();
    nonogram_quick_place(visited);
    backtrack(visited,0,0);
    auto stop = chrono::high_resolution_clock::now();
    cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << "ms" << endl;
    //Show cursor
    cout << "\033[?25h";
}
