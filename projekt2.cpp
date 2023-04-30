#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <future>

class dzialanie
{
public:
	int liczba_1, liczba_2;
	char znak;

	int oblicz()
	{
		int wynik;
		switch (znak)
		{
		case '+': wynik = liczba_1 + liczba_2; break;
		case '-': wynik = liczba_1 - liczba_2; break; 
		case '*': wynik = liczba_1 * liczba_2; break;
		case '/' : 
		{
			if (liczba_2 != 0)
				wynik = liczba_1 / liczba_2;
			break;
		}
		case '^': wynik = pow(liczba_1, liczba_2); break;
		} 
		return wynik;
	}
};
using namespace std;
bool czy_cyfra(char znak);
string zamien_notacje(string przed_zamiana);
int oblicz_calosc(string dzialania);

int main()
{
	string zwykla;
	string odwr_polska;

	cout << "DOMINIKA KROL 193588 ALICJA LISKIEWICZ 193096 ACiR gr.2" << endl << "PROJEKT 2, ZADANIE 9" << endl;
	cout << "Podaj ciag dzialan: " << endl;
	getline(cin, zwykla);
	odwr_polska = zamien_notacje(zwykla);

	future <int> wynik_obliczen = async(oblicz_calosc, odwr_polska);
	cout << "Zapis dzialan w odwroconej notacji polskiej: " << endl << odwr_polska << endl;
	cout << "Wynik obliczen: " << endl;
	int wynik_koncowy = wynik_obliczen.get();
	cout << wynik_koncowy << endl;
	return 0;
}

bool czy_cyfra(char znak)
{
	if (znak == '0' or znak == '1' or znak == '2' or znak == '3' or znak == '4' or znak == '5' or znak == '6' or znak == '7' or znak == '8' or znak == '9')
		return true;
	return false;
}

string zamien_notacje(string przed_zamiana)
{
	stack <char> znaki;
	bool czy_byla_cyfra=false;
	string po_zamianie;

	for (int i = 0; i < przed_zamiana.length(); i++)
	{
		if (czy_cyfra(przed_zamiana[i]) == true)
		{
			if (czy_byla_cyfra == true)
				po_zamianie += przed_zamiana[i];
			else
			{
				po_zamianie += ' ';
				po_zamianie += przed_zamiana[i];
				czy_byla_cyfra = true;
			}
		}
		else if (przed_zamiana[i] == '+' or przed_zamiana[i] == '-')
		{
			while (!znaki.empty() and znaki.top()!='(')
			{
				po_zamianie += ' ';
				po_zamianie += znaki.top();
				znaki.pop();
			}
			czy_byla_cyfra = false;
			znaki.push(przed_zamiana[i]);
		}
		else if (przed_zamiana[i] == '*' or przed_zamiana[i] == '/')
		{
			while (!znaki.empty() and (znaki.top() == '^' or znaki.top() == '*' or znaki.top() == '/'))
			{
				po_zamianie += ' ';
				po_zamianie += znaki.top();
				znaki.pop();
			}
			czy_byla_cyfra = false;
			znaki.push(przed_zamiana[i]);
		}
		else if (przed_zamiana[i] == '^')
		{
			while (!znaki.empty() and znaki.top() == '^')
			{
				po_zamianie += ' ';
				po_zamianie += znaki.top();
				znaki.pop();
			}
			czy_byla_cyfra = false;
			znaki.push(przed_zamiana[i]);
		}
		else if (przed_zamiana[i] == '(')
		{
			czy_byla_cyfra = false;
			znaki.push(przed_zamiana[i]);
		}
		else if (przed_zamiana[i] == ')')
		{
			while (!znaki.empty() and znaki.top() != '(')
			{
				po_zamianie += ' ';
				po_zamianie += znaki.top();
				znaki.pop();
				czy_byla_cyfra = false;
			}
		}
		else if (przed_zamiana[i] == ' ')
		{
			czy_byla_cyfra = false;
		}
	}
	while (!znaki.empty())
	{
		if (znaki.top() == '(')
			znaki.pop();
		else
		{
			po_zamianie += ' ';
			po_zamianie += znaki.top();
			znaki.pop();
		}
	}
	return po_zamianie;
}

int oblicz_calosc(string dzialania)
{
	stack <string> liczby;
	int wynik_koncowy;
	string l; 
	bool czy_byla_cyfra = false;

	for (int i = 0; i < dzialania.length(); i++)
	{
		if (czy_cyfra(dzialania[i]) == true)
		{
			l += dzialania[i];
			czy_byla_cyfra = true;
		}
		else if(dzialania[i] == ' ')
		{
			if (czy_byla_cyfra==true)
			{
				liczby.push(l);
				czy_byla_cyfra = false;
				l = {};
			}
		}
		else if (dzialania[i] == '+' or dzialania[i] == '-' or dzialania[i] == '*' or dzialania[i] == '/' or dzialania[i] == '^')
		{
			dzialanie* a = new dzialanie; 
			a->liczba_2 = atoi(liczby.top().c_str());
			liczby.pop();
			a->liczba_1 = atoi(liczby.top().c_str());
			liczby.pop();
			a->znak = dzialania[i];
			int pomocniczy = a->oblicz();
			delete a;
			string pomocniczy2 = to_string(pomocniczy);
			liczby.push(pomocniczy2);
		}
	}
	if (!liczby.empty())
	{
		wynik_koncowy = atoi(liczby.top().c_str());
		liczby.pop();
	}
	return wynik_koncowy;
}
