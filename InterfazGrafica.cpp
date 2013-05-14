#include "InterfazGrafica.h"

void WidgetGrafico::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  //Dibuja el area de trazado
  painter.fillRect(TrX(0), TrY(0), TrX(1)-TrX(0), TrY(1)-TrY(0), QColor(255, 255, 128));
  painter.setPen(Qt::black);
  painter.drawRect(TrX(0), TrY(0), TrX(1)-TrX(0), TrY(1)-TrY(0));

  //Dibuja las divisiones de la grafica
  DibujarEscalas(painter, 50, 100, 5, 500, 1000, 50);

  painter.setRenderHint(QPainter::Antialiasing);

  painter.setPen(Qt::blue);
  painter.drawLine(TrX(0), TrY(0), TrX(1), TrY(1));
  painter.setPen(QPen(QColor(0, 128, 0), 10, Qt::SolidLine, Qt::RoundCap));
  painter.drawLine(TrX(0), TrY(0.5), TrX(1), TrY(1));
}

float WidgetGrafico::TrX(float coordenada) {
  return floor((0.1 + coordenada) /1.2*this->width());
}

float WidgetGrafico::TrY(float coordenada) {
  return floor((1.1 - coordenada)/1.2*this->height());
}

void WidgetGrafico::DibujarEscalas(QPainter &painter, float Xmin, float Xmax, float Xpaso,
                                                      float Ymin, float Ymax, float Ypaso)
{
  QString Cadena;
  QRect Rect;
  int i;
  int posicion;
  int numDivisiones;
/*
  time_t diff;

  memset(&Inicio, 1, sizeof(Inicio));
  memset(&Fin, 1, sizeof(Fin));

  tm Inicio;
  tm Fin;
  Inicio.tm_year = 2000;
  Inicio.tm_mday = 1;

  Fin.tm_year = 2013;
  Fin.tm_mon = 4;
  Fin.tm_mday = 13;

  diff = difftime(mktime(&Inicio), mktime(&Fin));
  diff /= (24*60*60*365);
*/
  //Dibuja las lineas de division vertical
  numDivisiones = (Xmax-Xmin)/Xpaso;
  for (i=0; i < numDivisiones+1; i++) {
    //Calcula la posicion horizontal de la linea
    posicion = TrX(float(i)/numDivisiones);

    //Dibuja la linea vertical (excepto en los bordes)
    if (i > 0 && i < numDivisiones) {
      painter.setPen(Qt::red);
      painter.drawLine(posicion, TrY(0), posicion, TrY(1));
    }

    //Imprime la etiqueta debajo
    Cadena.sprintf("%.1f", i*Xpaso + Xmin);
    Rect = painter.boundingRect(posicion, TrY(0), 0, 0, Qt::AlignHCenter | Qt::AlignTop, Cadena);
    painter.setPen(Qt::black);
    painter.drawText(Rect, Qt::AlignHCenter | Qt::AlignTop, Cadena);
  }

  //Dibuja las lineas de division horizontal
  numDivisiones = (Ymax-Ymin)/Ypaso;
  for (i=0; i < numDivisiones+1; i++) {
    //Calcula la posicion vertical de la linea
    posicion = TrY(float(i)/numDivisiones);

    //Dibuja la linea horizontal (excepto en los bordes)
    if (i > 0 && i < numDivisiones) {
      painter.setPen(Qt::red);
      painter.drawLine(TrX(0), posicion, TrX(1), posicion);
    }

    //Imprime la etiqueta a la izquierda
    Cadena.sprintf("%.1f", i*Ypaso + Ymin);
    Rect = painter.boundingRect(TrX(0), posicion, 0, 0, Qt::AlignRight | Qt::AlignBottom, Cadena);
    painter.setPen(Qt::black);
    painter.drawText(Rect, Qt::AlignRight | Qt::AlignBottom, Cadena);
  }

//  painter.drawText(0, 0, 100, 100, 0, Cadena.sprintf("%i", diff));
}
