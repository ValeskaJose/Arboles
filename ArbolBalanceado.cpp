#include <iostream>
#include <cmath> 
#include <algorithm> 

template <typename T>
class Nodo {
public:
    T valor;
    int altura;
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(const T& val)
        : valor(val), altura(1), izquierdo(nullptr), derecho(nullptr) {}
};
template <typename T>
class AVL {
private:
    Nodo<T>* raiz;

    int obtenerAltura(Nodo<T>* nodo) {
        return nodo ? nodo->altura : 0;
    }

    int obtenerBalance(Nodo<T>* nodo) {
        return nodo ? obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho) : 0;
    }

    Nodo<T>* rotarDerecha(Nodo<T>* y) {
        Nodo<T>* x = y->izquierdo;
        Nodo<T>* T2 = x->derecho;
        x->derecho = y;
        y->izquierdo = T2;

        y->altura = std::max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        x->altura = std::max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;

        return x; 
    }
    Nodo<T>* rotarIzquierda(Nodo<T>* x) {
        Nodo<T>* y = x->derecho;
        Nodo<T>* T2 = y->izquierdo;

        y->izquierdo = x;
        x->derecho = T2;
        x->altura = std::max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        y->altura = std::max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;

        return y;
    }

    Nodo<T>* balancear(Nodo<T>* nodo) {
        int balance = obtenerBalance(nodo);

        if (balance > 1 && obtenerBalance(nodo->izquierdo) >= 0)
            return rotarDerecha(nodo);

        if (balance > 1 && obtenerBalance(nodo->izquierdo) < 0) {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }

        if (balance < -1 && obtenerBalance(nodo->derecho) <= 0)
            return rotarIzquierda(nodo);

        if (balance < -1 && obtenerBalance(nodo->derecho) > 0) {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }
    Nodo<T>* obtenerMinimo(Nodo<T>* nodo) {
        while (nodo && nodo->izquierdo) {
            nodo = nodo->izquierdo;
        }
        return nodo;
    }

    Nodo<T>* eliminarNodo(Nodo<T>* raiz, const T& valor) {
        Nodo<T>* actual = raiz;
        Nodo<T>* padre = nullptr;

        while (actual) {
            if (valor < actual->valor) {
                padre = actual;
                actual = actual->izquierdo;
            } else if (valor > actual->valor) {
                padre = actual;
                actual = actual->derecho;
            } else {
                
                if (!actual->izquierdo || !actual->derecho) {
                    Nodo<T>* temp = actual->izquierdo ? actual->izquierdo : actual->derecho;
                    if (!padre) {
                        raiz = temp;
                    } else if (padre->izquierdo == actual) {
                        padre->izquierdo = temp;
                    } else {
                        padre->derecho = temp;
                    }
                    delete actual;
                    actual = nullptr;
                } else {
                    
                    Nodo<T>* temp = obtenerMinimo(actual->derecho);
                    actual->valor = temp->valor;
                    actual->derecho = eliminarNodo(actual->derecho, temp->valor);
                }
                break;
            }
        }

        if (actual) {
           
            actual->altura = std::max(obtenerAltura(actual->izquierdo), obtenerAltura(actual->derecho)) + 1;
            return balancear(actual);
        }
        return raiz;
    }

public:
    AVL() : raiz(nullptr) {}

    void insertar(const T& valor) {
        if (!raiz) {
            raiz = new Nodo<T>(valor);
            return;
        }

        Nodo<T>* actual = raiz;
        Nodo<T>* padre = nullptr;
        while (actual) {
            padre = actual;
            if (valor < actual->valor) {
                actual = actual->izquierdo;
            } else if (valor > actual->valor) {
                actual = actual->derecho;
            } else {
                return;
            }
        }

        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (valor < padre->valor)
            padre->izquierdo = nuevo;
        else
            padre->derecho = nuevo;

        actual = raiz;
        while (actual) {
            actual->altura = std::max(obtenerAltura(actual->izquierdo), obtenerAltura(actual->derecho)) + 1;
            Nodo<T>* nuevoActual = balancear(actual);

            if (actual == raiz) {
                raiz = nuevoActual;
                break;
            }
            actual = padre;
        }
    }

    void eliminar(const T& valor) {
        raiz = eliminarNodo(raiz, valor);
    }

    void imprimirEnOrden(Nodo<T>* nodo) {
        Nodo<T>* actual = nodo;
        Nodo<T>* anterior = nullptr;
        while (actual) {
            if (!actual->izquierdo) {
                std::cout << actual->valor << " ";
                actual = actual->derecho;
            } else {
                anterior = actual->izquierdo;
                while (anterior->derecho && anterior->derecho != actual) {
                    anterior = anterior->derecho;
                }

                if (!anterior->derecho) {
                    anterior->derecho = actual;
                    actual = actual->izquierdo;
                } else {
                    anterior->derecho = nullptr;
                    std::cout << actual->valor << " ";
                    actual = actual->derecho;
                }
            }
        }
    }

    void imprimir() {
        imprimirEnOrden(raiz);
        std::cout << std::endl;
    }
    void verificarArbol() {
        Nodo<T>* actual = raiz;
        Nodo<T>* anterior = nullptr;
        while (actual) {
            if (!actual->izquierdo) {
                int balance = obtenerBalance(actual);
                if (std::abs(balance) > 1) {
                    std::cout << "El nodo con valor " << actual->valor << " esta desbalanceado. Balance: " << balance << std::endl;
                    return;
                }
                actual = actual->derecho;
            } else {
                anterior = actual->izquierdo;
                while (anterior->derecho && anterior->derecho != actual) {
                    anterior = anterior->derecho;
                }

                if (!anterior->derecho) {
                    anterior->derecho = actual;
                    actual = actual->izquierdo;
                } else {
                    anterior->derecho = nullptr;
                    int balance = obtenerBalance(actual);
                    if (std::abs(balance) > 1) {
                        std::cout << "El nodo con valor " << actual->valor << " esta desbalanceado. Balance: " << balance << std::endl;
                        return;
                    }
                    actual = actual->derecho;
                }
            }
        }

        std::cout << "El arbol esta correctamente balanceado." << std::endl;
    }
};

int main() {
    AVL<int> arbol;

    int opcion, valor;
    do {
        std::cout << "\nMenu: \n";
        std::cout << "1. Insertar valor\n";
        std::cout << "2. Eliminar valor\n";
        std::cout << "3. Mostrar arbol en orden\n";
        std::cout << "4. Verificar balance del arbol\n";
        std::cout << "5. Salir\n";
        std::cout << "Ingrese su opcion: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            std::cout << "Ingrese el valor a insertar: ";
            std::cin >> valor;
            arbol.insertar(valor);
            break;
        case 2:
            std::cout << "Ingrese el valor a eliminar: ";
            std::cin >> valor;
            arbol.eliminar(valor);
            break;
        case 3:
            std::cout << "arbol en orden: ";
            arbol.imprimir();
            break;
        case 4:
            arbol.verificarArbol();
            break;
        case 5:
            std::cout << "Saliendo...\n";
            break;
        default:
            std::cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);

    return 0;
}
