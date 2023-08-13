#include <bits/stdc++.h>
#include <conio.h>
using namespace std;
#define FILL '$'
#define CROSS ' '
#define UNFILL '.'


enum state {square,cross,uncertain,wrong};

vector<vector<char>> board;
vector<vector<int>> rows;
vector<vector<int>> cols;
vector<vector<char>> solution;
bool foundSolution = false;
bool isStepByStep = false;
bool transposeMode = false;

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
    if (transposeMode){
        for(int i = 0; i < board[0].size(); i++){
            for(int j = 0; j < board.size(); j++){
                cout << board[j][i];
            }
            cout << endl;
        }
        return;
    }
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
    if (row == board.size()){
        if (isNonogramComplete()){
            print_board();
            cout << "Found solution" << endl;
            foundSolution = true;
            solution = board;
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
    if (isStepByStep){
        print_board();
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
                    board[row][col] = FILL;
                    if (checkNonogramRowSoFar(row,col) && checkNonogramColSoFar(row,col)){
                        backtrack(visited,next_row,next_col);
                    }
                    board[row][col] = CROSS;
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


void superposition (string &s, int size, vector<int> &rows,vector<bool> &super_postion_cross, vector<bool> &super_postion_square, int index = 0, int rows_index = 0, int current_filled_square = 0){
    //cout << "String: " << s << " Index: " << index << " Rows_index: " << rows_index << " Current_filled_square: " << current_filled_square << endl;
    if (index == size){
        if (rows_index == rows.size() || (rows_index == rows.size() - 1 && current_filled_square == rows[rows_index])){
            //cout << s << endl;
            for (int i = 0; i < size; i++){
                if (s[i] == CROSS){
                    super_postion_square[i] = false;
                }
                else if (s[i] == FILL){
                    super_postion_cross[i] = false;
                }
            }
        }
        return;
    }
    int sum = 0;
    for (int i = rows_index; i < rows.size(); i++){
        sum += rows[i];
    }
    if (sum  - current_filled_square > size - index){
        return;
    }



   if (s[index] != UNFILL){
    if (s[index] == FILL){
        superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index, current_filled_square + 1);
    }
    else{
        if (current_filled_square == 0){
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index, 0);
        }
        else if (current_filled_square < rows[rows_index]){
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index, current_filled_square + 1);
        }
        else if (current_filled_square == rows[rows_index]){
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index + 1, 0);
        }
    }
   }
   else{
        if (rows_index == rows.size()){
            //cout << "Choose CROSS" << endl;
            s[index] = CROSS;
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index, 0);
            s[index] = UNFILL;
        }
        else if (current_filled_square == 0){
            //cout << "Choose BOTH" << endl;
            s[index] = CROSS;
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index, 0);
            s[index] = FILL;
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index, 1);
            s[index] = UNFILL;
        }
        else if (current_filled_square < rows[rows_index]){
            //cout << "Choose FILL" << endl;
            s[index] = FILL;
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index, current_filled_square + 1);
            s[index] = UNFILL;
        }
        else if (current_filled_square == rows[rows_index]){
            //cout << "Choose CROSS" << endl;
            s[index] = CROSS;
            superposition(s, size, rows, super_postion_cross, super_postion_square, index + 1, rows_index + 1, 0);
            s[index] = UNFILL;
        }
   }
}


int main(int argc, const char * argv[]) {
    
    if (argc < 3){
        cout << "Usage: ./nonogram <input_file> <0 for only show solution, 1 for showing the steps> <0 for normal mode, 1 for transpose mode if not specified, it will determined by the input file>" << endl;
        return 0;
    }
    if (argc == 4){
        transposeMode = argv[3][0] == '1';
    }
    system("cls");
    fstream file;
    file.open(argv[1]);
    if (!file){
        cout << "File not found" << endl;
        return 0;
    }
    //Hide cursor
    cout << "\033[?25l";
    cout << "Loading..."<< endl;
    isStepByStep = argv[2][0] == '1';
    string line;
    int row_size,col_size;
    line = "";
    getline(file,line);
    if (transposeMode){
        col_size = parseInt(line)[1];
        row_size = parseInt(line)[0];
    }
    else{
        row_size = parseInt(line)[1];
        col_size = parseInt(line)[0];
    }
    if (!transposeMode && row_size <= (col_size * 3) / 4){
        transposeMode = true;
        swap(row_size,col_size);
    }

    board.resize(row_size);
    for (int i = 0; i < row_size; i++)
    {
        board[i] = vector<char>(col_size,UNFILL);
    }
    rows.resize(row_size);
    cols.resize(col_size);
    getline(file,line);
    if (transposeMode){
        for (int i = 0; i < row_size; i++)
        {
            getline(file,line);
            rows[i] = parseInt(line);
        }
        getline(file,line);
        for (int i = 0; i < col_size; i++)
        {
            getline(file,line);
            cols[i] = parseInt(line);
        }
    }
    else{
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
    }
    file.close();
    vector<vector<bool>> visited(row_size,vector<bool>(col_size,false));
    string fname = string(argv[1]).find_last_of("/") == string::npos ? string(argv[1]) : string(argv[1]).substr(string(argv[1]).find_last_of("/")+1);
    auto superpos_time_start = chrono::high_resolution_clock::now();
    for (int r = 0; r < board.size(); r++){
        int sum_of_row = 0;
        for (int i = 0; i < rows[r].size(); i++){
            sum_of_row += rows[r][i];
        }
        string row = "";
        for (int c = 0; c < board[r].size(); c++){
            row += board[r][c];
        }
        vector<bool> super_postion_cross(col_size,true);
        vector<bool> super_postion_square(col_size,true);
        superposition(row, col_size, rows[r], super_postion_cross, super_postion_square);
        bool isCollided = false;
        for (int c = 0; !isCollided && c < col_size; c++){
            if (super_postion_cross[c] && super_postion_square[c]){
                isCollided = true;
            }
            else if (super_postion_cross[c]){
                board[r][c] = CROSS;
                visited[r][c] = true;
            }
            else if (super_postion_square[c]){
                board[r][c] = FILL;
                visited[r][c] = true;
            }
        }
        if (isCollided){
            cout << "No solution" << endl;
            return 0;
        }
        //cout << "Row " << r << " done" << endl;
    }
    for(int c = 0; c < board[0].size(); c++){
        string col = "";
        for (int r = 0; r < board.size(); r++){
            col += board[r][c];
        }
        vector<bool> super_postion_cross(row_size,true);
        vector<bool> super_postion_square(row_size,true);
        superposition(col, row_size, cols[c], super_postion_cross, super_postion_square);
        bool isCollided = false;
        for (int r = 0; !isCollided && r < row_size; r++){
            if (super_postion_cross[r] && super_postion_square[r]){
                isCollided = true;
            }
            else if (super_postion_cross[r]){
                board[r][c] = CROSS;
                visited[r][c] = true;
            }
            else if (super_postion_square[r]){
                board[r][c] = FILL;
                visited[r][c] = true;
            }
        }
        if (isCollided){
            cout << "No solution" << endl;
            return 0;
        }
        //cout << "Col " << c << " done" << endl;
    }
    auto superpos_time_stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(superpos_time_stop-superpos_time_start);
    if (duration.count() > 60000000){
        cout << "Superposition time taken: " << duration.count() / 60000000.0 << " minutes" << endl;
    }
    else if (duration.count() > 1000000){
        cout << "Superposition time taken: " << duration.count() / 1000000.0 << " seconds" << endl;
    }
    else if (duration.count() > 1000){
        cout << "Superposition time taken: " << duration.count() / 1000.0 << " milliseconds" << endl;
    }
    else{
        cout << "Superposition time taken: " << duration.count() << " microseconds" << endl;
    }
    cout << "Solving " << fname << "..." << endl;
    auto start = chrono::high_resolution_clock::now();
    nonogram_quick_place(visited);
    backtrack(visited,0,0);
    auto stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    //Print time in minutes if it takes more than 1 minute
    //Print time in seconds if it takes less than 1 minute
    //Print time in milliseconds if it takes less than 1 second
    //Otherwise print time in microseconds
    if (duration.count() > 60000000){
        cout << "Time taken: " << duration.count() / 60000000.0 << " minutes" << endl;
    }
    else if (duration.count() > 1000000){
        cout << "Time taken: " << duration.count() / 1000000.0 << " seconds" << endl;
    }
    else if (duration.count() > 1000){
        cout << "Time taken: " << duration.count() / 1000.0 << " milliseconds" << endl;
    }
    else{
        cout << "Time taken: " << duration.count() << " microseconds" << endl;
    }
    cout << "\033[?25h";
    fstream output;



    output.open("./outputs/" + fname,ios::out);
    if (output.fail()){
        cout << "Failed to open output file" << endl;
        return 0;
    }
    if (transposeMode){
        for (int c = 0; c < board[0].size(); c++){
            for (int r = 0; r < board.size(); r++){
                output << board[r][c];
            }
            output << endl;
        }
    }
    else{
        for (int r = 0; r < board.size(); r++){
            for (int c = 0; c < board[r].size(); c++){
                output << board[r][c];
            }
            output << endl;
        }
    }
    if (duration.count() > 60000000){
        output << "Time taken: " << duration.count() / 60000000.0 << " minutes" << endl;
    }
    else if (duration.count() > 1000000){
        output << "Time taken: " << duration.count() / 1000000.0 << " seconds" << endl;
    }
    else if (duration.count() > 1000){
        output << "Time taken: " << duration.count() / 1000.0 << " milliseconds" << endl;
    }
    else{
        output << "Time taken: " << duration.count() << " microseconds" << endl;
    }
    output.close();
    cout << "Output file: " << "./outputs/" + fname << endl;
    return 0;
}
