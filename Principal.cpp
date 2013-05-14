#include <QtGui>
#include <math.h>
#include <stdlib.h>
#include <time.h>

time_t diff;

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
//  float p
} LECTURA, *pLECTURA;

class WidgetGrafico: public QWidget
{
protected:
  void paintEvent(QPaintEvent *event);
};

QWidget *pVentanaPrincipal = NULL;

float TrX(float coordenada) {
  return floor((0.1 + coordenada) /1.2*pVentanaPrincipal->width());
}

float TrY(float coordenada) {
  return floor((1.1 - coordenada)/1.2*pVentanaPrincipal->height());
}

void DibujarEscalas(QPainter &painter,
                    float Xmin, float Xmax, float Xpaso,
                    float Ymin, float Ymax, float Ypaso)
{
  QString Cadena;
  QRect Rect;
  int i;
  int posicion;
  int numDivisiones;

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

  painter.drawText(0, 0, 100, 100, 0, Cadena.sprintf("%i", diff));
}

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

int main(int argv, char **args)
{
  FILE *pArchivo;
  tm Inicio;
  tm Fin;

  memset(&Inicio, 1, sizeof(Inicio));
  memset(&Fin, 1, sizeof(Fin));

  Inicio.tm_year = 2000;
  Inicio.tm_mday = 1;

  Fin.tm_year = 2013;
  Fin.tm_mon = 4;
  Fin.tm_mday = 13;

  diff = difftime(mktime(&Inicio), mktime(&Fin));
  diff /= (24*60*60*365);

  QApplication app(argv, args);
  WidgetGrafico VentanaPrincipal;
  pVentanaPrincipal = &VentanaPrincipal;

  pArchivo = fopen("13_05_03", "r");


  VentanaPrincipal.showMaximized();
  return app.exec();
}

/*
 * Consumo diario
 * Grafico de ciclo de trabajo (muestra)
 * Valor promedio de ciclo de trabajo
*/
