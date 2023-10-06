#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
using namespace std;

string readConfig();
void configureBoard(string boardRead);
void populateRefs();
short updateValues();
void displayBoard(short values[81]);
void identifyMultiples(short index);
void identifyUnique(short index);
void cleanSet(short type, short ind);
short countInstance(set<short> arr[9], set<short> elem);
void ai();
void backtrackSetup(short vals[81]);
void backtrack(short n, short depth);
bool checkValid(short index);

struct cell
{
    short col;
    short row;
    short box;
    set<short> possible;
    short value;
};

cell board[81];
short values[81];
vector<short> zeroIndices;
vector<vector<short>> zeroPossibles;
vector<short> current;

short rowsRef[9][9] = {
    {0,1,2,3,4,5,6,7,8},
    {9,10,11,12,13,14,15,16,17},
    {18,19,20,21,22,23,24,25,26},
    {27,28,29,30,31,32,33,34,35},
    {36,37,38,39,40,41,42,43,44},
    {45,46,47,48,49,50,51,52,53},
    {54,55,56,57,58,59,60,61,62},
    {63,64,65,66,67,68,69,70,71},
    {72,73,74,75,76,77,78,79,80}
};

short colsRef[9][9] = {
    {0,9,18,27,36,45,54,63,72},
    {1,10,19,28,37,46,55,64,73},
    {2,11,20,29,38,47,56,65,74},
    {3,12,21,30,39,48,57,66,75},
    {4,13,22,31,40,49,58,67,76},
    {5,14,23,32,41,50,59,68,77},
    {6,15,24,33,42,51,60,69,78},
    {7,16,25,34,43,52,61,70,79},
    {8,17,26,35,44,53,62,71,80}
};

short boxesRef[9][9] = {
    {0,1,2,9,10,11,18,19,20},
    {3,4,5,12,13,14,21,22,23},
    {6,7,8,15,16,17,24,25,26},
    {27,28,29,36,37,38,45,46,47},
    {30,31,32,39,40,41,48,49,50},
    {33,34,35,42,43,44,51,52,53},
    {54,55,56,63,64,65,72,73,74},
    {57,58,59,66,67,68,75,76,77},
    {60,61,62,69,70,71,78,79,80}
};



int main()
{
    string read = readConfig();
    configureBoard(read);
    short changed = 0;
    do
    {
        changed = 0;
        for (short i=0; i<81; i++)
        {
            if (board[i].value != 0)
            {
                identifyMultiples(i);
            }
        }
        for (short x=0; x<81; x++)
        {
            identifyUnique(x);
        }
        for (short i=0; i<9; i++)
        {
            cleanSet(0,i);
            cleanSet(1,i);
            cleanSet(2,i);
        }
        changed += updateValues();
    } while (changed != 0);
    updateValues();
    backtrackSetup(values);
    backtrack(zeroIndices.size(),0);
    return 0;
}

string readConfig()
{
    ifstream initial("config.txt");
    string full = "";
    string current;
    while (getline(initial, current))
    {
        for (char c : current)
        {
            if (c=='.')
            {
                full+='0';
            }
            else if (c!='|' && c!='-')
            {
                full+=c;
            }
        }
    }
    initial.close();
    return full;
}

void configureBoard(string boardRead)
{
    for (short i=0; i<81; i++)
    {
        short row = i/9;
        short col = i%9;
        short box = ((row/3)*3)+(col/3);
        board[i].row = row;
        board[i].col = col;
        board[i].box = box;
        board[i].value = boardRead[i]-'0';
        if (board[i].value == 0)
        {
            board[i].possible = {1,2,3,4,5,6,7,8,9};
        }
        else
        {
            board[i].possible = {board[i].value};
        }
    }
}

short updateValues()
{
    short changed = 0;
    for (short i=0; i<81; i++)
    {
        if (values[i] != board[i].value)
        {
            values[i] = board[i].value;
            changed++;
        }
    }
    return changed;
}

void displayBoard(short values[81])
{
    for (short i=0; i<81; i++)
    {
        char display = (values[i]==0)?' ':values[i]+'0';
        if (i%27 == 26 && i!=80)
        {
            cout << display << "\n-----------\n";
        }
        else if (i%9 == 8)
        {
            cout << display << endl;
        }
        else if (i%3 == 2)
        {
            cout << display << '|';
        }
        else
        {
            cout << display;
        }
    }
}

void identifyMultiples(short index)
{
    // function only called if value of that cell is non zero
    // board[index].box {value, box, possibles, row, col}
    // rowsRef[box] {rowsRef, colsRef, boxesRef}
    short val = board[index].value;
    for (short rowIndex: rowsRef[board[index].row])
    {
        board[rowIndex].possible.erase(val);

    }
    for (short colIndex: colsRef[board[index].col])
    {
        board[colIndex].possible.erase(val);
    }
    for (short boxIndex: boxesRef[board[index].box])
    {
        board[boxIndex].possible.erase(val);
    }
    board[index].possible.insert(val);
}

void identifyUnique(short index)
{
    short box = board[index].box;
    short row = board[index].row;
    short col = board[index].col;
    set <short> possible = board[index].possible;
    short checks[3][9] = {{},{},{}};
    short i=0;
    for (short b: boxesRef[box])
    {
        checks[0][i] = b;
        i++;
    }
    i=0;
    for (short r: rowsRef[row])
    {
        checks[1][i] = r;
        i++;
    }
    i=0;
    for (short c: colsRef[col])
    {
        checks[2][i] = c;
        i++;
    }
    for (short j=0; j<3; j++)
    {
        for (short p: possible)
        {
            short count = 0;
            for (short c: checks[j])
            {
                count += board[c].possible.count(p);
            }
            if (count == 1 && board[index].value == 0)
            {
                board[index].possible = {p};
                board[index].value = p;
                break;
            }
        }
    }
}

void onePossible(short index)
{
    if (board[index].value == 0 && board[index].possible.size() == 1)
    {
        board[index].value = *board[index].possible.begin();
    }
}

short countInstance(set<short> arr[9], set<short> elem)
{
    short count = 0;
    for (short i=0; i<9; i++)
    {
        if (arr[i] == elem)
        {
            count += 1;
        }
    }
    return count;
}

void cleanSet(short type, short ind)
{
    short referenceSet[9];
    switch (type)
    {
        case 0:
            for (short x=0; x<9; x++)
            {
                referenceSet[x] = boxesRef[ind][x];
            }
            break;
        case 1:
            for (short x=0; x<9; x++)
            {
                referenceSet[x] = rowsRef[ind][x];
            }
            break;
        case 2:
            for (short x=0; x<9; x++)
            {
                referenceSet[x] = colsRef[ind][x];
            }
            break;
    }
    set<short> possibles[9] = {};
    for (short x=0; x<9; x++)
    {
        possibles[x] = board[referenceSet[x]].possible;
    }
    for (set<short> s: possibles)
    {
        if (s.size() != 1)
        {
            if (countInstance(possibles, s) == s.size())
            {
                for (short x=0; x<9; x++)
                {
                    if (board[referenceSet[x]].possible != s)
                    {
                        for (short elem: s)
                        {
                            board[referenceSet[x]].possible.erase(elem);
                        }
                    }
                }
            }
        }
    }
}

void backtrackSetup(short vals[81])
{
    for (short i=0; i<81; i++)
    {
        if (vals[i]==0)
        {
            zeroIndices.push_back(i);
            vector<short> pushing;
            for (short x: board[i].possible)
            {
                pushing.push_back(x);
            }
            zeroPossibles.push_back(pushing);
        }
    }
}

bool checkValid(short index)
{
    short box = board[index].box;
    short row = board[index].row;
    short col = board[index].col;
    short val = board[index].value;
    for (short b: boxesRef[box])
    {
        if (b != index && board[b].value == val)
        {
            return false;
        }
    }
    for (short r: rowsRef[row])
    {
        if (r != index && board[r].value == val)
        {
            return false;
        }
    }
    for (short c: colsRef[col])
    {
        if (c != index && board[c].value == val)
        {
            return false;
        }
    }
    return true;
}

void backtrack(short n, short depth)
{
    if (depth==n)
    {
        bool completed = true;
        for (short i=0; i<81; i++)
        {
            if (values[i]==0)
            {
                completed=false;
                break;
            }
        }
        if (completed == false)
        {
            cout << "Sudoku not solvable!";
        }
        else
        {
            displayBoard(values);
        }
    }
    else
    {
        for (short x: zeroPossibles[depth])
        {
            short copy[81];
            for (short i=0; i<81; i++)
            {
                copy[i]=values[i];
            }
            values[zeroIndices[depth]] = x;
            if (checkValid(zeroIndices[depth]) == true)
            {
                backtrack(n, depth+1);
            }
            for (short i=0; i<81; i++)
            {
                values[i]=copy[i];
            }
        }
    }
}