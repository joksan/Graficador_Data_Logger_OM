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

typedef struct
{
  time_t tiempo;
  float minimo;
  float maximo;
  float promedio;
} DATO_GRAFICA;

typedef enum {
  TG_Consumo, TG_Tensiones, TG_Corrientes, TG_Potencia_Fase, TG_Potencia_Total, TG_Factor_Potencia
} TIPO_GRAFICA;

extern vector<DATO_LECTURA> Lecturas;
extern vector<DATO_GRAFICA> DatosGrafica1;
extern vector<DATO_GRAFICA> DatosGrafica2;
extern vector<DATO_GRAFICA> DatosGrafica3;

bool LeerArchivo(const char *NombreArchivo);
void GenerarDatosGrafica(unsigned int CantidadPuntos, TIPO_GRAFICA TipoGrafica, float & EscalaMaxima);

#endif//MANEJO_DATOS_H
