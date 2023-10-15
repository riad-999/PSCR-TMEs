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

template <typename Iterator>
size_t count(Iterator begin, Iterator end) {
	size_t cnt = 0;
	for (Iterator it = begin; it != end; ++it) 
		++cnt;
	return cnt;
}

template <typename Iterator_, typename T>
size_t count_equal(Iterator_ begin, Iterator_ end, T& val) {
	size_t cnt = 0; 
	for (Iterator_ it = begin; it != end; ++it) {
		if (*it == val)
			++cnt;
	}
	return cnt;
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

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " diffrent words." << endl;

	string war = "war";
	cout << "size of container: " << words.size() << "count: " << count(words.begin(), words.end()) << endl;
	cout << "number: " << nombre_lu << ", count: " << count_equal(words.begin(), words.end(), war) << endl;
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

template <typename K, typename V> 
class Pair {
	public: 
		K key;
		V value;
		Pair(K key, V value): key(key), value(value){}
};

template <typename D>
class Chainon {
	public: 
		D data;
		Chainon* next;
		Chainon(D data, Chainon* next = nullptr): data(data), next(next){}
};

template <typename P>
class List {
	public: 
		Chainon<P>* tete;
		List(): tete(nullptr){}
		void push_back(Chainon<P>& elt) {
			if (tete == nullptr) {
				tete = &elt;
				return;
			}
			Chainon<P>* ptr = tete;
			for (; ptr->next != nullptr; ptr = ptr->next) {}
			ptr->next = &elt;
		}
		bool empty() {
			return tete == nullptr;
		}
		~List() {
			Chainon<P>* ptr = tete;
			Chainon<P>* tmp;
			while (ptr != nullptr) {
				tmp = ptr->next;
				delete ptr;
				ptr = tmp;
			}
		}
};

template <typename KT, typename VT>
class HashMap {
	vector<List<Pair<KT, VT>>> tab;
	size_t size;

	public: 
		HashMap(){
			size = 256;
			tab.resize(size);
			for (size_t i = 0; i < size; i++) {
				List<Pair<KT, VT>> list;
				tab[i] = list;
			}
		}
		VT& get(KT key) {
			size_t index = std::hash<KT>{}(key) % size;
			auto& list = tab[index];
			if (list.empty()) {
				return zero;
				// std::cout << "no such key as: " << key << std::endl;
				// assert(false);
			}
			auto ptr = list.tete;
			while(ptr) {
				if(ptr->data.key == key) 
					return ptr->data.value;
				ptr = ptr->next;
			}
			return zero;
			// std::cout << "no such key as: " << key << std::endl;
			// assert(false);
		}
		void add(KT key) {
			size_t index = std::hash<string>{}(key) % size;
			auto& list = tab[index];
			if(list.empty()) {
				Pair<KT, VT> pair(key, 1);
				Chainon<Pair<KT, VT>>* ch = new Chainon(pair);
				list.push_back(*ch);
			}
			else {
				auto ptr = list.tete;
				while(ptr) {
					if(ptr->data.key == key) {
						ptr->data.value++;
						return;
					}
					ptr = ptr->next;
				}
				Pair<KT, VT> pair(key, 1);
				Chainon<Pair<KT, VT>>* ch = new Chainon(pair);
				list.push_back(*ch);
			}
		}
		size_t get_size() {
			return size;
		}
		int& operator[](KT key) {
			return get(key);
		}
		auto& operator[](size_t index) {
			return tab[index];
		}
		class Iterator {
			vector<List<Pair<KT, VT>>>& tab;
			size_t index;
			Chainon<Pair<KT, VT>>* it;

			Iterator(vector<List<Pair<KT, VT>>>& t, size_t ind = 0, Chainon<Pair<KT, VT>>* i = nullptr): tab(t), index(ind), it(i) {} 

		};
		Iterator begin() {
			Iterator iter(tab, 0, tab[0].tete);
			return iter;
		};
		Iterator end() {
			Chainon<Pair<KT, VT>>* ptr = nullptr;
			Chainon<Pair<KT, VT>>* end = ptr;
			ptr = tab[tab.size() - 1].tete;
			while(ptr){
				end = ptr;
				ptr = ptr->next;
			}
			Iterator iter(tab, tab[tab.size() - 1], tab[0].tete, end);
			return iter;
		};
};


void word_count_distinct_with_hashmap(HashMap<string, int>& hash) {
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

void hashmap_to_vector(HashMap<string, int>& hash, vector<Pair<string, int>>& vect) {
	for (size_t i = 0; i < hash.get_size(); i++) {
		auto ptr = hash[i].tete;
		while(ptr) {
			vect.push_back(ptr->data);
			ptr = ptr->next;
		}
	}
}

void sort_vector(vector<Pair<string, int>>& vect) {
	std::sort(vect.begin(), vect.end(), [] (const Pair<string, int>& p1, const Pair<string, int>& p2) {
    	return p1.value > p2.value;
	});

	for (int i = 0; i < 10; i++) {
		cout << i + 1 << ") " << vect[i].key << " = " << vect[i].value << endl;
	}
}

void tme2() {
	vector<Pair<string, int>> vect;
	HashMap<string, int> hash;
	word_count_distinct_with_hashmap(hash);
	hashmap_to_vector(hash, vect);
	sort_vector(vect);
}

void tme3_1() {
	word_count_distinct();

}

int main () {
	tme3_1();
    return 0;
}
