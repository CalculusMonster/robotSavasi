#include <iostream>
#include <vector>
#include <string>
#include <ctime>
//#include <cstdlib>

using namespace std;

const int SIZE = 20;
string arena[SIZE][SIZE];

#define randnum(min, max) ((rand() % ((max) + 1 - (min))) + (min))

class Oyuncular {
public:
    int x, y;
    string isim;

    Oyuncular(const string& tip, int id) {
        isim = tip + to_string(id);
        do {
            x = randnum(0, SIZE - 1);
            y = randnum(0, SIZE - 1);
        } while (arena[x][y] != ". ");
        arena[x][y] = isim;
    }

    virtual void move() = 0; // saf sanal
    virtual ~Oyuncular() {}
};

class Player : public Oyuncular {
public:
    Player(int id) : Oyuncular("P", id) {}

    void move() override {
        cout << isim << " icin yon gir (Num Pad ile Sabit kalmak icin 5): ";
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
            default: cout << "Hatalı giriş!\n"; return;
        }

        int nx = x + dx, ny = y + dy;
        if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && arena[nx][ny] == ". ") {
            arena[x][y] = ". ";
            x = nx; y = ny;
            arena[x][y] = isim;
        }
    }
};

class Shooter : public Oyuncular {
public:
    Shooter(int id) : Oyuncular("S", id) {}

    void move() override {
        int dirs[4][2] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };
        int i = randnum(0, 3);
        int nx = x + dirs[i][0], ny = y + dirs[i][1];

        if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && arena[nx][ny] == ". ") {
            arena[x][y] = ". ";
            x = nx; y = ny;
            arena[x][y] = isim;
        }
    }
};

class Freezer : public Oyuncular {
public:
    Freezer(int id) : Oyuncular("F", id) {}

    void move() override {
        vector<pair<int,int>> komsular;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && arena[nx][ny] == ". ")
                    komsular.push_back({nx, ny});
            }
        }

        if (!komsular.empty()) {
            int i = randnum(0, komsular.size() - 1);
            arena[x][y] = ". ";
            x = komsular[i].first;
            y = komsular[i].second;
            arena[x][y] = isim;
        }
    }
};

class Jumper : public Oyuncular {
public:
    Jumper(int id) : Oyuncular("J", id) {}

    void move() override {
        int nx, ny;
        do {
            nx = randnum(0, SIZE - 1);
            ny = randnum(0, SIZE - 1);
        } while (arena[nx][ny] != ". ");

        arena[x][y] = ". ";
        x = nx; y = ny;
        arena[x][y] = isim;
    }
};

void arenayaz() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << arena[i][j];
        }
        cout << endl;
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

    while (true) {
        arenayaz();
        cout << "\n--- TUR BASLADI ---\n";
        for (auto& r : robotlar) {
            r->move();
             system("cls");
        }
    }

    // delete işlemi eklenebilir
}
