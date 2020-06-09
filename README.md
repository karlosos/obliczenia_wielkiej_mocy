# ZUT Obliczenia dużej mocy 

> Zadania z przedmiotu obliczenia dużej mocy. Przerobiono współbieżność w C++11, openmp i inteltbb

<div align='center'>
<img src='https://i.imgur.com/cQnkzbW.png'>
</div>

## Zadanie 2 - mnożenie macierzy

Napisać program w C++ 11 Threads. do mnożenia macierzy o dużych rozmiarach. Zbadać transponowanie macierzy B do poprawy trafień w cache. Zmierzyć czasy sekwencyjnych i równoległych wykonań kodu.


### For developers

Compiling and running program:

```
g++ -pthread example.cpp && ./a.out
```

How to do multithreading in C++: 
https://www.geeksforgeeks.org/multithreading-in-cpp/

Multiplication of matrix using threads:
https://www.geeksforgeeks.org/multiplication-of-matrix-using-threads/

## Zadanie 3 - fraktal mandelbrota

C++11 Threads
https://rosettacode.org/wiki/Mandelbrot_set#PPM_non_interactive


Kompilujemy program, uwspółbieżnamy główną pętle

1. zapis do pliku w każdej iteracji jest problemem, przenieść go za pętlę, użyć tablicy do zapisu danych np. rozszerzyć o dwa wymiary tablicę color (usuną słowo static, nie alokować w pamięci funkcji)
2. przenieść zmienne które są używane do lokalnych zmiennych funkcji wątka, resztę przenieść do global
3. pokolorwać tło względem numeru wątka
 
Do zdanial:
* plik cpp
* czasy
* plik graficzny z kolorami tła ppm

## Zadanie 4 - trojkąt sierpińskiego

Na prośbę studenta udostępniam kolejne zadanie.Termin 27 marca.

Cel zadania: poznanie współbieżnej rekurencji.
Główny wątek powołuje 3 wątki, następnie każdy z nich następne 3 itd. Czyli rekurencja.
Rysujemy w tablicy binarnej trójkąt. Bok niech będzie potęga dwójki, nie będzie problemu z ułamkami.
Trójkąt prostokątny z kątem prostym w lewym i dolnym rogu ekranu. Do rysowania ciągle ta sama funkcja.

Program główny rysuje głowny trójkąt. Kolejne są o połowę mniejsze w każdym narożniku itd. Wątki mnożą sie i
rysują dopóki nie przekroczą granicy maks. zagnieżdzenia. Wtedy zwaracają sterowanie do rodzica itd. aż do głównego wątka.

Uwaga wątki uruchamiamy a potem wszystkie 3 "joinujemy", inaczej utracimy współbieżność. Nie wykonujemy pomiarów czasu.
Zapis do PPM. Co przekazujemy do funkcji? Wystarczy x,y wierzchołka lewego górnego oraz poziom zagniezdzenia. Rozmiar wyliczymy z początkowego rozmiaru
z zmiennej globalnej podzielonej przez 2 tyle razy ile poziom. W ten sposób obliczymy pozostałe 2 punkty.

## Zadanie 5 - spirala ulama

Brak instrukcji do zadania.

## Zadanie 6 - labirynt

Współbieżność w programie jest podobna do fraktala Sierpińskeigo. Będzie rekurencyjna. Nie badamy przyspieszenia i czasów.

Wprowadzamy synchronizację w postaci obiektów mutex.
W własnym zakresie generujemy przykładowy labirynt. Tablica będzie wypełniona początkowa 0 -zerami gdzie korytarz oraz -1 gdzie jest ściana.
W dowolnym punkcie korytarza zaczynamy działanie programu - pierwszy wątek. Bada on sąsiednie komórki czy są korytarzem (na górze, w dole, lewo i prawo).

 
Jeżeli ich liczba to 1
  * kontynuuje przechodzenie wpisując tam własny id > 0 (sekcja krytyczna)

Jeżeli 2 lub 3
  * kontynuuje przechodzenie na jednym z nich wpisując tam własny id > 0, (sekcja krytyczna)
  * w pozostałych nowe tworzy wątki  i wrzuca na własną listę

Jeżeli 0
  * wątek przerywa ale wcześniej join dla każdego dziecka na liście

 

Synchronizacja

Obiekt mutex pilnuje rozdawanie kolejnego numeru (licznika) wątkom. Wartość pobieramy i inkrementujemy w sekcji krytycznej.
Startujemy od 1. Liczba ta jest również indeksem w tablicy kolorów.
Przechodząc komórkę wpisujemy tam liczbę i zapisując do PPM umieszczamy właściwy kolor wątka. Ściany rysujemy innym odznaczającym się kolorem.
Tablica mutexów, tam gdzie są korytarze odwiedzając każdą komórkę musimy to zrobić w sekcji krytycznej. Możliwe są cykle w ścieżkach zatem wątki mogą się "zderzyć".
W sekcji krytycznej ustawiamy swój numer na danych koordynatach labiryntu.
Program się kończy gdy wątki rekurencyjnie się zakończą a pierwszy wątek też nie będzie już miał drogi, Sekwencyjnie zapisujemy PPM.

Potrzebne zatem tablice

* labirytnu
* mutexów
* PPM
* kolorów

## Zadanie 7 - fraktal mandelbrota openmp

Celem zadania jest przyspieszenie oraz zachowanie skalowalności obliczeń dla poznanego już programu generującego fraktal Mandelbrota

1. Usuń wielowątkowość C++11 Threads i zastąp ją pragmami omp paralel, for oraz klauzul private i shared. Porównanie plików wyjściowych wersji sekwencyjnej i równoległej. Zapoznanie się z pojęciami deterministyczności oraz zgodności kodu równoległego zsekwencyjnym odpowiednikiem. Kod funkcji wraca do main. Pomiar czasu zastąp przez omp_get_wtime.
2. Zmierzenie czasu kodu dla wielu wątków oraz policzenie przyspieszenia dla wątków 2,4 ewent. 8 oraz różnych rozmiarów problemów (obrazka). Pojęcie skalowalności.
3. Wprowadzenie pojęcia harmogramowania iteracji czyli zapoznanie się z klauzulą schedule i jej parametrami w pragmie omp for. Modyfikacje parametrów schedule. Interpretacja wyników: student ma za zadanie zrozumieć dlaczego domyślny podział nie jest efektywny dla większej liczby wątków niż 2. Pokolorować tłowg nr wątka omp_get_thread_num().
4. Wyjaśnienie działania kodu. Pomoże w tym pkt 5.
5. Zlicz sumę iteracji pętli wewn. wątków, dlaczego są nie równe dla wątków >= 4. Statystyka ma się wyświetlać na ekranie.
6. Zapoznanie się z pojęciami równoważenie obciążenia (load balancing) i czasy przestojów (idle-time).
7. Strojenie klauzuli schedule, zapoznanie się z harmonogramowaniem dynamicznym i wielkością paczki (chunk size).
8. Wykonanie sprawozdania na podstawie wyników z punktów 2, 4 i 5, tabele z wynikami czasowymi i wykresy przyspieszeń, analiza porównawcza dla różnych parametrów schedule oraz wnioski i statystyki z pkt 5. Obrazki z różnymi harmonogramowaniami.
9. Co to jest guided, czy w tym zadaniu pomoże? Uzasadnij w sprawozdaniu. Wyjaśnij dlaczego czasy nie są złe dla 2 wątków. Pojęcie ziarnistości.
10. Na wyjściu: wersja elektroniczna sprawozdania oraz program cpp i pliki ppm.

## Zadanie 8 - spirala ulama i trójkąt sierpińskiego openmp

Przepisz programy Spirala Ulama z  zagniezdzona pragma parallel for  i fraktal Sierpińskiego do openmp z użyciem pragmy task i taskwait. Do oceny sam kod.

## Zadanie 9 - labirynt openmp

Labirynt w openmp.   Rekurencja - task i taskwait. Pobieranie licznika: pragma omp critical. Tablica mutex zastępujemy omp_lock_t i funkcjami (patrz tutorial openmp). Do sprawdzenia: tylko kod.

## Zadanie 10 - filtry inteltbb

Intel TBB, filtry graficzne

Wybieramy obraz, zdjęcie. Zapisujemy do PPM.

Wczytujemy do tablicy color ten plik (to nowość, trzeba wczytać najpierw nagłówek, tablice alokujemy tak dużą jaka rozdzielczość obrazka).

Przepuszczamy filtr przez obrazek.

http://www.algorytm.org/przetwarzanie-obrazow/filtrowanie-obrazow.html

Wybieramy maskę filtru. I dla każdego piksela sumujemy iloczyn wag i sąsiadów, ewentualnie dzieląc przez stałą.

Filtry obliczamy dla każdego kanału RGB osobno!

Efekt zapisujemy na dysk w PPM. Wybrać filtry i obraz tak by przekszałcenie było widoczne.

Czytamy o parallelfor, zrównoleglamy w TBB, używamy lambdy. Mierzymy czasy, można zrobić to tak jak w C++ 11, można użyć tick_count z przestrzeni tbb.

Na zajęciach lab omówimy zadanie. Na wykładzie omówimy wstęp do TBB i jak działa parallel_for.

## Zadanie 11 - fraktal lapunowa inteltbb

Mierzymy czasy, przyspieszenie zrównoleglonej aplikacji fraktal Lapunowa za pomocą parallelfor i blocked_range dla obu pętli (w pionie i poziomie). Zmieniamy wielkość ziarna (dla chętnych też liczba wątków). Zdajemy kod programu oraz powtarzamy testy dla różnych wielkości obrazka. Czy wątki mają tyle samo obliczeń? Krótkie sprawozdanie do 2 stron. Źrodła fratkala Lapunowa w załaczniku.

 

Uwaga. Zamiast zagnieżdżonych parallelfor możecie użyć `blockedrange2d`

```
    for( size_t i=r.rows().begin(); i!=r.rows().end(); ++i )     

        for( size_t j=r.cols().begin(); j!=r.cols().end(); ++j )
```
 

`parallel_for( blocked_range2d<size_t>(0, M, 16, 0, N, 32)`,    `16` i `32` to gs w wierszach i kolumnach - 6 argumentów, jest też przeciązona wersja bez podawania gs z 4 argumentami (0,M,0,N)

## Zadanie 12 - game of life inteltbb

Gra w życie (symulacja). Poczytać.

Plansza aktualna i następna. Dwie tablice. 

2 wątki, uruchamiamy za pomoca tbb invoke.

Obydwie skanują aktualną. Reprezentują własną kolonię. Na początku losowo sieją swoje kolonie po planszy.

Reguły gry wymyślacie sami lub szukacie jakie są dla dwóch kolonii (symbioza czy zwalczanie). Zliczając liczbę sąsiadów decydujecie o narodzinach komórki lub śmierci.

Zapis migawek (plansz) w ppm do foldera albo animacji klatek (zdaję się na wasze pomysły).

Uwaga. Wątki skanując aktualną tablicę wpisują do nowej swoje komórki. Jeśli zajęte już to nie. A więc tablica mutexów tbb jak w labiryncie (chronić odczyt i zapis). Wybrać podstawowy rodzaj mutexu.

Zdajemy pliki graficzne najlepiej w formie animowanego gif itp. oraz kod cpp.

## Zadanie 13 - line of sight inteltbb

Promień świata i parallel scan. Przykład omawiany na poprzednim przykładzie. Dla 2 wątków.
Przykład wygenerowania słupków w PPM w kodzie załączonym.
Źródło światła w prawym górnym ekranu.
Wykonujemy pre-scan i final-scan zaczynając od paczek z prawej strony (ich wielkość dowolna).
Program wypisuje konsoli etapy: wątek nr 1 lub 2, paczka, maksimu, pre-scan lub final, 
Słupki ich górne piksele malujemy (lub je całe) na czerwono gdy nieoświtlone tg mniejszy, zasłonięte, na zielono w przeciwnym razie. Obejrzyj przykład w podręczniku. Kolorujemy słupki, nie rysujemy prostych do źródła światlła.
Zapisujemy w ppm i wraz z kodem zdajemy program.
