#include "InterfazGrafica.h"
#include "ManejoDatos.h"

//-------------------------------------------------------------------------------------------------

ClaseVentanaPrincipal::ClaseVentanaPrincipal():
ArregloVertical(this),
BarraDesplazamiento(Qt::Horizontal)
{
  //Agrega las entradas a la barra de menu
  QMenu *pMenuArchivo = BarraMenu.addMenu("&Archivo");
  QMenu *pMenuDatos = BarraMenu.addMenu("&Datos");

  setWindowTitle("Graficador Data Logger");

  //Agrega los comandos en cada uno de los menus
  pMenuArchivo->addAction("&Abrir captura de datos...", this, SLOT(AbrirArchivo()));
  pMenuArchivo->addAction("&Salir", this, SLOT(salir()));
  pMenuDatos->addAction("&Procesar Datos", this, SLOT(ProcesarDatos()));

  ArregloVertical.setMenuBar(&BarraMenu);
  ArregloVertical.addWidget(&AreaGrafico);
  ArregloVertical.addWidget(&BarraDesplazamiento);

  QObject::connect(&BarraDesplazamiento, SIGNAL(sliderMoved(int)), this, SLOT(update()));
//  LeerArchivo("log/13_05_02");
//  GenerarDatosGrafica(100, TG_Tensiones);
}

void ClaseVentanaPrincipal::AbrirArchivo() {
  QString NombreArchivo;

  NombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo de log", NULL, "Archivos de log (*)");
  if (NombreArchivo.isNull()) return;

  LeerArchivo(NombreArchivo.toUtf8());
}

void ClaseVentanaPrincipal::ProcesarDatos()
{
  GenerarDatosGrafica(10, TG_Tensiones);
  update();
}

void ClaseVentanaPrincipal::salir()
{
  qApp->quit();
}

//-------------------------------------------------------------------------------------------------

void WidgetGrafico::paintEvent(QPaintEvent *event)
{
  QPainter painter;

  //Dibuja el area de trazado
  painter.begin(this);
  painter.drawLine(0, 0, 100, 100);
  painter.fillRect(TrX(0), TrY(0), TrX(1)-TrX(0), TrY(1)-TrY(0), QColor(255, 255, 128));
  painter.setPen(Qt::black);
  painter.drawRect(TrX(0), TrY(0), TrX(1)-TrX(0), TrY(1)-TrY(0));
  painter.end();

  //Dibuja las divisiones de la grafica
  DibujarEscalas(0, 100, 10, 0, 500, 50);

  if (DatosGrafica1.empty()) return;

  float maximo, minimo;

  maximo=DatosGrafica1[0].maximo;
  minimo=DatosGrafica1[0].minimo;
  for (unsigned int i=1; i<DatosGrafica1.size(); i++) {
    if (DatosGrafica1[i].maximo > maximo) maximo = DatosGrafica1[i].maximo;
    if (DatosGrafica1[i].minimo < minimo) minimo = DatosGrafica1[i].minimo;
  }

//  QString cadena;
  painter.begin(this);
  painter.setPen(QPen(Qt::blue, 5));
  for (unsigned int i=1; i<DatosGrafica1.size(); i++) {
    painter.drawPoint(TrX(float(i)/DatosGrafica1.size()), TrY(DatosGrafica1[i].promedio/500));
    //cadena.sprintf("%f", DatosGrafica1[i].promedio);
    //painter.drawText(0, i*16, cadena);
  }

/*
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::blue);
  painter.drawLine(TrX(0), TrY(0), TrX(1), TrY(1));
  painter.setPen(QPen(QColor(0, 128, 0), 10, Qt::SolidLine, Qt::RoundCap));
  painter.drawLine(TrX(0), TrY(0.5), TrX(1), TrY(1));
*/

/*
  QString cadena;
  time_t ahora;
  double diff;

  tm inicio;
  tm fin;

  time(&ahora);
  fin = *localtime(&ahora);

  memset(&inicio, 0, sizeof(inicio));

  inicio.tm_year = 2013-1900;
  inicio.tm_mon = 4;
  inicio.tm_mday = 1;

  diff = difftime(mktime(&fin), mktime(&inicio));

  painter.drawText(0, 0, width()/2, 15, 0, cadena.sprintf("%s", asctime(&fin)));
  painter.drawText(width()/2, 0, width()/2, 15, 0, cadena.sprintf("%li", mktime(&fin)));

  painter.drawText(0, 15, width()/2, 15, 0, cadena.sprintf("%s", asctime(&inicio)));
  painter.drawText(width()/2, 15, width()/2, 15, 0, cadena.sprintf("%li", mktime(&inicio)));

  painter.drawText(0, 30, width()/2, 15, 0, cadena.sprintf("%f", diff/3600/24));
*/
  painter.end();
}

void WidgetGrafico::DibujarEscalas(float Xmin, float Xmax, float Xpaso,
                                   float Ymin, float Ymax, float Ypaso)
{
  QPainter painter;
  QString Cadena;
  QRect Rect;
  int i;
  int posicion;
  int numDivisiones;

  //Inicia el dibujado en el widget
  painter.begin(this);

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

  //Finaliza el dibujado
  painter.end();
}

float WidgetGrafico::TrX(float coordenada)
{
  return floor((0.1 + coordenada) /1.2*this->width());
}

float WidgetGrafico::TrY(float coordenada)
{
  return floor((1.1 - coordenada)/1.2*this->height());
}
