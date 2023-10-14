#include "Banque.h"
#include <iostream>
#include <mutex>

using namespace std;

namespace pr {

	void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
		Compte & debiteur = comptes[deb];
		Compte & crediteur = comptes[cred];
		if (debiteur.debiter(val)) {
			crediteur.crediter(val);
		}
	}
	void Banque::transfert_atomic(size_t deb, size_t cred, unsigned int val) {
		// no need to use recursive_mutex with unique_lock
		Compte & debiteur = comptes[deb];
		Compte & crediteur = comptes[cred];
		recursive_mutex& mdeb = debiteur.get_mutex();
		recursive_mutex& mcred = crediteur.get_mutex();

		if (deb < cred) {
			unique_lock<recursive_mutex> l1(mdeb);
			unique_lock<recursive_mutex> l2(mcred);
			if (debiteur.debiter(val)) {
				crediteur.crediter(val);
			}
		}
		else {
			unique_lock<recursive_mutex> l1(mcred);
			unique_lock<recursive_mutex> l2(mdeb);
			if (debiteur.debiter(val)) {
				crediteur.crediter(val);
			}
		}
	}
	size_t Banque::size() const {
		return comptes.size();
	}
	bool Banque::comptabiliser (int attendu) {
		int bilan = 0;
		int id = 0;
		
		for (auto & compte : comptes) {
			recursive_mutex& m = compte.get_mutex();
			m.lock();
			if (compte.getSolde() < 0) {
				cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
			}
			bilan += compte.getSolde();
			id++;
		}
		for (auto & compte : comptes) {
			recursive_mutex& m = compte.get_mutex();
			m.unlock();
		}
		if (bilan != attendu) {
			cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
		}
		else {
			cout << "Bilan comptable Correcte` : attendu " << attendu << " obtenu : " << bilan << endl;	
		}
		return bilan == attendu;
	}
	int Banque::get_solde(size_t id) const {
		return comptes[id].getSolde();
	}
}
