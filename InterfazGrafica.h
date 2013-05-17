#include <QtGui>

#ifndef INTERFAZ_GRAFICA_H
#define INTERFAZ_GRAFICA_H

class WidgetGrafico: public QWidget
{
  Q_OBJECT

protected:
  //Funcion sobrecargada para pintar la ventana
  void paintEvent(QPaintEvent *event);

  //Funciones de dibujado de elementos graficos
  void DibujarEscalas(float Xmin, float Xmax, float Xpaso,
                      float Ymin, float Ymax, float Ypaso);

  //Funciones de transformacion de coordenadas
  float TrX(float coordenada);
  float TrY(float coordenada);
};

class ClaseVentanaPrincipal: public QWidget
{
  Q_OBJECT

public:
  ClaseVentanaPrincipal();

public slots:
  void AbrirArchivo();
  void ProcesarDatos();
  void salir();

protected:
  QVBoxLayout ArregloVertical;
  QMenuBar BarraMenu;
  WidgetGrafico AreaGrafico;
  QScrollBar BarraDesplazamiento;
};

#endif//INTERFAZ_GRAFICA_H
