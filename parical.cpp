//frank edewardo viveros barros//
#include <iostream>
#include <cstring>
#include <cstdlib> 
// Leonardo Mercado y Steven Montaño
using namespace std;

struct Pasajero {
    char nombre[20];
    Pasajero* siguiente;
};

struct Viaje {
    int precioDelViaje;
    char destino[30];
    char matricula[10];
    char nombreembarcacion[30];
    char dia[3];
    char mes[3];
    char anio[5];
    char identificador[11];
    int capacidad;
    int numPasajeros;
    Pasajero* primero;
    Pasajero* ultimo;

    Viaje* izq;
    Viaje* der;
    int altura;
};

Viaje* raiz = nullptr;
Viaje* aux = nullptr;

void generarIdentificador(Viaje* v) {
    strncpy(v->identificador, v->matricula, 2);  
    v->identificador[2] = '\0';
    strcat(v->identificador, v->anio);  
    strcat(v->identificador, v->mes);   
    strcat(v->identificador, v->dia);   
}

Viaje* crearViaje() {
    aux = ((struct Viaje*)malloc(sizeof(struct Viaje)));  
    if (aux == nullptr) {
        cout << "Error al asignar memoria." << endl;
        exit(1);
    }

    cout << "Ingrese el precio del viaje: ";
    cin >> aux->precioDelViaje;

    cout << "Ingrese el destino: ";
    cin >> aux->destino;

    cout << "Ingrese la matricula: ";
    cin >> aux->matricula;

    cout << "Ingrese el nombre del viaje: ";
    cin >> aux->nombreembarcacion;

    cout << "Ingrese el dia (DD): ";
    cin >> aux->dia;

    cout << "Ingrese el mes (MM): ";
    cin >> aux->mes;

    cout << "Ingrese el año (AAAA): ";
    cin >> aux->anio;

    cout << "Ingrese la capacidad de la embarcacion: ";
    cin >> aux->capacidad;

    aux->numPasajeros = 0;
    aux->primero = nullptr;
    aux->ultimo = nullptr;

    generarIdentificador(aux);

    aux->izq = nullptr;
    aux->der = nullptr;
    aux->altura = 1;

    return aux;
}

void registrarPasajero(struct Viaje* v) {
    if (v == nullptr) {
        cout << "Viaje no encontrado." << endl;
        return;
    }
    if (v->numPasajeros >= v->capacidad) {
        cout << "No se pueden registrar mas pasajeros." << endl;
        return;
    }
    Pasajero* p = ((struct Pasajero*)malloc(sizeof(struct Pasajero)));
    if (p == nullptr) {
        cout << "Error al asignar memoria." << endl;
        return;
    }
    cout << "Ingrese el nombre del pasajero(Sin espacios): ";
    cin >> p->nombre;
    p->siguiente = nullptr;

    if (v->ultimo == nullptr) {
        v->primero = v->ultimo = p;
    } else {
        v->ultimo->siguiente = p;
        v->ultimo = p;
    }
    v->numPasajeros++;
    cout << "Pasajero registrado exitosamente." << endl;
}

void listarPasajeros(struct Viaje* v) {
    if (v == nullptr) {
        cout << "Viaje no encontrado." << endl;
        return;
    }
    if (v->primero == nullptr) {
        cout << "No hay pasajeros registrados en este viaje." << endl;
        return;
    }
    cout << "Lista de pasajeros para el viaje con identificador " << v->identificador << ":" << endl;
    Pasajero* temp = v->primero;
    while (temp != nullptr) {
        cout << temp->nombre << endl;
        temp = temp->siguiente;
    }
}

int obtenerAltura(struct Viaje* n) {
    return (n == nullptr) ? 0 : n->altura;
}

int mayor(int a, int b) {
    return (a > b) ? a : b;
}

Viaje* rotarDerecha(struct Viaje* y) {
    Viaje* x = y->izq;
    Viaje* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    return x;
}

Viaje* rotarIzquierda(struct Viaje* x) {
    Viaje* y = x->der;
    Viaje* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    return y;
}

int obtenerBalance(struct Viaje* n) {
    return (n == nullptr) ? 0 : obtenerAltura(n->izq) - obtenerAltura(n->der);
}

Viaje* insertar(struct Viaje* nodo, struct Viaje* aux) {
    if (nodo == nullptr) {
        return aux;
    }

    if (strcmp(aux->identificador, nodo->identificador) < 0) {
        nodo->izq = insertar(nodo->izq, aux);
    } else if (strcmp(aux->identificador, nodo->identificador) > 0) {
        nodo->der = insertar(nodo->der, aux);
    } else {
        return nodo;
    }

    nodo->altura = 1 + mayor(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && strcmp(aux->identificador, nodo->izq->identificador) < 0) {
        return rotarDerecha(nodo);
    }

    if (balance < -1 && strcmp(aux->identificador, nodo->der->identificador) > 0) {
        return rotarIzquierda(nodo);
    }

    if (balance > 1 && strcmp(aux->identificador, nodo->izq->identificador) > 0) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && strcmp(aux->identificador, nodo->der->identificador) < 0) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void inOrden(Viaje* nodo) {
    if (nodo != nullptr) {
        inOrden(nodo->izq);
        cout << nodo->identificador << " ";
        inOrden(nodo->der);
    }
}

Viaje* buscarViaje(Viaje* nodo, const char* identificador) {
    if (nodo == nullptr || strcmp(nodo->identificador, identificador) == 0) {
        return nodo;
    }

    if (strcmp(identificador, nodo->identificador) < 0) {
        return buscarViaje(nodo->izq, identificador);
    }

    return buscarViaje(nodo->der, identificador);
}

void mostrarDetallesViaje(struct Viaje* v) {
    if (v == nullptr) {
        cout << "Viaje no encontrado." << endl;
        return;
    }
    cout << "Detalles del viaje:" << endl;
    cout << "Precio del viaje: " << v->precioDelViaje << endl;
    cout << "Destino: " << v->destino << endl;
    cout << "Matricula: " << v->matricula << endl;
    cout << "Nombre de la embarcacion: " << v->nombreembarcacion << endl;
    cout << "Fecha: " << v->dia << "/" << v->mes << "/" << v->anio << endl;
    cout << "Identificador: " << v->identificador << endl;
}

Viaje* nodoConMenorValor(struct Viaje* nodo) {
    Viaje* actual = nodo;
    while (actual->izq != nullptr)
        actual = actual->izq;
    return actual;
}

Viaje* eliminarViaje(struct Viaje* raiz, const char* identificador) {
    if (raiz == nullptr)
        return raiz;

    if (strcmp(identificador, raiz->identificador) < 0)
        raiz->izq = eliminarViaje(raiz->izq, identificador);
    else if (strcmp(identificador, raiz->identificador) > 0)
        raiz->der = eliminarViaje(raiz->der, identificador);
    else {
        if ((raiz->izq == nullptr) || (raiz->der == nullptr)) {
            Viaje* temp = raiz->izq ? raiz->izq : raiz->der;
            if (temp == nullptr) {
                temp = raiz;
                raiz = nullptr;
            } else
                *raiz = *temp;
            free(temp);
        } else {
            Viaje* temp = nodoConMenorValor(raiz->der);
            strcpy(raiz->identificador, temp->identificador);
            raiz->der = eliminarViaje(raiz->der, temp->identificador);
        }
    }

    if (raiz == nullptr)
        return raiz;

    raiz->altura = 1 + mayor(obtenerAltura(raiz->izq), obtenerAltura(raiz->der));

    int balance = obtenerBalance(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) >= 0)
        return rotarDerecha(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotarIzquierda(raiz->izq);
        return rotarDerecha(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) <= 0)
        return rotarIzquierda(raiz);

    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotarDerecha(raiz->der);
        return rotarIzquierda(raiz);
    }

    return raiz;
}



int main() {
    int opc = 0;
    do "---welcome this is my menu---" << endl;
        cout << "1. Registrar Pasajero en Viaje" << endl;
        cout << "2. Listar Pasajeros" << endl;
        cout << "3. Registrar un viaje" << endl;
        cout << "4. Mostrar todos los viajes" << endl;
        cout << "5. Buscar Viaje" << endl;
        cout << "6. Eliminar Viaje " << endl;
        cout << "7. Salir" << endl;
        cin >> opc;
        switch (opc) {
            case 1: {
                char identificador[11];
                cout << "Ingrese el identificador del viaje: ";
                cin >> identificador;
                Viaje* v = buscarViaje(raiz, identificador);
                registrarPasajero(v);
                break;
            }
            case 2: {
                char identificador[11];
                cout << "Ingrese el identificador del viaje: ";
                cin >> identificador;
                Viaje* v = buscarViaje(raiz, identificador);
                listarPasajeros(v);
                break;
            }
            case 3: {
                crearViaje();
                raiz = insertar(raiz, aux);
                break;
            }
            case 4:
                cout << "InOrden: ";
                inOrden(raiz);
                cout << endl;
                break;
            case 5: {
                char identificador[11];
                cout << "Ingrese el identificador del viaje: ";
                cin >> identificador;
                Viaje* v = buscarViaje(raiz, identificador);
                mostrarDetallesViaje(v);
                break;
            }
            case 6: {
                char identificador[11];
                cout << "Ingrese el identificador del viaje: ";
                cin >> identificador;
                raiz = eliminarViaje(raiz, identificador);
                break;
            }
            
        }
    } while (opc != 7);

    return 0;
    
    }