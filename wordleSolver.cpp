#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <utility>
#include <random>

using namespace std;

// initialise message strings (for functions outside of main)
const string messageAddPairCheck {"Press ENTER to add another pair, type SPACE + ENTER to finish"};

/** Take each line of a text file, and save it to a vector
*
* function: loadWords
* param: null
* return: vector<string>
*/
vector<string> loadWords() {
    // Open the file
    ifstream file("words.txt");

    // Check if the file is open
    if (!file.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
    }

    // Vector to store the words
    vector<string> words;
    string word;

    // Read each word from the file and store it in the vector
    while (file >> word) {
        words.push_back(word);
    }

    // Close the file
    file.close();

    return words;
}

/** Print each string in a given vector line by line
*
* function: showWordList
* param: vector<string> words
* return: null
*/

void showWordList (const vector<string>& words) {
    cout << "Remaining possible words:" << endl;
    for (const string& word: words) {
        cout << word << ",\n";
    }
    cout << endl;
}

/** Show count of given vector of strings,
* with title: "Words left: "
*
* function: showWordCount
* param: vector<string> words
* return: null
*/
void showWordCount (const vector<string>& words) {
    cout << "Words left: ";
    cout << words.size() << endl;
    cout << endl;
}

/** Remove all strings from vector filteredWords, based on the strings in vector wordsToCull
*
* function: cull
* param: vector<string> wordsToCull, vector<string> filteredWords
* return: null (values modified by reference)
*/
void cull (vector<string>& wordsToCull, vector<string>& filteredWords) {
    unordered_set removeSet(wordsToCull.begin(), wordsToCull.end());

    filteredWords.erase(ranges::remove_if(filteredWords,[&removeSet](const string& word) {
        return removeSet.contains(word);
    }).begin(), filteredWords.end());

    wordsToCull = {};
}

/** Filter user input to save yellow letters to a list
 * yellowTriple struct is used instead of using a tuple for readability
*
* function: filterInputYellow 
* param: string input
* return: vector<yellowTriple> yellowTriples
*/

struct yellowTriple {
    int pos;
    char letter;
    int count;
};

vector<yellowTriple> filterInputYellow(const string& input) {
    vector<yellowTriple> yellowTriples {};

    for (int charPos {0}; const char& letter: input) {
        if (letter == ' ') {
            // skip
        } else {
            auto letterCount = ranges::count(input, letter);
            yellowTriples.emplace_back(charPos, letter, letterCount);
        }
        charPos++; // iterator for char position
    }

    return yellowTriples;
}

/** Filter user input to save green letters to a list
*
* function: filterInputGreen
* param: string input
* return: vector<pair<int, char>> listOfGreenPairs
*/
vector<pair<int, char>> filterInputGreen(const string &input) {
    vector<pair<int, char>> listOfGreenPairs;

    for (int charPos {0}; const char& letter: input) {
        if (letter == ' ') {
            // skip
        } else {
            listOfGreenPairs.emplace_back(charPos, letter);
        }
        charPos++; // iterator for char position
    }

    return listOfGreenPairs;
}

/** Display a list of remaining possible words, based on the user's input, for the Wordle game,
 * or generate a random starter word
 * (based on the Wordle list of words from 2024)
*
* function: main
* param: null
* return: null
*/
int main() {

    // initialise message strings
    const string messageGrey {"Enter new GREY letters\n"
                              "(DO NOT include letters that are green/yellow somewhere else in the word)\n"
                              "(If there are no new grey letters, just press ENTER)"};
    const string messageGreen{"Enter new GREEN letters:\n"
                              "Type letters out in order, using spaces for non-green letters\n"
                              "example: word is 'hello' where 'e' & 'o' are green = ' e  o'\n"
                              "(If there are no new green letters, just press ENTER)"};
    const string messageYellow{"Enter new YELLOW letters:\n"
                              "Type letters out in order, using spaces for non-yellow letters\n"
                              "example: word is 'hello' where 'e' & 'o' are yellow = ' e  o'\n"
                              "(If there are no new green letters, just press ENTER)"};

    const string messageLoading {"loading..."};
    const string messageChoice {"Welcome to the wordle solver!\n"
                                "w -> add a word\n"
                                "r -> reset words\n"
                                "s -> generate starter word\n"
                                "q -> quit"};



    // get words from text file, then make a modifiable copy of the list
    const vector<string> words = loadWords();
    vector<string> filteredWords = words;
    vector<string> wordsToCull {};

    // Start menu, loop until user quits
    string input;

    cout << "Total possible words: ";
    cout << filteredWords.size() << endl;
    cout << messageChoice << endl;

    getline(cin, input);
    char choice = input[0];

    while (choice != 'q') {
        if (choice == 'w') {
            // get grey letters, then remove all words that contain them
            cout << messageGrey << endl;
            getline(cin, input);

            cout << messageLoading << endl;
            for (const char letter: input) {
                for (const string& word: filteredWords) {
                    if (word.find(letter) != string::npos) {
                        wordsToCull.push_back(word);
                    }
                }
            }

            cull(wordsToCull, filteredWords);
            showWordCount(filteredWords);

            // get green letters, with position, erase if green letter is not in word + correct pos
            vector<pair<int, char>> listOfGreenPairs;

            string inputGreen {};
            cout << messageGreen << endl;
            getline(cin, inputGreen);
            listOfGreenPairs = filterInputGreen(inputGreen);

            //getInputPairs(messageGreen, messageGreenLoop, listOfGreenPairs);

            if (!listOfGreenPairs.empty()) {
                cout << messageLoading << endl;
                for (string& word: filteredWords) {
                    for (const auto& p: listOfGreenPairs) {

                        // Creates a checker. Once this value is true, it will stop the loop as the word is correct,
                        // if the value is false by the end of the loop, the word will be deleted
                        bool checker {false};
                        // find char pos
                        size_t charPos = word.find(p.second);
                        // while char is still found in word
                        while (charPos != string::npos && checker == false) {
                            if (charPos == p.first) {
                                checker = true;
                            } else {
                                checker = false;
                            }
                            // update char pos, checking after current pos
                            charPos = word.find(p.second, (charPos + 1));
                        }
                        if (checker == false) {
                            wordsToCull.push_back(word);
                            break;
                        }
                    }
                }
                cull(wordsToCull, filteredWords);
                showWordCount(filteredWords);
            }

            // get yellow letters, with position, erase if yellow letter is in the same position as the same char as the word
            vector<yellowTriple> yellowTriples;

            string inputYellow {};
            cout << messageYellow << endl;
            getline(cin, inputYellow);

            yellowTriples = filterInputYellow(inputYellow);

            //getInputTriples(messageYellow, messageYellowLoop, yellowTriples);

            if (!yellowTriples.empty()) {
                cout << messageLoading << endl;
                for (string& word: filteredWords) {
                    for (const auto& yellowLetter: yellowTriples) {

                        // Get count of yellowLetter occurrence in word
                        int posCounter {0};
                        size_t charPos = word.find(yellowLetter.letter);
                        while (charPos != string::npos) {
                            posCounter++;
                            // update char pos, checking after current pos
                            charPos = word.find(yellowLetter.letter, (charPos + 1));
                        }

                        // If occurrence of yellowLetter in word is not == yellowLetter count (or the default of 1), cull word
                        if (posCounter < (yellowLetter.count + ranges::count(inputGreen, yellowLetter.letter))) {
                            wordsToCull.push_back(word);
                            break;
                        }

                        // If occurrence of yellowLetter == yellowLetter count, then check the positioning of letter/s,
                        // and cull if word letter position == yellow position
                        charPos = word.find(yellowLetter.letter);
                        while (charPos != string::npos) {
                            if (charPos == yellowLetter.pos) {
                                wordsToCull.push_back(word);
                                break;
                            }
                            // update char pos, checking after current pos
                            charPos = word.find(yellowLetter.letter, (charPos + 1));
                        }
                    }
                }
                cull(wordsToCull, filteredWords);
                showWordCount(filteredWords);
            }

            // List out all remaining words
            showWordList(filteredWords);
            showWordCount(filteredWords);
        } else if (choice == 's') {
            random_device hardwareRandomNumber; //generate a random number from hardware
            unsigned int seed { hardwareRandomNumber() }; // generate seed from hardwareRandomNumber
            mt19937 engine(seed); // seed the Mersenne Twister engine with generated seed
            uniform_int_distribution<vector<string>::size_type> distribution(0, words.size() - 1); // define distribution to get random indices
            vector<string>::size_type randomIndex = distribution(engine); // get a random index from the generated distribution

            // get a random string using generated index
            const string& starterWord = words[randomIndex];
            cout << "Starter word = " << starterWord << endl;
        } else if (choice == 'r') {
            filteredWords = words;
        } else {
            cout << "invalid input" << endl;
        }
        cout << messageChoice << endl;
        getline(cin, input);
        choice = input[0];
        }

}


