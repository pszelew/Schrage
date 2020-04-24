// Schrage.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h> 
#include <string>
#include <list>
#include <queue> 
#include "heap_sort.h"
#include "chrono"
#include <vector>

using namespace std;


struct result_shrage
{
	list<zadanie> lista_zadan;
	int dlugoscUsz;
};

struct CompareR {
	bool operator()(zadanie const& p1, zadanie const& p2)
	{
		if (p1.R < p2.R) return false;
		if (p1.R == p2.R)
		{
			if (p1.id < p2.id) return false;
				else return true;
		}
		if (p1.R > p2.R) return true;
	}
};

struct CompareQ {
	bool operator()(zadanie const& p1, zadanie const& p2)
	{
		//return p1.Q < p2.Q;
		if (p1.Q < p2.Q) return true;
		if (p1.Q == p2.Q)
		{
			if (p1.id < p2.id) return false;
				else return true;
		}
		if (p1.Q > p2.Q) return false;
	}
};




void wypisz_kolejnosc(list<zadanie> list)
{
	int n = list.size();
	for (int i = 0; i < n; i++)
	{
		cout << list.front().id << ", ";
		list.pop_front();
	}
	cout << endl;
}


int comparer_quick(const void* a, const void* b)
{
	if ((*(zadanie*)a).R < (*(zadanie*)b).R) return -1;
	if ((*(zadanie*)a).R == (*(zadanie*)b).R)
	{
		if ((*(zadanie*)a).id < (*(zadanie*)b).id) return -1;
		else return 1;
	}
	if ((*(zadanie*)a).R > (*(zadanie*)b).R) return 1;
}


int compareq_quick(const void* a, const void* b)
{

	if ((*(zadanie*)a).Q < (*(zadanie*)b).Q) return 1;
	if ((*(zadanie*)a).Q == (*(zadanie*)b).Q)
	{
		if ((*(zadanie*)a).id < (*(zadanie*)b).id) return -1;
		else return 1;
	}
	if ((*(zadanie*)a).Q > (*(zadanie*)b).Q) return -1;
}


void sort_r(int n, zadanie tab[])
{
	qsort(tab, n, sizeof(zadanie), comparer_quick);
}


void sort_q(int n, zadanie tab[])
{
	qsort(tab, n, sizeof(zadanie), compareq_quick);
}

result_shrage schrage_normal(int n, zadanie tab[])
{
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;

	//dwie tablice zawierajace zadania nieprzygotowane oraz gotowe
	zadanie *N = new zadanie[n];
	zadanie *G = new zadanie[n];
	int size_N = n;
	int size_G = 0;
	//dodajemy wszystkie zadania do tabliy zadan nie gotowych
	for (int i = 0; i < n; i++)
	{
		N[i] = tab[i];
	}
	//posortuj zbior N -> najmniejsze n na poczatku
	sort_r(size_N, N);

	while (size_G || size_N)
	{
		//odswiez zbior zadan gotowych
		while (size_N && N[0].R <= t)
		{
			e = N[0];
			pe = N[0].P;
			G[size_G++] = e; //powieksz tablice i dodaj na koniec 
			swap(N[0], N[--size_N]); //zamien piewszy z ostatnim elementem i zmniejsz tablice
			//posortuj tablice N oraz G
			sort_r(size_N, N);
			sort_q(size_G, G);
		}
		//wyznacz t_next
		{
			if (size_N)
				t_next = N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (size_G == 0)
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G[0];
			//pe to czas wykonania nastepnego zadania
			pe = G[0].P;
			//wykonujemy cale zadanie j
			
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				swap(G[0], G[--size_G]); //zamien piewszy z ostatnim elementem i zmniejsz tablice

				//posortowac tablice G
				sort_q(size_G, G);
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);

				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if (!lista_zadan.empty() && lista_zadan.back().id != e.id)
				lista_zadan.push_back(e);
			if (lista_zadan.empty())
				lista_zadan.push_back(e);
		}
	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}


result_shrage schrage_interrupt(int n, zadanie tab[])
{
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;
	
	//dwie tablice zawierajace zadania nieprzygotowane oraz gotowe
	zadanie *N = new zadanie[n];
	zadanie *G = new zadanie[n];
	int size_N = n;
	int size_G = 0;
	//dodajemy wszystkie zadania do tabliy zadan nie gotowych
	for (int i = 0; i < n; i++)
	{
		N[i] = tab[i];
	}
	//posortuj zbior N -> najmniejsze n na poczatku
	sort_r(size_N, N);

	while (size_G || size_N)
	{	
		//odswiez zbior zadan gotowych
		while (size_N && N[0].R <= t)
		{
			e = N[0];
			pe = N[0].P;
			G[size_G++] = e; //powieksz tablice i dodaj na koniec 
			swap(N[0], N[--size_N]); //zamien piewszy z ostatnim elementem i zmniejsz tablice
			//posortuj tablice N oraz G
			sort_r(size_N, N);
			sort_q(size_G, G);
		}
		//wyznacz t_next
		{
			if (size_N)
				t_next = N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (size_G==0)
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G[0];
			//pe to czas wykonania nastepnego zadania
			pe = G[0].P;
			//wykonujemy zadanie j w czesci
			if (t_next < t + pe)
			{
				//aktualizacja czasu zadania
				pe = pe - (t_next - t);
				e.P = pe;
				swap(G[0], G[--size_G]); //zamien piewszy z ostatnim elementem i zmniejsz tablice
				G[size_G++] = e;
				//przechodzimy do nastepnej chwili czasowej
				t = t_next;

				//posortowac tablice G
				sort_q(size_G, G);

			}
			//wykonujemy cale zadanie j
			else
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				swap(G[0], G[--size_G]); //zamien piewszy z ostatnim elementem i zmniejsz tablice

				//posortowac tablice G
				sort_q(size_G, G);
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);

				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if (!lista_zadan.empty() && lista_zadan.back().id != e.id)
				lista_zadan.push_back(e);
			if (lista_zadan.empty())
				lista_zadan.push_back(e);
		}
	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}





result_shrage schrage_stl_queue_normal(int n, zadanie tab[])
{
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;
	//zadanie* dynamiczna = new zadanie[n];
	//kolejka zawierajaca zadania jeszcze nie gotowe. Najpierw mamy zadania z malym R
	priority_queue<zadanie, vector<zadanie>, CompareR> N;
	//kolejka zawierajaca zadania juz gotowe do wykonania. Najpierw mamy zadania z duzym Q
	priority_queue<zadanie, vector<zadanie>, CompareQ> G;
	//dodajemy wszystkie zadania do kolejki zadan nie gotowych
	for (int i = 0; i < n; i++)
	{
		N.push(tab[i]);
	}

	while (!G.empty() || !N.empty())
	{
		//odswiez zbior zadan gotowych
		while (!N.empty() && N.top().R <= t)
		{
			e = N.top();
			pe = N.top().P;
			G.push(N.top());
			N.pop();
		}
		//wyznacz t_next
		{
			if (!N.empty())
				t_next = N.top().R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (G.empty())
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G.top();
			//pe to czas wykonania nastepnego zadania
			pe = G.top().P;
	
			//wykonujemy cale zadanie j
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				G.pop();
				
				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);

				//przesuwamy sie do opdowieniej chwili czasowej
				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if (!lista_zadan.empty() && lista_zadan.back().id != e.id)
				lista_zadan.push_back(e);
			if (lista_zadan.empty())
				lista_zadan.push_back(e);
		}

	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}

result_shrage schrage_stl_queue_interrupt(int n, zadanie tab[])
{
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;
	//zadanie* dynamiczna = new zadanie[n];
	//kolejka zawierajaca zadania jeszcze nie gotowe. Najpierw mamy zadania z malym R
	priority_queue<zadanie, vector<zadanie>, CompareR> N;
	//kolejka zawierajaca zadania juz gotowe do wykonania. Najpierw mamy zadania z duzym Q
	priority_queue<zadanie, vector<zadanie>, CompareQ> G;
	//dodajemy wszystkie zadania do kolejki zadan nie gotowych
	for (int i = 0; i < n; i++)
	{
		N.push(tab[i]);
	}

	while (!G.empty() || !N.empty())
	{
		//odswiez zbior zadan gotowych
		while (!N.empty() && N.top().R <= t)
		{
			e = N.top();
			pe = N.top().P;
			G.push(N.top());
			N.pop();
		}
		//wyznacz t_next
		{
			if (!N.empty())
				t_next = N.top().R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (G.empty())
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania. Tutaj bedzie roznica w algorytmie
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G.top();
			//pe to czas wykonania nastepnego zadania
			pe = G.top().P;
			//wykonujemy zadanie j w czesci
			if (t_next < t + pe)
			{
				//aktualizacja czasu zadania
				pe = pe - (t_next - t);
				e.P = pe;
				G.pop();
				G.push(e);
				//przechodzimy do nastepnej chwili czasowej
				t = t_next;
			}
			//wykonujemy cale zadanie j
			else
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				G.pop();
				//przesuwamy sie do opdowiedniej chwili czasowej
				
				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);
		
				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if(!lista_zadan.empty() && lista_zadan.back().id!=e.id)
				lista_zadan.push_back(e);
			if(lista_zadan.empty())
				lista_zadan.push_back(e);
		}
	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}

result_shrage schrage_heap_normal(int n, zadanie tab[])
{
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;

	//kopce sluzace do przechowywania elementow
	zadanie *heap_N = new zadanie[n];
	int size_N = n;
	zadanie *heap_G = new zadanie[n];
	int size_G = 0;
	//przekopiowanie elementow do kopca
	for (int i = 0; i < n; i++)
	{
		heap_N[i] = tab[i];
	}
	//odtworzenie kopca	
	make_heap(heap_N, size_N, true); //kopiec, w ktorym minimalizujemy R

	while (size_G || size_N)
	{
		//odswiez zbior zadan gotowych
		while (size_N && heap_N[0].R <= t)
		{
			e = heap_N[0];
			pe = heap_N[0].P;
			push(heap_G, size_G, false, heap_N[0]);
			pop(heap_N, size_N, true);
		}
		//wyznacz t_next
		{
			if (size_N)
				t_next = heap_N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (!size_G)
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania. Tutaj bedzie roznica w algorytmie
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = heap_G[0];
			//pe to czas wykonania nastepnego zadania
			pe = heap_G[0].P;
			//wykonujemy cale zadanie j
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				pop(heap_G, size_G, false);
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);
				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if (!lista_zadan.empty() && lista_zadan.back().id != e.id)
				lista_zadan.push_back(e);
			if (lista_zadan.empty())
				lista_zadan.push_back(e);
		}
	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}


result_shrage schrage_heap_interrupt(int n, zadanie tab[])
{
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;
	
	//kopce sluzace do przechowywania elementow
	zadanie *heap_N = new zadanie[n];
	int size_N = n;
	zadanie *heap_G = new zadanie[n];
	int size_G = 0;
	//przekopiowanie elementow do kopca
	for (int i = 0; i < n; i++)
	{
		heap_N[i] = tab[i];
	}
	//odtworzenie kopca	
	make_heap(heap_N, size_N, true); //kopiec, w ktorym minimalizujemy R

	while (size_G || size_N)
	{
		//odswiez zbior zadan gotowych
		while (size_N && heap_N[0].R <= t)
		{
			e = heap_N[0];
			pe = heap_N[0].P;
			push(heap_G,size_G, false, heap_N[0]);
			pop(heap_N, size_N, true);	
		}
		//wyznacz t_next
		{
			if (size_N)
				t_next = heap_N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (!size_G)
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania. Tutaj bedzie roznica w algorytmie
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = heap_G[0];
			//pe to czas wykonania nastepnego zadania
			pe = heap_G[0].P;
			//wykonujemy zadanie j w czesci
			if (t_next < t + pe)
			{
				//aktualizacja czasu zadania
				pe = pe - (t_next - t);
				e.P = pe;
				pop(heap_G, size_G, false);
				push(heap_G, size_G, false, e);
				//przechodzimy do nastepnej chwili czasowej
				t = t_next;
			}
			//wykonujemy cale zadanie j
			else
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				pop(heap_G, size_G, false);
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);
				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if (!lista_zadan.empty() && lista_zadan.back().id != e.id)
				lista_zadan.push_back(e);
			if (lista_zadan.empty())
				lista_zadan.push_back(e);
		}
	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}

result_shrage schrage_stl_heap_normal(int n, zadanie tab[])
{
	//chciałbym otrzymać dane w vectorze
	vector<zadanie> N;
	for (int i = 0; i < n; i++)
	{
		N.push_back(tab[i]);
	}
	vector<zadanie> G;
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;
	//odtworzenie kopca	

	std::make_heap(N.begin(), N.end(), CompareR()); //kopiec, w ktorym minimalizujemy R
	while (!G.empty() || !N.empty())
	{
		//odswiez zbior zadan gotowych
		while (!N.empty() && N[0].R <= t)
		{
			e = N[0];
			pe = N[0].P;
			G.push_back(e);
			push_heap(G.begin(), G.end(), CompareQ());
			pop_heap(N.begin(), N.end(), CompareR());
			N.pop_back();
		}
		//wyznacz t_next
		{
			if (!N.empty())
				t_next = N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (G.empty())
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania. Tutaj bedzie roznica w algorytmie
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G[0];
			//pe to czas wykonania nastepnego zadania
			pe = G[0].P;
			//wykonujemy cale zadanie
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				pop_heap(G.begin(), G.end(), CompareQ());
				G.pop_back();
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);
				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if (!lista_zadan.empty() && lista_zadan.back().id != e.id)
				lista_zadan.push_back(e);
			if (lista_zadan.empty())
				lista_zadan.push_back(e);
		}
	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}

result_shrage schrage_stl_heap_interrupt(int n, zadanie tab[])
{
	//chciałbym otrzymać dane w vectorze
	vector<zadanie> N;
	for (int i = 0; i < n; i++)
	{
		N.push_back(tab[i]);
	}
	vector<zadanie> G;
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	list<zadanie> lista_zadan;
	//odtworzenie kopca	

	std::make_heap(N.begin(), N.end(), CompareR()); //kopiec, w ktorym minimalizujemy R
	while (!G.empty() || !N.empty())
	{
		//odswiez zbior zadan gotowych
		while (!N.empty() && N[0].R <= t)
		{
			e = N[0];
			pe = N[0].P;
			G.push_back(e);
			push_heap(G.begin(), G.end(),CompareQ());
			pop_heap(N.begin(), N.end(), CompareR());
			N.pop_back();
		}
		//wyznacz t_next
		{
			if (!N.empty())
				t_next = N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (G.empty())
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania. Tutaj bedzie roznica w algorytmie
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G[0];
			//pe to czas wykonania nastepnego zadania
			pe = G[0].P;
			//wykonujemy zadanie j w czesci
			if (t_next < t + pe)
			{
				//aktualizacja czasu zadania
				pe = pe - (t_next - t);
				e.P = pe;
				pop_heap(G.begin(), G.end(), CompareQ());
				G.pop_back();
				G.push_back(e);
				push_heap(G.begin(), G.end(), CompareQ());
				//przechodzimy do nastepnej chwili czasowej
				t = t_next;
			}
			//wykonujemy cale zadanie j
			else
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				pop_heap(G.begin(), G.end(), CompareQ());
				G.pop_back();
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);
				t = t + pe;
			}
			//dodajemy zadanie do listy rozwiazan
			if (!lista_zadan.empty() && lista_zadan.back().id != e.id)
				lista_zadan.push_back(e);
			if (lista_zadan.empty())
				lista_zadan.push_back(e);
		}
	}
	result_shrage ret;
	ret.lista_zadan = lista_zadan;
	ret.dlugoscUsz = c;

	return ret;
}


int main()
{
	int n;
	int temp;
	int pi[99];
	zadanie tab[99];
	list<zadanie> lista_zadan;
	result_shrage res1;
	result_shrage res2;

	int total = 0;
	int total_duration = 0;
	int temp_dur;


	cout << "*****************Schrage bez kopcow***********************" << endl;
	for (int g = 0; g < 9; g++)
	{
		ifstream data("schr.data.txt");
		string s = "";
		string nazwa = "data." + to_string(g)+":";
		while (s != nazwa)
		{
			data >> s;
		}
		data >> n;
		for (int i = 0; i < n; i++)
		{
			tab[i].id = i;
			data >> tab[i].R;
			data >> tab[i].P;
			data >> tab[i].Q;
		}
		auto t1 = std::chrono::high_resolution_clock::now();
		res1 = schrage_normal(n, tab);
		auto t2 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania normalnego algorytmu: " << temp_dur << "us\n";
		auto t3 = std::chrono::high_resolution_clock::now();
		res2 = schrage_interrupt(n, tab);
		auto t4 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania algorytmu z podzialem: " << temp_dur << "us\n";

		lista_zadan = res1.lista_zadan;
		temp = res1.dlugoscUsz;
		cout << "schrpmtn: " << res2.dlugoscUsz << endl;
		cout << "Kolejnosc zestaw "<< g << ": "<<endl;
		wypisz_kolejnosc(lista_zadan);
		cout << "czas zestaw " << g << ": ";
		cout << temp << endl;
		total += temp;
	}
	
	cout << "Calkowity czas wykonywania testu to: " <<total_duration << "us\n";
	total = 0;
	total_duration = 0;
	cout << endl << endl<<"*****************Schrage na kolejce priorytetowej z STL***********************" << endl;
	for (int g = 0; g < 9; g++)
	{
		ifstream data("schr.data.txt");
		string s = "";
		string nazwa = "data." + to_string(g) + ":";
		while (s != nazwa)
		{
			data >> s;
		}
		data >> n;
		for (int i = 0; i < n; i++)
		{
			tab[i].id = i;
			data >> tab[i].R;
			data >> tab[i].P;
			data >> tab[i].Q;
		}
		auto t1 = std::chrono::high_resolution_clock::now();
		res1 = schrage_stl_queue_normal(n, tab);
		auto t2 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania normalnego algorytmu: " << temp_dur << "us\n";
		auto t3 = std::chrono::high_resolution_clock::now();
		res2 = schrage_stl_queue_interrupt(n, tab);
		auto t4 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania algorytmu z podzialem: " << temp_dur << "us\n";
		lista_zadan = res1.lista_zadan;
		temp = res1.dlugoscUsz;
		cout << "schrpmtn: " << res2.dlugoscUsz << endl;
		cout << "Kolejnosc zestaw " << g << ": "<<endl;
		wypisz_kolejnosc(lista_zadan);
		cout << "czas zestaw " << g << ": ";
		cout << temp << endl;
		total += temp;
	}

	cout << "Calkowity czas wykonywania testu to: " << total_duration << "us\n";
	total = 0;
	total_duration = 0;
	cout << endl << endl<<"*****************Schrage na kopcach wlasnych***********************" << endl;
	for (int g = 0; g < 9; g++)
	{
		ifstream data("schr.data.txt");
		string s = "";
		string nazwa = "data." + to_string(g) + ":";
		while (s != nazwa)
		{
			data >> s;
		}
		data >> n;
		for (int i = 0; i < n; i++)
		{
			tab[i].id = i;
			data >> tab[i].R;
			data >> tab[i].P;
			data >> tab[i].Q;
		}
		auto t1 = std::chrono::high_resolution_clock::now();
		res1 = schrage_heap_normal(n, tab);
		auto t2 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania normalnego algorytmu: " << temp_dur << "us\n";
		auto t3 = std::chrono::high_resolution_clock::now();
		res2 = schrage_heap_interrupt(n, tab);
		auto t4 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania algorytmu z podzialem: " << temp_dur << "us\n";

		lista_zadan = res1.lista_zadan;
		temp = res1.dlugoscUsz;
		cout << "schrpmtn: " << res2.dlugoscUsz << endl;
		cout << "Kolejnosc zestaw " << g << ": "<<endl;
		wypisz_kolejnosc(lista_zadan);
		cout << "czas zestaw " << g << ": ";
		cout << temp << endl;
		total += temp;
	}

	cout << "Calkowity czas wykonywania testu to: " << total_duration << "us\n";
	total = 0;
	total_duration = 0;
	cout << endl << endl << "*****************Schrage na kopcach Z STL***********************" << endl;
	for (int g = 0; g < 9; g++)
	{
		ifstream data("schr.data.txt");
		string s = "";
		string nazwa = "data." + to_string(g) + ":";
		while (s != nazwa)
		{
			data >> s;
		}
		data >> n;
		for (int i = 0; i < n; i++)
		{
			tab[i].id = i;
			data >> tab[i].R;
			data >> tab[i].P;
			data >> tab[i].Q;
		}
		auto t1 = std::chrono::high_resolution_clock::now();
		res1 = schrage_stl_heap_normal(n, tab);
		auto t2 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania normalnego algorytmu: " << temp_dur << "us\n";
		auto t3 = std::chrono::high_resolution_clock::now();
		res2 = schrage_stl_heap_interrupt(n, tab);
		auto t4 = std::chrono::high_resolution_clock::now();
		temp_dur = chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
		total_duration += temp_dur;
		std::cout << "Czas wykonywania algorytmu z podzialem: " << temp_dur << "us\n";

		lista_zadan = res1.lista_zadan;
		temp = res1.dlugoscUsz;
		cout << "schrpmtn: " << res2.dlugoscUsz << endl;
		cout << "Kolejnosc zestaw " << g << ": " << endl;
		wypisz_kolejnosc(lista_zadan);
		cout << "czas zestaw " << g << ": ";
		cout << temp << endl;
		total += temp;
	}

	cout << "Calkowity czas wykonywania testu to: " << total_duration <<"us\n";
	total = 0;
	total_duration = 0;

	system("pause");
}


// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
