#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct { //definir struct
	char titulo[40];
	char director[30];
	int anio_publicacion;
	int rating;
	char genero[20];
} PELICULA;

// --- Prototipos ---
int validarGenero(char x[]);
void ingresarPelicula(FILE *archivo);
void mostrarPorGenero(FILE *archivo, char genero[]);
void ordenarPeliculas(PELICULA arr[], int tam);


int main() {
    int opcion; //switch
    FILE *archivo;
    char genero[20]; //case 2

    do {
        printf("\n---- MENU ----\n");
        printf("1. Ingresar pelicula\n");
        printf("2. Mostrar por genero\n");
        printf("3. Ordenar peliculas (por anio)\n");
        printf("0. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                archivo = fopen("peliculas.txt", "a");
                if (!archivo) {
                	printf("ERROR AL ABRIR ARCHIVO.\n");
                	break;
				}
				
                ingresarPelicula(archivo);
                
                fclose(archivo);
                break;

            case 2:
                 printf("Ingrese el genero: ");
                 do {
                	fgets(genero, sizeof(genero), stdin);
                 	genero[strcspn(genero, "\n")] = '\0'; //quitar salto linea (ojo comillas dobles y simples)
                 	validarGenero(genero);
                 	if (!validarGenero(genero)) {
                 		printf("ERROR, ingrese un genero valido: ");
					 }
				 } while (!validarGenero(genero));
				 
                archivo = fopen("peliculas.txt", "r");
                
                if (!archivo) {
                	printf("ERROR AL ABRIR EL ARCHIVO.\n");
                	break;
				}
				
                mostrarPorGenero(archivo, genero);
                
                fclose(archivo);
                break;

            case 3: {
                 /*
                 1. Abrir archivo en modo lectura
                 2. Guardar en un array de PELICULA cada pelicula.
                 3. Llamar a ordenarPeliculas y pasarle el array.
                 4. Mostrar peliculas ordenadas.
                 */
                 
                archivo = fopen("peliculas.txt", "r");
                if (!archivo) {
                	printf("ERROR AL ABRIR EL ARCHIVO.\n");
                	break;
				}
				
				int contador = 0;
				PELICULA arreglo[100];
				
				while (fscanf(archivo, "%39[^,],%29[^,],%d,%d,%19[^\n]\n", arreglo[contador].titulo, arreglo[contador].director, &arreglo[contador].anio_publicacion, &arreglo[contador].rating, arreglo[contador].genero) == 5) { //5 es el n de elementos del struct, ojo con los &
					contador++; //este contador es para saber cuantas películas hay en la lista
				}
				fclose(archivo);
				
				ordenarPeliculas(arreglo, contador);
				
				int i;
				printf("=== PELICULAS ORDENADAS POR ANIO (mayor a menor) ===\n");
				for(i = 0; i < contador; i++) {
					printf("%s (%d) - %s, Rating: %d, Genero: %s\n", arreglo[i].titulo, arreglo[i].anio_publicacion, arreglo[i].director, arreglo[i].rating, arreglo[i].genero);
				}
				
                break;
            }

            case 0:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 0);

    system("pause");
    return 0;
}

/*===FUNCIONES===*/

int validarGenero(char x[]) {
	char *validos[] = {"Accion", "Drama", "Comedia", "Terror"}; //* porque tienen longitudes 
	int i;
	for (i = 0; i < 4; i++) { 
		if (strcasecmp(x, validos[i]) == 0) {
			return 1; //FUNCIONA COMO BREAK
		}
	}
	return 0; //si no breakeo, retorna 0
}

void ingresarPelicula(FILE *archivo) {
	PELICULA p;
	printf("Ingresar pelicula:\n");
	
	printf("\nIngrese el titulo de la pelicula: ");
	fgets(p.titulo, sizeof(p.titulo), stdin);
	p.titulo[strcspn(p.titulo, "\n")] = '\0';
	
	printf("\nIngrese el director de la pelicula: ");
	fgets(p.director, sizeof(p.director), stdin);
	p.director[strcspn(p.director, "\n")] = '\0';
	
	printf("\nIngrese el anio de publicacion (1985 - 2025): ");
	do {
		scanf("%d", &p.anio_publicacion);
		getchar();
		if (p.anio_publicacion < 1985 || p.anio_publicacion > 2025) {
			printf("ERROR, ingrese un anio de publicacion valido (1985 - 2025): ");
		}
	} while (p.anio_publicacion < 1985 || p.anio_publicacion > 2025);
	
	printf("\nIngrese el rating (1 - 10): ");
	do {
		scanf("%d", &p.rating);
		getchar();
		if (p.rating < 1 || p.rating > 10) {
			printf("ERROR, ingrese un rating valido (1 - 10): ");
		}
	} while (p.rating < 1 || p.rating > 10);
	
	printf("\nIngrese el genero (Accion, Drama, Comedia o Terror): ");
	do {
		fgets(p.genero, sizeof(p.genero), stdin);
		p.genero[strcspn(p.genero, "\n")] = '\0';
		validarGenero(p.genero);
		if (!validarGenero(p.genero)) {
			printf("ERROR, ingrese un genero valido: ");
		}
	} while (!validarGenero(p.genero));

	fprintf(archivo, "%s,%s,%d,%d,%s\n", p.titulo, p.director, p.anio_publicacion, p.rating, p.genero);
	printf("\nPelicula ingresada correctamente!\n");
}

void mostrarPorGenero(FILE *archivo, char genero[]) {
	PELICULA p;
	int bandera = 0;
	
	while (fscanf(archivo, "%39[^,],%29[^,],%d,%d,%19[^\n]\n", p.titulo, p.director, &p.anio_publicacion, &p.rating, p.genero) == 5) {
		if (strcasecmp(p.genero, genero) == 0) {
			bandera = 1;
			printf("%s (%d) - %s, rating: %d, GENERO: %s", p.titulo, p.anio_publicacion, p.director, p.rating, p.genero);
		}
	}
	if (bandera == 0) { // o !bandera
		printf("No se encontraron peliculas por el genero ingresado.\n");
	}
}

void ordenarPeliculas(PELICULA arr[], int tam) {
	int i, j;
	PELICULA aux;
	for (i = 0; i < tam - 1; i++) { //BUBBLE SORT
		for (j = 0; j < tam - i - 1; j++) {
			if (arr[j].anio_publicacion < arr[j + 1].anio_publicacion) {
			aux = arr[j];
			arr[j] = arr[j+1];
			arr[j+1] = aux;	
			}
		}
	}
}
