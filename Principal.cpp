#include "InterfazGrafica.h"
#include "ManejoDatos.h"
#include "Principal.h"

WidgetGrafico *pVentanaPrincipal = NULL;

int main(int argv, char **args)
{
  QApplication app(argv, args);
  WidgetGrafico VentanaPrincipal;
  pVentanaPrincipal = &VentanaPrincipal;
  QMenuBar MenuPrincipal(&VentanaPrincipal);
//  QMenu *pMenuArchivo = MenuPrincipal.addMenu("&Archivo");

//  MenuPrincipal.addAction("Salir", &VentanaPrincipal, SLOT(salir()));
//  pMenuArchivo->addAction("Salir");
  LeerArchivo("log/13_05_02");
  VentanaPrincipal.showMaximized();
  return app.exec();
}

/*
 * TODO:
 * - Consumo diario
 * - Grafico de ciclo de trabajo (muestra)
 * - Valor promedio de ciclo de trabajo
*/
