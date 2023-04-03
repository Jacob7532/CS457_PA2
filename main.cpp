//Programming Assignment 1
//CS457
//Jacob Ayers

using namespace std;

#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>


void create(string command[], int j);
void drop(string command[]);
void useDatabase(string command[], string directory);
void select(string command[]);
void alter(string command[]);
void insert(string command[]);
void update(string command[]);
void dele(string command[]);

int main(void){
    int z = 0;
    string command[100], temp, directory;
    mkdir("databases", 0777);
    chdir("databases");
    directory = get_current_dir_name();
    while(z != 1){

        int j=0;

        //parse through the command inputs and store them in an array called command
        getline(cin, temp);
        stringstream ss(temp);
        while(ss >> command[j]){
            j++;
        }
        
        //remove every paraenthesis, comma, and semicolon
        for(int k=0; k<j; k++){
            command[k].erase(remove(command[k].begin(), command[k].end(), '('), command[k].end());
            command[k].erase(remove(command[k].begin(), command[k].end(), ')'), command[k].end());
            command[k].erase(remove(command[k].begin(), command[k].end(), ','), command[k].end());
            command[k].erase(remove(command[k].begin(), command[k].end(), ';'), command[k].end());
        }

        //database create command
        if(command[0] == "CREATE"){
            create(command, j);
        }

        //database remove command
        else if(command[0] == "DROP" && command[1] == "DATABASE"){
            drop(command);
        }

        //database use command
        else if(command[0] == "USE"){
            useDatabase(command, directory);
        }

        //selection command
        else if(command[0] == "SELECT"){
            select(command);
        }

        //alter command
        else if(command[0] == "Alter"){
            alter(command);
        }

        //insert items into the table
        else if(command[0] == "insert"){
            insert(command);
        }

        else if(command[0] == "update"){
            update(command);
        }

        else if(command[0] == "delete"){
            dele(command);
        }

        //closing command
        if(command[0] == ".EXIT"){
            z = 1;
        }
    }

    cout << "All done." << endl;
    return 0;
}



void create(string command[], int j){
    if(command[1] == "DATABASE"){
        //creates a directory using command[2] as name
        //if directory already exists, prints error
        if(mkdir(command[2].c_str(), 0777) == -1){
            cout << "!Failed to create database " << command[2] << " because it already exists." << endl;
        }
        else{
            cout << "Database " << command[2] << " created." << endl;
        }
    }
    else if(command[1] == "TABLE"){
        //creates a file using command[2] as the name
        //if file already exists, prints error
        fstream tmp;
        tmp.open(command[2].c_str());
        if(tmp.is_open()){
            cout << "!Failed to create table " << command[2] << " because it already exists." << endl;
        }
        else{
            FILE * file = fopen(command[2].c_str(), "w");
            //determin how many variables will be in the table
            int numVars = (j-3)/2;
            //add the variables as columns to the file
            for(int k=0; k<numVars; k++){
                fprintf(file, "[%s, %s] ", command[k+3].c_str(), command[k+4].c_str());
                k++;
            }
            fclose(file);
            cout << "Table " << command[2] << " created." << endl;
        }
        //closes file
    }
}



void drop(string command[]){
    if(command[1] == "DATABASE"){
        //removes directory using command[2] as name
        //if directory does not exist, prints error
        if(rmdir(command[2].c_str())==-1){
            cout << "!Failed to delete database " << command[2] << " because it does not exist." << endl;
        }
        else{
            cout << "Database " << command[2] << " deleted." << endl;
        }
    }
    else if(command[1] == "TABLE"){
        //removes file using command[2] as the name
        //if file does not exist, prints error
        fstream tmp;
        tmp.open(command[2].c_str());
        if(!tmp.is_open()){
            cout << "!Failed to delete table " << command[2] << " because it does not exist." << endl;
        }
        else{
            remove(command[2].c_str());
            cout << "Table " << command[2] << " deleted." << endl;
        }
    }
}



void useDatabase(string command[], string directory){
    //enters into directory using command[1] as name
    //if directory does not exist, prints error
    //return current directory
        if(get_current_dir_name() != directory){
           chdir("..");
        }
        if(chdir(command[1].c_str())==-1){
                cout << "!Failed to use database " << command[1] << " because it does not exist." << endl;
        }
        else{
                cout << "Using database " << command[1] << "." << endl;
        }
}



void select(string command[]){
    if(command[1] == "*"){
        //opens file using command[3] as the name
        //if file does not exist, prints error
        fstream file;
        file.open(command[3].c_str());
        if(!file.is_open()){
            cout << "!Failed to query table " << command[3] << " because it does not exist." << endl;
        }
        else{
            //read through file into a vector
            vector<string> lines;
            string fileLine;
            while (getline(file, fileLine)){
                lines.push_back(fileLine);
            }
            //print out the vector
            for(int k = 0; k<lines.size(); k++){
                cout << lines[k] << endl;
            }
        }
    }
    else{
        //opens file using command[3] as the name
        //if file does not exist, prints error
        //determin how many variables are being selected
        int numSelect = 0;
        for(int k = 1; command[k] != "FROM"; k++){
            numSelect++;
        }
        fstream file;
        file.open(command[numSelect+2].c_str());
        if(!file.is_open()){
            cout << "!Failed to query table " << command[3] << " because it does not exist." << endl;
        }
        else if(command[numSelect+3] == "WHERE"){
            //read through file into a vector
            vector<string> lines;
            string fileLine;
            while (getline(file, fileLine)){
                lines.push_back(fileLine);
            }
            //determin how many variables are in the first line of the vector
            int numVars = 0;
            for(int k = 0; k<lines[0].length(); k++){
                if(lines[0][k] == '['){
                    numVars++;
                }
            }
            //determin which variables are being selected
            int select[numSelect];
            int count = 0;
            for(int k = 1; k<numSelect+1; k++){
                if(command[k] != "FROM"){
                    for(int l = 0; l<numVars; l++){
                        if(command[k] == lines[0].substr(1+((l*8)+l), 3)){
                            select[count] = l;
                            count++;
                        }
                    }
                }
            }
            //determin which variable is being compared
            int compare;
            for(int k = 0; k<numVars; k++){
                if(command[numSelect+4] == lines[0].substr(1+((k*8)+k), 3)){
                    compare = k;
                }
            }
            //determin which operator is being used
            string op;
            if(command[numSelect+5] == "="){
                op = "=";
            }
            else if(command[numSelect+5] == ">"){
                op = ">";
            }
            else if(command[numSelect+5] == "<"){
                op = "<";
            }
            else if(command[numSelect+5] == ">="){
                op = ">=";
            }
            else if(command[numSelect+5] == "<="){
                op = "<=";
            }
            else if(command[numSelect+5] == "!="){
                op = "!=";
            }
            //determin which values are being compared
            string value = command[numSelect+6];
            //print out the vector
            for(int k = 0; k<lines.size(); k++){
                if(op == "="){
                    if(lines[k].substr(1+((compare*8)+compare), 3) == value){
                        for(int l = 0; l<numSelect; l++){
                            cout << lines[k].substr(1+((select[l]*8)+select[l]), 3) << " ";
                        }
                    }

                }
            }            
        }
        else{
            //read through file into a vector
            vector<string> lines;
            string fileLine;
            while (getline(file, fileLine)){
                lines.push_back(fileLine);
            }
            //determin how many variables are in the first line of the vector
            int numVars = 0;
            for(int k = 0; k<lines[0].length(); k++){
                if(lines[0][k] == '['){
                    numVars++;
                }
            }
            //determin which variables are being selected
            int select[numSelect];
            int count = 0;
            for(int k = 1; k<numSelect+1; k++){
                if(command[k] != "FROM"){
                    for(int l = 0; l<numVars; l++){
                        if(command[k] == lines[0].substr(1+((l*8)+l), 3)){
                            select[count] = l;
                            count++;
                        }
                    }
                }
            }
            //print out the selected variables
            for(int k = 0; k<lines.size(); k++){
                for(int l = 0; l<numSelect; l++){
                    cout << lines[k].substr(1+((select[l]*8)+select[l]), 3) << " ";
                }
                cout << endl;
            }
        }
    }
}



void alter(string command[]){
    //opens file using command[2] as the name
    //if file does not exist, prints error
    ifstream file;
    file.open(command[2].c_str());
    if(!file.is_open()){
        cout << "!Failed to modify table " << command[3] << " because it does not exist." << endl;
    }
    //if command[3] is ADD, add the variable as a column to the file
    else if(command[3] == "ADD"){
        vector<string> lines;
        string fileLine;
        //read lines and push into vector
        while (getline(file, fileLine)){
            lines.push_back(fileLine);
        }
        //determin how many variables are in the first line of the vector
        int numVars = 0;
        for(int k = 0; k<lines[0].length(); k++){
            if(lines[0][k] == '['){
                numVars++;
            }
        }
        //append the first line of the vector with the new variable
        lines[0] = lines[0] + "[" + command[4] + ", " + command[5] + "] ";
        //open file and write the vector to the overwriting the old file
        ofstream file;
        file.open(command[2].c_str());
        for(int k = 0; k<lines.size(); k++){
            file << lines[k] << endl;
        }
        cout << "Table " << command[2] << " modified." << endl;
    }
}



void insert(string command[]){
    //opens file using command[3] as the name
    //if file does not exist, prints error
    ifstream file;
    file.open(command[3].c_str());
    if(!file.is_open()){
        cout << "!Failed to insert into table " << command[3] << " because it does not exist." << endl;
    }
    else{
        vector<string> lines;
        string fileLine;
        //read lines and push into vector
        while (getline(file, fileLine)){
            lines.push_back(fileLine);
        }
        //determin how many variables are in the first line of the vector
        int numVars = 0;
        for(int k = 0; k<lines[0].length(); k++){
            if(lines[0][k] == '['){
                numVars++;
            }
        }
        //use vector insert to add all new variables to the vector each variable being inside []
        for(int k = 0; k<numVars; k++){
            lines.insert(lines.begin()+k+1, "[" + command[k+4] + "]");
        }
        //write the vector back into the file overwriting the old file
        ofstream file;
        file.open(command[3].c_str());
        for(int k = 0; k<lines.size(); k++){
            file << lines[k] << endl;
        }
        cout << "1 new record inserted." << endl;
    }
}



void update(string command[]){
    //opens file using command[1] as the name
    //if file does not exist, prints error
    ifstream file;
    file.open(command[1].c_str());
    if(!file.is_open()){
        cout << "!Failed to update record(s) " << command[1] << " because it does not exist." << endl;
    }
    else{
        vector<string> lines;
        string fileLine;
        //read lines and push into vector
        while (getline(file, fileLine)){
            lines.push_back(fileLine);
        }
        //determin how many variables are in the first line of the vector
        int numVars = 0;
        for(int k = 0; k<lines[0].length(); k++){
            if(lines[0][k] == '['){
                numVars++;
            }
        }
        //determin which variable is being updated
        int update;
        for(int k = 0; k<numVars; k++){
            if(command[3] == lines[0].substr(1+((k*8)+k), 3)){
                update = k;
            }
        }
        //determin which rows of the vector is being updated
        int compare;
        for(int k = 0; k<numVars; k++){
            if(command[5] == lines[0].substr(1+((k*8)+k), 3)){
                compare = k;
            }
        }
        //update the rows
        for(int k = 1; k<lines.size(); k++){
            if(lines[k].substr(1+((compare*8)+compare), 3) == command[7]){
                lines[k] = lines[k].replace(1+((update*8)+update), 3, command[9]);
            }
        }
        //write the vector back into the file overwriting the old file
        ofstream file;
        file.open(command[1].c_str());
        for(int k = 0; k<lines.size(); k++){
            file << lines[k] << endl;
        }
        //return how many rows were updated
        int count = 0;
        for(int k = 1; k<lines.size(); k++){
            if(lines[k].substr(1+((compare*8)+compare), 3) == command[7]){
                count++;
            }
        }
        cout << count << " record(s) modified." << endl;
    }
}



void dele(string command[]){
    //opens file using command[2] as the name
    //if file does not exist, prints error
    ifstream file;
    file.open(command[1].c_str());
    if(!file.is_open()){
        cout << "!Failed to delete record(s) " << command[1] << " because it does not exist." << endl;
    }
    else{
        vector<string> lines;
        string fileLine;
        //read lines and push into vector
        while (getline(file, fileLine)){
            lines.push_back(fileLine);
        }
        //determin how many variables are in the first line of the vector
        int numVars = 0;
        for(int k = 0; k<lines[0].length(); k++){
            if(lines[0][k] == '['){
                numVars++;
            }
        }
        //determin which variable is being compared
        int compare;
        for(int k = 0; k<numVars; k++){
            if(command[4] == lines[0].substr(1+((k*8)+k), 3)){
                compare = k;
            }
        }
        //determin which rows of the vector is being compared
        vector<int> rows;
        for(int k = 0; k<lines.size(); k++){
            if(command[5] == "="){
                if(command[6] == lines[k].substr(1+((compare*8)+compare), 3)){
                    rows.push_back(k);
                }
            }
            else if(command[5] == ">"){
                if(command[6] < lines[k].substr(1+((compare*8)+compare), 3)){
                    rows.push_back(k);
                }
            }
            else if(command[5] == "<"){
                if(command[6] > lines[k].substr(1+((compare*8)+compare), 3)){
                    rows.push_back(k);
                }
            }
        }
        //delete the rows from the vector
        for(int k = 0; k<rows.size(); k++){
            lines.erase(lines.begin()+rows[k]-k);
        }
        //write the vector back into the file overwriting the old file
        ofstream file;
        file.open(command[1].c_str());
        for(int k = 0; k<lines.size(); k++){
            file << lines[k] << endl;
        }
        cout << rows.size() << " records deleted." << endl;
    }
}