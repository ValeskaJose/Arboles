#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

class Persona {
public:
    string nombre;
    Persona* padre;
    Persona* madre;
    Persona** hijos; 
    Persona** hermanos; 
    int numHijos;
    int numHermanos;
    int capacidadHijos;
    int capacidadHermanos;

    Persona(const string& nombre) : nombre(nombre), padre(nullptr), madre(nullptr), numHijos(0), numHermanos(0), capacidadHijos(2), capacidadHermanos(2) {
        hijos = new Persona*[capacidadHijos];  
        hermanos = new Persona*[capacidadHermanos]; 
    }

    void agregarHijo(Persona* hijo) {
        if (numHijos >= capacidadHijos) {
            capacidadHijos *= 2;
            Persona** nuevoArreglo = new Persona*[capacidadHijos];
            for (int i = 0; i < numHijos; ++i) {
                nuevoArreglo[i] = hijos[i];
            }
            delete[] hijos;
            hijos = nuevoArreglo;
        }
        hijos[numHijos++] = hijo;
    }

    void agregarHermano(Persona* hermano) {
        for (int i = 0; i < numHermanos; ++i) {
            if (hermanos[i] == hermano) return; 
        }
        if (numHermanos >= capacidadHermanos) {
            capacidadHermanos *= 2;
            Persona** nuevoArreglo = new Persona*[capacidadHermanos];
            for (int i = 0; i < numHermanos; ++i) {
                nuevoArreglo[i] = hermanos[i];
            }
            delete[] hermanos;
            hermanos = nuevoArreglo;
        }
        hermanos[numHermanos++] = hermano;
    }

    void mostrar() {
        cout << nombre << endl;

        if (numHermanos > 0) {
            cout << "  Hermanos: ";
            for (int i = 0; i < numHermanos; ++i) {
                cout << hermanos[i]->nombre << " ";
            }
            cout << endl;
        }

        if (numHijos > 0) {
            cout << "  Hijos: ";
            for (int i = 0; i < numHijos; ++i) {
                cout << hijos[i]->nombre << " ";
            }
            cout << endl;
        }
    }
};
void construirArbolGenealogico(const string& archivo, unordered_map<string, Persona*>& personas) {
    ifstream archivoCSV(archivo);
    string linea;
    
    while (getline(archivoCSV, linea)) {
        stringstream ss(linea);
        string nombre, padre, madre;

        getline(ss, nombre, ',');
        getline(ss, padre, ',');
        getline(ss, madre, ',');

        if (personas.find(nombre) == personas.end()) {
            personas[nombre] = new Persona(nombre);
        }
        Persona* persona = personas[nombre];

        Persona* padrePersona = nullptr;
        Persona* madrePersona = nullptr;

        if (!padre.empty()) {
            if (personas.find(padre) == personas.end()) {
                personas[padre] = new Persona(padre);
            }
            padrePersona = personas[padre];
            persona->padre = padrePersona;
        }

        if (!madre.empty()) {
            if (personas.find(madre) == personas.end()) {
                personas[madre] = new Persona(madre);
            }
            madrePersona = personas[madre];
            persona->madre = madrePersona;
        }

        if (padrePersona) padrePersona->agregarHijo(persona);
        if (madrePersona) madrePersona->agregarHijo(persona);
        if (padrePersona) {
            for (int i = 0; i < padrePersona->numHijos; ++i) {
                if (padrePersona->hijos[i] != persona) {
                    persona->agregarHermano(padrePersona->hijos[i]);
                    padrePersona->hijos[i]->agregarHermano(persona);
                }
            }
        }

        if (madrePersona) {
            for (int i = 0; i < madrePersona->numHijos; ++i) {
                if (madrePersona->hijos[i] != persona) {
                    persona->agregarHermano(madrePersona->hijos[i]);
                    madrePersona->hijos[i]->agregarHermano(persona);
                }
            }
        }
    }
}

void mostrarArbolGenealogico(Persona* raiz) {
    if (raiz != nullptr) {
        raiz->mostrar();
        for (int i = 0; i < raiz->numHijos; ++i) {
            mostrarArbolGenealogico(raiz->hijos[i]);
        }
    }
}

int main() {
    unordered_map<string, Persona*> personas;
    string archivo = "arbol_genealogico.csv";  

    construirArbolGenealogico(archivo, personas);
    if (!personas.empty()) {
        Persona* raiz = personas.begin()->second; 
        cout << "arbol genealogico:\n";
        mostrarArbolGenealogico(raiz);
    }

    for (auto& persona : personas) {
        delete[] persona.second->hijos;
        delete[] persona.second->hermanos;
        delete persona.second;
    }

    return 0;
}
