#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const string FILE_PATH_USERS = "C:\\Users\\luist\\CLionProjects\\USERDB\\grupos_users.txt";
const string FILE_PATH_LOGIN = "C:\\Users\\luist\\CLionProjects\\USERDB\\login.txt";

class Usuario {
public:
    int ID;
    string NombreUsuario;
    string NombreLegal;
    int edad;

    Usuario(int id, const string& nombreUsuario, const string& nombreLegal, int edad)
        : ID(id), NombreUsuario(nombreUsuario), NombreLegal(nombreLegal), edad(edad) {}

    friend ostream& operator<<(ostream& os, const Usuario& p) {
        return os << "(" << p.ID << ", " << p.edad << ": " << p.NombreUsuario << ", " << p.NombreLegal << ")";
    }
};

void displayUser(const Usuario& user) {
    cout << "ID: " << user.ID << endl;
    cout << "Nombre: " << user.NombreUsuario << endl;
    cout << "Nombre legal: " << user.NombreLegal << endl;
    cout << "Edad: " << user.edad << endl;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

bool login() {
    string UsuarioLogin, Clave;
    bool loginSuccessful = false;

    cout << "Ingrese su usuario: ";
    cin >> UsuarioLogin;
    cout << "Ingrese su clave: ";
    cin >> Clave;

    UsuarioLogin = trim(UsuarioLogin);
    Clave = trim(Clave);

    ifstream inputFile(FILE_PATH_LOGIN);

    if (!inputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de login." << endl;
        return false;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string fileUsername, filePassword;

        getline(ss, fileUsername, ',');
        getline(ss, filePassword, ',');

        fileUsername = trim(fileUsername);
        filePassword = trim(filePassword);

        transform(UsuarioLogin.begin(), UsuarioLogin.end(), UsuarioLogin.begin(), ::tolower);
        transform(fileUsername.begin(), fileUsername.end(), fileUsername.begin(), ::tolower);

        if (UsuarioLogin == fileUsername && Clave == filePassword) {
            loginSuccessful = true;
            break;
        }
    }

    inputFile.close();

    if (!loginSuccessful) {
        cout << "Acceso denegado. Usuario o clave incorrectos." << endl;
    }

    return loginSuccessful;
}

vector<string> ExtraerDatos() {
    vector<string> lines;
    ifstream inputFile(FILE_PATH_USERS);

    if (!inputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de grupos." << endl;
        return lines;
    }

    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();
    return lines;
}

void GuardarDatos(const vector<string>& lines) {
    ofstream outputFile(FILE_PATH_USERS);

    if (!outputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para guardar." << endl;
        return;
    }

    for (const auto& line : lines) {
        outputFile << line << endl;
    }

    outputFile.close();
}

Usuario ExtraerDatosDeUsuario(const string& line) {
    stringstream ss(line);
    int idGrupo, idUsuario, edad;
    string nombreGrupo, nombreUsuario, nombreLegal;

    ss >> idGrupo >> nombreGrupo >> idUsuario >> nombreUsuario >> nombreLegal >> edad;
    return Usuario(idUsuario, nombreUsuario, nombreLegal, edad);
}

void case1() {
    int grupoID, BusqID;
    cout << "Elija el grupo por ID: ";
    cin >> grupoID;

    vector<string> lines = ExtraerDatos();
    bool grupoEncontrado = false;
    bool encontrado = false;

    for (const auto& line : lines) {
        stringstream ss(line);
        int idGrupo, idUsuario, edad;
        string nombreGrupo, nombreUsuario, nombreLegal;

        ss >> idGrupo >> nombreGrupo >> idUsuario >> nombreUsuario >> nombreLegal >> edad;

        if (idGrupo == grupoID) {
            grupoEncontrado = true;
            cout << "Busque un ID en el grupo " << nombreGrupo << ": ";
            cin >> BusqID;

            if (BusqID == idUsuario) {
                cout << "Usuario encontrado: ";
                cout << "(" << idUsuario << ", " << edad << ": " << nombreUsuario << ", " << nombreLegal << ")" << endl;
                encontrado = true;
                break;
            }
        }
    }

    if (!grupoEncontrado) {
        cout << "Grupo con ID " << grupoID << " no encontrado." << endl;
    } else if (!encontrado) {
        cout << "Usuario con ID " << BusqID << " no encontrado en el grupo." << endl;
    }
}

void case2() {
    int grupoID, BusqID2;
    cout << "Elija el grupo por ID: ";
    cin >> grupoID;

    vector<string> lines = ExtraerDatos();
    bool grupoEncontrado = false;
    bool encontrado = false;

    for (auto& line : lines) {
        stringstream ss(line);
        int idGrupo, idUsuario, edad;
        string nombreGrupo, nombreUsuario, nombreLegal;

        ss >> idGrupo >> nombreGrupo >> idUsuario >> nombreUsuario >> nombreLegal >> edad;

        if (idGrupo == grupoID) {
            grupoEncontrado = true;
            cout << "Elija el usuario por ID para editar en el grupo " << nombreGrupo << ": ";
            cin >> BusqID2;

            if (BusqID2 == idUsuario) {
                encontrado = true;
                displayUser(Usuario(idUsuario, nombreUsuario, nombreLegal, edad));

                cout << "Ingrese el nuevo nombre de usuario: ";
                cin.ignore();
                getline(cin, nombreUsuario);

                stringstream newLine;
                newLine << idGrupo << " " << nombreGrupo << " " << idUsuario << " " << nombreUsuario << " " << nombreLegal << " " << edad;
                line = newLine.str();

                cout << "Usuario actualizado" << endl;
                displayUser(Usuario(idUsuario, nombreUsuario, nombreLegal, edad));
                break;
            }
        }
    }

    if (!grupoEncontrado) {
        cout << "Grupo con ID " << grupoID << " no encontrado." << endl;
    } else if (!encontrado) {
        cout << "Usuario con ID " << BusqID2 << " no encontrado en el grupo." << endl;
    } else {
        GuardarDatos(lines);
    }
}

void case3() {
    int grupoID;
    cout << "Elija el grupo por ID para ver todos los usuarios: ";
    cin >> grupoID;

    vector<string> lines = ExtraerDatos();
    bool grupoEncontrado = false;

    for (const auto& line : lines) {
        stringstream ss(line);
        int idGrupo, idUsuario, edad;
        string nombreGrupo, nombreUsuario, nombreLegal;

        ss >> idGrupo >> nombreGrupo >> idUsuario >> nombreUsuario >> nombreLegal >> edad;

        if (idGrupo == grupoID) {
            grupoEncontrado = true;
            cout << "Usuarios en el grupo " << nombreGrupo << ":" << endl;
            cout << "(" << idUsuario << ", " << edad << ": " << nombreUsuario << ", " << nombreLegal << ")" << endl;
        }
    }

    if (!grupoEncontrado) {
        cout << "Grupo con ID " << grupoID << " no encontrado." << endl;
    }
}

bool SwitchReset() {
    char choice;
    cout << "\nDesea volver al menu principal? (Y/N): ";
    cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

int main() {
    bool loggedIn = false;

    while (!loggedIn) {
        loggedIn = login();
        if (!loggedIn) {
            cout << "Acceso denegado. Intente de nuevo." << endl;
        }
    }

    int opcion;
    bool opc = false;
    int IterationCounter = 0;

    do {
        cout << "\nElija una opcion:" << endl;
        cout << "1) Buscar por ID en grupo" << endl;
        cout << "2) Editar usuario por ID en grupo" << endl;
        cout << "3) Ver todos los usuarios en un grupo" << endl;
        cout << "4) Salir" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: case1(); break;
            case 2: case2(); break;
            case 3: case3(); break;
            case 4: cout << "Saliendo del sistema" << endl; return 0;
            default:
                cout << "Opcion no valida" << endl;
                cin.clear();
                cin.ignore();
                break;
        }

        opc = SwitchReset();
        IterationCounter++;

        if (IterationCounter > 5) {
            cout << "Exceso de intentos fallidos" << endl;
            break;
        }
    } while (opc);

    return 0;
}
