//Rae Decastro
//CS320-01

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<algorithm>
#include<list>
#include<time.h>

class PasswordGenerator {           //Class that generates random password, creates combinations,return combination to main function one by one
	std::list<std::string> tokens;
	std::list<std::string>::iterator itr;
	int iterationLength;
	std::string combinations[3000][5];
	int combisize;
	int combitr;

public:
	PasswordGenerator(std::list<std::string> tkn) {    //Default Constructor for PasswordGenerator class
		tokens = tkn;                //List
		itr = tokens.begin();       //iterator for list 
		iterationLength = 0;        //Iterator fot num of tokens
		combisize = 0;             // Size of combinations made
		combitr = 0;               //iterator to iterate all combination
	}
	std::string getRandomPassword(int numWords) {
		std::string password="";
		int tmSize = 0;
		int chk = 0;
		srand(time(NULL));
		for (int i = 0; i < tokens.size(); i++) {      //To iterate all the words in our list
			if(numWords-tmSize < tokens.size()-i)      //If the available string is equal to the required token length then we cannot ignore tokens
				chk = rand() % 2;                //random selection(If '1' then word is added to password otherwise ignored)
			if (chk == 1) {                      
				chk = 0;
				std::advance(itr, 1);            //iterating list sequentially
				continue;                        //ignoring the current word
			}
			tmSize += 1;
			if (tmSize == numWords+1)            //if length of password equals req password length
				break;
			password += itr->c_str();
			password += " ";
			std::advance(itr, 1);
		}
		itr = tokens.begin();                    //reseting the list iterator
		return password;
	}
	void createCombination(std::string arr, int r) {       //Main function which uses a recursive function to generate combinations
		std::string temp[100];
		int tmpSize = 0;
		std::stringstream ss;
		ss << arr;
		for (int i = 0; i < arr.length()-1; i++) {         //Array of the unique words
			ss >> temp[i];
		}
		
		std::string data[5];                               //A temperary array to hold combination
		combinationUtil(temp, data, 0, tokens.size()-1, 0, r);
	}
	void combinationUtil(std::string arr[], std::string data[],
		int start, int end,
		int index, int r)            //A recursive function to find all combinations of desired length
	{
		if (index == r)              //Checking if combination has been made
		{
			for (int j = 0; j < r; j++) {
				combinations[combisize][j] = data[j];
			}
			return;
		}

        //Loop till end of array while making sure that including 
        //one element at index will make a combination with
        //remaining elements at remaining positions
		for (int i = start; i <= end &&
			end - i + 1 >= r - index; i++) 
		{
			combisize += 1;
			data[index] = arr[i];
			combinationUtil(arr, data, i + 1,
				end, index + 1, r);
		}
	}
	void setIterationLength(int numWords) {  //sets the iteration length
		iterationLength = numWords;
	}
	std::string next() {  //return a combination sequentially every time the func is called
		std::string temp="";
		for (int i = 0; i < iterationLength; i++) {
			temp += combinations[combitr][i];
			temp += " ";
		}
		combitr++;
		return  temp;
	}
	bool hasNext() {  //Checks if all combinations have been received 
		if (combitr == combisize)
			return 0;
		return 1;
	}
	int getResultIndex() { //Return the combination number where password was found
		return combitr;
	}
	
};
std::list<std::string> tokenDetector(std::string input){   //function to read file and store unique words
	std::fstream file;
	file.open(input.c_str());
	std::list<std::string> tokens;
	std::string temp[100];
	int tmSize = 0;
	int chk = 0;
	std::string token;
	while (file >> token) {       //Loop till endof file(Null is received)
		token.erase(remove(token.begin(), token.end(), ','), token.end());//erases , from words
		token.erase(remove(token.begin(), token.end(), '.'), token.end());//erases . from words
		for (int i = 0; i < tmSize; i++) {    //checking if word is unique
			//std::cout << "token: " << token << " temp: " << temp[i] << std::endl;
			if (token == temp[i]) {
				chk = 1;
				break;
			}
		}
		if (chk == 1) {
			chk = 0;
			continue;
		}
		tokens.push_back(token);  //adding to list
		temp[tmSize] = token;
		tmSize += 1;
	}
	return tokens;
}
int main (int argc, char *argv []) {
	char line[30];
	std::string input;
	std::cout << "->";
	std::cin.getline(line, sizeof(line));
	std::stringstream ss(line);
	ss >> input;
	if (input != "password") { //Checking arguments
		std::cout << "Not enough arguments\nSyntax: (program_name file_name)" << std::endl;
		return 0;

	}
	ss >> input;
	std::cout << input << std::endl;
	std::fstream file;
	file.open(input.c_str());
	if (file) { //If file is in directory
		file.close();
		int num;
		std::list<std::string> tokens;
		std::cout << "File opened successfully\nEnter the number of tokens for generating passwords: ";
		std::cin >> num;
		if (num < 1 || num > 5) {   //Checking if num is in range
			std::cout << "Number of tokens can be between 1and 5, Enter between 1-5: ";
			std::cin >> num;
		}
		std::string temp;
		tokens=tokenDetector(input);
		for (auto const& i : tokens) {
			//std::cout << i << std::endl;
			temp += i.c_str();
			temp += " ";
		}
		//calling all the functions of the class
		PasswordGenerator gen_pass(tokens);
		std::string password=gen_pass.getRandomPassword(num);
		std::cout << "password created: " << password << std::endl << std::endl;
		gen_pass.setIterationLength(num);
		gen_pass.createCombination(temp,num);
		while (gen_pass.hasNext()) {
			std::string comb = gen_pass.next();

			//std::cout << comb << std::endl;
			if (comb == password) {          //If password is found
				std::cout << "Password Found: " << comb << std::endl << "Combination number: " << gen_pass.getResultIndex() << std::endl;
				break;
			}
			
		}
	}
	else { //if file not in directory
		std::cout << "File not in directory" << std::endl;
		return 0;
	}
}
