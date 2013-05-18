#include "InterfazGrafica.h"
#include "ManejoDatos.h"

ClaseVentanaPrincipal *pVentanaPrincipal = NULL;

//-------------------------------------------------------------------------------------------------

ClaseVentanaPrincipal::ClaseVentanaPrincipal():
ArregloVertical(this),
BarraMenu(),
AreaGrafico(),
BarraDesplazamiento(Qt::Horizontal)
{
  //Agrega las entradas a la barra de menu
  QMenu *pMenuArchivo = BarraMenu.addMenu("&Archivo");
  QMenu *pMenuDatos = BarraMenu.addMenu("&Datos");

  setWindowTitle("Graficador Data Logger");

  //Agrega los comandos en cada uno de los menus
  pMenuArchivo->addAction("&Abrir captura de datos...", this, SLOT(AccionAbrirArchivo()));
  pMenuArchivo->addAction("&Salir", this, SLOT(AccionSalir()));
  pMenuDatos->addAction("&Procesar Datos", this, SLOT(AccionProcesarDatos()));

  ArregloVertical.setMenuBar(&BarraMenu);
  ArregloVertical.addWidget(&AreaGrafico);
  ArregloVertical.addWidget(&BarraDesplazamiento);

  QObject::connect(&BarraDesplazamiento, SIGNAL(valueChanged(int)), this, SLOT(CambiarPosicionGrafico(int)));
}

void ClaseVentanaPrincipal::AccionAbrirArchivo()
{
  QString NombreArchivo;

  NombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo de log", NULL, "Archivos de log (*)");
  if (NombreArchivo.isNull()) return;

  LeerArchivo(NombreArchivo.toUtf8());
  AreaGrafico.ActualizarDatosGrafico();
}

void ClaseVentanaPrincipal::AccionSalir()
{
  qApp->quit();
}

void ClaseVentanaPrincipal::AccionProcesarDatos()
{
  AreaGrafico.ActualizarDatosGrafico();
}

void ClaseVentanaPrincipal::AccionarBarraDesplazamiento(QAbstractSlider::SliderAction Accion)
{
  BarraDesplazamiento.triggerAction(Accion);
}

void ClaseVentanaPrincipal::CambiarPosicionGrafico(int posicion) {
  AreaGrafico.CambiarPosicion(posicion);
}

void ClaseVentanaPrincipal::RedimensionarBarraDesplazamiento(int dimension, int ventana) {
  BarraDesplazamiento.setRange(0, dimension);
  BarraDesplazamiento.setPageStep(ventana);
}

void ClaseVentanaPrincipal::showEvent(QShowEvent *event) {
  LeerArchivo("log/prueba");
  //LeerArchivo("log/13_05_02");
  AreaGrafico.ActualizarDatosGrafico();

  event->accept();
}

//-------------------------------------------------------------------------------------------------

WidgetGrafico::WidgetGrafico():
TipoGrafica(TG_Tensiones),
escala(1.0),
DimensionTotal(0),
posicion(0),
CantidadPuntos(0)
{
}

void WidgetGrafico::CambiarPosicion(int valor) {
  posicion = valor;
  update();
}

void WidgetGrafico::CambiarEscala(float valor)
{
  escala = valor;
  ActualizarDatosGrafico();
}

void WidgetGrafico::ActualizarDatosGrafico() {
  //Primeramente recorta la escala para impedir que se presenten mas valores que los que realmente
  //existen en las lecturas
  if (escala < 1.0) escala = 1.0;

  //A continuacion calcula la magnitud total (en pixeles) del area de grafico. Notese que la escala
  //actua como un multiplicador que amplia proporcionalmente el area grafica de la ventana.
  DimensionTotal = escala*width();

  //Se sub muestrean las lecturas procurando que se generen suficientes datos como para llenar la
  //dimension total del grafico (este proceso produce a lo sumo tantos puntos como lecturas se
  //tengan, dado que no se desea sobre muestrear)
  GenerarDatosGrafica(DimensionTotal, TipoGrafica);

  //Se verifica si se lograron crear tantos puntos como se solicitaron
  if (DatosGrafica1.size() == DimensionTotal)
    //Si es asi, la cantidad de puntos a dibujar sera igual al ancho de la ventana (cada muestra
    //se dibujara en una columna de 1 pixel)
    CantidadPuntos = width();
  else {
    //De no obtenerse la cantidad de puntos solicitados, entonces se determina la cantidad de
    //puntos que terminaran por mostrarse
    CantidadPuntos = DatosGrafica1.size()/escala;

    //Si la cantidad de puntos que lograran mostrarse esta por debajo de 11 (10 intervalos),
    //entonces se limita a esta misma cantidad y se ajustan tanto la escala como la dimension total
    //acordemente
    if (CantidadPuntos < 11) {
      CantidadPuntos = 11;
      escala = DatosGrafica1.size()/11;
      DimensionTotal = escala*width();
    }
  }

//  QString Cadena;
//  Cadena.sprintf("%li, %i", DatosGrafica1.size(), CantidadPuntos);
//  QMessageBox::information(pVentanaPrincipal, NULL, Cadena);
  pVentanaPrincipal->RedimensionarBarraDesplazamiento(DatosGrafica1.size() - CantidadPuntos,
                                                      CantidadPuntos/2);

  //Se redibuja el area de la ventana
  update();
}

void WidgetGrafico::wheelEvent(QWheelEvent * event)
{
  static int DeltaAcumulado = 0;

  DeltaAcumulado += event->delta();

  switch (event->modifiers()) {
  case Qt::ControlModifier:
    while (DeltaAcumulado >=  120) { DeltaAcumulado -= 120; escala *= 1.25; }
    while (DeltaAcumulado <= -120) { DeltaAcumulado += 120; escala /= 1.25; }
    ActualizarDatosGrafico();

    event->accept();
    break;
  case Qt::NoModifier:
    while (DeltaAcumulado >=  120) {
      DeltaAcumulado -= 120;
      pVentanaPrincipal->AccionarBarraDesplazamiento(QAbstractSlider::SliderPageStepSub);
    }
    while (DeltaAcumulado <= -120) {
      DeltaAcumulado += 120;
      pVentanaPrincipal->AccionarBarraDesplazamiento(QAbstractSlider::SliderPageStepAdd);
    }

    event->accept();
    break;
  default:
    event->ignore();
    break;
  }
}

void WidgetGrafico::paintEvent(QPaintEvent *event)
{
  QPainter painter;
  unsigned int i;

  //Dibuja el area de trazado
  painter.begin(this);
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

  painter.begin(this);
  if (DatosGrafica1.size() < DimensionTotal) {
    QPainterPath Curva1, Curva2, Curva3;
    Curva1.moveTo(TrX(0), TrY(DatosGrafica1[posicion].promedio/500));
    Curva2.moveTo(TrX(0), TrY(DatosGrafica2[posicion].promedio/500));
    Curva3.moveTo(TrX(0), TrY(DatosGrafica3[posicion].promedio/500));
    for (i=1; i<CantidadPuntos; i++) {
      Curva1.lineTo(TrX(float(i)/(CantidadPuntos-1)),
                   TrY(DatosGrafica1[i+posicion].promedio/500));
      Curva2.lineTo(TrX(float(i)/(CantidadPuntos-1)),
                   TrY(DatosGrafica2[i+posicion].promedio/500));
      Curva3.lineTo(TrX(float(i)/(CantidadPuntos-1)),
                   TrY(DatosGrafica3[i+posicion].promedio/500));
    }
    painter.setPen(QPen(Qt::red, 1));
    painter.drawPath(Curva1);
    painter.setPen(QPen(Qt::green, 1));
    painter.drawPath(Curva2);
    painter.setPen(QPen(Qt::blue, 1));
    painter.drawPath(Curva3);

    for (i=0; i<CantidadPuntos; i++) {
      painter.setPen(QPen(Qt::red, 5));
      painter.drawPoint(TrX(float(i)/(CantidadPuntos-1)),
                        TrY(DatosGrafica1[i+posicion].promedio/500));
      painter.setPen(QPen(Qt::green, 5));
      painter.drawPoint(TrX(float(i)/(CantidadPuntos-1)),
                        TrY(DatosGrafica2[i+posicion].promedio/500));
      painter.setPen(QPen(Qt::blue, 5));
      painter.drawPoint(TrX(float(i)/(CantidadPuntos-1)),
                        TrY(DatosGrafica3[i+posicion].promedio/500));
    }
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
  event->accept();
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
      painter.setPen(Qt::magenta);
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
      painter.setPen(Qt::magenta);
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
