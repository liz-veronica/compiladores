/*
 *	Analizador L�xico	
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Pr�ctica de Programaci�n Nro. 1
 *	
 *	Descripcion:
 *	Implementa un analizador l�xico que reconoce n�meros, identificadores, 
 * 	palabras reservadas, operadores y signos de puntuaci�n para un lenguaje
 * 	con sintaxis tipo Pascal.
 *	
 */

/*********** Inclusi�n de cabecera **************/
#include "anlex.h"


/************* Variables globales **************/

token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *archivo;			// Fuente json
FILE *salida;
char lexema[TAMLEX];	// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea

/**************** Funciones **********************/


// Rutinas del analizador lexico

void error(const char* mensaje)
{
	fprintf(salida,"Lin %d: Error Lexico. %s\n",numLinea,mensaje);	
}

const char* nombreToken(int compLex) {
    switch (compLex) {
        case L_CORCHETE: return "L_CORCHETE";
        case R_CORCHETE: return "R_CORCHETE";
        case L_LLAVE: return "L_LLAVE";
        case R_LLAVE: return "R_LLAVE";
        case COMA: return "COMA";
        case DOS_PUNTOS: return "DOS_PUNTOS";
        case LITERAL_CADENA: return "LITERAL_CADENA";
        case LITERAL_NUM: return "LITERAL_NUM";
        case PR_TRUE: return "PR_TRUE";
        case PR_FALSE: return "PR_FALSE";
        case PR_NULL: return "PR_NULL";
        case EOF_TOKEN: return "EOF";
        default: return "ERROR";
    }
}

void getToken()
{
	int i=0;
	char c=0;
	entrada e;

	while((c=fgetc(archivo))!=EOF)
	{
		
		if (c==' ' || c=='\t')
			continue;	//eliminar espacios en blanco
		if(c=='\n')
		{
			//incrementar el numero de linea
			numLinea++;
			strcpy(lexema, "\n");
            t.compLex = -2; // marcador de fin de linea
            return;
		}



		if (c == '[') { strcpy(lexema, "["); t.compLex = L_CORCHETE; }
        else if (c == ']') { strcpy(lexema, "]"); t.compLex = R_CORCHETE; }
        else if (c == '{') { strcpy(lexema, "{"); t.compLex = L_LLAVE; }
        else if (c == '}') { strcpy(lexema, "}"); t.compLex = R_LLAVE; }
        else if (c == ',') { strcpy(lexema, ","); t.compLex = COMA; }
        else if (c == ':') { strcpy(lexema, ":"); t.compLex = DOS_PUNTOS; }

        else if (c == '"') {
            i = 0;
        while ((c = fgetc(archivo)) != '"' && c != EOF && c != '\n') {
            lexema[i++] = c;
        }
        if (c == '"') {
            lexema[i] = '\0';  // No guardamos las comillas
            t.compLex = LITERAL_CADENA;
        } else {
            lexema[i] = '\0';
            error("Cadena no cerrada");
            return;
        }
}
        else if (isdigit(c) || c == '-') {
            i = 0;
            do {
                lexema[i++] = c;
                c = fgetc(archivo);
            } while (isdigit(c) || c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-');
            lexema[i] = '\0';
            if (c != EOF) ungetc(c, archivo);
            t.compLex = LITERAL_NUM;
        }
        else if (isalpha(c)) {
            i = 0;
            do {
                lexema[i++] = c;
                c = fgetc(archivo);
            } while (isalpha(c));
            lexema[i] = '\0';
            if (c != EOF) ungetc(c, archivo);

            if (strcasecmp(lexema, "true") == 0) t.compLex = PR_TRUE;
            else if (strcasecmp(lexema, "false") == 0) t.compLex = PR_FALSE;
            else if (strcasecmp(lexema, "null") == 0) t.compLex = PR_NULL;
            else {
                error("Palabra no reconocida");
                return;
            }
        }
        else {
            char msg[40];
            sprintf(msg, "Caracter no esperado: %c", c);
            error(msg);
            continue;
        }

        e.compLex = t.compLex;
        strcpy(e.lexema, lexema);
        insertar(e);
        t.pe = buscar(lexema);
        return;
    }

	t.compLex = EOF_TOKEN;
    strcpy(lexema, "EOF");
    t.pe = buscar(lexema);
	
}

int main(int argc,char* args[])
{
	if (argc < 2) {
        printf("Debe pasar el archivo fuente JSON como argumento.\n");
        return 1;
    }

    archivo = fopen(args[1], "rt");
    salida = fopen("output.txt", "wt");

    if (!archivo) {
        printf("Archivo no encontrado.\n");
        return 1;
    }

	// inicializar analizador lexico

	initTabla();
	initTablaSimbolos();
	
    /*
    while (1) {
        getToken();
        if (t.compLex == EOF_TOKEN) {
            fprintf(salida, "EOF\n");
            break;
        }
        if (t.compLex == -2) {
            fprintf(salida, "\n");
        } else {
            fprintf(salida, "%s ", nombreToken(t.compLex));
        }
    }
    */

    parser();
	

    fclose(archivo);
    fclose(salida);

	return 0;
}
