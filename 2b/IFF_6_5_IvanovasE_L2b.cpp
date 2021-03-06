// IFF_6_5_IvanovasE_L2b.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "omp.h"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "cmath"
#include "mutex"
#include "iomanip"


using namespace std;

int Size;
mutex mtx;

//Sukuriamas dvimatis masyvas, kurį naudos skaitymo gijos, ir pagal šiuos duomenis šalins reikšmes masyvo
int SkaitymoMasyvas[5][8] = 
{
	{ 1,  1,1,1,0,1,1,1 },
	{ 1, 20,1,1,0,1,1,1 },
	{ 5, 1,20,1,1,1,1,1 },
	{ 0, 1,20,5,1,1,1,1 },
	{ 0, 1,1,17,5,1,1,1 }
};





//Pasirinktos duomenų struktūros klasė (string,int,double)
class Zaidejas
{
	
public:

	// Data Members 
	string Vardas;
	int Ivarciai;
	double Ugis;

	Zaidejas() {} 

	Zaidejas(string vardas, int ivarciai, double ugis)
	{
		Vardas = vardas;
		Ivarciai = ivarciai;
		Ugis = ugis;
	}

	operator std::string() const { return Vardas + " " + std::to_string(Ivarciai) + " " + std::to_string(Ugis); }

	// Member Functions() 
	int getIvarciai() 
	{
		return Ivarciai;
	}
};

//Konteinerinė klasė, kuri talpina Zaidejas klasės objektus
class Komanda
{

private:
	
	int cap; //Masyvo dydis
	Zaidejas *Zaidejai;
	

public:
	string Pavadinimas; //Zaidejų grupės pavadinimas
	int Count; //Masyve esančių narių skaičius
	Komanda()
	{
		Pavadinimas = "";
		cap = 50;
		Count = 0;
		Zaidejai = new Zaidejas[cap];
	}

	Komanda(string pavadinimas)
	{
		Pavadinimas = pavadinimas;
		cap = 50;
		Count = 0;
		Zaidejai = new Zaidejas[cap];
	}

	//Deti metodas prideda zaidėjus į masyvą ir padidina jame esančių narių skaičių
	void Deti(Zaidejas zaidejas)
	{
		Zaidejai[Count] = zaidejas;
		Count++;
	}
	// Imti metodas grąžina masyvo narį pasirinktoje pozicijoje - index
	Zaidejas Imti(int index)
	{
		return Zaidejai[index];
	}
};

//Duomenų struktūros rikiavimo lauko, kiekio klasė
class _Data
{
public:
	int rikiavimoLaukas;
	int kiekis;

	_Data() {};
	_Data(int rikiavimolaukas)
	{
		rikiavimoLaukas = rikiavimolaukas;
		kiekis = 0;
	}

	//Metodas prideda prie buvusio kiekio prideda 1
	void Prideti()
	{
		kiekis++;
	}

	//Metodas iš buvusio kiekio atima 1
	void Atimti()
	{
		kiekis--;
	}

	//Grąžinama rikiavimo lauko reikšmė
	int getRikiavimoLaukas()
	{
		return rikiavimoLaukas;
	}

	//Grąžinama kiekio lauko reikšmė
	int getKiekis()
	{
		return kiekis;
	}

	operator std::string() const { return std::to_string(rikiavimoLaukas) + " " + std::to_string(kiekis); }

};

//Konteinerinė klase, kuri talpina _Data klasės objektus
class _Konteineris
{
private:
	int Count;
	int cap;
	_Data *Ivarciai;

public:
	_Konteineris()
	{
		cap = 50;
		Count = 0;
		Ivarciai = new _Data[cap];
	}

	//Grąžinama masyvo reikšmė pagal nurodytą poziciją - index
	_Data Imti(int index)
	{
		return Ivarciai[index];
	}

	//Grąžinama reikšmė, kuri objektų yra masyve
	int kiekis()
	{
		return Count;
	}

	//Objektai dedami į masyvą nuosekliai, netikrinant ar masyve jau yra buvęs toks pat rikiavimo laukas
	void DetiPaprastai(_Data reiksme)
	{
		#pragma omp critical //Uždedama apsauga, kad kelios gijos negalėtų pasiekti šio kodo dalies
		{
			Ivarciai[Count] = reiksme;
			Count++;
		}
	}

	//Metodas grąžina reikšmę true, jei masyvas yra tuščias, priešingu atveju false
	bool Tuscias()
	{
		
		if (Count > 0)
		{
			return true;
		}
		return false;
	}

	//Objektai į masyvą dedami atsižvelgiant į tai ar jau prieš tai buvo objektas, kurio rikiavimo lauko reikšmė jau buvo masyve, jei buvo prie šio objekto lauko
	//kiekio yra pridedamas 1, priešingu atveju yra įdedamas objektas į poziciją, kuri jam priklauso pagal surikiavimą
	void Deti(_Data reiksme)
	{
		
		#pragma omp critical //Uždedama apsauga, kad kelios gijos negalėtų pasiekti šio kodo dalies
		{
			bool arYra = false;
			//tikrinama ar jau yra objektas, kurio rikiavimo laukas jau yra masyve, jei yra prie šio objekto lauko kiekio pridedamas 1 ir sustabdomas ciklas
			for (int i = 0; i < Count; i++)
			{
				if (Ivarciai[i].getRikiavimoLaukas() >= 0)
				{
					if (Ivarciai[i].getRikiavimoLaukas() == reiksme.getRikiavimoLaukas())
					{
						Ivarciai[i].Prideti();
						arYra = true;
						break;
					}
				}
			}

			//Jeigu objekto dar nėra masyve jis yra pridedamas į jam skirtą vietą pagal rikiavima, kitu masyvo nariai yra pastumiami
			if (!arYra)
			{
				Count++;
				reiksme.Prideti();
				for (int i = 0; i < Count; i++)
				{
					if (Ivarciai[i].getRikiavimoLaukas() >= 0)
					{
						if (reiksme.getRikiavimoLaukas() < Ivarciai[i].getRikiavimoLaukas())
						{
							_Data temp = Ivarciai[i];
							_Data temp2;
							Ivarciai[i] = reiksme;

							for (int j = i + 1; j < Count; j++)
							{
								temp2 = Ivarciai[j];
								Ivarciai[j] = temp;
								temp = temp2;
							}
							break;
						}
					}
					else
					{
						Ivarciai[i] = reiksme;

						break;
					}
				}
			}
		} 
	}


	//Objektai yra pašalinami iš masyvo. Jei objekto kiekio lauko reikšmė yra >1, tada objekto kiekio lauko reikšmė mažinama 1, kitu atveju objektas yra pašalinamas
	//iš masyvo, o kitos masyvo reikšmės pastumiamos
	void Salinti(_Data reiksme)
	{
		#pragma omp critical //Uždedama apsauga, kad kelios gijos negalėtų pasiekti šio kodo dalies
		{

			for (int i = 0; i < Count; i++)
			{
				if (Ivarciai[i].getRikiavimoLaukas() >= 0)
				{
					if (Ivarciai[i].getRikiavimoLaukas() == reiksme.getRikiavimoLaukas())
					{
						if (Ivarciai[i].getKiekis() == 1) //Jei reikšmė yra lygi 1 objektas yra pašalinamas iš masyvo ir kitos reikšmės pastumiamos
						{
							for (int j = 0; j < Count; j++)
							{
								if (j + 1 != Count)
								{
									Ivarciai[j] = Ivarciai[j + 1];
								}
								else
								{
									Ivarciai[j] = -1;
								}
							}
							Count--;
						}
						else //Kitu atveju objekto kiekio lauko reikšmė mažinama 1
						{
							Ivarciai[i].Atimti();
							break;
						}
					}
				}
			}
		}
	}
};

Komanda P[6];
_Konteineris K[6];

//Nuskaitomi pasirinktos duomenų struktūros duomenys (string, int, double) ir surašomi į konteinerinę klasę Komanda - P
void Read(int &length)
{
	string line;
	string arr[3];
	string Vardas;
	int ivarciuSk;
	double ugis;
	int komanduSk = 0;

	//ifstream myfile("C://Users//Elvis//source//repos//IFF_6_5_IvanovasE_L2b//IFF_6_5_IvanovasE_L2b_dat_1.txt"); // Duomenų failas, iš kurio pašalins viską
	//ifstream myfile("C://Users//Elvis//source//repos//IFF_6_5_IvanovasE_L2b//IFF_6_5_IvanovasE_L2b_dat_2.txt"); // Duomenų failas, iš kurio nieko nešalins
	ifstream myfile("C://Users//Elvis//source//repos//IFF_6_5_IvanovasE_L2b//IFF_6_5_IvanovasE_L2b_dat_3.txt"); // Duomenų failas, iš kurio pašalins dalį objektų
	length = 0;

	while (getline(myfile, line))
	{
		int i = 0;
		stringstream ssin(line);
		while (ssin.good() && i<3)
		{
			ssin >> arr[i];
			++i;
		}
		if (i !=3)
		{
			komanduSk++;
			P[komanduSk] = Komanda(arr[0]);
		}
		else
		{
			Vardas = arr[0];
			ivarciuSk = std::stoi(arr[1]);
			ugis = std::stod(arr[2]);

			Zaidejas temp = Zaidejas(Vardas, ivarciuSk, ugis);
			P[komanduSk].Deti(temp);
		}	

	}
	myfile.close();
}

//Iš konteinerinės klasės Komanda - P duomenys surašomi i kitą konteinerinę klasę _Konteineris - K, joje talpinami rikiavimo laukai ir kiekiai
void Perrasyti()
{
	_Data temp;
	for (int i = 0; i < 6; i++)
	{
		K[i] = _Konteineris();
		for (int j = 0; j < P[i].Count; j++)
		{
			temp = _Data(P[i].Imti(j).getIvarciai());
			K[i].DetiPaprastai(temp);
		}
	}
}

//Rašančiosios gijos metodas. Visos gijos deda duomenis į vieną bendrą konteinerinės klasės _Konteineris objektą K[0]
void Rasytojas(int nr)
{
	_Data temp;
	for (int i = 0; i < K[nr].kiekis(); i++)
	{
		temp = _Data(K[nr].Imti(i).getRikiavimoLaukas());
		K[0].Deti(temp);
	}
}

//Skaitančiosios gijos metodas. Visos gijos šaliną duomenis iš bendro konteinrio K[0]
void Skaitytojas(int nr)
{
	_Data temp;
	bool pap;
	for (int i = 0; i < 8; i++)
	{
		temp = _Data(SkaitymoMasyvas[nr][i]);
		K[0].Salinti(temp);
	}

	pap = K[0].Tuscias(); //Patikrinamas ar masyvas tuščias, jei ne gijos dar karta šalina duomenis
	if (!pap) 
	{
		for (int i = 0; i < 8; i++)
		{
			temp = _Data(SkaitymoMasyvas[nr][i]);
			K[0].Salinti(temp);
		}
	}
}

//Duomenys spausdinami į rezultatų failą lentele
void PrintTable()
{
	ofstream myfile("IFF_6_5_IvanovasE_L2b.rez.txt");
	
	for (int i = 1; i < 6; i++)
	{
		myfile << P[i].Pavadinimas << endl;
		myfile << "   Vardas         Ivarciai    Ugis" << endl;
		for (int j = 0; j < P[i].Count; j++)
		{
			myfile << left;
			myfile << (j + 1) << ") " << setw(15)<< P[i].Imti(j).Vardas << setw(12)<< P[i].Imti(j).Ivarciai<< P[i].Imti(j).Ugis << endl;
			
		}
	}

	myfile << "************Strukturos duomenys***************" << endl;
	myfile << "Nr.    Ivarciai    Kiekis" << endl;
	
	for (int i = 0; i < K[0].kiekis(); i++)
	{
		if (K[0].Imti(i).getRikiavimoLaukas()>=0)
		{
			if (i<9)
			{
				myfile << " " << (1 + i) << ")       " << setw(11) << K[0].Imti(i).getRikiavimoLaukas()  << K[0].Imti(i).getKiekis() << endl;
			}
			else
			{
				myfile << left;
				myfile  << (1 + i) << ")       " << setw(11) << K[0].Imti(i).getRikiavimoLaukas() << K[0].Imti(i).getKiekis() << endl;
			}
		}
	}

	myfile.close();
}

int main()
{
	int length = 0;
	Read(length);
	/*for (int j = 1; j < 6; j++)
	{
		cout << P[j].Pavadinimas << endl;
		for (int i = 0; i < P[1].Count; i++)
		{

			cout << P[j].Imti(i).Vardas + " " << P[j].Imti(i).Ivarciai << " " << P[j].Imti(i).Ugis << endl;
		}
	}*/
	Perrasyti();

	/*for (int j = 1; j < 6; j++)
	{
		
		for (int i = 0; i < P[1].Count; i++)
		{

			cout << K[j].Imti(i).getRikiavimoLaukas() << " " << K[j].Imti(i).getKiekis() << endl;
		}
	}*/
	int rasytojuSk = 5; // nustatomas rašančiųjų gijų skaičius
	omp_set_num_threads(10); //nustatomas bendras gijų skaičius
	
	int threadNumber = 0;
	
#pragma omp parallel private(threadNumber)
	{
		threadNumber = omp_get_thread_num();
		
		if (threadNumber<rasytojuSk) // pirmosios 5 gijos priskiriamos rašančiojioms
		{
			Rasytojas(threadNumber + 1);
		}			
		else // sekančios 5 gijos priskiriamos skaitančioms
		{
			Skaitytojas(threadNumber - 4);
		}
	}


	/*for (int i = 0; i < K[0].kiekis(); i++)
	{
		cout << K[0].Imti(i).getRikiavimoLaukas() << " " << K[0].Imti(i).getKiekis() << endl;
	}*/
	
	PrintTable(); //Spausdinami rezultatų duomenys
    return 0;
}


