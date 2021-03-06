#include <iostream>
#include <fstream>
using namespace std;

/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

/*
u nastavku je prikazan primjer ispitnog zapisnika (izvještaj o održanom ispitu) sa detaljima/stavkama

Datum ispita: 07.09.2017
Predmet: Programiranje II
IZVJESTAJ O ODRZANOM ISPITU
Br indeksa Ime prezime Komisijsko*** Ocjena
16001	A. Joldic	DA	5
15251	D. Music	NE	7 Ponisteno
16014	E. Babovic	DA	8
UKUPNO POLOZILO:	2
PROSJECNA OCJENA:	6.67
*** komisijski ispit: ako student pristupa četvrti put i dalje (računajući i poništene rezultate)
*/

char * crt = "\n-------------------------------------------\n";
struct Datum {
	int _dan;
	int _mjesec;
	int _godina;
	void Unos(int d, int m, int g) {
		_dan = d;
		_mjesec = m;
		_godina = g;
	}
	void Ispis() {
		cout << _dan << "." << _mjesec << "." << _godina << " ";
	}
	char *getDatumKaoNizKaraktera(){
		char *fullDate = new char[13];
		char dan[3], mjesec[3], godina[5];
		_itoa_s(_dan, dan, 3, 10);
		_itoa_s(_mjesec, mjesec, 3, 10);
		_itoa_s(_godina, godina, 5, 10);

		strcpy_s(fullDate, 13, dan);
		strcat_s(fullDate, 13, "-");
		strcat_s(fullDate, 13, mjesec);
		strcat_s(fullDate, 13, "-");
		strcat_s(fullDate, 13, godina);

		return fullDate;
	}
};
struct Student {
	char _imePrezime[20];
	int _brojIndeksa;
	int _polozioECTS;

	void Unos(char* naziv, float cijena) {
		strcpy_s(_imePrezime, naziv);
		_brojIndeksa = cijena;
		_polozioECTS = 0;
	}
	void DodajECTS(float ects) {
		_polozioECTS += ects;
	}
	void UmanjiECTS(float ects) {
		_polozioECTS -= ects;
	}
	void Ispis() {
		cout << _imePrezime << ", Indeks: " << _brojIndeksa << ", ECTS: " << _polozioECTS << " " << endl;
	}
};

struct ZapisnikStavka {
	int _ocjena;
	Student* _student; //ZapisnikStavka će pokazivati na objekat tipa student koji se nalazi u instanci tipa Fakultet, pa nije potrebno vršiti nove alokacije
	bool _ponisteno;
	bool _komisijskoPolaganje;

	void Unos(Student* proizvod, int ocjena, bool komisijskoPolaganje) {
		_ocjena = ocjena;
		_student = proizvod;
		_komisijskoPolaganje = komisijskoPolaganje;
		_ponisteno = false;
	}

	void Ispis() {
		cout << _student->_imePrezime << "\t" << _ocjena << "\t" << _student->_brojIndeksa << "\t";
		if (_ponisteno)
			cout << " Ponisteno";
		cout << endl;
	}
};
struct IspitniZapisnik {
	Datum _datum;
	ZapisnikStavka ** _stavke;//svaki put se dodaje po jedna stavka
	int _brojStavki;
	int _ects;
	char _predmet[20];
	void Unos(Datum datum, char* predmet, int ects) {
		_datum = datum;
		_brojStavki = 0;
		_stavke = nullptr;
		_ects = ects;
		strcpy_s(_predmet, predmet);
	}
	/*Prosjecnu ocjenu izračunati koristeći lambda funkciju koja se poziva koristeći objekat
	tipa auto. u prosjek računati i poništene rezultate (stavke)*/
	float GetProsjecnaOcjena() {
		// kreiranje lambda funkcije i prosljedjivanja objekta funkciji (this je pokazivac na objekat koji poziva funkciju)
		auto prosjek = [this]()->float{
			int prosjekVar = 0;
			for (size_t i = 0; i < _brojStavki; i++)
			{
				prosjekVar = prosjekVar + _stavke[i]->_ocjena;
			}
			return (float)prosjekVar / _brojStavki;
		};

		return prosjek();
	}
	/*
	<function>DodajZapisnikStavku</function>
	<>Prvo se kreira pomocni pokazivac na pokazivac naziva "temp" koji je za jedan veci od predhodnog niza, a zatim se alocira u dinamickoj memoriji
	niz pokazivaca tipa "ZapisnikStavka". Petljom se za svaki pokazivac alocira prostor u memoriji gdje
	ce biti smjesten objekat tipa ZapisnikStavka, a zatim se taj objekat alocira tj. dodjeljuje mu se vrijednost
	(kopira se iz starog niza).

	Ukratko: Pokazivac na pokazivac pokaziva na niz pokazivaca, a svaki taj pokazivac u nizu ce cuvati adresu jednog
	objekta tj. pokazivati na jedan objekat. (Nelogicno al' ajde....)

	Zatim se brise stari niz iz kojega su kopirane vrijednosti, dodaje dodaje novi objekat, pa se presumjerava stvarni niz na pomocni i pomocni presmjeravamo na nullptr da "zaboravi" niz na koji je pokazivao
	i na kraju se uvecava brojac.</>
	*/
	void DodajZapisnikStavku(Student* proizvod, int ocjena, bool komisijskoPolaganje){

		ZapisnikStavka **temp = new ZapisnikStavka*[_brojStavki + 1];

		for (size_t i = 0; i < _brojStavki; i++){
			temp[i] = new ZapisnikStavka;
			temp[i]->Unos(_stavke[i]->_student, _stavke[i]->_ocjena, _stavke[i]->_komisijskoPolaganje);
			delete _stavke[i];
		}

		if (_stavke != nullptr){
			delete[] _stavke;
		}

		temp[_brojStavki] = new ZapisnikStavka;
		temp[_brojStavki]->Unos(proizvod, ocjena, komisijskoPolaganje);

		_stavke = temp;
		temp = nullptr;

		_brojStavki++;
	}
	void Dealociraj(){
		for (size_t i = 0; i < _brojStavki; i++)
		{
			delete _stavke[i];
		}
		delete[] _stavke;
		_stavke = nullptr;
	}
};
const int Max = 100;

/*unutar objekta tipa Fakultet ce se cuvati podaci o svim studentima i ispitnim zapisnicima, a dodaju se pomocu globalnih funkcija*/
struct Fakultet {
	//umjesto brojaca koristiti default.ne vrijednosti
	Student* Studenti[Max] = { NULL };
	IspitniZapisnik* Zapisnici[Max] = { NULL };
	
	void Dealociraj() {
		for (size_t i = 0; i < Max; i++) {
			if (Studenti[i] != nullptr)
				delete Studenti[i];
			Studenti[i] = NULL;
		}
		for (size_t i = 0; i < Max; i++) {
			if (Zapisnici[i] != nullptr){
				Zapisnici[i]->Dealociraj();
				delete Zapisnici[i];
			}

			Zapisnici[i] = NULL;
		}
	}
	void Ispis(){
		for (size_t i = 0; i < Max; i++)
		{
			if (Zapisnici[i] != nullptr){
				cout << Zapisnici[i]->GetProsjecnaOcjena() << endl;
			}
		}
	}
};

void dodaj_student(Fakultet &fakultet, char *ime_prezime, int broj_indexa){
	for (size_t i = 0; i < Max; i++)
	{
		if (fakultet.Studenti[i] == nullptr) {
			fakultet.Studenti[i] = new Student;
			fakultet.Studenti[i]->Unos(ime_prezime, broj_indexa);
			return;
		}
		else if (fakultet.Studenti[i]->_brojIndeksa == broj_indexa)
			return;
	}
}
void kreiraj_zapisnik(Fakultet &fakultet, int dan, int mjesec, int godina, char *predmet, int ects){
	for (size_t i = 0; i < Max; i++)
	{
		if (fakultet.Zapisnici[i] == nullptr){
			fakultet.Zapisnici[i] = new IspitniZapisnik;
			Datum datum;
			datum.Unos(dan, mjesec, godina);
			fakultet.Zapisnici[i]->Unos(datum, predmet, ects);
			return;
		}
		else if (strcmp(fakultet.Zapisnici[i]->_predmet, predmet) == 0)
			return;
	}
}
bool dodaj_stavku_na_zapisnik(Fakultet &fakultet, int index, int ocjena){
	Student *studentSaProislijedjenimIndexom = nullptr; // pokazivac sluzi za cuvanje adrese studenta sa primljenim indexom

	for (size_t i = 0; i < Max; i++) // trazenje da li postoji student sa primljenim indexom
	if (fakultet.Studenti[i]->_brojIndeksa == index){
		studentSaProislijedjenimIndexom = fakultet.Studenti[i];
		break;
	}

	for (int i = Max - 1; i >= 0; i--)
	{
		if (fakultet.Zapisnici[i] != nullptr && studentSaProislijedjenimIndexom != nullptr){
			// provjera da li student postoji u zapisniku
			for (size_t j = 0; j < fakultet.Zapisnici[i]->_brojStavki; j++)
			{
				if (fakultet.Zapisnici[i]->_stavke[j]->_student->_brojIndeksa == index){
					fakultet.Zapisnici[i]->_stavke[j]->_ocjena = ocjena;
					if (ocjena > 5)
						studentSaProislijedjenimIndexom->DodajECTS(fakultet.Zapisnici[i]->_ects); // dodajemo studentu ects bodove
					return false;
				}
			}

			fakultet.Zapisnici[i]->DodajZapisnikStavku(studentSaProislijedjenimIndexom, ocjena, false); // posto iz maina nije prosljedjeno da li je komisijski radjen ispit po defaultu stavljamo da je false :|
			if (ocjena > 5)
				studentSaProislijedjenimIndexom->DodajECTS(fakultet.Zapisnici[i]->_ects); // dodajemo studentu ects bodove
			return true;
		}
	}
	return false;
}
bool ponisti_stavku_rezultat(Fakultet &fakultet, int index){
	// petlja krece sa zada iz razloga jer trazimo posljednje dodani zapisnik
	// trebalo bi trazit po datumu ali radi pojednostavljenja pretpostavimo da su zapisnici dodavani slijdeno po datumu
	for (int i = Max - 1; i > 0; i--)
	{
		if (fakultet.Zapisnici[i] != nullptr){
			for (size_t j = 0; j < fakultet.Zapisnici[i]->_brojStavki; j++)
			{
				if (fakultet.Zapisnici[i]->_stavke[j]->_student->_brojIndeksa == index)
				{
					fakultet.Zapisnici[i]->_stavke[j]->_ponisteno = true;
					fakultet.Zapisnici[i]->_stavke[j]->_student->UmanjiECTS(fakultet.Zapisnici[i]->_ects); // umanjivanje ects bodova
					return true;
				}
			}
			return false;
		}
	}
}
bool printaj(Fakultet &fakultet){
	bool svePonistene = true;
	for (int i = Max - 1; i >= 0; i--)
	{
		if (fakultet.Zapisnici[i] != nullptr){
			if (fakultet.Zapisnici[i]->_brojStavki == 0) return false; // u slucaju da nema stavki u zapisniku

			for (size_t j = 0; j < fakultet.Zapisnici[i]->_brojStavki; j++)
			if (!fakultet.Zapisnici[i]->_stavke[j]->_ponisteno) svePonistene = false;

			if (svePonistene) return false;
			// ispis na ekran i upis u fajl
			int size = strlen(fakultet.Zapisnici[i]->_predmet) + 16;
			char *naziv_fajla = new char[size];
			strcpy_s(naziv_fajla, size, fakultet.Zapisnici[i]->_predmet);
			strcat_s(naziv_fajla, size, "_");
			strcat_s(naziv_fajla, size, fakultet.Zapisnici[i]->_datum.getDatumKaoNizKaraktera());
			strcat_s(naziv_fajla, size, ".txt");

			ofstream upis_u_fajl(naziv_fajla);
			if (upis_u_fajl.fail()) return false; // provjera da se nije desila greska prilikom kreiranja fajla
			cout << "Datum ispita: " << fakultet.Zapisnici[i]->_datum.getDatumKaoNizKaraktera() << endl;
			upis_u_fajl << "Datum ispita : " << fakultet.Zapisnici[i]->_datum.getDatumKaoNizKaraktera() << endl;
			cout << "Predmet: " << fakultet.Zapisnici[i]->_predmet << endl;
			upis_u_fajl << "Predmet: " << fakultet.Zapisnici[i]->_predmet << endl;
			cout << "IZVJESTAJ O ODRZANOM ISPITU \n Br indeksa \t Ime prezime \t Komisijsko*** \t Ocjena" << endl;
			upis_u_fajl << "IZVJESTAJ O ODRZANOM ISPITU \n Br indeksa \t Ime prezime \t Komisijsko*** \t Ocjena" << endl;

			for (size_t j = 0; j < fakultet.Zapisnici[i]->_brojStavki; j++)
			{

				cout << fakultet.Zapisnici[i]->_stavke[j]->_student->_brojIndeksa << "\t" << fakultet.Zapisnici[i]->_stavke[j]->_student->_imePrezime << "\t";
				upis_u_fajl << fakultet.Zapisnici[i]->_stavke[j]->_student->_brojIndeksa << "\t" << fakultet.Zapisnici[i]->_stavke[j]->_student->_imePrezime << "\t";
				if (fakultet.Zapisnici[i]->_stavke[j]->_komisijskoPolaganje){
					cout << "DA";
					upis_u_fajl << "DA";
				}
				else{
					cout << "NE";
					upis_u_fajl << "NE";
				}
				cout << "\t" << fakultet.Zapisnici[i]->_stavke[j]->_ocjena << "\t";
				upis_u_fajl << "\t" << fakultet.Zapisnici[i]->_stavke[j]->_ocjena << "\t";
				if (fakultet.Zapisnici[i]->_stavke[j]->_ponisteno){
					cout << "Ponisteno";
					upis_u_fajl << "Ponisteno";
				}
				cout << endl;
				upis_u_fajl << endl;
			}
			delete[] naziv_fajla;
			upis_u_fajl.close();
			return true;
		}
	}
	return false;
}
void main() {
	Fakultet fakultet;

	/*dodaje podatke o novom studentu u fakultet */
	//fakultet, imePrezime, broj indeksa
	dodaj_student(fakultet, "A. Joldic", 16001);
	dodaj_student(fakultet, "D. Music", 15251);
	dodaj_student(fakultet, "E. Babovic", 16014);
	dodaj_student(fakultet, "E. Junuz", 16015);
	dodaj_student(fakultet, "J. Azemovic", 16014);//ne dodaje studenta jer je broj indeksa zauzet
	/*kraira novi zapisnik*/
	//fakultet, datum, predmet, ects krediti
	kreiraj_zapisnik(fakultet, 6, 9, 2017, "Programiranje 2", 7);

	/*dodaje stavku na posljednji kreiran zapisnik, a ects studenta uveća ako je student položio ispit. */

	if (dodaj_stavku_na_zapisnik(fakultet, 16001, 5))
		cout << "Stavka (rezultat) uspjesno dodata!" << endl;

	/*ako je student vec dodat u zapisnik, onda se vrsi samo modifikacija njegove ocjene*/
	if (!dodaj_stavku_na_zapisnik(fakultet, 16001, 6))
		cout << "Student vec dodat na zapisnik. Ocjena je ispravljena!" << endl;

	dodaj_stavku_na_zapisnik(fakultet, 15251, 5);
	dodaj_stavku_na_zapisnik(fakultet, 16015, 6);
	dodaj_stavku_na_zapisnik(fakultet, 16014, 8);

	kreiraj_zapisnik(fakultet, 7, 9, 2017, "Racunarske mreze", 5);
	//naredne stavke se dodaju na novi zapisnik
	dodaj_stavku_na_zapisnik(fakultet, 16015, 5);
	dodaj_stavku_na_zapisnik(fakultet, 16014, 10);
	dodaj_stavku_na_zapisnik(fakultet, 16001, 6);


	/*u posljednjem zapisniku stavku za studenta 16001 ponistiti, a studentu umanjiti ects bodove.
	funkcija vraca false u slucaju da se student ne nalazi na posljednjem zapisniku*/

	if (ponisti_stavku_rezultat(fakultet, 16001))
		cout << "Stavka (rezultat) uspjesno ponisten!" << endl;

	if (!ponisti_stavku_rezultat(fakultet, 19001))
		cout << "Student nije pronadjen!" << endl;

	/* funkcija printaj prikazuje posljednji ispitni zapisnik u formatu prikazanom na pocetku zadatka.
	* pored prikazivanja, identican sadrzaj ispitnog zapisnika je potrebno je pasiti u tekstualni fajl.
	* naziv fajla treba biti Predmet_datum.txt, npr. ProgramiranjeII_2017-09-01.txt. zapisnik se nece
	prikazati niti sacuvati u fajl ukoliko ne posjeduje niti jednu stavku ili su sve stavke ponistene*/
	if (printaj(fakultet))
		cout << "Ispitni zapisnik uspjesno prikazan i spasen u fajl!" << endl;
	fakultet.Dealociraj();

	system("pause");
}