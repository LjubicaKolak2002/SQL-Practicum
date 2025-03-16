#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<algorithm>
#include <unordered_set>
using namespace std;

void izbaciPonovljenaSlova(string& left, string& right) {
	unordered_set<char> log;
	right.erase(remove_if(right.begin(), right.end(), [&](char const c) { return !(log.insert(c).second); }), right.end());
	unordered_set<char> log2;
	left.erase(remove_if(left.begin(), left.end(), [&](char const c) { return !(log2.insert(c).second); }), left.end());
}


bool comp(char c1, char c2) {
	return tolower(c1) < tolower(c2);
}


void abecedno(vector<string>& left, vector<string>& right, int& n) {
	for (int i = 0; i < n; i++) {
		sort(left[i].begin(), left[i].end(), comp);
		sort(right[i].begin(), right[i].end(), comp);
	}
}

void unija(vector<string>& left, vector<string>& right, int& n, string& l, string& r) {
	for (int i = 0; i < n; i++) {
		if (l == left[i]) {
			r = r + right[i];
		}
	}
	izbaciPonovljenaSlova(l, r);
}

void refleksivnost(string& left, string& right) {
	right = left + right;
	izbaciPonovljenaSlova(left, right);
}

void tranzitivnost(vector<string>& left, vector<string>& right, int& n, string& l, string& r) {
	for (int i = 0; i < n; i++) {
		if (r.find(left[i]) != string::npos) {
			r += right[i];
		}

		else {
			int j;
			string s = "";
			for (j = 0; j < left[i].size(); j++) {
				if (r.find(left[i][j]) != string::npos) {
					s.push_back(left[i][j]);
				}
			}

			if (s == left[i]) {
				r += right[i];
			}
		}
	}
	izbaciPonovljenaSlova(l, r);
}


string prosirivanjePomocna(vector<string>& left, vector<string>& right, int& n, string relacija) {
	bool flag = false;
	string dostupnaSlova = "";

	for (int i = 0; i < relacija.length(); i++) {
		for (int j = 0; j < n; j++) {
			if ((left[j].find(relacija[i]) != string::npos) || (right[j].find(relacija[i]) != string::npos)) {
				flag = true;
			}
		}

		if (!flag) {
			dostupnaSlova += relacija[i];
		}
		flag = false;
	}
	return dostupnaSlova;
}

void prosirivanje(vector<string>& left, vector<string>& right, string& l, string& r, int& n, int indeks, string relacija) {
	vector<string> prosirivanje;
	string prijePromjeneL = l;
	string prijePromjeneR = r;

	for (int i = 0; i < n; i++) {
		string potencijalnoProsiravanje = "";
		for (int j = 0; j < left[i].size(); j++) {
			if (r.find(left[i][j]) == string::npos) {
				potencijalnoProsiravanje.push_back(left[i][j]);
			}
		}
		prosirivanje.push_back(potencijalnoProsiravanje);
	}

	vector<string> prosirivanje2;
	for (int i = 0; i < prosirivanje.size(); i++) {
		if (prosirivanje[i] != "") {
			prosirivanje2.push_back(prosirivanje[i]);
		}
	}

	if (prosirivanje2.size() != 0) {
		string najmanji = prosirivanje2[0];
		for (int i = 0; i < prosirivanje2.size(); i++) {
			if (prosirivanje2[i].size() < najmanji.size()) {
				najmanji = prosirivanje2[i];
			}
		}

		vector<string> tmp;
		for (int i = 0; i < prosirivanje2.size(); i++) {
			if (prosirivanje2[i].size() == najmanji.size()) {
				tmp.push_back(prosirivanje2[i]);
			}
		}

		vector<string> leftProsiren;
		for (int i = 0; i < tmp.size(); i++) {
			if (std::find(leftProsiren.begin(), leftProsiren.end(), tmp[i]) == leftProsiren.end()) {
				leftProsiren.push_back(tmp[i]);
			}
		}

		int j = leftProsiren.size();
		if (j > 0) {
			left[indeks] += leftProsiren[0];
			right[indeks] += leftProsiren[0];
			tranzitivnost(left, right, n, left[indeks], right[indeks]);
			string l2 = "";
			string r2 = "";
			int k = 1;

			while (j > 1) {
				n++;
				l2 = prijePromjeneL + leftProsiren[k];
				r2 = prijePromjeneR + leftProsiren[k];
				left.push_back(l2);
				right.push_back(r2);
				tranzitivnost(left, right, n, left[n - 1], right[n - 1]);
				k++;
				j--;
			}
		}
	}

	else {
		string dostupni = prosirivanjePomocna(left, right, n, relacija);
		if (dostupni != "") {
			l += dostupni;
			r += dostupni;
		}
	}
}


void glavniAlgoritam(vector<string>& left, vector<string>& right, int& n, string relacija) {
	for (int i = 0; i < n; i++) {
		unija(left, right, n, left[i], right[i]);
	}

	for (int i = 0; i < n; i++) {
		while (right[i] != relacija) {
			refleksivnost(left[i], right[i]);
			string right_prije_tranzitivnosti = "";

			do {
				right_prije_tranzitivnosti = right[i];
				tranzitivnost(left, right, n, left[i], right[i]);
			} while (right[i] != right_prije_tranzitivnosti);

			prosirivanje(left, right, left[i], right[i], n, i, relacija);
			abecedno(left, right, n);
		}
	}
}

vector<string> ispisiKandidate(vector<string>& left) {
	int min = left[0].size();
	int n = left.size();

	for (int i = 0; i < n; i++) {
		if (left[i].size() < min)
			min = left[i].size();
	}

	vector<string> novi_left;
	for (int i = 0; i < n; i++) {
		if (left[i].size() == min)
			novi_left.push_back(left[i]);
	}
	return novi_left;
}


void ispisi_vector(vector<string>& vec) {
	cout << "{";
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i];
		if (i < vec.size() - 1)
			cout << ", ";
	}
	cout << "}";
}

bool checkIfSubstring(vector<string>& ro, string str) {
	for (int i = 0; i < ro.size(); i++) {
		if (ro[i].find(str) != string::npos)
			return true;

		else {
			int j;
			string str_novi = "";
			for (j = 0; j < str.size(); j++) {
				if (ro[i].find(str[j]) != string::npos)
					str_novi.push_back(str[j]);
			}
			if (str_novi == str)
				return true;
		}
	}
	return false;
}



bool treca_nf_pomocna(vector<string>& ro, string str) {
	if (checkIfSubstring(ro, str))
		return false;
	return true;
}

void ispis3NF(vector<string>& ro, vector<string>& left, vector<string>& right, vector<string>& left_ro) {

	for (int i = 0; i < left.size(); i++) {
		if (treca_nf_pomocna(ro, left[i] + right[i])) {
			cout << "Za " << left[i] << "->" << right[i] << " => " << left[i] + right[i] + " nije u ro " << " => " << " ro = ";
			ro.push_back(left[i] + right[i]);
			left_ro.push_back(left[i]);
			ispisi_vector(ro);
		}

		else {
			cout << "Za " << left[i] << "->" << right[i] << " => " << left[i] + right[i] + " je u ro " << " => " << " ro = ";
			ispisi_vector(ro);
		}
		cout << endl;

	}
}


void ispis3NFkljuc(vector<string>& ro, string kljuc, vector<string>& left_ro) {

	if (treca_nf_pomocna(ro, kljuc)) {
		cout << "Za kljuc " << kljuc << " => " << "Kljuc nije u ro" << " => " << " ro = ";
		ro.push_back(kljuc);
		left_ro.push_back(kljuc);
		ispisi_vector(ro);
	}

	else {
		cout << "Za kljuc " << kljuc << " => " << "Kljuc je u ro " << " => " << " ro = ";
		ispisi_vector(ro);

	}
}

void treca_nf(vector<string>& left, vector<string>& right, string kljuc) {

	vector <string> ro;
	vector<string> left_ro;
	cout << "ro = {}" << endl;

	ispis3NF(ro, left, right, left_ro);

	ispis3NFkljuc(ro, kljuc, left_ro);

	cout << endl << endl << "Izlaz: ro:=";
	ispisi_vector(ro);
	cout << endl << "Lijeve strane FO koje su usle u dekompoziciju + primarni kljuc: ";
	ispisi_vector(left_ro);
}


bool checkIfSubstringBC(string ovisnost, string relacija) {
	for (int i = 0; i < relacija.size(); i++) {
		if (relacija.find(ovisnost) != string::npos) {
			return true;
		}
		else {
			int j;
			string str = "";
			for (j = 0; j < ovisnost.size(); j++) {
				if (relacija.find(ovisnost[j]) != string::npos) {
					str.push_back(ovisnost[j]);
				}
			}

			if (str == ovisnost) {
				return true;
			}
		}
	}
	return false;
}

string deleteFromS(string right, string relacija) {

	for (int j = 0; j < right.size(); j++) {  
		relacija.erase(remove(relacija.begin(), relacija.end(), right[j]), relacija.end());
	}
	return relacija;
}


void boyce_code_nf(vector<string>& left, vector<string>& right, string relacija, vector<string>& kljucevi) {

	vector <string> vec;
	for (int i = 0; i < left.size(); i++) {
		cout << "S = {" << relacija << "}" << endl;

		if (checkIfSubstringBC(left[i] + right[i], relacija)) {
			cout << "Za " << left[i] << "->" << right[i] << " => "; 
			cout << left[i] + right[i] << " je u S => "; 
			relacija = deleteFromS(right[i], relacija);
			cout << "S = " << "{" << left[i] + right[i] << "} U " << "{" << relacija << "}" << endl;  
			cout << endl;
			vec.push_back(left[i] + right[i]);
		}

		else { 
			cout << "Za " << left[i] << "->" << right[i] << " => ";
			cout << left[i] + right[i] << " nije u S" << endl;
			cout << left[i] << "->" << right[i] << " je izgubljena funkcionalna ovisnost tijekom dekompozicije." << endl;
			cout << endl;
		}
	}


	vector <string> kljucURelaciji;

	for (int i = 0; i < kljucevi.size(); i++) {
		if (relacija == kljucevi[i]) {
			kljucURelaciji.push_back(kljucevi[i]);
			vec.push_back(relacija);
		}
		else {
			if (checkIfSubstringBC(kljucevi[i], relacija)) { 
				kljucURelaciji.push_back(kljucevi[i]);
				if (!checkIfSubstring(vec, relacija))
					vec.push_back(relacija);


			}
		}
	}

	if (kljucURelaciji.size() == 0) {
		cout << endl << "{" << relacija << "}" << " nije u BCNF " << endl << endl;
		ispisi_vector(vec);
	}

	else {
		cout << endl << "{" << relacija << "}" << " je u BCNF jer: ";
		for (int i = 0; i < kljucURelaciji.size(); i++) {
			string str2;

			for (int j = 0; j < relacija.size(); j++) {
				if (kljucURelaciji[i].find(relacija[j]) == -1)
					str2.push_back(relacija[j]);
			}

			if (kljucURelaciji[i] == relacija)
				cout << "{" << kljucURelaciji[i] << "->" << relacija << "}";
			else
				cout << endl << "{" << kljucURelaciji[i] << "->" << str2 << "}";
		}

	}

	cout << endl << endl << "ro:= ";
	ispisi_vector(vec);
	cout << endl << endl <<  "U BCNF su: ";
	ispisi_vector(kljucURelaciji);
}


int main() {
	vector<string> left, right;
	int n;
	string relacija;
	cout << "Unesite relaciju:" << endl;
	cin >> relacija;

	sort(relacija.begin(), relacija.end());
	cout << endl;
	cout << "Unesite broj funkcionalnih ovisnosti: ";
	cin >> n;
	string l, r;
	for (int i = 0; i < n; i++) {
		cout << endl << "Unesite lijevu stranu funkcionalnih ovisnosti: ";
		cin >> l;
		cout << endl << "Unesite desnu stranu funkcionalnih ovisnosti: ";
		cin >> r;
		left.push_back(l);
		right.push_back(r);
	}

	vector<string> novi_left = left;
	vector<string> novi_right = right;

	for (int i = 0; i < left.size(); i++) {
		cout << left[i] << "->" << right[i] << "  ";
	}

	glavniAlgoritam(left, right, n, relacija);
	cout << endl;
	cout << endl << "-------------------------------------------------------------------------------------------" << endl;

	cout << endl << "Svi pronadeni kljucevi:" << endl;
	std::sort(left.begin(), left.end());
	auto last = std::unique(left.begin(), left.end());
	left.erase(last, left.end());

	for (int i = 0; i < left.size(); i++) {
		cout << left[i] << endl;
	}

	cout << endl << "-------------------------------------------------------------------------------------------" << endl;


	cout << endl << "Kandidati za primarne kljuceve su: " << endl;
	vector<string> kandidati_kljucevi = ispisiKandidate(left);
	for (int i = 0; i < kandidati_kljucevi.size(); i++) {
		cout << kandidati_kljucevi[i] << endl;
	}

	cout << endl << "-------------------------------------------------------------------------------------------" << endl;

	cout << endl << "DEKOMPOZICIJA U 3.NF" << endl;
	for (int i = 0; i < kandidati_kljucevi.size(); ++i) {
		cout << endl << "Rjesenje dekompozicije u 3.NF. za kljuc : " << kandidati_kljucevi[i] << endl;
		treca_nf(novi_left, novi_right, kandidati_kljucevi[i]);
		cout << endl << endl;
		cout << endl << "-------------------------------------------------------------------------------------------" << endl;
	}

	cout << endl;
	cout << endl << "Rjesenje dekompozicije u BC.NF." << endl << endl;
	boyce_code_nf(novi_left, novi_right, relacija, kandidati_kljucevi);
}