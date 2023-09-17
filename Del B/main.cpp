#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_PERSONER = 10;
const int MAX_TRANSAKTIONER = 100;

// Representerar en enskild transaktion (ett transaktions objekt)
 class Transaktion
 {
 private:
   string datum;
   string typ;
   string namn;
   double belopp;
   int    antal_kompisar;
   string *kompisar;

 public:
   Transaktion();
   Transaktion(const Transaktion& t);
   ~Transaktion();
   Transaktion& operator=( const Transaktion& t);
   string hamtaNamn() const;
   double hamtaBelopp() const;
   int    hamtaAntalKompisar() const;
   bool   finnsKompis(const string &namnet) const;
   bool   lasIn(istream &is, bool pOutput);
   void   skrivUt(ostream &os) const;
   void   skrivTitel(ostream &os) const;
 };

 // Representerar en enskild person (ett person objekt)
 class Person
 {
 private:
   string namn;
   double betalat_andras; // ligger ute med totalt
   double skyldig;        // skyldig totalt

 public:
   Person();
   Person(const string &namn, double betalat_andras, double skyldig);
   string hamtaNamn();
   double hamtaBetalat();
   double hamtaSkyldig();
   void   skrivUt(ostream &os);
   double skallFranPotten() {
    return betalat_andras - skyldig;
}


    // Jag skapade en setter för att kunna ändra värdet för den privata variabeln betala_andras
   void setBetalat(double belopp)
    {
        betalat_andras = belopp;
    }

    // Jag skapade ännu en setter för att kunna ändra värdet för den privata variabeln skyldig
    void setSkyldig(double belopp)
    {
        skyldig = belopp;
    }
};

// Representerar en lista av personer
class PersonLista
 {
 private:
   int    antal_personer;
   Person *personer;

 public:
   PersonLista();
   ~PersonLista();
   void   laggTill(Person ny_person);
   void   skrivUtOchFixa(ostream &os);
   double summaSkyldig() const;
   double summaBetalat() const;
   bool   finnsPerson(const string& namn);

   // Jag skapade denna metod för att kunna returnera en referens på en specifik person bserat på namnet
   Person& getPersonByName(const string& namn);
 };

// Representerar en lista av transaktioner
  class TransaktionsLista
 {
private:
   int         antal_transaktioner;
   Transaktion *transaktioner;

 public:
   TransaktionsLista();
   ~TransaktionsLista();
   void   lasIn(istream & is, bool pOutput);
   void   skrivUt(ostream & os);
   void   laggTill(Transaktion & t);
   double totalKostnad() const;
   double liggerUteMed(const string &namnet);
   double arSkyldig(const string &namnet);
   PersonLista fixaPersoner();
 };

// Huvudprogram
int main()
{
  cout << "Startar med att läsa från en fil." << endl;

  TransaktionsLista transaktioner;
  std::ifstream     is("resa.txt");

  // Jag skapade detta för att programmet tryckte ut en massa text när det startades utan att jag ville det
  transaktioner.lasIn(is, false); // läser in utan att trycka ut en massa text i början

  // UI sätts igång och avslutas när operation = 0
  int operation = 1;
  while (operation != 0)
    {
      cout << endl;
      cout << "Välj i menyn nedan:" << endl;
      cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
      cout << "1. Skriv ut information om alla transaktioner." << endl;
      cout << "2. Läs in en transaktion från tangentbordet." << endl;
      cout << "3. Beräkna totala kostnaden." << endl;
      cout << "4. Hur mycket är en viss person skyldig?" << endl;
      cout << "5. Hur mycket ligger en viss person ute med?" << endl;
      cout << "6. Lista alla personer mm och FIXA" << endl;

      cin >> operation;
      cout << endl;

      switch (operation)
        {
          case 1:
            {
              transaktioner.skrivUt(cout);
              break;
            }
          case 2:
            {
              Transaktion transaktion;
              cout << "Ange transaktion i följande format" << endl;
              transaktion.skrivTitel(cout);
              transaktion.lasIn(cin, true); // läser in med utskrift
              transaktioner.laggTill(transaktion);
              break;
            }
          case 3:
            {
              cout << "Den totala kostnanden för resan var "
                   << transaktioner.totalKostnad() << endl;
              break;
            }
          case 4:
            {
              cout << "Ange personen: ";
              string namn;
              cin >> namn;
              double ar_skyldig = transaktioner.arSkyldig(namn);
              if (ar_skyldig == 0.)
                cout << "Kan inte hitta personen " << namn << endl;
              else
                cout << namn << " är skyldig " << ar_skyldig << endl;
              break;
            }
          case 5:
            {
              cout << "Ange personen: ";
              string namn;
              cin >> namn;
              double ligger_ute_med = transaktioner.liggerUteMed(namn);
              if (ligger_ute_med == 0.)
                cout << "Kan inte hitta personen " << namn << endl;
              else
                cout << namn << " ligger ute med " << ligger_ute_med << endl;
              break;
            }
          case 6:
            {
              cout << "Nu skapar vi en personarray och reder ut det hela!" << endl;
              PersonLista lista = transaktioner.fixaPersoner();
              lista.skrivUtOchFixa(cout);
              break;
            }
        }
    }

  std::ofstream os("transaktioner.txt");
  transaktioner.skrivUt(os);


  return 0;
}

// Klassimplementationer

// Definitioner av Transaktion-klassens funktioner

// Standard konstruktor, skapar ett tomt transaktion objekt
Transaktion::Transaktion()
{
    datum = "";
    typ = "";
    namn = "";
    belopp = 0.0;
    antal_kompisar = 0;
    kompisar = nullptr;
}

// Kopieringskonstruktor
// Behövs för att skapa en kopia av den befintliga instansen "t" som är i parametrarna
// Viktigt pga. att det ger oss mer kontroll över hur objekt kopieras, och detta behövs för att motverka minnes läckor eller andra minnesfel när vi dynamiskt allokerar minne
Transaktion::Transaktion(const Transaktion& t)
{
    // Kopierar värden för datum, typ, namn, belopp, antal_kompisar från "t" till den nya transaktion instansen
    datum = t.datum;
    typ = t.typ;
    namn = t.namn;
    belopp = t.belopp;
    antal_kompisar = t.antal_kompisar;

    // Skapar en ny dynamisk array, där storleken är antalet kompisar i transaktionen
    kompisar = new string[antal_kompisar];

    // kopierar namnen från instansen "t" till den nya kopian
    for (int i = 0; i < antal_kompisar; i++)
    {
        kompisar[i] = t.kompisar[i];
    }
}

// Destruktor som används för att frigöra minne
Transaktion::~Transaktion()
{
    delete[] kompisar;
}

// Hämtar namnet på en transaktion
string Transaktion::hamtaNamn() const
{
    return namn;
}

// Tilldelningsoperator
// Behövs för att kopier värden från instans "t" till en annan instans
Transaktion& Transaktion::operator=( const Transaktion& t)
{
    // Kollar att "t" inte är den aktuella instansen en så kallad self-assignment check
    if (this != &t)
    {
        // Raderar den befintliga dynamiska arrayen för att motverka minnesläckor
        delete[] kompisar;

        // Kopierar värden från "t" till den atuella instansen
        datum          = t.datum;
        typ            = t.typ;
        namn           = t.namn;
        belopp         = t.belopp;
        antal_kompisar = t.antal_kompisar;

        // Skapar en ny dynamisk array, storleken avgörs av antalet kompissar i transaktionen
        kompisar       = new string[antal_kompisar];

        // Kopierar kompisar från "t" till den aktuella instansen
        for (int i=0; i < antal_kompisar; i++)
        {
            kompisar[i] = t.kompisar[i];
        }
    }

    // Returnerar en referens till the aktuella instansen
    return *this;
}

// Hämtar beloppet för en transaktion
double Transaktion::hamtaBelopp() const
{
    return belopp;
}

// Hämtar antalet kompisar för en transaktion
int Transaktion::hamtaAntalKompisar() const
{
    return antal_kompisar;
}

// Kollar om en kompis finns i transaktionen, och kollar om namnet för kompisen matchar ett befintligt namn i kompisar arrayen
bool Transaktion::finnsKompis(const string &namnet) const
{
    for (int i = 0; i < antal_kompisar; i++)
        {
            if (kompisar[i] == namnet)
            {
                return true;
            }
        }
    return false;
}

// Läser in data för en transaktion
bool Transaktion::lasIn(istream &is, bool pOutput)
{
    // Här skapar jag pOutput, jag lägger upp det så att pOutput blir false innan man har valt att skriva in en ny transaktion
    // Detta för att programmet inte ska skriva ut instruktionerna för lasIn direkt efter filinläsning
    if (kompisar != nullptr)
    {
        delete[] kompisar;
        kompisar = nullptr;
    }

    if(pOutput)
    {
          cout << "Ange datum (YYMMDD): ";
          if (!(is >> datum))
            {
                return false;
            }

          cout << "Ange typ av transaktion: ";
          if (!(is >> typ))
            {
                return false;
            }

          cout << "Ange namn: ";
          if (!(is >> namn))
            {
                return false;
            }

          cout << "Ange belopp: ";
          if (!(is >> belopp))
            {
                return false;
            }

          cout << "Ange antal kompisar: ";
          if (!(is >> antal_kompisar))
            {
                return false;
            }

          kompisar = new string[antal_kompisar]; // Allokerar dynamiskt minne för kompisar arrayen
          cout << "Ange namn på kompisar:" << endl;
          for (int i = 0; i < antal_kompisar; i++)
            {
                cout << "Kompis " << (i + 1) << ": ";
                if (!(is >> kompisar[i]))
                {
                    delete[] kompisar; // Frigör minnet om inläsningen misslyckas
                    kompisar = nullptr; // Att sätta kompisar = nullptr är viktigt för att undvika dubbel frigöring
                    return false;}
            }
    }
    else
    {
        // Läser in filen utan att skriva ut alla instruktioner
        if (!(is >> datum >> typ >> namn >> belopp >> antal_kompisar))
        {
            return false;
        }

        kompisar = new string[antal_kompisar]; // Allokerar dynamiskt minne för kompisar arrayen

        for (int i = 0; i < antal_kompisar; i++)
          {
              if(!(is >> kompisar[i]))
              {
                  delete[] kompisar; // Frigör minnet om inläsningen misslyckas
                  kompisar = nullptr; // Att sätta kompisar = nullptr är viktigt för att undvika dubbel frigöring
                  return false;}
          }
    }

    // True om all data lästes in korrekt
    return true;
}

// Skriver ut data för en transaktion
void Transaktion::skrivUt(ostream& os) const
{
    os << datum << "\t" << typ << "\t" << namn << "\t" << belopp << "\t" << antal_kompisar;
    for (int i = 0; i < antal_kompisar; i++)
    {
        os << "\t" << kompisar[i];
    }
    os << endl;
}


// Skriver ut titlarna för transaktionstabellen
void Transaktion::skrivTitel(ostream& os) const
{
    os << "Datum\tTyp\tNamn\tBelopp\tAntal och lista av kompisar" << endl;
}

// Definitioner av Person-klassens funktioner

// Standard konstruktor
Person::Person()
{
    namn = "";
    betalat_andras = 0.0;
    skyldig = 0.0;
}

// Konstruktor med parametrar, används för att skapa objekt med specifika värden
Person::Person(const string &namn, double betalat_andras, double skyldig)
{
    this->namn = namn;
    this->betalat_andras = betalat_andras;
    this->skyldig = skyldig;
}

// Hämtar namnet för en person
string Person::hamtaNamn()
{
    return namn;
}

// Hämtar beloppet en person betalat för andra
double Person::hamtaBetalat()
{
    return betalat_andras;
}

// Hämtar beloppet som en person är skyldig
double Person::hamtaSkyldig()
{
    return skyldig;
}

// Skriver ut information om en person
void Person::skrivUt(ostream &os)
{
    os << namn << " ligger ute med " << betalat_andras << " och är skyldig " << skyldig << ". ";
    double franPotten = skallFranPotten();
    if (franPotten > 0)
    {
        os << "Skall ha " << franPotten << " från potten!";
    }
    else if (franPotten < 0)
    {
        os << "Skall lägga " << -franPotten << " till potten!";
    }
    else
    {
        os << "Är i balans!";
    }
    os << endl;
}


// Definitioner av PersonLista-klassens funktioner

// Standard konstruktor
PersonLista::PersonLista()
{
    antal_personer = 0;
    personer = nullptr;
}

//Destruktor som frigör minne
PersonLista::~PersonLista()
{
    delete[] personer; // Frigör minnet som allokerats för personer arrayen
}


// Lägger till en ny person till listan
void PersonLista::laggTill(Person ny_person) {
    Person* nyLista = new Person[antal_personer + 1];
    for (int i = 0; i < antal_personer; i++) {
        nyLista[i] = personer[i];
    }
    nyLista[antal_personer] = ny_person;
    delete[] personer;
    personer = nyLista;
    antal_personer++;
}

// Skriver ut alla personer i listan
void PersonLista::skrivUtOchFixa(ostream &os)
{
    os << "Personlista:" << endl;
    for (int i = 0; i < antal_personer; i++)
    {
        personer[i].skrivUt(os);
        os << endl;
    }
}

// Beräknar den totala summan som alla personer i listan är skyldig
double PersonLista::summaSkyldig() const
{
    double total = 0.0;
    for (int i = 0; i < antal_personer; i++)
    {
        total += personer[i].hamtaSkyldig();
    }
    return total;
}


// Beräknar den totala summan som alla personer i listan har betalat åt andra
double PersonLista::summaBetalat() const
{
    double total = 0.0;
    for (int i = 0; i < antal_personer; i++)
    {
        total += personer[i].hamtaBetalat();
    }
    return total;
}

// Kollara om ett specifikt namn finns med i listan av personer
bool PersonLista::finnsPerson(const string& namn)
{
    for (int i = 0; i < antal_personer; i++)
    {
        if (personer[i].hamtaNamn() == namn)
        {
            return true;
        }
    }
    return false;
}

// Hämtar en referens till ett person objekt med det angivna namnet för att uppdatera dess värde, finns inte ett objekt med det namnet så skickas ett felmeddelande
Person& PersonLista::getPersonByName(const string& namn)
{
    for (int i = 0; i < antal_personer; i++)
        {
            if (personer[i].hamtaNamn() == namn)
                {
                    return personer[i];
                }
        }
    // Onödigt i detta program, men använder man denna funktion i något annat sammanhang så kan det vara bra
    cout<<"Personen " + namn + " hittades inte!"<< endl;
}

// Definitioner av TransaktionsLista-klassens funktioner

// Standard konstruktor
TransaktionsLista::TransaktionsLista()
{
    antal_transaktioner = 0;
    transaktioner = nullptr;
}

// DEstruktor för att frigöra minne
TransaktionsLista::~TransaktionsLista()
{
    delete[] transaktioner;
}

// Används för att läsa in transaktioner från en ström, här använder jag en bool också för att inte skriva ut alla laggtill instruktioner vid filinläsning
void TransaktionsLista::lasIn(istream & is, bool pOutput)
{
    Transaktion t;
    while (t.lasIn(is, pOutput))
    {
        laggTill(t);
    }
}

// Skriver ut alla transaktioner
void TransaktionsLista::skrivUt(ostream & os)
{
    Transaktion t;
    t.skrivTitel(os);
    for (int i = 0; i < antal_transaktioner; i++)
    {
        transaktioner[i].skrivUt(os);
    }
}

// Lägger till en transaktion till en lista
void TransaktionsLista::laggTill(Transaktion& t) {
    Transaktion* nyLista = new Transaktion[antal_transaktioner + 1];
    for (int i = 0; i < antal_transaktioner; i++) {
        nyLista[i] = Transaktion(transaktioner[i]);

    }
    nyLista[antal_transaktioner] = t;
    delete[] transaktioner;
    transaktioner = nyLista;
    antal_transaktioner++;
}


// Räknar ut den totala kostnaden för alla transaktioner i listan
double TransaktionsLista::totalKostnad() const
{
    double total = 0.0;
    for (int i = 0; i < antal_transaktioner; i++)
    {
        total += transaktioner[i].hamtaBelopp();
    }
    return total;
}

// räknar ut vad en person med ett specifikt namn lagt ut baserat på alla transaktioner
double TransaktionsLista::liggerUteMed(const string &namnet)
{
    double total = 0.0;
    for (int i = 0; i < antal_transaktioner; i++)
    {
        if (transaktioner[i].hamtaNamn() == namnet)
        {
            total += (transaktioner[i].hamtaBelopp() / (transaktioner[i].hamtaAntalKompisar() + 1)) * transaktioner[i].hamtaAntalKompisar();
        }

    }
    return total;
}

// Räknar ut hur mycket en person med ett specifikt namn är skyldig baserat på alla transaktioner
double TransaktionsLista::arSkyldig(const string &namnet)
{
    double total = 0.0;
    for (int i = 0; i < antal_transaktioner; i++)
    {
        if (transaktioner[i].finnsKompis(namnet))
        {
            total += (transaktioner[i].hamtaBelopp() / (transaktioner[i].hamtaAntalKompisar() + 1));
        }
    }
    return total;
}

// Skapar en lista av personer med dessas totala lagt ut och skylding belopp
PersonLista TransaktionsLista::fixaPersoner()
{
    PersonLista lista;
    for (int i = 0; i < antal_transaktioner; i++)
    {
        Transaktion t = transaktioner[i];
        string namn = t.hamtaNamn();
        double ligger_ute_med = liggerUteMed(namn);
        double ar_skyldig = arSkyldig(namn);

        // Kollar om personen med det specifika namnet finns i listan
        if (!lista.finnsPerson(namn))
        {
            // Om de inte finns så skapas de och läggs till i listan
            Person p(namn, ligger_ute_med, ar_skyldig);
            lista.laggTill(p);
        }
        else
        {
            // Annars så hittas personen, och hens värde uppdateras
            Person& p = lista.getPersonByName(namn);
            p.setBetalat(ligger_ute_med);
            p.setSkyldig(ar_skyldig);
        }
    }

    return lista;
}
