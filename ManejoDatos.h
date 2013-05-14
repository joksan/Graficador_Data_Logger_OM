#ifndef MANEJODATOS_H
#define MANEJODATOS_H

#include <vector>
using namespace std;

typedef struct {
  float fecha;
  float hora;
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
  float pf1;
  float pf2;
  float pf3;
  float MaxDmd;
} DATO_LECTURA, *pDATO_LECTURA;

extern vector<DATO_LECTURA> Lecturas;

#endif // MANEJODATOS_H
