#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "ArgumentManager.h"
#include "matrix.h"

using namespace std;

int countMove(vector<vector<int>> &v) {
	int counter = 0;
	for (vector<int> i : v) { //v passed paramter 
		for (int j : i) { //nested LOOP
			if (j != 2048 && j > 0) {
				counter++; //can use counter++ instead
			}
		}
	}
	return counter;
}
//converts "v" matrix into string to save ram as keyholder
string matrxToString(vector<vector<int>> v) { //v = matrix
	string convertS = ""; 
	for (vector<int> r : v) {
		for (int i : r) {
			convertS+= to_string(i) + ",";
		}
		convertS+=";";
	}
	return convertS;
}

string pathFinder(string swap, int count, vector<vector<int> > v, unordered_map<string, string> &ram) {
	vector<vector<int> > match = v;
	string leastSwaps = "";
	int smallestCount = 13;

	if (count > 12) return "";
	if (swap == "1") 
	{
		mUp(match);
	}
	else if (swap == "2") 
	{
		mRight(match);
	} 
	else if (swap == "3") 
	{
		mDown(match);
	} 
	else if (swap == "4") 
	{     
		mLeft(match);
	}
    string keyHolder;
	string ram_K;
    keyHolder = matrxToString(match);
    ram_K = keyHolder + to_string(count);
	if (ram.count(ram_K))
	{
		return ram[ram_K];
	}
//According to this configuration, we already have an answer so we search up what we saved in the ram
//config = state of board + remaining moves 
	
	
//check if path is correct we will return the movelist 
	if (countMove(match) == 1)
	{
		return swap;
	}
	
//iterates each move
	for (int i = 1; i< 5; i++) 
	{
		string swap = pathFinder(to_string(i), count + 1, match, ram); //will give the most optimal answer
		
//"" means we dont have an answer
//swap.length() reports how many swaps/moves it will take to find a solution if we took the to_string(i) move
//parent calls the recursion function for up -> right -> down -> left
//smallestCount and leastSwaps saves the least amount of moves out of its up, right, down, left child
		if (swap.length() < smallestCount && swap != "") 
		{
			smallestCount = swap.length();
			leastSwaps = swap;
		}
	}
	
//if child does not report optimal move, then it will return "" to the parent and the next parent above it 
//prior to returning to its' parent, we save config as "", to avoid any heavy or unecessary computations to decrease runtime
	if (leastSwaps == "") 
	{
		ram[ram_K] = "";
	} 
	else 
	{
		ram[ram_K] =  (swap + leastSwaps);
	}
	return ram[ram_K];
}

int main(int argc, char *argv[]) 
{
	ArgumentManager am(argc, argv);
	string infilename = am.get("input");
	string outfilename = am.get("output");
	ifstream in(infilename);
	ofstream out(outfilename);
	
	int length;
	vector<vector<int> > v;
	in >> length;
	for (int i = 0; i< length; i++) 
	{
		vector<int> tmp;
		for (int j = 0; j < length; j++) 
		{
			int data;
			in >> data;
			tmp.push_back(data);
		}
		v.push_back(tmp);
	} //receiving input from matrix
	
	if(countMove(v) == 1) 
	{
		out << 0;
		return 0;
	} //this is to stop computation once vector is solved


	unordered_map<string, string> ram;

	string convertS = "";
    int smallestLength = 13;
	for (int i = 1; i< 5; i++)
	{
		string swap = pathFinder(to_string(i), 1, v, ram);
		if (swap != "" && swap.length() < smallestLength) {
			smallestLength = swap.length();
			convertS = swap;
		}
	}
	//ram saves the answer to boards and boards without any solutions as ("")
	
	if (convertS.length() == 0) 
	{
		out << "Impossible";
	} 
	else 
	{
		out << convertS;
	}
	
	return 0;
}