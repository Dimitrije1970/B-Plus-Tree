#include <cstdio>
#include <fstream>
#include <string>
#include "b_plus_tree.h"

#define ll long long

using namespace std;

int main() {
	int first_choice = -1, second_choice = 0, m;

	BPlusTree* bpt = nullptr;

	while (1) {
		if (first_choice == 1) {

			cout << "Unesi red B+ stabla (3-10): "; cin >> m; cout << endl;
			if (m < 3 || m > 10) {
				cout << "Pogresno unet broj.";
				break;
			}
			m--;

			bpt = new BPlusTree(m);

			string file_names[3] = {"CustomerAccount20.txt", "CustomerAccount250.txt", "CustomerAccount2500.txt" };

			cout << "1. " << file_names[0] << endl;
			cout << "2. " << file_names[1] << endl;
			cout << "3. " << file_names[2] << endl;
			cout << "Unesi redni broj fajla na osnovu kog se kreira indeks: "; cin >> second_choice; cout << endl;
			if (second_choice < 1 || second_choice > 3) {
				cout << "Pogresno unet broj.";
				break;
			}
			second_choice--;

			ifstream input_file;
			input_file.open(file_names[second_choice]);
			if (!input_file.is_open()) break;

			ll ca_id = 0, ca_b_id = 0, ca_c_id = 0, ca_tax_st = 0;
			string ca_name = "";
			double ca_bal = 0.0;

			string line;

			int cnt = 0;
			while (getline(input_file, line)) {
				string temp = "";
				char del = '|';

				for (int i = 0; i < (int)line.size();i++) {
					if (line[i] != del && i != (int)line.size() - 1) {
						temp += line[i];
					}
					else {
						if (cnt == 0) {
							ca_id = stoll(temp);
							cnt++;
						}
						else if (cnt == 1) {
							ca_b_id = stoll(temp);
							cnt++;
						}
						else if (cnt == 2) {
							ca_c_id = stoll(temp);
							cnt++;
						}else if (cnt == 3) {
							ca_name = temp;
							cnt++;
						}
						else if (cnt == 4) {
							ca_tax_st = stoll(temp);
							cnt++;
						}
						else if (cnt == 5) {
							ca_bal = stod(temp);
							cnt++;
						}

						temp = "";
					}
				}
				cnt = 0;
				bpt->insert_data(ca_id, ca_b_id, ca_c_id, ca_tax_st, ca_name, ca_bal);
			}

			input_file.close();
		}
		else if (first_choice == 2) {
			std::cout << "Radi ispisivanja svaki kljuc je sabran po modulu 10000." << std::endl << std::endl;

			if (bpt) {
				if (second_choice == 0) {
					bpt->print_tree(bpt->get_root());
				}
				else {
					std::cout << "Ne postoji adekvatan nacin da se iterativno ispise stablo ove velicine." << std::endl;
				}

			}

			std::cout << std::endl << std::endl;

		}
		else if (first_choice == 3) {
			ll ca_id = 0, ca_b_id = 0, ca_c_id = 0, ca_tax_st = 0;
			string ca_name = "";
			double ca_bal = 0.0;

			string line;

			cout << "Unesi podatke za novi zapis u formatu - CA_ID|CA_B_ID|CA_C_ID|CA_NAME|CA_TAX_ST|CA_BAL: ";
			cin >> line;

			int cnt = 0;
			string temp = "";
			char del = '|';
			for (int i = 0; i < (int)line.size(); i++) {
				if (line[i] != del && i != (int)line.size() - 1) {
					temp += line[i];
				}
				else {
					if (cnt == 0) {
						ca_id = stoll(temp);
						cnt++;
					}
					else if (cnt == 1) {
						ca_b_id = stoll(temp);
						cnt++;
					}
					else if (cnt == 2) {
						ca_c_id = stoll(temp);
						cnt++;
					}else if (cnt == 3) {
						ca_name = temp;
						cnt++;
					}
					else if (cnt == 4) {
						ca_tax_st = stoll(temp);
						cnt++;
					}
					else if (cnt == 5) {
						ca_bal = stod(temp);
						cnt++;
					}

					temp = "";
				}
			}
			cnt = 0;
			bpt->insert_data(ca_id, ca_b_id, ca_c_id, ca_tax_st, ca_name, ca_bal);

		}
		else if (first_choice == 4) {
			ll key;
			cout << "Unesi kljuc za brisanje: ";  cin >> key;

			Node* lowest = nullptr;
			if(bpt) lowest = bpt->get_root();
			BPlusTree* bpt_n = new BPlusTree(m);

			while (lowest && lowest->get_pointers()[0]) {
				lowest = lowest->get_pointers()[0];
			}

			while (lowest) {
				for (int i = 0; i < lowest->get_num(); i++) {
					if (lowest->get_keys()[i] != key) {
						bpt_n->insert_data(lowest->get_keys()[i], lowest->get_data()[i]->ca_b_id, lowest->get_data()[i]->ca_c_id, lowest->get_data()[i]->ca_tax_st, lowest->get_data()[i]->ca_name, lowest->get_data()[i]->ca_bal);
					}
				}

				lowest = lowest->get_pointers()[lowest->get_num()];
			}

			delete bpt;
			bpt = bpt_n;

			cout << "Zapis uspesno obrisan." << endl << endl;

		}
		else if (first_choice == 5) {
			ll key; 
			int num_of_steps = 0, index = 0;;
			cout << "Unesi kljuc za pretragu: "; cin >> key;

			if (bpt) {
				Node* result = bpt->search_bpt(key, &num_of_steps);
				if (result) {
					for (int i = 0; i < result->get_num(); i++) if (result->get_keys()[i] == key) index = i;
					cout << result->get_keys()[index] << "|" << result->get_data()[index]->ca_b_id << "|" << result->get_data()[index]->ca_c_id << "|" << result->get_data()[index]->ca_name << "|" << result->get_data()[index]->ca_tax_st << "|" << result->get_data()[index]->ca_bal << std::endl;
				}
				else {
					cout << "Kljuc " << key << " nije pronadjen." << endl;
				}

				cout << "Broj koraka: " << num_of_steps << endl << endl;
			}

		}
		else if (first_choice == 6) {
			int k, num_of_steps = 0, index = 0;;

			ofstream output_file;
			output_file.open("rez1.txt");


			cout << "Unesi broj kljuceva za pretragu: "; cin >> k;
			cout << "Kljucevi: " << endl;

			if (bpt) {
				for (int i = 0; i < k; i++) {
					ll temp; cin >> temp;
					Node* result = bpt->search_bpt(temp, &num_of_steps);
					if (result) {
						for (int i = 0; i < result->get_num(); i++) if (result->get_keys()[i] == temp) index = i;
						output_file << result->get_keys()[index] << "|" << result->get_data()[index]->ca_b_id << "|" << result->get_data()[index]->ca_c_id << "|" << result->get_data()[index]->ca_name << "|" << result->get_data()[index]->ca_tax_st << "|" << result->get_data()[index]->ca_bal << std::endl;
					}
					else {
						output_file << "Kljuc " << temp << " nije pronadjen." << endl;
					}
				}
			}

			std::cout << "Broj koraka: " << num_of_steps << std::endl << std::endl;
			output_file.close();

		}
		else if (first_choice == 7) {
			int k, num_of_steps = 0, index = 0, i, cnt = 0;
			ll key;

			ofstream output_file;
			output_file.open("rez2.txt");


			cout << "Unesi broj kljuceva za pretragu: "; cin >> k;
			cout << "Prvi kljuc: "; cin >> key;

			if (bpt) {
				Node* result = bpt->search_bpt(key, &num_of_steps);
				for (i = 0; i < result->get_num(); i++) if (result->get_keys()[i] == key) index = i;

				while (cnt < k) {
					if (result && index < result->get_num()) {
						output_file << result->get_keys()[index] << "|" << result->get_data()[index]->ca_b_id << "|" << result->get_data()[index]->ca_c_id << "|" << result->get_data()[index]->ca_name << "|" << result->get_data()[index]->ca_tax_st << "|" << result->get_data()[index]->ca_bal << std::endl;
						index++;
						if (index == result->get_num()) {
							result = result->get_pointers()[result->get_num()];
							index = 0;
						}
					}
					else {
						output_file << "Kljuc nije pronadjen." << endl;
					}

					cnt++;
				}
			}

			std::cout << "Broj koraka: " << num_of_steps+k << std::endl << std::endl;
			output_file.close();

		}
		else if (first_choice == 8) {
			delete bpt;
			cout << "Stablo uspesno obrisano." << endl << endl;
		}
		else if (first_choice == 0) {
			break;
		}

		cout << "1. Formiraj B+ stablo." << endl;
		cout << "2. Ispisi stablo." << endl;
		cout << "3. Unesi novi zapis o racunu." << endl;
		cout << "4. Obrisi zapis po primarnom kljucu." << endl;
		cout << "5. Pretrazi zapis po primarnom kljucu." << endl;
		cout << "6. Pretrazi k podataka." << endl;
		cout << "7. Pretrazi k sukcesivnih podataka." << endl;
		cout << "8. Obrisi stablo iz memorije." << endl;
		cout << "0. Prekini program." << endl;
		cout << "Unesi redni broj opcije: ";
		cin >> first_choice; cout << endl;
	}

	return 0;
}