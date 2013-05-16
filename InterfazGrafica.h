#include <QtGui>

#ifndef INTERFAZ_GRAFICA_H
#define INTERFAZ_GRAFICA_H

class WidgetGrafico: public QWidget
{
  Q_OBJECT

public:
  WidgetGrafico();

public slots:
  void AbrirArchivo();
  void ProcesarDatos();
  void salir();

protected:
  QMenuBar MenuPrincipal;

  //Funcion sobrecargada para pintar la ventana
  void paintEvent(QPaintEvent *event);

  //Funciones de dibujado de elementos graficos
  void DibujarEscalas(float Xmin, float Xmax, float Xpaso,
                      float Ymin, float Ymax, float Ypaso);

  //Funciones de transformacion de coordenadas
  float TrX(float coordenada);
  float TrY(float coordenada);
};

#endif//INTERFAZ_GRAFICA_H
