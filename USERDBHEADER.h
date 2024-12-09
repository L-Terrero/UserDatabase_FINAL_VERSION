#ifndef USERDBMAIN_H
#define USERDBMAIN_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Usuario {
public:
    int ID;
    int edad;
    string NombreUsuario;
    string NombreLegal;

    Usuario(int id, const string& nombreUsuario, const string& nombreLegal, int edad);

    friend ostream& operator<<(ostream& os, const Usuario& p);
};

struct Grupo {
    int id;
    string nombre;
    vector<Usuario> usuarios;
};

bool login();
void displayUser(const Usuario& user);
void case1(const vector<Grupo>& grupos);
void case2(vector<Grupo>& grupos);
void case3(const vector<Grupo>& grupos);
void DisplayGrupos(const vector<Grupo>& grupos);

#endif // USERDBMAIN_H
