/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Jhenny Cervera Vargas
 * Codigo: 20182841
 * Created on 24 de agosto de 2021, 04:02 PM
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>    
#include <fstream>
#include <cstring>
using namespace std;

/*
 * 
 */

#define TAM 500

void imprimirLinea(ofstream &arch, char car, int cant) {
    arch.fill(car);
    arch << setw(cant) << car << endl;
    arch.fill(' ');
}

void cargarConductores(int *&licencia, char **&conductor) { //se pasa con & para modificar la variable

    ifstream ArchCond("Conductores.csv", ios::in);

    if (!ArchCond) {
        cout << "No se pudo abrir el archivo conductores csv" << endl;
        exit(1);
    }

    int numCond = 0;
    char buffCond[TAM][40];
    int buffLicen[TAM];

    while (1) {
        ArchCond >> buffLicen[numCond];
        if (ArchCond.eof()) break;
        ArchCond.get(); //lee la coma
        ArchCond.getline(buffCond[numCond], TAM);
        numCond++;
    }

    buffLicen[numCond] = -1;
    //strcpy(buffCond[numCond], "NULL");

    numCond++;

    licencia = new int[numCond];
    conductor = new char*[numCond];

    for (int i = 0; i < numCond; i++) {
        licencia[i] = buffLicen[i];
        conductor[i] = new char[strlen(buffCond[i]) + 1];
        strcpy(conductor[i], buffCond[i]);
    }

    for (int i = 0; i < numCond; i++) {
        cout << left << setw(20) << licencia[i] << conductor[i] << endl;
    }
    cout << numCond;
    ArchCond.close();
}

void cargarInfracciones(int *&infraccion, char *&gravedad, double *&multa) {

    ifstream ArchInfra("Infracciones.csv", ios::in);

    if (!ArchInfra) {
        cout << "No se pudo abrir el archivo infracciones.csv" << endl;
        exit(1);
    }
    int numInfrac = 0, auxInfrac[TAM];
    char auxD[TAM], auxGrav[10], aux, grav[TAM];
    double auxMult[TAM];


    while (1) {
        ArchInfra >> auxInfrac[numInfrac];
        if (ArchInfra.eof()) break;
        ArchInfra.get(); //leer la coma
        ArchInfra.getline(auxD, TAM, ','); //lee incluyendo la ,
        ArchInfra.getline(auxGrav, 10, ',');
        ArchInfra >> auxMult[numInfrac];
        if (strcmp(auxGrav, "Leve") == 0) {
            grav[numInfrac] = 'L';
        } else if (strcmp(auxGrav, "Grave") == 0) {
            grav[numInfrac] = 'G';
        } else {
            grav[numInfrac] = 'M';
        }
        numInfrac++;
    }
    auxInfrac[numInfrac] = -1;
    grav[numInfrac] = 'N';
    auxMult[numInfrac] = -1.0;
    numInfrac++;
    infraccion = new int[numInfrac];
    gravedad = new char[numInfrac];
    multa = new double[numInfrac];

    for (int i = 0; i < numInfrac; i++) {
        infraccion[i] = auxInfrac[i];
        gravedad[i] = grav[i];
        multa[i] = auxMult[i];
    }

    //impresion
    cout << endl;
    for (int i = 0; i < numInfrac; i++) {
        cout << setw(10) << infraccion[i] << setw(10) << gravedad[i] << multa[i] << endl;
    }
    ArchInfra.close();
}

int buscarLiOcodInfrac(int licen, int *licencia) {

    for (int i = 0; licencia[i] != -1; i++) {
        if (licencia[i] == licen) {
            return i;
        }
    }
    return -1;
}

int buscarFaltas(char auxplaca[8], char buffPlaca[TAM][8], int numFaltas) {
    int i;
    for (i = 0; i < numFaltas; i++) {
        if (strcmp(auxplaca, buffPlaca[i]) == 0) {
            return i;
        }
    }
    return i;

}

void procesarFaltasLeves(int *&licencia1, int*& cantidad, double*&monto, char** &placa,
        int *licencia, char**conductor, int *infraccion, char*gravedad,
        double*multa) {

    ifstream ArchInfra("RegistroDeFaltas.csv", ios::in);

    if (!ArchInfra) {
        cout << "No se pudo abrir el archivo registro de faltas.csv" << endl;
        exit(1);
    }
    int licen, codInfr, numFaltas = 0;
    char auxplaca[8], auxFecha[11];

    int buffLic[TAM], buffCant[TAM];
    double buffMonto[TAM];
    char buffPlaca[TAM][8];

    while (1) {
        ArchInfra>>licen;
        if (ArchInfra.eof())break;
        ArchInfra.get(); //coma
        ArchInfra.getline(auxplaca, 8, ',');
        ArchInfra.getline(auxFecha, 11, ',');
        ArchInfra>>codInfr;

        if (buscarLiOcodInfrac(licen, licencia) != -1) { //devuelve la pos o sino -1    //PREG para que el arr licencia
            int posInfrac = buscarLiOcodInfrac(codInfr, infraccion); //buscar infraccion
            if (posInfrac != -1) {
                if (gravedad[posInfrac] == 'L') {
                    int posFalt = buscarFaltas(auxplaca, buffPlaca, numFaltas);
                    if (posFalt != numFaltas) { // si ya está en el arreglo
                        //aumenar cant y mnonto
                        buffCant[posFalt]++;
                        buffMonto[posFalt] += multa[posInfrac];

                    } else { //es una falta nueva
                        buffLic[numFaltas] = licen;
                        strcpy(buffPlaca[numFaltas], auxplaca);
                        buffCant[numFaltas] = 1;
                        buffMonto[numFaltas] = multa[posInfrac];
                        numFaltas++;

                    }
                }
            }
        }
    }

    buffLic[numFaltas] = -1;
    //buffCant[numFaltas]=-1;  //no es necesario, verificar con el arreglo de licencia para el fin   
    numFaltas++;
    licencia1 = new int[numFaltas];
    cantidad = new int[numFaltas];
    monto = new double[numFaltas];
    placa = new char*[numFaltas];

    for (int i = 0; i < numFaltas; i++) {
        licencia1[i] = buffLic[i];
        cantidad[i] = buffCant[i];
        monto[i] = buffMonto[i];
        placa[i] = new char[8];
        strcpy(placa[i], buffPlaca[i]);
    }

    //     for(int i=0; i<numFaltas; i++){
    //         cout<<left<<i+1<<setw(15)<<')'<<setw(15)<<licencia1[i]<<setw(15)<<placa[i]<<setw(10)<<cantidad[i]<<monto[i]<<endl;
    //    }


}

void procesarFaltasGraves(int *&licencia1, int*& cantidad, double*&monto, char** &placa,
        int *licencia, char**conductor, int *infraccion, char*gravedad,
        double*multa) {
    ifstream ArchInfra("RegistroDeFaltas.csv", ios::in);

    if (!ArchInfra) {
        cout << "No se pudo abrir el archivo registro de faltas.csv" << endl;
        exit(1);
    }
    int licen, codInfr, numFaltas = 0;
    char auxplaca[8], auxFecha[11];

    int buffLic[TAM], buffCant[TAM];
    double buffMonto[TAM];
    char buffPlaca[TAM][8];

    while (1) {
        ArchInfra>>licen;
        if (ArchInfra.eof())break;
        ArchInfra.get(); //coma
        ArchInfra.getline(auxplaca, 8, ',');
        ArchInfra.getline(auxFecha, 11, ',');
        ArchInfra>>codInfr;

        if (buscarLiOcodInfrac(licen, licencia) != -1) { //devuelve la pos o sino -1    //PREG para que el arr licencia
            int posInfrac = buscarLiOcodInfrac(codInfr, infraccion); //buscar infraccion
            if (posInfrac != -1) {
                if (gravedad[posInfrac] == 'G') {
                    int posFalt = buscarFaltas(auxplaca, buffPlaca, numFaltas);
                    if (posFalt != numFaltas) { // si ya está en el arreglo
                        //aumenar cant y mnonto
                        buffCant[posFalt]++;
                        buffMonto[posFalt] += multa[posInfrac];

                    } else { //es una falta nueva
                        buffLic[numFaltas] = licen;
                        strcpy(buffPlaca[numFaltas], auxplaca);
                        buffCant[numFaltas] = 1;
                        buffMonto[numFaltas] = multa[posInfrac];
                        numFaltas++;

                    }
                }
            }
        }
    }

    buffLic[numFaltas] = -1;
    //buffCant[numFaltas]=-1;  //no es necesario, verificar con el arreglo de licencia para el fin   
    numFaltas++;
    licencia1 = new int[numFaltas];
    cantidad = new int[numFaltas];
    monto = new double[numFaltas];
    placa = new char*[numFaltas];

    for (int i = 0; i < numFaltas; i++) {
        licencia1[i] = buffLic[i];
        cantidad[i] = buffCant[i];
        monto[i] = buffMonto[i];
        placa[i] = new char[8];
        strcpy(placa[i], buffPlaca[i]);
    }

    //     for(int i=0; i<numFaltas; i++){
    //         cout<<left<<i+1<<setw(15)<<')'<<setw(15)<<licencia1[i]<<setw(15)<<placa[i]<<setw(10)<<cantidad[i]<<monto[i]<<endl;
    //    }

}

void procesarFaltasMGraves(int *&licencia1, int*& cantidad, double*&monto, char** &placa,
        int *licencia, char**conductor, int *infraccion, char*gravedad,
        double*multa) {

    ifstream ArchInfra("RegistroDeFaltas.csv", ios::in);

    if (!ArchInfra) {
        cout << "No se pudo abrir el archivo registro de faltas.csv" << endl;
        exit(1);
    }
    int licen, codInfr, numFaltas = 0;
    char auxplaca[8], auxFecha[11];

    int buffLic[TAM], buffCant[TAM];
    double buffMonto[TAM];
    char buffPlaca[TAM][8];

    while (1) {
        ArchInfra>>licen;
        if (ArchInfra.eof())break;
        ArchInfra.get(); //coma
        ArchInfra.getline(auxplaca, 8, ',');
        ArchInfra.getline(auxFecha, 11, ',');
        ArchInfra>>codInfr;

        if (buscarLiOcodInfrac(licen, licencia) != -1) { //devuelve la pos o sino -1    //PREG para que el arr licencia
            int posInfrac = buscarLiOcodInfrac(codInfr, infraccion); //buscar infraccion
            if (posInfrac != -1) {
                if (gravedad[posInfrac] == 'M') {
                    int posFalt = buscarFaltas(auxplaca, buffPlaca, numFaltas);
                    if (posFalt != numFaltas) { // si ya está en el arreglo
                        //aumenar cant y mnonto
                        buffCant[posFalt]++;
                        buffMonto[posFalt] += multa[posInfrac];

                    } else { //es una falta nueva
                        buffLic[numFaltas] = licen;
                        strcpy(buffPlaca[numFaltas], auxplaca);
                        buffCant[numFaltas] = 1;
                        buffMonto[numFaltas] = multa[posInfrac];
                        numFaltas++;

                    }
                }
            }
        }
    }

    buffLic[numFaltas] = -1;
    //buffCant[numFaltas]=-1;  //no es necesario, verificar con el arreglo de licencia para el fin   
    numFaltas++;
    licencia1 = new int[numFaltas];
    cantidad = new int[numFaltas];
    monto = new double[numFaltas];
    placa = new char*[numFaltas];

    for (int i = 0; i < numFaltas; i++) {
        licencia1[i] = buffLic[i];
        cantidad[i] = buffCant[i];
        monto[i] = buffMonto[i];
        placa[i] = new char[8];
        strcpy(placa[i], buffPlaca[i]);
    }

    //     for(int i=0; i<numFaltas; i++){
    //         cout<<left<<i+1<<setw(15)<<')'<<setw(15)<<licencia1[i]<<setw(15)<<placa[i]<<setw(10)<<cantidad[i]<<monto[i]<<endl;
    //    }


}

void imprimirRepLeves(ofstream& ArchRep, int*licencia1, int*cantidad, double *monto,
        char**placa) {

    ArchRep << "FALTAS LEVES" << endl;
    imprimirLinea(ArchRep, '=', TAM);
    ArchRep << left << setw(10) << "No." << setw(15) << "Licencia" << setw(10) << "Placa" << setw(15) << "Cantidad" << "Monto" << endl;
    imprimirLinea(ArchRep, '-', TAM);
    for (int i = 0; licencia1[i] != -1; i++) {

        if (i < 9) {
            ArchRep << left << i + 1 << setw(10) << ')' << setw(15) << licencia1[i]
                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
        } else if (i < 99) {
            ArchRep << left << i + 1 << setw(9) << ')' << setw(15) << licencia1[i]
                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
        } else {
            ArchRep << left << i + 1 << setw(8) << ')' << setw(15) << licencia1[i]
                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
        }
    }
    imprimirLinea(ArchRep, '*', TAM);

}

void imprimirRepGraves(ofstream& ArchRep, int*licencia1, int*cantidad, double *monto,
        char**placa) {

    ArchRep << "FALTAS GRAVES" << endl;
    imprimirLinea(ArchRep, '=', TAM);
    ArchRep << left << setw(10) << "No." << setw(15) << "Licencia" << setw(10) << "Placa" << setw(15) << "Cantidad" << "Monto" << endl;
    imprimirLinea(ArchRep, '-', TAM);
    for (int i = 0; licencia1[i] != -1; i++) {
        if (i < 9) {
            ArchRep << left << i + 1 << setw(10) << ')' << setw(15) << licencia1[i]
                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
        } else if (i < 99) {
            ArchRep << left << i + 1 << setw(9) << ')' << setw(15) << licencia1[i]
                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
        } else {
            ArchRep << left << i + 1 << setw(8) << ')' << setw(15) << licencia1[i]
                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
        }
    }
    imprimirLinea(ArchRep, '*', TAM);
}

void imprimirRepMGraves(ofstream& ArchRep, int*licencia1, int*cantidad, double *monto,
        char**placa) {
    
    ArchRep << "FALTAS MUY GRAVES" << endl;
    imprimirLinea(ArchRep, '=', TAM);
    ArchRep << left << setw(10) << "No." << setw(15) << "Licencia" << setw(10) << "Placa" << setw(15) << "Cantidad" << "Monto" << endl;
    imprimirLinea(ArchRep, '-', TAM);
    for (int i = 0; licencia1[i] != -1; i++) {
//        if (i < 9) {
//            ArchRep << left << i + 1 << setw(10) << ')' << setw(15) << licencia1[i]
//                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
//        } else if (i < 99) {
//            ArchRep << left << i + 1 << setw(9) << ')' << setw(15) << licencia1[i]
//                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
//        } else {
//            ArchRep << left << i + 1 << setw(8) << ')' << setw(15) << licencia1[i]
//                    << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
//        }
        int offset= i%10;
        ArchRep << left << i + 1 << setw(10-offset) << ')' << setw(15) << licencia1[i]
                   << setw(15) << placa[i] << setw(10) << cantidad[i] << monto[i] << endl;
    }
    imprimirLinea(ArchRep, '*', TAM);

}

void liberarMemoria(int * licencia1, int *cantidad, double *monto, char**placa) { //PREG si esta bien liberado
    int i;
    for (i = 0; licencia1[i] != -1; i++) {
        delete[] placa[i]; //elimino cada cadena del arreglo
    }
    //delete[] placa[i]; //elimino ultima cadena de flag

    delete[] licencia1; //elimino arreglo
    delete[] cantidad;
    delete[] monto;
    delete[] placa;

}

void reporteDeFaltas(int *licencia, char**conductor, int *infraccion, char*gravedad,
        double*multa) {

    ofstream ArchRep("reporte.txt", ios::out);

    int *licencia1, *cantidad; // utilizar la misma licencia como contenedor
    double *monto;
    char**placa;

    procesarFaltasLeves(licencia1, cantidad, monto, placa, licencia, conductor, infraccion, gravedad, multa);
    imprimirRepLeves(ArchRep, licencia1, cantidad, monto, placa);
    liberarMemoria(licencia1, cantidad, monto, placa);

    procesarFaltasGraves(licencia1, cantidad, monto, placa, licencia, conductor, infraccion, gravedad, multa);
    imprimirRepGraves(ArchRep, licencia1, cantidad, monto, placa);
    liberarMemoria(licencia1, cantidad, monto, placa);

    procesarFaltasMGraves(licencia1, cantidad, monto, placa, licencia, conductor, infraccion, gravedad, multa);
    imprimirRepMGraves(ArchRep, licencia1, cantidad, monto, placa);
    liberarMemoria(licencia1, cantidad, monto, placa);

    ArchRep.close();
}

int main(int argc, char** argv) {

    char **conductor, *gravedad; //arreglo de cadena de caracteres conductor  , arreglo de caracteres
    int *licencia, *infraccion;
    double *multa;


    cargarConductores(licencia, conductor);
    //    reporteConductores(licencia, conductor);
    cargarInfracciones(infraccion, gravedad, multa);
    //    reporteInfracciones(infraccion, gravedad, multa);
    reporteDeFaltas(licencia, conductor, infraccion, gravedad, multa);


    return 0;
}

