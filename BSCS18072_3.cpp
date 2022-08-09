// BSCS18072_3.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;

// my set of operators and brakets include ( +, -, *, /, %, =, <, >, !, (, ), {, }, [, ] )
// assumption: it is not specified, so i am not including decimal points in valid terminals
// assumption: all productions with the same symbol on left side will come consecutively and not in random order throughout the grammar
// @ will represent epsilon symbol
// grammar will always start from starting symbol


// a function to check for error in the grammar, return 1 on first error found, else returns 0 if no errors found
bool checkCFG(vector<vector<string>> p, vector<string> NT) {
    for (int i = 0; i < p.size(); i++) {
        /*at max we can have 10 literals on right side,
        also considering the 1 literal on left side, size must not be greater than 11*/
        if (p[i].size() > 11) {
            cout << "Invalid CFG\nLine " << i+1 << ": More than 10 literals on right hand side.\n";
            return 1;
        }

        //checking that the left side only contains non-terminals
        for (int j = 0; j < p[i][0].size(); j++) {
            if (p[i][0][j] == ' ') {
                cout << "Invalid CFG\nLine " << i + 1 << ": space character found.\n";
                return 1;
            }
            if (p[i][0][j] < 65 || p[i][0][j] > 90) {
                cout << "Invalid CFG\nLine " << i+1 << ": Invalid non terminal.\n";
                return 1;
            }
        }

        // checking for right hand side of the current (i) production
        for (int j = 1; j < p[i].size(); j++) {
            //checking for elements of size 1 on right hand side
            if (p[i][j].size() == 1) {
                /*checking for right hand side if its a non-terminal of size 1, if yes, then
                we check if this non-terminal is also at least once present on left hand side*/
                if (p[i][j][0] >= 65 && p[i][j][0] <= 90) {
                    bool ispresent = false;
                    for (int k = 0; k < NT.size(); k++) {
                        if (NT[k] == p[i][j])
                            ispresent = true;
                    }
                    if (!ispresent) {
                        cout << "Invalid CFG\nLine " << i+1 << ": A non terminal present on right hand side does not exit on left hand side.\n";
                        return 1;
                    }
                }
                else if (p[i][j][0] == ' ') {
                    cout << "Invalid CFG\nLine " << i + 1 << ": space character found.\n";
                    return 1;
                }

                //checking if its not even any operator, bracket, epsilon, small alphabet or a digit
                else if (!(p[i][j][0] == '+' | p[i][j][0] == '-' | p[i][j][0] == '*' |
                         p[i][j][0] == '/' | p[i][j][0] == '%' | p[i][j][0] == '=' |
                         p[i][j][0] == '<' | p[i][j][0] == '>' | p[i][j][0] == '!' |
                         p[i][j][0] == '(' | p[i][j][0] == ')' | p[i][j][0] == '{' |
                         p[i][j][0] == '}' | p[i][j][0] == '[' | p[i][j][0] == ']' |
                         p[i][j][0] == '@' | (p[i][j][0] >= 97 && p[i][j][0] <= 122) |
                         (p[i][j][0] >= 48 && p[i][j][0] <= 57))) {
                        cout << "Invalid CFG\nLine " << i+1 << ": Invalid literal on right hand side.\n";
                        return 1;
                }
            }

            else if (p[i][j].size() > 1) {
                bool isTerminalAlphabets = false;
                bool isTerminalDigits = false;
                bool isNon_terminal = false;
                if (p[i][j][0] >= 65 && p[i][j][0] <= 90) {
                    isNon_terminal = true;
                }
                else if (p[i][j][0] >= 97 && p[i][j][0] <= 122) {
                    isTerminalAlphabets = true;
                }
                else if (p[i][j][0] >= 48 && p[i][j][0] <= 57) {
                    isTerminalDigits = true;
                }
                for (int k = 0; k < p[i][j].size(); k++) {
                    if (p[i][j][k] == ' ') {
                        cout << "Invalid CFG\nLine " << i + 1 << ": space character found.\n";
                        return 1;
                    }
                    if (isNon_terminal && !(p[i][j][k] >= 65 && p[i][j][k] <= 90))
                        isNon_terminal = false;
                    if (isTerminalAlphabets && !(p[i][j][k] >= 97 && p[i][j][k] <= 122))
                        isTerminalAlphabets = false;
                    if (isTerminalDigits && !(p[i][j][k] >= 48 && p[i][j][k] <= 57))
                        isTerminalDigits = false;

                    if (!isNon_terminal && !isTerminalAlphabets && !isTerminalDigits) {
                        cout << "Invalid CFG\nLine " << i+1 << ": Invalid literal on right hand side.\n";
                        return 1;
                    }
                }

                //if its a non terminal we check if this non terminal is at least once present on left hand side
                if (isNon_terminal) {
                    bool ispresent = false;
                    for (int m = 0; m < NT.size(); m++) {
                        if (NT[m] == p[i][j])
                            ispresent = true;
                    }
                    if (!ispresent) {
                        cout << "Invalid CFG\nLine " << i+1 << ": A non terminal present on right hand side does not exit on left hand side.\n";
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void remove_duplicate(vector<vector<string>>& SET) {
    for (int i = 0; i < SET.size(); i++) {
        for (int j = 2; j < SET[i].size(); j++) {
            for (int k = j - 1; k > 0; k--) {
                if (SET[i][j] == SET[i][k]) {
                    SET[i].erase(SET[i].begin() + j);
                    j--;
                }
            }
        }
    }
}

void resolving_non_terminals(vector<vector<string>>& SET) {
    for (int i = 0; i < SET.size(); i++) {
        int index = 0;//to find the row index of the first set of non_terminal
        for (int j = 1; j < SET[i].size(); j++) {
            
            if (SET[i][j][0] > 64 && SET[i][j][0] <= 90) { // if its a non_terminal
                for (int k = 0; k < SET.size(); k++) {
                    if (SET[i][j] == SET[k][0]) {
                        index = k;
                        break;
                    }
                }

                SET[i].erase(SET[i].begin() + j); // removing the non_terminal
                for (int m = 1; m < SET[index].size(); m++) {
                    if (SET[index][m] != SET[i][0]) {
                        SET[i].push_back(SET[index][m]);
                    }
                }
                j--;
            }
        }
    }
}

int main(){
    string filename = "sample.txt";
    fstream file;
    file.open(filename);
    vector<stringstream> lines;
    string l; // to read file line by line

//-----------------reading line by line-----------------
    while (getline(file, l)) {
        lines.push_back(stringstream(l));
        //cout << l << endl;
    }
    string word; //to read file word by word
    vector<string> NT; // to store non terminals
    //int size_NT = NT.size(); // size of the vector of non terminals
    vector<vector<string>> productions(lines.size()); // 2D vector to store productions

    for (int i = 0; i < lines.size(); i++) {
        int j = 0;  //j indicates the word number in each line
        while (getline(lines[i], word, '\t')){
            productions[i].push_back(word);
            if (j == 0) { // if it the first word in a line we store it in non terminals vector
                bool duplicate = false;
                for (int k = 0; k < NT.size(); k++) {
                    if (NT[k] == word) {
                        duplicate = true;
                        break;
                    }
                }
                if (!duplicate) 
                    NT.push_back(word);
                //if (NT.size() == 0){ // when the non terminals vector is empty we simply store the word
                //    NT.push_back(word);
                //    size_NT++;
                //}
                //// when vector is not empty we avoid storing duplicates
                //else if (NT.size() != 0 && NT[size_NT - 1] != word) { 
                //    NT.push_back(word);
                //    size_NT++;
                //}
            }
            j++;
        }
    }

   
    bool CFGisNotValid = checkCFG(productions, NT);

    if (CFGisNotValid) {
        //-------------vectors to store first and follow sets--------------
        vector<vector<string>> FIRST(NT.size());
        vector<vector<string>> FOLLOW(NT.size());

        for (int i = 0; i < NT.size(); i++) {
            FIRST[i].push_back(NT[i]);
            FOLLOW[i].push_back(NT[i]);
        }

        //----------------------computing first sets-----------------------

        for (int i = 0; i < productions.size(); i++) {
            int index = 0; // index to check which non-terminal are we dealing with
            for (int k = 0; k < NT.size(); k++) {
                if (productions[i][0] == FIRST[k][0]) {
                    index = k;
                    break;
                }
            }
            if (productions[i][1][0] < 65 || productions[i][1][0] > 90) { // this means its a terminal
                FIRST[index].push_back(productions[i][1]); // we add that to first set
            }
        }

        // we iterate throught productions again to deal with non terminals this time
        for (int i = 0; i < productions.size(); i++) {
            int index = 0; // index to check which non-terminal are we dealing with
            for (int k = 0; k < NT.size(); k++) {
                if (productions[i][0] == FIRST[k][0]) {
                    index = k;
                    break;
                }
            }
            int x = 1; // this is the index for word count in a production, starting from 1 because element at index 0 is the left hand side
            while (x < productions[i].size()) {
                if (productions[i][x][0] > 64 && productions[i][x][0] <= 90) { // this means its a non-terminal
                    /*we add the non_terminal to the first set and
                    then check if the First set of the non_terminal contains an epsilon*/
                    FIRST[index].push_back(productions[i][x]);
                    int in = 0; // to find the index of productions[i][x] in the first set to access its first set and check for epsilon
                    for (int m = 0; m < NT.size(); m++) {
                        if (productions[i][x] == FIRST[m][0]) {
                            in = m;
                            break;
                        }
                    }
                    bool epsilon = false;
                    for (int n = 1; n < FIRST[in].size(); n++) {
                        if (FIRST[in][n] == "@") {
                            epsilon = true;
                            break;
                        }
                    }
                    if (epsilon == false) {
                        break;
                    }
                    //if next symbol is a termial after the non_terminal containing an epsilon in its FIRST set
                    if (epsilon == true && (productions[i][x + 1][0] < 65 || productions[i][x + 1][0] > 90)) {
                        FIRST[index].push_back(productions[i][x + 1]);
                        break;
                    }
                    x++;
                }
                else break;
            }
        }
        // removing duplicates from first sets
        remove_duplicate(FIRST);

        //to resolve non_terminals to terminals
        resolving_non_terminals(FIRST);

        // removing duplicates from first sets
        remove_duplicate(FIRST);

        //for (int i = 0; i < FIRST.size(); i++) {
        //    cout << "===NEW LINEE====\n";
        //    for (int j = 0; j < FIRST[i].size(); j++) {
        //        cout << FIRST[i][j] << "\n";
        //    }
        //    cout << endl;
        //}

        /*-------------------computing the follow set-------------------*/
        FOLLOW[0].push_back("$"); // adding $ in follow set of starting symbol

        for (int i = 0; i < NT.size(); i++) {
            for (int j = 0; j < productions.size(); j++) {
                if (productions[j].size() > 2) {
                    for (int k = 1; k < productions[j].size() - 1; k++) {
                        if (productions[j][k] == NT[i]) {
                            // if the follow is a terminal
                            if (productions[j][k + 1][0] < 65 || productions[j][k + 1][0] >90) {
                                FOLLOW[i].push_back(productions[j][k + 1]);
                            }
                            //if the follow is a non-terminal
                            else if (productions[j][k + 1][0] > 64 && productions[j][k + 1][0] <= 90) {
                                //find out which non_terminal follows the non_terminal we are working on
                                int index = 0;
                                for (int m = 0; m < NT.size(); m++) {
                                    if (productions[j][k + 1] == NT[m]) {
                                        index = m;
                                        break;
                                    }
                                }
                                //adding the first set of the non-terminal that follows into the follow set
                                for (int n = 1; n < FIRST[index].size(); n++) {
                                    if (FIRST[index][n] != "@")
                                        FOLLOW[i].push_back(FIRST[index][n]);
                                    else if (FIRST[index][n] == "@" && productions[j][0] != NT[i]) {
                                        int indx = 0;
                                        for (int f = 0; f < NT.size(); f++) {
                                            if (productions[j][0] == NT[f]) {
                                                indx = f;
                                                break;
                                            }
                                        }
                                        bool dont = false;
                                        for (int h = 0; h < FOLLOW[indx].size();h++) {
                                            if (FOLLOW[indx][h] == NT[i])
                                                dont = true;
                                            else if (FOLLOW[indx][h][0] < 65 || FOLLOW[indx][h][0] >90) {
                                                FOLLOW[i].push_back(FOLLOW[indx][h]);
                                            }
                                                
                                            
                                        }
                                        if(!dont){ FOLLOW[i].push_back(productions[j][0]); }
                                
                                    }
                                }
                            }
                        }
                        //If it is the last element  in production
                        if (k == productions[j].size() - 2) {
                            if (productions[j][k + 1] == NT[i]) {
                                if (productions[j][0] != NT[i]) {
                                    int indx = 0;
                                    for (int f = 0; f < NT.size(); f++) {
                                        if (productions[j][0] == NT[f]) {
                                            indx = f;
                                            break;
                                        }
                                    }
                                    bool dont = false;
                                    for (int h = 0; h < FOLLOW[indx].size(); h++) {
                                        if (FOLLOW[indx][h] == NT[i])
                                            dont = true;
                                        else if (FOLLOW[indx][h][0] < 65 || FOLLOW[indx][h][0] >90) {
                                            FOLLOW[i].push_back(FOLLOW[indx][h]);
                                        }
                                    }
                                    if (!dont) { FOLLOW[i].push_back(productions[j][0]); }
                                }
                            }
                        }
                    }
                }
                if (productions[j].size() == 2) {
                    if (productions[j][1] == NT[i]) {
                        if (productions[j][0] != NT[i])
                            FOLLOW[i].push_back(productions[j][0]);
                    }
                }
            }
        }


        // removing duplicates from first sets
        remove_duplicate(FOLLOW);

        //resolving non_terminals to terminals
       resolving_non_terminals(FOLLOW);

       // removing duplicates from first sets
       remove_duplicate(FOLLOW);

       for (int s = 0; s < FOLLOW.size(); s++) {
           cout << "===NEW FOLLOW LINEE====\n";
           for (int r = 0; r < FOLLOW[s].size(); r++) {
               cout << FOLLOW[s][r] << "\n";
           }
           cout << endl;
       }
  


        ofstream out_first("BSCS18072_first.txt");
        ofstream out_follow("BSCS18072_follow.txt");

        //writing to first set file
        for (int i = 0; i < FIRST.size(); i++) {
            for (int j = 0; j < FIRST[i].size(); j++) {
                out_first << FIRST[i][j] << "\t";
            }
            out_first << endl;
        }

        //writing to follow set file
        for (int i = 0; i < FOLLOW.size(); i++) {
            for (int j = 0; j < FOLLOW[i].size(); j++) {
                out_follow << FOLLOW[i][j] << "\t";
            }
            out_follow << endl;
        }
    }

    return 0;
}