#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

#define randnum(min, max) ((rand() % ((max) + 1 - (min))) + (min))
const int SIZE = 20;

enum Durum { CANLI, OLU };

string arena[SIZE][SIZE];

class Oyuncular {
public:
    int x, y;
    string isim;
    int skor = 0;
    Durum durum = CANLI;

    Oyuncular(const string& tip, int id) {
        isim = tip + to_string(id);
        do {
            x = randnum(0, SIZE - 1);
            y = randnum(0, SIZE - 1);
        } while (arena[x][y] != ". ");
        arena[x][y] = isim;
    }

    virtual void move(vector<Oyuncular*>& robotlar) = 0;

    bool hayattaMi() const {
        return durum == CANLI;
    }

    void oldur() {
        durum = OLU;
        skor--;
        arena[x][y] = ". ";
    }

    void olduren() {
        skor += 2;
    }

    virtual ~Oyuncular() {}
};

void arenayaz() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << arena[i][j] << " ";
        }
        cout << endl;
    }
}

class Player : public Oyuncular {
public:
    Player(int id) : Oyuncular("P", id) {}

    void move(vector<Oyuncular*>& robotlar) override {
        if (!hayattaMi()) return;

        cout << isim << " icin yon gir (Num Pad gibi, 5 = sabit): ";
        char komut;
        cin >> komut;

        int dx = 0, dy = 0;
        switch (komut) {
            case '7': dx = -1; dy = -1; break;
            case '8': dx = -1; dy = 0; break;
            case '9': dx = -1; dy = 1; break;
            case '4': dx = 0; dy = -1; break;
            case '5': dx = 0; dy = 0; break;
            case '6': dx = 0; dy = 1; break;
            case '1': dx = 1; dy = -1; break;
            case '2': dx = 1; dy = 0; break;
            case '3': dx = 1; dy = 1; break;
            default: cout << "Hatali giris!\n"; return;
        }

        int nx = (x + dx + SIZE) % SIZE;
        int ny = (y + dy + SIZE) % SIZE;

        for (auto& r : robotlar) {
            if (r != this && r->hayattaMi() && r->x == nx && r->y == ny) {
                r->oldur();
                this->olduren();
                break;
            }
        }
        arena[x][y] = ". ";
        x = nx; y = ny;
        arena[x][y] = isim;
    }
};

class Shooter : public Oyuncular {
public:
    Shooter(int id) : Oyuncular("S", id) {}

    void move(vector<Oyuncular*>& robotlar) override {
        if (!hayattaMi()) return;

        int dirs[4][2] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };
        int i = randnum(0, 3);
        int nx = (x + dirs[i][0] + SIZE) % SIZE;
        int ny = (y + dirs[i][1] + SIZE) % SIZE;

        for (auto& r : robotlar) {
            if (r != this && r->hayattaMi() && r->x == nx && r->y == ny) {
                r->oldur();
                this->olduren();
                break;
            }
        }
        arena[x][y] = ". ";
        x = nx; y = ny;
        arena[x][y] = isim;
    }
};

class Freezer : public Oyuncular {
public:
    Freezer(int id) : Oyuncular("F", id) {}

    void move(vector<Oyuncular*>& robotlar) override {
        if (!hayattaMi()) return;

        vector<pair<int,int>> komsular;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = (x + dx + SIZE) % SIZE;
                int ny = (y + dy + SIZE) % SIZE;
                komsular.push_back({nx, ny});
            }
        }

        if (!komsular.empty()) {
            int i = randnum(0, komsular.size() - 1);
            int nx = komsular[i].first;
            int ny = komsular[i].second;

            for (auto& r : robotlar) {
                if (r != this && r->hayattaMi() && r->x == nx && r->y == ny) {
                    r->oldur();
                    this->olduren();
                    break;
                }
            }

            arena[x][y] = ". ";
            x = nx; y = ny;
            arena[x][y] = isim;
        }
    }
};

class Jumper : public Oyuncular {
public:
    Jumper(int id) : Oyuncular("J", id) {}

    void move(vector<Oyuncular*>& robotlar) override {
        if (!hayattaMi()) return;

        int nx, ny;
        do {
            nx = randnum(0, SIZE - 1);
            ny = randnum(0, SIZE - 1);
        } while (arena[nx][ny] != ". ");

        for (auto& r : robotlar) {
            if (r != this && r->hayattaMi() && r->x == nx && r->y == ny) {
                r->oldur();
                this->olduren();
                break;
            }
        }

        arena[x][y] = ". ";
        x = nx; y = ny;
        arena[x][y] = isim;
    }
};

bool oyunBittiMi(const vector<Oyuncular*>& robotlar) {
    int sayac = 0;
    for (auto c : robotlar) {
        if (c->hayattaMi()) sayac++;
    }
    return sayac <= 1;
}

void skorTablosu(const vector<Oyuncular*>& robotlar) {
    cout << "\nSkor Tablosu:\n";
    for (auto r : robotlar) {
        cout << r->isim << " | Durum: " << (r->hayattaMi() ? "CANLI" : "OLU")
             << " | Puan: " << r->skor << endl;
    }
}

int main() {
    srand(time(0));

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            arena[i][j] = ". ";

    int p, s, f, j;
    cout << "Oyuncu sayisi (min 1): "; cin >> p;
    cout << "Shooter sayisi: "; cin >> s;
    cout << "Freezer sayisi: "; cin >> f;
    cout << "Jumper sayisi: "; cin >> j;

    vector<Oyuncular*> robotlar;
    for (int i = 1; i <= p; i++) robotlar.push_back(new Player(i));
    for (int i = 1; i <= s; i++) robotlar.push_back(new Shooter(i));
    for (int i = 1; i <= f; i++) robotlar.push_back(new Freezer(i));
    for (int i = 1; i <= j; i++) robotlar.push_back(new Jumper(i));

    while (!oyunBittiMi(robotlar)) {
        arenayaz();
        cout << "\n--- TUR BASLADI ---\n";
        for (auto& r : robotlar) {
            r->move(robotlar);
        }
        skorTablosu(robotlar);
        cout << "\nEnter'a basin...";
        cin.ignore();
        cin.get();
        system("cls");
    }

    arenayaz();
    cout << "\nOyun Bitti!\nKazanan: ";
    for (auto& r : robotlar)
        if (r->hayattaMi())
            cout << r->isim << endl;

    for (auto& r : robotlar)
        delete r;

    return 0;
}
