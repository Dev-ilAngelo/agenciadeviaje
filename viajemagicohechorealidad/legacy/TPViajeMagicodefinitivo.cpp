#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ------------------------Prototipo de FUNCIONES ------------------------
// Función que carga todos los datos de los pasajeros
void cargaPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[],
                    char des[][4], int cont[], int precio[], float importe[], char destinosNombre[][20]);
// Función que valida que el DNI ingresado sea correcto
int validacionDNI();
// Función que valida el código de destino ingresado y si hay cupo
int validacionDestino(char des[], int pasajes[]);
// Ordena pasajeros por apellido y luego por nombre
void ordenarPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4]);
// Muestra pasajeros con información completa
void MostrarPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4], char destinosNombre[][20]);
// Muestra pasajeros agrupados por destino
void MostrarPasajerosDestinos(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4], char destinosNombre[][20]);
// Muestra lista de destinos con cantidad de pasajeros y recaudación
void mostrarListaDestinos(char des[][4], char destinosNombre[][20], int precio[], int pasajeros[], int np);
// Muestra estadísticas: ocupación, menores de 5 años, destino más solicitado
void estadisticas(int pasajeros[], int edades[], char destinosNombre[][20], char des[][4], int np);
// Buscar pasajero por DNI
void BuscarPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4], float importe[]);
// ------------------------ PROGRAMA PRINCIPAL ------------------------
int main()
{
    int op, pasajeros;

    // Datos pasajeros
    int edad[240], dni[240];
    float importe[240];
    char nombre[240][100], apellido[240][100], viaje[240][4];

    // Datos viajes
    int precioDestino[4] = {25000, 14000, 19000, 23000};
    int contPasajeros[4] = {0}; // Inicializamos en 0
    char codigoDestino[4][4] = {"BRA", "MDQ", "MZA", "BRC"};
    char destino[4][20] = {"Brasil", "Mar del Plata", "Mendoza", "Bariloche"};

    // Pedimos cantidad de pasajeros
    do {
        printf("Ingrese la cantidad de pasajeros que contrataron un viaje: ");
        scanf("%d", &pasajeros);
        if (pasajeros <= 0) {
            printf("Ingrese un numero positivo por favor\n");
        }
    } while (pasajeros < 1);

    // Carga, ordenamiento y menú
    cargaPasajeros(pasajeros, dni, nombre, apellido, edad, viaje, contPasajeros, precioDestino, importe, destino);
    ordenarPasajeros(pasajeros, dni, nombre, apellido, edad, viaje);

    do {
        printf("\nIngrese una opcion:\n");
        printf("1. Mostrar pasajeros ordenados por apellido y nombre\n");
        printf("2. Mostrar pasajeros por destino y ordenados por apellido y nombre\n");
        printf("3. Mostrar lista de destinos\n");
        printf("4. Buscar pasajero por DNI\n");
        printf("5. Mostrar estadisticas\n");
        printf("6. Salir\n");
        scanf("%d", &op);

        switch(op) {
            case 1: MostrarPasajeros(pasajeros, dni, nombre, apellido, edad, viaje, destino); break;
            case 2: MostrarPasajerosDestinos(pasajeros, dni, nombre, apellido, edad, viaje, destino); break;
            case 3: mostrarListaDestinos(viaje, destino, precioDestino, contPasajeros, pasajeros); break;
            case 4: BuscarPasajeros(pasajeros, dni, nombre, apellido, edad, viaje, importe); break;
            case 5: estadisticas(contPasajeros, edad, destino, viaje, pasajeros); break;
            case 6: printf("Muchas gracias por viajar con nosotros. ADIOS!!\n"); break;
            default: printf("Opcion no valida!\n"); break;
        }
    } while(op != 6);

    return 0;
}
// ------------------------ FUNCIONES IMPLEMENTADAS ------------------------

void cargaPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[],
                    char des[][4], int cont[], int precio[], float importe[], char destinosNombre[][20])
{
    int valdes;
    char desti[4], tarjeta;

    for(int i = 0; i < np; i++) {
        printf("\n\n--------- Datos del pasajero %d ---------\n\n", i+1);
        // Validación DNI único
        do {
            dni[i] = validacionDNI();
            for(int z=0; z<i; z++) {
                if(dni[i] == dni[z]) dni[i] = 1; // Marcamos como inválido
            }
            if(dni[i] == 1) printf("Este DNI ya fue cargado, intente otro.\n");
        } while(dni[i] == 1);

        // Ingreso de apellido y nombre
        printf("Ingrese apellido/s: ");
        getchar(); // Limpiar buffer
        fgets(ape[i], sizeof(ape[i]), stdin);
        ape[i][strcspn(ape[i], "\n")] = '\0';

        printf("Ingrese nombre/s: ");
        fgets(nom[i], sizeof(nom[i]), stdin);
        nom[i][strcspn(nom[i], "\n")] = '\0';

        // Edad
        do {
            printf("Ingrese la edad del pasajero: ");
            scanf("%d", &edad[i]);
        } while(edad[i] < 1);

        // Selección y validación de destino
        do {
            printf("Ingrese el codigo del destino (mayusculas): ");
            scanf("%s", desti);
            valdes = validacionDestino(desti, cont);

            if(valdes == 4) printf("Destino completo, elija otro.\n");
            else if(valdes == 5) printf("Codigo invalido, intente nuevamente.\n");
        } while(valdes < 0 || valdes > 3);

        // Guardar datos del destino y calcular importe
        cont[valdes]++;
        importe[i] = precio[valdes];
        strcpy(des[i], desti);

        printf("Gracias por reservar un viaje a %s\n", destinosNombre[valdes]);

        // Recargo por menores de 5 años
        if(edad[i] <= 5) importe[i] += 2000;

        // Recargo por tarjeta
        do {
            printf("Paga con tarjeta? (S/N): ");
            scanf(" %c", &tarjeta);
        } while(tarjeta != 's' && tarjeta != 'S' && tarjeta != 'n' && tarjeta != 'N');

        if(tarjeta == 's' || tarjeta == 'S') {
            importe[i] += (importe[i] * 0.05);
        }

        printf("El total de su viaje es: $%.2f\n", importe[i]);
    }
}

int validacionDNI() {
    char dniarreglo[20];
    int dni;
    int dnivalido = 0;

    do {
        dnivalido = 1;
        printf("Ingrese el DNI del pasajero: ");
        int c;
        while((c = getchar()) != '\n' && c != EOF); // Limpiar buffer

        fgets(dniarreglo, sizeof(dniarreglo), stdin);
        dniarreglo[strcspn(dniarreglo, "\n")] = '\0';

        int longitud = strlen(dniarreglo);
        for(int i=0; i<longitud; i++) {
            if(dniarreglo[i] < '0' || dniarreglo[i] > '9') dnivalido = 0;
        }

        if(dnivalido) {
            if(longitud < 7 || longitud > 8) dnivalido = 0;
            else {
                dni = atoi(dniarreglo);
                if(dni < 5000000 || dni > 60000001) dnivalido = 0;
            }
        }

        if(!dnivalido) printf("DNI invalido. Intente nuevamente.\n");

    } while(!dnivalido);

    return dni;
}

int validacionDestino(char des[], int pasajes[]) {
    int dest;
    if(strcmp(des, "BRA") == 0) dest = 0;
    else if(strcmp(des, "MDQ") == 0) dest = 1;
    else if(strcmp(des, "MZA") == 0) dest = 2;
    else if(strcmp(des, "BRC") == 0) dest = 3;
    else dest = 5;

    if(dest >= 0 && dest <= 3 && pasajes[dest] >= 60) dest = 4; // destino lleno
    return dest;
}

void ordenarPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4]) {
    int dniaux, edadaux;
    char nomaux[100], apeaux[100], desaux[4];

    for(int i=0; i<np; i++) {
        for(int z=i+1; z<np; z++) {
            if(strcmp(ape[i], ape[z]) > 0 || (strcmp(ape[i], ape[z]) == 0 && strcmp(nom[i], nom[z]) > 0)) {
                // Intercambio completo
                dniaux = dni[i]; dni[i] = dni[z]; dni[z] = dniaux;
                edadaux = edad[i]; edad[i] = edad[z]; edad[z] = edadaux;
                strcpy(nomaux, nom[i]); strcpy(nom[i], nom[z]); strcpy(nom[z], nomaux);
                strcpy(apeaux, ape[i]); strcpy(ape[i], ape[z]); strcpy(ape[z], apeaux);
                strcpy(desaux, des[i]); strcpy(des[i], des[z]); strcpy(des[z], desaux);
            }
        }
    }
}

void MostrarPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4], char destinosNombre[][20]){
    for(int i=0; i<np; i++) {
        printf("\nPasajero: %s, %s\n", ape[i], nom[i]);
        printf("DNI: %d\n", dni[i]);
        printf("Edad: %d\n", edad[i]);
        // Mostrar nombre del destino usando un arreglo
        if(strcmp(des[i], "BRA") == 0) printf("Destino: %s\n", destinosNombre[0]);
        else if(strcmp(des[i], "MDQ") == 0) printf("Destino: %s\n", destinosNombre[1]);
        else if(strcmp(des[i], "MZA") == 0) printf("Destino: %s\n", destinosNombre[2]);
        else if(strcmp(des[i], "BRC") == 0) printf("Destino: %s\n", destinosNombre[3]);
    }
}

void MostrarPasajerosDestinos(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4], char destinosNombre[][20]){
    for(int i=0; i<4; i++) {
        printf("\n-Codigo de destino %s\n", des[i]);
        for(int z=0; z<np; z++) {
            if(strcmp(des[z], des[i]) == 0) {
                printf("  Pasajero: %s, %s\n", ape[z], nom[z]);
                printf("  DNI: %d\n", dni[z]);
                printf("  Edad: %d\n", edad[z]);
            }
        }
    }
}

void mostrarListaDestinos(char des[][4], char destinosNombre[][20], int precio[], int pasajeros[], int np){
    int acumDestino, acumTotal=0;

    for(int i=0; i<4; i++) {
        acumDestino = 0;
        printf("\nPara el destino %s se reservaron %d pasajes, recaudando: ", destinosNombre[i], pasajeros[i]);
        for(int z=0; z<np; z++) {
            if(strcmp(des[i], des[z]) == 0) {
                acumDestino += precio[i];
                acumTotal += precio[i];
            }
        }
        printf("$%d\n", acumDestino);
    }
    printf("\nTotal recaudado: $%d\n", acumTotal);
}

void estadisticas(int pasajeros[], int edades[], char destinosNombre[][20], char des[][4], int np){
    float por, pormen;
    int mayor = 0, menores;
    for(int i=0; i<4; i++){
        por = (pasajeros[i]*100.0)/60;
        printf("%s tiene un %.2f %% de ocupacion\n", destinosNombre[i], por);
        if(pasajeros[i] > pasajeros[mayor]) mayor = i;
    }
    printf("Destino mas solicitado: %s\n", destinosNombre[mayor]);

    for(int i=0; i<4; i++){
        menores=0;
        for(int z=0; z<np; z++){
            if(strcmp(des[z], des[i])==0 && edades[z]<=5) menores++;
        }
        pormen = (menores*100.0)/np;
        printf("Destino %s tiene %.2f %% de menores de 5 años\n", destinosNombre[i], pormen);
    }
}

void BuscarPasajeros(int np, int dni[], char nom[][100], char ape[][100], int edad[], char des[][4], float importe[]){
    int auxDni, cont=0, bandera=-1;

    auxDni = validacionDNI();
    do{
        if(dni[cont] == auxDni) bandera = cont;
        else cont++;
    } while(cont < np && bandera == -1);

    if(bandera != -1){
        printf("DNI encontrado:\n");
        printf("Pasajero: %s, %s\n", ape[bandera], nom[bandera]);
        printf("Edad: %d\n", edad[bandera]);
        printf("Codigo del destino: %s\n", des[bandera]);
        printf("Importe a pagar: $%.2f\n", importe[bandera]);
    } else {
        printf("No existe pasajero con ese DNI\n");
    }
}
