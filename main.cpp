#include <iostream>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

#define randnum(min, max) ((rand() % ((max) + 1 - (min))) + (min))

string arena[20][20];

class Oyuncular {
public:
    int x, y;
    string isim;

    Oyuncular(const string& tip, int sayi) {
        isim = tip + to_string(sayi);
        do {
            x = randnum(0, 19);
            y = randnum(0, 19);
        } while (arena[x][y] != ". ");
        arena[x][y] = isim;
    }

    virtual ~Oyuncular() {}
};

class Robotlar : public Oyuncular {
public:
    Robotlar(const string& tip, int sayi) : Oyuncular(tip, sayi) {}
};

class Freezer : public Robotlar {
public:
    Freezer(int sayi) : Robotlar("F", sayi) {}
};

class Jumper : public Robotlar {
public:
    Jumper(int sayi) : Robotlar("J", sayi) {}
};

class Shooter : public Robotlar {
public:
    Shooter(int sayi) : Robotlar("S", sayi) {}
};

class Player : public Oyuncular {
public:
    Player(int sayi) : Oyuncular("P", sayi) {}
};

void arenayaz() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            cout << arena[i][j];
        }
        cout << endl;
    }
}

int main() {
    srand(time(0));


    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            arena[i][j] = ". ";

    int pSayisi, sSayisi, jSayisi, fSayisi;

    cout << "Kac Player olsun? "; cin >> pSayisi;
    cout << "Kac Shooter olsun? "; cin >> sSayisi;
    cout << "Kac Jumper olsun? "; cin >> jSayisi;
    cout << "Kac Freezer olsun? "; cin >> fSayisi;

    vector<Oyuncular*> oyuncular;

    for (int i = 1; i <= pSayisi; i++) oyuncular.push_back(new Player(i));
    for (int i = 1; i <= sSayisi; i++) oyuncular.push_back(new Shooter(i));
    for (int i = 1; i <= jSayisi; i++) oyuncular.push_back(new Jumper(i));
    for (int i = 1; i <= fSayisi; i++) oyuncular.push_back(new Freezer(i));

    cout << "\nArena durumu:\n";
    arenayaz();

    for (auto oyuncu : oyuncular) {
        delete oyuncu;
    }

    return 0;
}
// TODO: add code here!
