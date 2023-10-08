#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <string>
#include <vector>
#include <climits>
#include <functional>
#include <cassert>
#include <algorithm>

int zero = 0;


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
	// cout << words[3] << endl;

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " repeated words." << endl;
}

// template<typename K, typename V>
// template<typename T>

// struct
class Pair {
	public: 
		string key;
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
		HashMap(){
			size = 256;
			tab.resize(size);
			for (size_t i = 0; i < size; i++) {
				List list = List();
				tab[i] = list;
			}
			// exit(0);
		}
		int& get(string key) {
			size_t index = std::hash<string>{}(key) % size;
			List& list = tab[index];
			if (list.empty()) {
				return zero;
				// std::cout << "no such key as: " << key << std::endl;
				// assert(false);
			}
			Chainon* ptr = list.tete;
			while(ptr) {
				if(ptr->data.key == key) 
					return ptr->data.value;
				ptr = ptr->next;
			}
			return zero;
			// std::cout << "no such key as: " << key << std::endl;
			// assert(false);
		}
		void add(string key) {
			size_t index = std::hash<string>{}(key) % size;
			List& list = tab[index];
			if(list.empty()) {
				Pair pair(key, 1);
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
				Pair pair(key, 1);
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
		List& operator[](size_t index) {
			return tab[index];
		}
		void test() {
			Chainon* ptr = tab[100].tete;
			while (ptr) {
				std::cout << ptr->data.key << " = " << ptr->data.value << std::endl;
				ptr = ptr->next;
			}
		}
};

void word_count_distinct_with_hashmap(HashMap& hash) {
	ifstream input = ifstream("./WarAndPeace.txt");
	// HashMap hash;
	// exit(0);
	int i = 0;
	// exit(0);
	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");

	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		// exit(0);
		hash.add(word);
		// exit(0);
		i++;
		if (i % 100 == 0) {
			std::cout << "mot: " << word << ", nbr: " << hash[word] << std::endl;
		}
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";


	cout << hash["war"] << ", " << hash["peace"] << ", " << hash["toto"] << endl;
}

void test() {

}

void hashmap_to_vector(HashMap& hash, vector<Pair>& vect) {
	for (size_t i = 0; i < hash.get_size(); i++) {
		Chainon* ptr = hash[i].tete;
		while(ptr) {
			vect.push_back(ptr->data);
			ptr = ptr->next;
		}
	}
}

void sort_vector(vector<Pair>& vect) {
	std::sort(vect.begin(), vect.end(), [] (const Pair& p1, const Pair& p2) {
    	return p1.value > p2.value;
	});

	for (int i = 0; i < 10; i++) {
		cout << i + 1 << ") " << vect[i].key << " = " << vect[i].value << endl;
	}
}

int main () {
	vector<Pair> vect;
	HashMap hash;
	word_count_distinct_with_hashmap(hash);
	hashmap_to_vector(hash, vect);
	sort_vector(vect);
    return 0;
}
