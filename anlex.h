/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************* Definiciones ********************/

//Codigos
#define L_CORCHETE     256
#define R_CORCHETE     257
#define L_LLAVE        258
#define R_LLAVE        259
#define COMA           260
#define DOS_PUNTOS     261
#define LITERAL_CADENA 262
#define LITERAL_NUM    263
#define PR_TRUE        264
#define PR_FALSE       265
#define PR_NULL        266
#define EOF_TOKEN      267
// Fin Codigos
#define TAMBUFF 	5
#define TAMLEX 		50
#define TAMHASH 	101

/************* Estructuras ********************/

typedef struct entrada{
	//definir los campos de 1 entrada de la tabla de simbolos
	int compLex;
	char lexema[TAMLEX];	
	struct entrada *tipoDato; // null puede representar variable no declarada	
	// aqui irian mas atributos para funciones y procedimientos...
	
} entrada;

typedef struct {
	int compLex;
	entrada *pe;
} token;

extern token t;
extern int numLinea;
const char* nombreToken(int compLex);

/************* Prototipos ********************/
void insertar(entrada e);
entrada* buscar(const char *clave);
void initTabla();
void initTablaSimbolos();
void getToken();

void parser();           // Función principal del parser
void match(int);         // Empareja el token actual con uno esperado
void json();
void element();
void array();
void element_list();
void object();
void attributes_list();
void attribute();
void attribute_name();
void attribute_value();
void sincronizar(const int[], int); // Manejo de errores







