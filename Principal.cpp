#include "InterfazGrafica.h"
#include "Principal.h"

WidgetGrafico *pVentanaPrincipal = NULL;

int main(int argv, char **args)
{
  QApplication app(argv, args);
  WidgetGrafico VentanaPrincipal;
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
*/
