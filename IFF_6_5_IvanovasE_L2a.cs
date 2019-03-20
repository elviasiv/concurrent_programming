using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Threading;

namespace IFF_6_5_IvanovasE_L2a
{
    class IFF_6_5_IvanovasE_L2a
    {
       
        public static Komanda[] P = new Komanda[7];
        public static _Konteineris[] K = new _Konteineris[7];
        public static int skaicius = 0;
        public static bool isfinished = false;

        public static int[] SkaitymoMasyvas1 = { 1,  1,1,1,0,1,1,1 };
        public static int[] SkaitymoMasyvas2 = { 1, 20,1,1,0,1,1,1 };
        public static int[] SkaitymoMasyvas3 = { 5, 1,20,1,1,1,1,1 };
        public static int[] SkaitymoMasyvas4 = { 0, 1,20,5,1,1,1,1 };
        public static int[] SkaitymoMasyvas5 = { 0, 1,1,17,5,1,1,1 };

        //Pasirinktos duomenų struktūros klasė (string, int, double)
        public class Zaidejas
        {
            string Vardas { get; set; }
            int Ivarciai { get; set; }
            double Ugis { get; set; }
            
            public Zaidejas() { }

            public Zaidejas(  string vardas, int ivarciai, double ugis)
            {
                
                Vardas = vardas;
                Ivarciai = ivarciai;
                Ugis = ugis;
            }

            public override string ToString()
            {
                return String.Format("{0,-15}{1,-12}{2:0.00}",  Vardas, Ivarciai, Ugis);
            }

            public int getIvarciai()
            {
                return Ivarciai;
            }
        }

        //Duomenų struktūros (rikiavimo lauko, kiekio) klasė
        public class _Data
        {
            int rikiavimoLaukas { get; set; }
            int kiekis { get; set; }

            

            public _Data() { }

            public _Data(int rikiavimolaukas)
            {
                rikiavimoLaukas = rikiavimolaukas;
                kiekis = 0;
            }

            public void Prideti()
            {
                kiekis++;
            }

            public void Atimti()
            {
                kiekis--;
            }

            public int getRikiavimoLaukas()
            {
                return rikiavimoLaukas;
            }

            public int getKiekis()
            {
                return kiekis;
            }

            public override string ToString()
            {
                return String.Format("{0,-11}{1,-9}", rikiavimoLaukas, kiekis);
            }
        }

        //_Data konteinerinė klasė turinti pašalinimo ir pridėjimo metodus
        public class _Konteineris
        {
            private _Data[] Ivarciai;

            public _Konteineris(int dydis)
            {
                Ivarciai = new _Data[dydis];
            }

            //Duomenys įdedami į masyvą surikiuotai
            public void Deti(_Data reiksme)
            {
                lock (Ivarciai)
                {
                    bool arYra = false;

                    for (int i = 0; i < Ivarciai.Count(); i++)
                    {
                        if (Ivarciai[i] != null)
                        {
                            if (Ivarciai[i].getRikiavimoLaukas() == reiksme.getRikiavimoLaukas())
                            {
                                Ivarciai[i].Prideti();
                                arYra = true;
                                break;
                            }
                        }

                    }

                    if (!arYra)
                    {
                        reiksme.Prideti();
                        for (int i = 0; i < Ivarciai.Count(); i++)
                        {
                            if (Ivarciai[i] != null)
                            {
                                if (reiksme.getRikiavimoLaukas() < Ivarciai[i].getRikiavimoLaukas())
                                {
                                    _Data temp = Ivarciai[i];
                                    _Data temp2;
                                    Ivarciai[i] = reiksme;

                                    for (int j = i + 1; j < Ivarciai.Count(); j++)
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
            
            //Duomenys šalinami iš masyvo ir pastumiami, jei nebėra duomenų
            public void Salinti(_Data reiksme)
            {
                
                lock (Ivarciai)
                {
                    for (int i = 0; i < Ivarciai.Count(); i++)
                    {
                        if (Ivarciai[i] != null)
                        {
                            if (Ivarciai[i].getRikiavimoLaukas() == reiksme.getRikiavimoLaukas())
                            {
                                if (Ivarciai[i].getKiekis() == 1)
                                {
                                    for (int j = i; j < Ivarciai.Count(); j++)
                                    {
                                        if (j + 1 != Ivarciai.Count())
                                        {
                                            Ivarciai[j] = Ivarciai[j + 1];
                                        }
                                        else
                                        {
                                            Ivarciai[j] = null;
                                        }

                                    }
                                }
                                else
                                {
                                    Ivarciai[i].Atimti();
                                    //arYra = true;
                                    break;
                                }

                            }
                        }
                    }
                }
            }

            
            public _Data Imti(int index)
            {
                return Ivarciai[index];
            }

            public int kiekis()
            {
                return Ivarciai.Count();
            }

            //Duomenys sudedami į masyva nerikiuotai
            public void DetiPaprastai(_Data reiksme, int Count)
            {
                Ivarciai[Count] = reiksme;
            }

            public bool Tuscias()
            {
                if (Ivarciai[0] == null)
                {
                    return true;
                }
                return false;
            }
            
        }

        //Zaidejas konteinerinė klasė
        public class Komanda
        {
            public string Pavadinimas { get; set; }
            private Zaidejas[] Zaidejai { get; set; }
            public int Count { get; private set; }

            public Komanda(string pavadinimas, int size)
            {
                Pavadinimas = pavadinimas;
                Zaidejai = new Zaidejas[size];
            }

            public Komanda() { }
            public void Deti(Zaidejas zaidejas)
            {

                Zaidejai[Count] = zaidejas;
                Count++;


            }

            public Zaidejas Imti(int index)
            {
                return Zaidejai[index];
            }


        }

        static void Main(string[] args)
        {
            //Failų pavadinimai
            string file1 = "IFF_6_5_IvanovasE_L2a_dat_1.txt";
            string file2 = "IFF_6_5_IvanovasE_L2a_dat_2.txt";
            string file3 = "IFF_6_5_IvanovasE_L2a_dat_3.txt";

            // nuskaitomi failai
            //P = Read(file1);
            P = Read(file2);
            //P = Read(file3);
            
            P[0] = new Komanda();
            
            //Rikiuojami laukai surasomi i kelis masyvus
            Perrasyti();

            K[0] = new _Konteineris(50);
           
            //sukuriamos gijos
            Thread Rasau1 = new Thread(() =>{  Rasytojas(1); });
            Thread Rasau2 = new Thread(() =>{  Rasytojas(2); });
            Thread Rasau3 = new Thread(() =>{  Rasytojas(3); });
            Thread Rasau4 = new Thread(() =>{  Rasytojas(4); });
            Thread Rasau5 = new Thread(() =>{  Rasytojas(5); });

            Thread Skaitau1 = new Thread(() =>{  Skaitytojas(SkaitymoMasyvas1); });
            Thread Skaitau2 = new Thread(() =>{  Skaitytojas(SkaitymoMasyvas2); });
            Thread Skaitau3 = new Thread(() =>{  Skaitytojas(SkaitymoMasyvas3); });
            Thread Skaitau4 = new Thread(() =>{  Skaitytojas(SkaitymoMasyvas4); });
            Thread Skaitau5 = new Thread(() =>{  Skaitytojas(SkaitymoMasyvas5); });

            Rasau1.Start();
            Rasau2.Start();
            Rasau3.Start();
            Rasau4.Start();
            Rasau5.Start();

            Skaitau1.Start();
            Skaitau2.Start();
            Skaitau3.Start();
            Skaitau4.Start();
            Skaitau5.Start();

            Rasau1.Join();
            Rasau2.Join();
            Rasau3.Join();
            Rasau4.Join();
            Rasau5.Join();

            isfinished = true;

            Skaitau1.Join();
            Skaitau1.Join();
            Skaitau1.Join();
            Skaitau1.Join();
            Skaitau1.Join();

            PrintTable();

            
        }

        // Metodas, kuris nuskaito duomenis iš failo
        public static Komanda[] Read(string file)
        {
            //string[] Gijos = new string[] { "Gija1", "Gija2", "Gija3", "Gija4", "Gija5" };
            Komanda[] P = new Komanda[7];
            int zaidejuSk = 1;
            string Pavadinimas;
            int komanduSk = 1;
            using (StreamReader reader = new StreamReader(@file))
            {

                string line = null;
                reader.ReadLine();
                line = reader.ReadLine();
                Pavadinimas = line;

                P[komanduSk] = new Komanda(Pavadinimas, 50);
                while (null != (line = reader.ReadLine()))
                {
                    string[] values = line.Split(' ');
                    if (values.Length == 1)
                    {
                        Pavadinimas = line;

                        komanduSk++;
                        P[komanduSk] = new Komanda(Pavadinimas, 50);
                        zaidejuSk = 1;
                    }
                    else
                    {
                        string Vardas = values[0];

                        int Ivarciai = int.Parse(values[1]);

                        double Ugis = double.Parse(values[2]);

                        Zaidejas naujas = new Zaidejas( Vardas, Ivarciai, Ugis);

                        P[komanduSk].Deti(naujas);
                    }

                }


            }

            return P;
        }

        //Pradinių duomenų išvedimas i konsolę
        public static void PradiniaiDuomenysKonsole()
        {
            for (int i = 0; i < 6; i++)
            {
                Console.WriteLine(P[i].Pavadinimas);
                for (int j = 0; j < P[i].Count; j++)
                {

                    Console.WriteLine(P[i].Imti(j).ToString());

                }
            }
            
        }

        //Iš pradinių duomenų masyvų paimami rikiavimo lauka ir yra įrašomi į duomenų struktūros(rikiavimo laukas, kiekis) masyvus
        public static void Perrasyti()
        {
            _Data temp;
            
            for (int i = 0; i < 6; i++)
            {
                K[i] = new _Konteineris(P[i].Count);
                for (int j = 0; j < P[i].Count; j++)
                {
                    temp = new _Data(P[i].Imti(j).getIvarciai());
                    K[i].DetiPaprastai(temp, j);
                }
            }
            
                
        }

        //Rašančiųjų gijų metodas
        public static void Rasytojas(int nr)
        {
            _Data temp;
            for (int i = 0; i < K[nr].kiekis(); i++)
            {
                
                    temp = new _Data(K[nr].Imti(i).getRikiavimoLaukas());
                    K[0].Deti(temp);
                
            }
        }

        //Šalinančiųjų gijų metodas
        public static void Skaitytojas(int[] nr)
        {
            _Data temp;
            bool pap;
            for (int i = 0; i < nr.Length; i++)
            {
                temp = new _Data(nr[i]);               
                K[0].Salinti(temp); 
            }

            Thread.Sleep(4);

            pap = K[0].Tuscias();
            
            if (!pap)
            {
                for (int i = 0; i < nr.Length; i++)
                {
                    temp = new _Data(nr[i]);

                    K[0].Salinti(temp);
                }
            }
            
        }

        //Pradiniu duomenų ir gautų rezultatu spausdinimas i failą
        public static void PrintTable()
        {
            using (StreamWriter writetext = new StreamWriter("IFF_6_5_IvanovasE_L2a.rez.txt"))
            {
                for (int i = 1; i < 6; i++)
                {
                    writetext.WriteLine(P[i].Pavadinimas);
                    writetext.WriteLine("   Vardas         Ivarciai    Ugis");
                    for (int j = 0; j < P[i].Count; j++)
                    {
                        writetext.WriteLine((j+1).ToString() + ") " + 
                            P[i].Imti(j).ToString());
                    }
                }

                writetext.WriteLine("************Struktūros duomenys***************");
                writetext.WriteLine("Nr.    Ivarciai    Kiekis");
                for (int i = 0; i < K[0].kiekis(); i++)
                {
                    if (K[0].Imti(i)!=null)
                    {
                        if (i<9)
                        {
                            writetext.WriteLine(" "+(i+1).ToString()+ ")       "+K[0].Imti(i).ToString());
                        }
                        else
                        {
                            writetext.WriteLine((i + 1).ToString() + ")       " + K[0].Imti(i).ToString());
                        }
                    }
                }
            }
        }
    }
}
