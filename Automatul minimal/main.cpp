#include <iostream>
#include <fstream>
#include <stdlib.h>

#define M1 8
#define M2 3

using namespace std;

class automat
{
    char a[M1][M2];
    char n[][M2];
    int tabel[M1][M1];
    int f[M1];
public:
    automat ()
    {
        citire(a);
        afisare_matrice(a);
        new_tabel(a, tabel);
        completare_tabel(a, tabel);
        afisare_tabel(tabel);
        for (int i = 0; i < M1; i++)
            f[i] = 0;
        comb (a, tabel);
    }

    void citire (char a[][M2])
    {
        ifstream f ("automat_minimal.txt");
        if (f == NULL)
        {
            cout << "Eroare la deschiderea fisierului.\n";
            exit (0);
        }
        for (int i = 0; i < M1; i++)
            for (int j = 0; j < M2; j++)
                f >> a[i][j];
        f.close();
        if (a[0][0] == '`')
            exit(0);
    }

    void afisare_matrice (char a[][M2])
    {
        cout <<"Stare\t0\t1\t\n\n";
        for (int i = 0; i < M1; i++)
        {
            cout << "q" << i << "\t";
            for (int j = 0; j < M2; j++)
                if (j == 2)
                    if (a[i][j] == '1')
                        cout << "Stare finala\t";
                    else
                        cout << "---\t";
                else
                    cout << "q" << a[i][j] << "\t";
            cout << endl;
        }
    }

    void new_tabel (char a[][M2], int tabel[][M1])
    {
        for (int i = 0; i < M1; i++)
            for (int j = 0; j < i; j++)
                if ((a[i][2] != a[j][2]))
                    tabel[i][j] = 1;
                else
                    tabel[i][j] = 0;
    }

    int maxim ()
    {
        int m = f[0];
        for (int i = 1; i < M1; i++)
            if (m < f[i])
                m = f[i];
        return m;
    }

    void afisare_tabel (int a[][M1])
    {
        cout << "\nTabelul este :";
        for (int i = 0; i < M1; i++)
        {
            for (int j = 0; j < M1; j++)
                if (i > j)
                    cout << a[i][j] << " ";
            cout << endl;
        }
    }

    void completare_tabel (char a[][M2], int t[][M1])
    {
        int in_00, in_01, in_10, in_11, ok = 1;
        while (ok)
        {
            ok = 0;
            for (int i = 0; i < M1; i++)
                for (int j = 0; j < i; j++)
                {
                    if (t[i][j] == 1)
                        continue;

                    in_00 = a[i][0] - '0';
                    in_01 = a[i][1] - '0';
                    in_10 = a[j][0] - '0';
                    in_11 = a[j][1] - '0';

                    if ((t[in_00][in_10] == 1) || (t[in_01][in_11] == 1))
                    {
                        t[i][j]++;
                        ok = 1;
                    }
                }
        }
    }

    void sterge ()
    {
        int mc = maxim(), con;
        while (mc)
        {
            con = 1;
            for (int i = 0; i < M1; i++)
            {
                if (f[i] == mc && con == 1)
                {
                    con = 0;
                    continue;
                }
                if (f[i] == mc && con == 0)
                {
                    f[i] = -1;
                }
            }
            mc--;
        }
    }

    void afisare_grupe()
    {
        int k, c = 0;

        cout << "Grupele sunt : \n";

        while (c <= maxim())
        {
            k = 0;
            for (int i = 0; i < M1; i++)
            {
                if (c == f[i])
                {
                    if (c == 0)
                    {
                        cout << "{" << i << "}\n";
                        continue;
                    }
                    if (k == 1)
                        cout << ", " << i;
                    else
                    {
                        cout << "{" << i;
                        k = 1;
                    }
                }
            }
            c++;
            if (k == 1)
                cout << "}\n";
        }
        cout << endl;
    }

    void modif()
    {
        int c = 0;
        for (int i = 0; i < M1; i++)
            if (c == f[i])
            {
                c++;
                i = 0;
            }

        if (c != maxim())
        {
            for (int i = 0; i < M1; i++)
                if (f[i] - 1 == c)
                    f[i]--;
        }

        c = 0;
        for (int i = 0; i < M1; i++)
            if (f[i] == 0)
                {
                    if (!c)
                        c++;
                    else
                        f[i] = maxim() + 1;
                }
    }

    void automat_min (char a[][M2])
    {
        int k = 0;
        int e0, e1;
        int v[M1];
        modif();

        for (int i = 0; i < M1; i++)
            v[i] = f[i];
        sterge();

        for (int i = 0; i < M1; i++)
        {
            if (f[i] == -1)
                continue;

            e0 = a[i][0] - '0';
            e1 = a[i][1] - '0';

            if (e0 == i)
                n[k][0] = k + '0';
            else
                n[k][0] = v[e0] + '0';
            if (e1 == i)
                n[k][1] = k + '0';
            else
                n[k][1] = v[e1] + '0';
            n[k][2] = a[i][2];
            k++;
        }
        cout << "\n***\n";
        afisare_final(n, k);
    }

    void afisare_final (char a[][M2], int k)
    {
        cout <<"Stare\t0\t1\t\n\n";
        for (int i = 0; i < k; i++)
        {
            cout << "q" << i << "\t";
            for (int j = 0; j < M2; j++)
                if (j == 2)
                    if (a[i][j] == '1')
                        cout << "Stare finala\t";
                    else
                        cout << "---\t";
                else
                    cout << "q" << a[i][j] << "\t";
            cout << endl;
        }
    }

    void comb (char a[][M2], int t[][M1])
    {
        cout << endl << "*****\n";
        for (int i = 0; i < M1; i++)
            for (int j = 0; j < i; j++)
            {
                if (t[i][j] == 1)
                    continue;
                cout << "q" << j << " si q" << i <<"  devin aceeasi stare.\n";
                a[i][0] = -1;
                f[i]++;
                f[j]++;
            }
        cout << "*****\n";
        afisare_grupe();
        automat_min(a);
    }
};

int main()
{
    automat a;
    return 0;
}
