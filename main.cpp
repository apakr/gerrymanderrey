//
//  Gerrymanderrey
//  Al Pakrosnis
//  
//  Windows 10, VS Code
//

#include <vector> // data structure used to store the information
#include <string> 
#include <iostream>
#include <fstream> // to read from a file
#include <math.h> // for absolute value
#include <sstream> // to read from a file into strings
#include <iomanip> // for stoi

using namespace std;

struct searchedState { // structure that represents a state: used in a vector to hold all of the read information from files
    bool isLoaded = false;
    string state;
    int votersEligible = 0;
    int totalVotes = 0;
    int districts = 0; 
    vector<int> districtDemVoters;
    vector<int> districtRepVoters;
    bool isGerry;
    int wastedDem = 0;
    int wastedRep = 0;

};

searchedState structReset(){ // helper function that is used to reset individual searchedState objects within the main vector
    searchedState newOne;
    return newOne;
}

bool isGerry(int districts, double wastedDem, double wastedRep, double totalVotes){ // helper function that calculates whether a state is gerrymandered & returns a bool
    double dem = wastedDem/totalVotes;
    double rep = wastedRep/totalVotes;
    double gap = fabs(dem-rep); // use of math.h
    if(districts < 3){
        return false;
    } else if (gap > .07) {
        return true;
    } else if (gap <= .07){
        return false;
    } // the control reaches the end of this function without seeing a return statement but that doesn't matter since the if-statement before this will always have one of its conditons true
}

int findOverHalf(double demVotes, double repVotes, double total){ // helper function in calculating the wasted votes within a district
    double almostOverHalf = (ceil((double(total))/2));
    if(int(total)%2 == 1){
        return int(almostOverHalf);
    } else {
        return int(almostOverHalf) + 1;
    } 
}

int calculateWasted(int demVotes, int repVotes, string party){ // a helper function to the primarily load function that does the total calculating of wasted votes
    int wasted;
    int total = demVotes+repVotes;
    int overHalf = findOverHalf(demVotes, repVotes, total); // a call to a helper function within a helper function
    if(party == "dem"){
        if(demVotes>repVotes){
            wasted = demVotes-overHalf;
        } else if (repVotes>demVotes){
            wasted = demVotes;
        }
    } else if(party == "rep"){
        if(demVotes>repVotes){
            wasted = repVotes;
        } else if (repVotes>demVotes){
            wasted = repVotes-overHalf;
        }
    }
    return wasted;
}

string makeNormalCase(string theWord){ // helper function that takes any string and turns it into "normal case" - essentially capitalizes the first letter of each word and makes the rest of the letters lowercase
    theWord[0] = toupper(theWord[0]);
    for(int i = 1; i < theWord.length(); i++){
        if(theWord[i-1] == ' '){
            theWord[i] = toupper(theWord[i]);
        } else {
            theWord[i] = tolower(theWord[i]);
        }
    }
    return theWord;
} // the importance of this function is in the fact that it allows for capitalization format of the input data be insensitive

bool isStateInVector(vector<searchedState> states, string state){ // helper function to a couple of other function - returns a true or false value based on if the given state is represented within the main vector
    for(int i = 0; i < states.size(); i++){
        if(makeNormalCase(states[i].state) == makeNormalCase(state)){
            return true;
        }
    }
    return false;
}

void loadFunction(vector<searchedState> &states, string files, bool &isFirstFileLoaded, bool &isSecondFileLoaded){ // this is the proverbial "meat" of the code, wherein the bulk of the math and vector usage and file reading occurs
    if(isFirstFileLoaded || isSecondFileLoaded){
        cout << "Already read data in, exit and start over." << endl << endl;
        return;
    }
    ifstream inFS; // declares inFS which will read open, read, and close the files
    files = files.substr(files.find(" ")+1, files.length());
    string file1 = files.substr(0,files.find(" "));
    string file2 = files.substr(files.find(" ")+1, files.length());
    inFS.open(file1);
    if(!inFS.is_open()){ // checks to make sure if the file was opened properly
        cout << "Invalid first file, try again." << endl << endl;
        return;
    }
    cout << "Reading: " << file1 << endl;
    string tempLine;
    searchedState tempSearchedState; // creates a temporary state struct which will be used to copy all the data over to the main vector
    while(!inFS.eof()){ // goes through until the end of the file and takes each line as input which will be refined for its data
        getline(inFS,tempLine);
        if(tempLine == ""){
            break;
        }
        tempSearchedState.state = tempLine.substr(0,tempLine.find(","));
        cout << "..." << tempSearchedState.state << "...";
        tempLine = tempLine.substr(tempLine.find(",")+1,tempLine.length()).append(",");

        while(tempLine.length() != 0){ // main chunk of code that refines each line and stores a state's data into its correct position within the main struct vector for later analysis 
            tempSearchedState.districts++;
            tempLine = tempLine.substr(tempLine.find(",")+1,tempLine.length());
            tempSearchedState.districtDemVoters.push_back(stoi(tempLine.substr(0,tempLine.find(","))));
            tempSearchedState.totalVotes+=stoi(tempLine.substr(0,tempLine.find(","))); // use of stoi
            tempLine = tempLine.substr(tempLine.find(",")+1,tempLine.length());
            tempSearchedState.districtRepVoters.push_back(stoi(tempLine.substr(0,tempLine.find(","))));
            tempSearchedState.totalVotes+=stoi(tempLine.substr(0,tempLine.find(",")));
            tempLine = tempLine.substr(tempLine.find(",")+1,tempLine.length());
        }
        cout << tempSearchedState.districts << " districts total" << endl;
        for(int i = 0; i < tempSearchedState.districts; i++){ // prepares the data from the files to be calculated to figure out if a state is gerrymandered or not
            tempSearchedState.wastedDem += calculateWasted(tempSearchedState.districtDemVoters[i], tempSearchedState.districtRepVoters[i], "dem");
            tempSearchedState.wastedRep += calculateWasted(tempSearchedState.districtDemVoters[i], tempSearchedState.districtRepVoters[i], "rep");
        }
        tempSearchedState.isGerry = isGerry(tempSearchedState.districts, tempSearchedState.wastedDem, tempSearchedState.wastedRep, tempSearchedState.totalVotes);
        states.push_back(tempSearchedState); // adds the temp struct to the vector
        tempSearchedState = structReset(); // then resets the tempState to be used again
    }
    cout << endl;
    inFS.close(); // closes the file so that the stream can be properly used to read from another file
    isFirstFileLoaded = true;
    inFS.open(file2);
    if(!inFS.is_open()){ // makes sure that the file was opened properly 
        cout << "Invalid second file, try again." << endl << endl;
        return;
    }
    cout << "Reading: " << file2 << endl;
    while(!inFS.eof()){ // runs through the whole file till the end getting each line individually prepared for further data refinement
        getline(inFS,tempLine);
        if(tempLine == ""){
            break;
        }
        string tempState = tempLine.substr(0,tempLine.find(","));
        tempLine = tempLine.substr(tempLine.find(",")+1,tempLine.length());
        if(isStateInVector(states, tempState)){
            cout << "..." << tempState << "..." << tempLine << " eligible voters" << endl;
        }
        for(int j = 0; j <states.size()-1; j++){ // goes through each state and stores the number of eligible voters from the file into its respective spot in the main vector
            if(states[j].state == tempState){ // only instance of a cyclomatic complexity above 2 in all the code, a new function could be made to reduce the complexity, however, I decided against it because another function, that is only used once and only replaces three lines, arguably adds more complexity than this simple if statement
                states[j].votersEligible = stoi(tempLine); // stoi again
                break;
            }
        }
    }
    cout << endl;
    inFS.close();
    isSecondFileLoaded = true;
    for(int k = 0; k <states.size()-1; k++){ // goes through and checks to make sure that every state that was created from the first file read that didn't have a corresponding entry in the second file read is not kept in the data set, and is thus reset within the vector
        if(states[k].votersEligible == 0){
            states[k] = structReset(); 
        }
    }
    return;
}


string isDataLoaded(bool firstFile, bool secondFile){ // helper function that quickly presents a string that can be used for terminal input to state whether data has been loaded; the inclusion of a separate function for this reduces the demand on the main vector file
    string answer = "No";
    if(firstFile && secondFile){
        answer = "Yes";
    }
    return answer;
}

void searchFunction(vector<searchedState> &states, string state, string &currentlyLoaded, bool isFirstFileLoaded, bool isSecondFileLoaded){ // a main function that is used to search for a state within the data set and prepare it for analysis
    string stateName = state.substr(state.find(" ") + 1, state.length());
    bool inVector = isStateInVector(states, stateName); 
    if(!isFirstFileLoaded){
        cout << "No data loaded, please load data first." << endl << endl;
        return;
    }
    if(inVector){
        currentlyLoaded = makeNormalCase(stateName);
    } else {
        cout << "State does not exist, search again." << endl << endl;
    }
}

int findPosition(vector<searchedState> states, string state){ // helper function which looks through the main struct vector and returns the positon of the state that is passed to it: used in a couple of other functions, quite useful and important
    int position;
    for(int i = 0; i < states.size(); i++){
        if(makeNormalCase(states[i].state) == makeNormalCase(state)){ // using makeNormalCase here makes sure that the program is fully case-insensitive, in case the input files have non capitalized state names. The state variable is already properly capitalized
            position = i;
            break;
        }
    }
    return position;
}

void yesGerried(vector<searchedState> states, int position){ // is a helper function to the stats function and prints out information on whether a state is gerrymandered or not & against which party
    string against;
    double dem = double(states[position].wastedDem)/double(states[position].totalVotes);
    double rep = double(states[position].wastedRep)/double(states[position].totalVotes);
    double gap = 100*fabs(dem-rep);
    if(dem > rep){
        against = "Democrats";
    } else if (rep > dem){
        against = "Republicans";
    }
    cout << "Gerrymandered: Yes" << endl;
    cout << "Gerrymandered against: " << against << endl;
    cout << "Efficiency Factor: " << fixed << setprecision(4) << gap << "%" << endl;
}

void statsFunction(vector<searchedState> &states, string state, bool isSecondFileLoaded){ // a primary function that shows the stats for a selected state on the topic of gerrymandering, as well as the proportion of wasted votes
    if(!isSecondFileLoaded){
        cout << "No data loaded, please load data first." << endl;
        return;
    }
    if(state == "N/A"){
        cout << "No state indicated, please search for state first." << endl;
        return;
    }
    int position = findPosition(states, state);
    string Gerry;
    if(states[position].isGerry){
        yesGerried(states, position);
    } else {
        Gerry = "No";
        cout << "Gerrymandered: " << Gerry << endl;
    }
    cout << "Wasted Democratic votes: " << states[position].wastedDem << endl;
    cout << "Wasted Republican votes: " << states[position].wastedRep << endl;
    cout << "Eligible voters: " << states[position].votersEligible << endl << endl;
}

void printPlot(vector<searchedState> states, int position, int district){ // helper function that actually does the printing of plots for the plot function
    double demVotes = states[position].districtDemVoters[district];
    double repVotes = states[position].districtRepVoters[district];
    int numOfD = int(floor(100*(demVotes/(demVotes+repVotes)))); // determines the number of D's to print based on some math, also uses the math.h libary
    int numOfR = 100-numOfD;
    string plot;
    for(int i = 1; i <= numOfD; i++){
        plot.append("D");
    }
    for(int j = 1; j <= numOfR; j++){
        plot.append("R");
    }
    cout << plot << endl;
}

void plotFunction(vector<searchedState> &states, string state, bool firstLoaded, bool secondLoaded){ // primary function that plots the distribution of votes within districts and allows for nuanced visual analysis of the data presented
    if(!(firstLoaded && secondLoaded)){
        cout << "No data loaded, please load data first." << endl;
        return;
    }
    int position = findPosition(states, state);
    for(int i = 0; i < states[position].districts; i++){ 
        cout << "District: " << i+1 << endl;
        printPlot(states, position, i);
    }
    cout << endl;
}

int main() {
    int exitStatus = 1; // declaration of all the variables that are useful within the main function, including the main struct vector, as well as other varaiables that keep track of the state of the program, like if files are loaded or if the main while loop should still be running
    bool isFirstFileLoaded = false;
    bool isSecondFileLoaded = false;
    string commandFull;
    string command;
    vector<searchedState> states;
    string currentlyLoaded = "N/A";

    cout << "Welcome to the Gerrymandering App!\n" << endl;
    while(exitStatus){ // while loop that runs through the course of the program and allows for data ananlysis of gerrymandering
        cout << "Data Loaded? " << isDataLoaded(isFirstFileLoaded, isSecondFileLoaded) << endl; 
        cout << "State: " << currentlyLoaded << endl << endl; 
        cout << "Enter command: ";
        getline(cin, commandFull);
        command = commandFull.substr(0, commandFull.find(" "));
        cout << endl << "-----------------------------\n" << endl;
        if(command == "load"){
            loadFunction(states, commandFull, isFirstFileLoaded, isSecondFileLoaded);
        } else if(command == "search"){
            searchFunction(states, commandFull, currentlyLoaded, isFirstFileLoaded, isSecondFileLoaded);
        } else if(command == "stats"){
            statsFunction(states, currentlyLoaded, isSecondFileLoaded);
        } else if(command == "plot"){
            plotFunction(states, currentlyLoaded, isFirstFileLoaded, isSecondFileLoaded);
        } else if(command == "exit"){
            break;
        } else {
            cout << "Error: No such command, try again" << endl;
        }
    }
    return 0;
}