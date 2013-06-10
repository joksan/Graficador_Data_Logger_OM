#include "InterfazGrafica.h"
#include "Principal.h"

int main(int argv, char **args)
{
  QApplication app(argv, args);
  ClaseVentanaPrincipal VentanaPrincipal;
  pVentanaPrincipal = &VentanaPrincipal;

  VentanaPrincipal.setMinimumSize(600, 400);
  VentanaPrincipal.show/*Maximized*/();

  return app.exec();
}

/*
 * TODO:
 * - Consumo diario
 * - Grafico de ciclo de trabajo (muestra)
 * - Valor promedio de ciclo de trabajo
 * - Obtencion de promedios y almacenamiento en tablas csv (para reporte de eficiencia)
*/
