#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
using namespace std;

const int NMISP = 5;
const int RNG_RANGE = 100;

void display_header(const int ndict, const int nmiss) {
  
  // Prints out the header at the start of the program and shows the number of words in the dictionary and mispelled set
  cout << "*** Typing Tutor" << endl;
  cout << "There are " << ndict << " words in the dictionary." << endl;
  cout << "There are " << nmiss << " commonly misspelled words." << endl;
  
}

void display_menu_options() {

  // Displays all menu options avaiable 
  cout << endl
       << "*** Select a menu option:" << endl
       << "1. Spell commonly misspelled words" << endl
       << "2. Type random words from the full dictionary" << endl
       << "3. Display some words from one of the lists" << endl
       << "4. Use binary search to lookup a word in the dictionary" << endl
       << "5. Exit the program." << endl
       << "Your choice: ";
  
}

vector<int> randn(const int n, const int max) {

  /* Returns a vector of random numbers based on the number of inputs given in the parameter
     which are each between 0 and the max value provided in the parameter. 
  */
  vector<int> randomNums;

  for (int i = 0; i < n; i++) {
    int rnum = rand() % (max+1); // removed + 1 here
    randomNums.push_back(rnum);
  }

  return randomNums;
}

ifstream open(const string &src_file) {
  // Opens and returns the object of the src file
  ifstream src{src_file};
  if (!src.is_open()) {
    cout << "Could not find file " << src_file << "." << endl;
    exit(1);
  }
  return src;
}

void extract_dict(vector<string> &dst, const string &src_file) {
  /* Opening the file provided in parameter using the open() function and
     storing in variable obj.
  */
  ifstream obj;
  obj = open(src_file);

  // Iterating and storing each line from the file in the dictionary provided in parameter.
  for (string line; getline(obj, line);) {
    dst.push_back(line);
  }
}

void extract_misspelled(vector<string> &misspelled, vector<string> &correct,
                        const string &src_file) {
  
  // Opening + storing the file provided in variable obj
  ifstream obj;
  obj = open(src_file);

  // Iterating through each line and seperating the correct vs incorrect
  // spellings in the file provided
  for (string line; getline(obj, line);) {

    int pos = 0;

    for (int i = 0; i < line.size(); i++) {
      if (line[i] == ' ') {
        pos = i;
      }
    }

    // Adding the incorrectly spelled and correctly spelled words to their
    // respective vectors.
    misspelled.push_back(line.substr(0, pos));
    correct.push_back(line.substr(pos + 1, (line.size() - pos)));
    
  }
  
}

void display_elems(const vector<string> &src, const vector<int> &positions) {

  /* Prints the elements of the vector provided in parameter src
     at the indices in the positions vector in parameter
  */
  
  for (int i = 0; i < positions.size(); i++) {
    int pos = positions.at(i);
    
    if (i != (positions.size() - 1)) {
      
      cout << src.at(pos) << " ";
      
    } 
    else {
      cout << src.at(pos);
    }
    
  }
  
}

void extract_spelling_answers(vector<string> &dst,
                              const vector<string> &correct,
                              const vector<int> positions) {

  /* Copies the elements at the indices provided in the positions vector 
     from the correct vector into the dst vector. 
  */
  
  for (int i = 0; i < positions.size(); i++) {

    int indx = positions.at(i);
    dst.push_back(correct.at(indx));
  }
  
}

void extract_spelling_mistakes(vector<string> &dst, const string &to_grade,
                               const vector<string> &correct) {

  /* Populates vector dst provided in parameter with spelling errors found
     from comparing the user's input to elements in the correct vector
  */
  
  int lastPos = 0;
  string word;
  vector<string> userAnswers;

  for (int i = 0; i < to_grade.size(); i++) {

    if (to_grade[i] == ' ') {
      word = to_grade.substr(lastPos, (i - lastPos));
      lastPos = i + 1;
      userAnswers.push_back(word);
    }
    if (i == (to_grade.size() - 1)) {
      string lastWord = to_grade.substr(lastPos, i - (lastPos - 1));
      userAnswers.push_back(lastWord);
    }
  }

  for (int x = 0; x < userAnswers.size(); x++) {
    if (userAnswers.at(x) != correct.at(x)) {
      string mistakeAndAnswer = userAnswers.at(x) + " -> " + correct.at(x);
      dst.push_back(mistakeAndAnswer);
    }
  }
}

void report_misspell_mistakes(const vector<string> &mistakes,
                              const int num_words) {

  
  /* Uses the vector provided in parameter to print the spelling mistake and
     grading to the user. 
  */
  
  if(mistakes.size() == 0){
    cout << "No misspelled words!" << endl;
    cout << num_words * 3 << " points: " << num_words << " spelled correctly x 3 points for each." << endl;
    cout << "0 point penalty: 0 spelled incorrectly x -6 points for each." << endl;
    cout << "Score: " << num_words * 3 << endl;
  }
  else{
    cout << "Misspelled words:" << endl;
    for(int i = 0; i < mistakes.size(); i++){
      cout << "    " << mistakes.at(i) << endl;
    }
    int incorrect = mistakes.size();
    int correct = num_words - incorrect;

    int points = correct * 3; 
    int penalty = incorrect * 6; 

    cout << points << " points: " << correct << " spelled correctly x 3 points for each." << endl;
    cout << penalty << " point penalty: " << incorrect << " spelled incorrectly x -6 points for each." << endl;
    cout << "Score: " << (points - penalty) << endl;
  }

}

void evaluate_user_misspells(const vector<string> &src,
                             const vector<int> &positions) {

  /* Reads users input for typing tests and calls functions to 
     find and report mistakes made in the user's answers.
  */
  
  string input;
  getline(cin, input);

  vector<string> mistakes;
  vector<string> answers;

  extract_spelling_answers(answers, src, positions);
  extract_spelling_mistakes(mistakes, input, answers);
  report_misspell_mistakes(mistakes, answers.size());
  
}

void misspelled(const vector<string> &mspelled, const vector<string> &correct) {

  /* Generates and displays commonly misspelled words and calls
     evaluate_user_misspells() to evaluate the user's performance. 
  */
  
  cout << endl << "*** Using commonly misspelled words." << endl;
  vector<int> randNums;

  randNums = randn(5, (mspelled.size()-1));
  display_elems(mspelled, randNums);
  cout << endl << "Type the words above with the correct spelling:" << endl;

  evaluate_user_misspells(correct, randNums);
}

void full_dict(const vector<string> &dict) {

  /* Prints the starting message for option 2 and displays
     5 random words from the full dictionary to spell and grades the
     user's answers with calls to the evaluate_user_misspells() function. 
  */
  
  cout << endl << "*** Using full dictionary." << endl;
  vector<int> randNums; 
  vector<string> correct;

  randNums = randn(5, (dict.size()-1));

  for(int i = 0; i < randNums.size(); i++){
    correct.push_back(dict.at(randNums.at(i)));
  }

  
  display_elems(dict, randNums);
  cout << endl << "Correctly type the words above:" << endl;
  evaluate_user_misspells(dict, randNums);

  
}

void display_words_prompt() {
  
  // Displays the data set choices for menu option 3
  
  cout << endl
       << "*** Displaying word list entries." << endl
       << "Which words do you want to display?" << endl
       << "A. Dictionary of all words" << endl
       << "B. Commonly misspelled words (wrongly spelled)" << endl
       << "C. Commonly misspelled words (correctly spelled)" << endl
       << "Your choice: ";
}

void print_range(const vector<string> &data) {

  /* Prompts the user for indices entry for menu option 3 
     and checks if the indices match appropriate properties and then
     prints out the words in the index range. 
  */
  
  int firstIndex; 
  int secondIndex;
  int range; 
  
  cout << "*** Enter the start and end indices between 0 and " << (data.size()-1) << ": ";
  range = data.size() - 1;

  cin >> firstIndex;
  cin >> secondIndex;

  // should be firstIndex > secondIndex inclusive or exclusive?
  if((firstIndex >= secondIndex) || ((firstIndex < 0 || firstIndex > range) || (secondIndex > range||secondIndex<0))){
    cout << "The end index must come after the start, and the indices must be in range." << endl;
  }
  else{
    for(int i = firstIndex; i <= secondIndex; i++){
      cout << i << ". " << data.at(i) << endl;
    }
  }

  
}

void display_in_range(const vector<string> &dict,
                      const vector<string> &mspelled,
                      const vector<string> &correct) {

  /* Prints the message for menu option 3 and calls print_range 
     with the appropriate data based on the user input for what 
     data set to use. 
  */
  
  char userInput;
  int firstIndex; 
  int secondIndex; 
  int range; 

  display_words_prompt();

  cin >> userInput;

  if(userInput == 'A' || userInput == 'a'){
    print_range(dict);
  }
  else if(userInput == 'B' || userInput == 'b'){
    print_range(mspelled);
  }
  else if(userInput == 'C' || userInput == 'c'){
    print_range(correct);
  }

  
}

void bsearch(const vector<string> &data) {

  /* Asks user for an input word to search for and runs a binary search 
     across the data set provided in parameter to search for the target word
     while printing out each element checked. 
  */
  
  string target; 
  cout << "*** Enter word to search: " << setw(1);
  cin >> target;

  int start = 0;
  int size = data.size(); 
  int mid;
  int counter = 1;
  string elem; 
  bool found = false;

  while(start < size){

    mid = (start + size) / 2;
    elem = data.at(mid);
    cout << setw(5) << counter << ". " << "Comparing to: " << elem << endl;
    counter += 1;

    if(elem < target){
      start = mid + 1;
    }
    else if(elem > target){
      size = mid; 
    }
    else if(elem == target){
      cout << target << " was found.";
      found = true;
      break;
    }
    
  }

  if(found == false){
    cout << target << " was NOT found.";
  }

  
  
}

void extract_all_data(vector<string> &dict, vector<string> &mspelled,
                      vector<string> &correct) {
  extract_dict(dict, "dictionary.txt");
  extract_misspelled(mspelled, correct, "misspelled.txt");
}

int main() {
  srand(1);
  vector<string> dict, mspelled, correct;
  extract_all_data(dict, mspelled, correct);
  display_header(dict.size(), correct.size());
  unsigned int choice;
  do {
    display_menu_options();
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (choice) {
    case 1:
      misspelled(mspelled, correct);
      break;
    case 2:
      full_dict(dict);
      break;
    case 3:
      display_in_range(dict, mspelled, correct);
      break;
    case 4:
      bsearch(dict);
      break;
    case 5:
      cout << "Exiting." << endl;
      break;
    default:
      cout << "No menu option " << choice << ". "
           << "Please try again." << endl;
    }
  } while ((choice != 5) && !cin.eof());

  return 0;
}