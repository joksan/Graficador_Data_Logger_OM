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

  //Escala horizontal (multiplicador de zoom) de la grafica. Un valor de 1.0 hace que se muestren
  //todos los puntos submuestreados del grafico en toda el area de la ventana, ocupando exactamente
  //toda su magnitud horizontal. Este valor no puede ser menor que 1.0.
  float escala;

  //Dimension horizontal del grafico. Representa la anchura total del grafico en pixeles. Si la
  //escala es de 1.0, este valor sera igual a la anchura de la ventana.
  unsigned int DimensionTotal;

  //Posicion de visualizacion del grafico. Representa la posicion en pixeles del punto dibujado mas
  //a la izquierda. Un valor de 0 causa que se muestren los datos sub muestreados desde el inicio.
  unsigned int posicion;

  //Cantidad de puntos mostrados. Indica la cantidad de puntos que caben en la ventana segun la
  //escala actual. Este valor es cuando menos 11 y cuando mucho igual al ancho en pixeles de la
  //ventana.
  unsigned int nPuntosMostrados;

  //Valor maximo de la escala vertical. La escala vertical sera siempre dividida en 10 porciones de
  //este valor, y sera siempre un numero redondo (0.1, 0.3, 4, 7, 20, 50, 200, 300, etc.)
  float MaximoVertical;
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
