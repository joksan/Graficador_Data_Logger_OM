#ifndef MANEJO_DATOS_H
#define MANEJO_DATOS_H

#include <time.h>
#include <vector>
using namespace std;

typedef struct
{
  time_t tiempo;
  float kwhTotal;
  float kwhTotalRev;
  float v1;
  float v2;
  float v3;
  float a1;
  float a2;
  float a3;
  float p1;
  float p2;
  float p3;
  float pTot;
  char pf1lc;
  float pf1;
  char pf2lc;
  float pf2;
  char pf3lc;
  float pf3;
  float maxDmd;
} DATO_LECTURA;

extern vector<DATO_LECTURA> Lecturas;

typedef struct
{
  float valor;
  time_t tiempo;
} DATO_GRAFICA;

extern vector<DATO_GRAFICA> DatosGrafica;

bool LeerArchivo(const char *NombreArchivo);

#endif//MANEJO_DATOS_H
