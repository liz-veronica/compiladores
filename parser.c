#include "anlex.h"
#include <stdio.h>

extern token t;
extern int numLinea;
const char* nombreToken(int compLex);

void match(int esperado);
void json();
void element();
void array();
void element_list();
void object();
void attributes_list();
void attribute();
void attribute_name();
void attribute_value();
void sincronizar(const int[], int);

void parser() {
    getToken();   // Obtener primer token
    json();
    if (t.compLex == EOF_TOKEN)
        //printf("\nSintaxis correcta\n");
        fprintf(salida, "\nTraduccion completa\n");
    else
        //printf("\nError: se encontraron tokens luego del fin del JSON\n");
        fprintf(salida, "\nError: se encontraron tokens luego del fin del JSON\n");
}

void match(int esperado) {
    if (t.compLex == esperado) {
        getToken();
    } else {
        //printf("Error sintáctico en línea %d: se esperaba '%s', se encontró '%s'\n",
        //       numLinea, nombreToken(esperado), nombreToken(t.compLex));
        fprintf(salida, "Error sintactico en linea %d: se esperaba '%s', se encontro '%s'\n",
                numLinea, nombreToken(esperado), nombreToken(t.compLex));       
        const int sync[] = {COMA, R_LLAVE, R_CORCHETE, EOF_TOKEN};
        sincronizar(sync, 4);
    }
}

void json() {
    element();
    if (t.compLex != EOF_TOKEN) {
        //printf("Error: se esperaba fin de archivo\n");
        fprintf(salida, "Error: se esperaba fin de archivo\n");
    }
}

void element() {
    if (t.compLex == L_LLAVE){
        fprintf(salida, "<objeto>");
        object();
        fprintf(salida, "</objeto>");
    } else if (t.compLex == L_CORCHETE){
        fprintf(salida, "<arreglo>");
        array();
        fprintf(salida, "</arreglo>");
    } else {
        //printf("Error en línea %d: se esperaba objeto o arreglo\n", numLinea);
        fprintf(salida, "Error en linea %d: se esperaba objeto o arreglo\n", numLinea);
        const int sync[] = {COMA, R_LLAVE, R_CORCHETE, EOF_TOKEN};
        sincronizar(sync, 4);
    }
}

void array() {
    match(L_CORCHETE);
    if (t.compLex == R_CORCHETE) {
        match(R_CORCHETE);
    } else {
        element_list();
        match(R_CORCHETE);
    }
}

void element_list() {
    element();
    while (t.compLex == COMA) {
        match(COMA);
        element();
    }
}

void object() {
    match(L_LLAVE);
    if (t.compLex == R_LLAVE) {
        match(R_LLAVE);
    } else {
        attributes_list();
        match(R_LLAVE);
    }
}

void attributes_list() {
    attribute();
    while (t.compLex == COMA) {
        match(COMA);
        attribute();
    }
}

void attribute() {
    if (t.compLex == LITERAL_CADENA) {
        fprintf(salida, "<atributo nombre=\"%s\">", t.pe->lexema);
    }
    attribute_name();
    match(DOS_PUNTOS);
    attribute_value();
    fprintf(salida, "</atributo>");
}

void attribute_name() {
    if (t.compLex == LITERAL_CADENA) {
        match(LITERAL_CADENA);
    } else {
        //printf("Error en línea %d: se esperaba una cadena como nombre de atributo\n", numLinea);
        fprintf(salida, "Error en linea %d: se esperaba una cadena como nombre de atributo\n", numLinea);
        const int sync[] = {DOS_PUNTOS, COMA, R_LLAVE};
        sincronizar(sync, 3);
    }
}

void attribute_value() {
    switch (t.compLex) {
        case L_LLAVE:
        case L_CORCHETE:
            element(); break;
        case LITERAL_CADENA:
        case LITERAL_NUM:
        case PR_TRUE:
        case PR_FALSE:
        case PR_NULL:
            fprintf(salida, "%s", t.pe->lexema);
            match(t.compLex); break;
        default:
            //printf("Error en línea %d: valor de atributo no válido\n", numLinea);
            fprintf(salida, "Error en linea %d: valor de atributo no valido\n", numLinea);
            const int sync[] = {COMA, R_LLAVE};
            sincronizar(sync, 2);
    }
}

void sincronizar(const int conjunto[], int n) {
    int i, encontrado = 0;
    while (t.compLex != EOF_TOKEN && !encontrado) {
        for (i = 0; i < n; i++) {
            if (t.compLex == conjunto[i]) {
                encontrado = 1;
                return;
            }
        }
        getToken();
    }
}
