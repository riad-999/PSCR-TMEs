#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <string>
#include <vector>
#include <climits>
#include <functional>
#include <cassert>



using namespace std;
using namespace std::chrono;

bool exists(vector<string>& words, const string& word) {
	for (size_t i = 0; i < words.size(); ++i) {
		if (words[i] == word) 
			return true;
	}
	return false;
}

void word_count_distinct() {
	ifstream input = ifstream("./WarAndPeace.txt");
	vector<string> words;

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	std::string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		if (!exists(words, word)) {
			if (nombre_lu % 100 == 0){
				// on affiche un mot "propre" sur 100
				cout << nombre_lu << ": "<< word << endl;
			}
			words.push_back(word);
			nombre_lu++;
		}

	}
	input.close();

	cout << words.size() << endl;
	cout << words[3] << endl;

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " diffrent words." << endl;
}

void word_count(const string& searched_word) {
	ifstream input = ifstream("./WarAndPeace.txt");
	vector<string> words;

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	std::string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		if (word == searched_word) {
			words.push_back(word);
			nombre_lu++;
		}
	}
	input.close();

	cout << words.size() << endl;
	cout << words[3] << endl;

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " repeated words." << endl;
}

// Template<typename K, typename V>

// struct
class Pair {
	public: 
		const string key;
		int value;
		Pair(string key, int value): key(key), value(value){}
};


class Chainon {
	public: 
		Pair data;
		Chainon* next;
		Chainon(Pair data, Chainon* next = nullptr): data(data), next(next){}
};

class List {
	public: 
		Chainon* tete;
		List(): tete(nullptr){}
		void push_back(Chainon& elt) {
			if (tete == nullptr) {
				tete = &elt;
				return;
			}
			Chainon* ptr = tete;
			for (; ptr->next != nullptr; ptr = ptr->next) {}
			ptr->next = &elt;
		}
		bool empty() {
			return tete == nullptr;
		}
		~List() {
			Chainon* ptr = tete;
			Chainon* tmp;
			while (ptr != nullptr) {
				tmp = ptr->next;
				delete ptr;
				ptr = tmp;
			}
		}
};

class HashMap {
	vector<List> tab;
	size_t size;

	public: 
		HashMap(): size(4092), tab(size) {}
		int& get(string key) {
			size_t index = std::hash<string>{}(key) % size;
			List list = tab[index];
			if (list.empty()) {
				std::cout << "no such key as: " << key << std::endl;
				assert(false);
			}
			Chainon* ptr = list.tete;
			while(ptr) {
				if(ptr->data.key == key) 
					return ptr->data.value;
				ptr = ptr->next;
			}
			std::cout << "no such key as: " << key << std::endl;
			assert(false);
		}
		void add(string key) {
			size_t index = std::hash<string>{}(key) % size;
			List list = tab[index];
			if(list.empty()) {
				Pair pair = Pair(key, 1);
				Chainon* ch = new Chainon(pair);
				list.push_back(*ch);
			}
			else {
				Chainon* ptr = list.tete;
				while(ptr) {
					if(ptr->data.key == key) {
						ptr->data.value++;
						return;
					}
					ptr = ptr->next;
				}
				Pair pair = Pair(key, 1);
				Chainon* ch = new Chainon(pair);
				list.push_back(*ch);
			}
		}
		size_t get_size() {
			return size;
		}
		int& operator[](string key) {
			return get(key);
		}
};


int main () {
	// word_count_distinct();
    return 0;
}
