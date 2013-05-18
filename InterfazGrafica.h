#include <QtGui>

#include "ManejoDatos.h"

#ifndef INTERFAZ_GRAFICA_H
#define INTERFAZ_GRAFICA_H

class WidgetGrafico: public QWidget
{
  Q_OBJECT

public:
  WidgetGrafico();
  void CambiarPosicion(int valor);
  void CambiarEscala(float valor);
  void ActualizarDatosGrafico();

protected:
  //Funcion sobrecargada para manejar eventos de scroll de mouse
  void wheelEvent(QWheelEvent * event);

  //Funcion sobrecargada para pintar la ventana
  void paintEvent(QPaintEvent *event);

  //Funciones de dibujado de elementos graficos
  void DibujarEscalas(float Xmin, float Xmax, float Xpaso,
                      float Ymin, float Ymax, float Ypaso);

  //Funciones de transformacion de coordenadas
  float TrX(float coordenada);
  float TrY(float coordenada);

  TIPO_GRAFICA TipoGrafica;
  float escala;
  unsigned int DimensionTotal;
  unsigned int posicion;
  unsigned int CantidadPuntos;
};

class ClaseVentanaPrincipal: public QWidget
{
  Q_OBJECT

public:
  ClaseVentanaPrincipal();

public slots:
  void AccionAbrirArchivo();
  void AccionSalir();
  void AccionProcesarDatos();
  void CambiarPosicionGrafico(int posicion);

public:
  void AccionarBarraDesplazamiento(QAbstractSlider::SliderAction Accion);
  void RedimensionarBarraDesplazamiento(int dimension, int ventana);

protected:
  void showEvent(QShowEvent *event);

  QVBoxLayout ArregloVertical;
  QMenuBar BarraMenu;
  WidgetGrafico AreaGrafico;
  QScrollBar BarraDesplazamiento;
};

extern ClaseVentanaPrincipal *pVentanaPrincipal;

#endif//INTERFAZ_GRAFICA_H
