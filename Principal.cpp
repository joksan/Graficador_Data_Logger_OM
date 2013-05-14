#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "InterfazGrafica.h"

int main(int argv, char **args)
{
  QApplication app(argv, args);
  WidgetGrafico VentanaPrincipal;

  FILE *pArchivo;
  pArchivo = fopen("13_05_03", "r");

  VentanaPrincipal.showMaximized();
  return app.exec();
}

/*
 * Consumo diario
 * Grafico de ciclo de trabajo (muestra)
 * Valor promedio de ciclo de trabajo
*/
