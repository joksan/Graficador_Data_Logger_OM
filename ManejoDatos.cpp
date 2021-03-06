#include <stdio.h>
#include <QtGui>

#include "Principal.h"
#include "ManejoDatos.h"

vector<DATO_LECTURA> Lecturas;
vector<DATO_GRAFICA> DatosGrafica1;
vector<DATO_GRAFICA> DatosGrafica2;
vector<DATO_GRAFICA> DatosGrafica3;
bool ModificandoLecturas = false;

bool LeerArchivo(const char *NombreArchivo)
{
  QProgressDialog DialogoProgreso(pVentanaPrincipal);
  FILE *pArchivo;
  long LongitudArch;
  long PosicionArch;
  char *PunteroRetorno;
  char LineaTexto[256];
  QString cadena;
  QStringList lista1;
  QStringList lista2;
  tm tmLectura;
  time_t TiempoLectura;
  DATO_LECTURA NuevoDato;
  int i;

  //Establece la bandera a verdadero para impedir que se generen datos de grafica por causa de
  //otras funciones (como por ejemplo al redibujar las ventanas)
  ModificandoLecturas = true;

  //Inicializa la barra de progreso
  DialogoProgreso.setWindowTitle("Abriendo archivo...");
  DialogoProgreso.setMinimum(0);
  DialogoProgreso.setMaximum(100);
  DialogoProgreso.show();

  //Abre el archivo para lectura
  pArchivo = fopen(NombreArchivo, "r");
  if (!pArchivo) {
    QMessageBox::critical(pVentanaPrincipal, "Error", "No se pudo abrir el archivo");
    return false;
  }

  //Determina el tamaño del archivo
  fseek(pArchivo, 0, SEEK_END);
  LongitudArch = ftell(pArchivo);
  fseek(pArchivo, 0, SEEK_SET);

  //Lee la primera linea de texto del archivo
  PunteroRetorno = fgets(LineaTexto, sizeof(LineaTexto), pArchivo);
  //Verifica que la lectura haya sido exitosa
  if (!PunteroRetorno || feof(pArchivo)) return false;

  //Limpia el vector con las lecturas antes de iniciar
  Lecturas.clear();

  for (i=0; ; i++) {
    //Actualiza la barra de progreso cada 1000 datos
    if (i % 1000 == 0) {
      //Antes de actualizar verifica si fue cancelada
      if (DialogoProgreso.wasCanceled()) {
        fclose(pArchivo);
        Lecturas.clear();
        ModificandoLecturas = false;
        return false;
      }
      PosicionArch = ftell(pArchivo);
      DialogoProgreso.setValue(PosicionArch*100/LongitudArch);
      qApp->processEvents();
    }

    //Lee una linea de texto del archivo
    PunteroRetorno = fgets(LineaTexto, sizeof(LineaTexto), pArchivo);
    //Verifica que la lectura haya sido exitosa
    if (!PunteroRetorno || feof(pArchivo)) break;

    //Traslada la cadena a un QString
    cadena = LineaTexto;

    //Descompone la cadena de texto en campos separados por espacios (nota: en caso de espacios
    //consecutivos se omiten los "campos vacios" generados)
    lista1 = cadena.split(" ", QString::SkipEmptyParts);

    if (lista1.size() != 18) {
      cadena.sprintf("La entrada #%i posee una cantidad incorrecta de campos", i+1);
      QMessageBox::critical(pVentanaPrincipal, "Error", cadena);
      return false;
    }

    //---------------------------------------------------------------------------------------------

    //Toma el primer elemento de la lista (la fecha) y lo descompone subsecuentemente en sus 3
    //numeros (mes/dia/año)
    lista2 = lista1[0].split("/");

    //Verifica que la fecha este correcta por medio de confirmar que se encuentren 3 numeros
    if (lista2.size() != 3) {
      cadena.sprintf("La fecha de la entrada #%i tiene el formato incorrecto", i+1);
      QMessageBox::critical(pVentanaPrincipal, "Error", cadena);
      return false;
    }

    //Convierte los numeros digitos de la fecha a enteros y los guarda en la estructura asociada
    tmLectura.tm_mday = lista2[0].toInt();
    tmLectura.tm_mon = lista2[1].toInt() - 1;
    tmLectura.tm_year = lista2[2].toInt() + 100;

    //Toma el segundo elemento de la lista (la hora) y la descompone subsecuentemente en sus 3
    //numeros (horas:minutos:segundos)
    lista2 = lista1[1].split(":");

    //Verifica que la hora este corrrecta al confirmar que posee 3 numeros
    if (lista2.size() != 3) {
      cadena.sprintf("La hora de la entrada #%i tiene el formato incorrecto", i+1);
      QMessageBox::critical(pVentanaPrincipal, "Error", cadena);
      return false;
    }

    //Convierte los numeros de la hora a datos enteros y los guarda en la estructura
    tmLectura.tm_hour = lista2[0].toInt();
    tmLectura.tm_min = lista2[1].toInt();
    tmLectura.tm_sec = lista2[2].toInt();

    //Convierte el tiempo capturado a time_t
    TiempoLectura = mktime(&tmLectura);

    //Agrega el tiempo a la nueva lectura
    NuevoDato.tiempo = TiempoLectura;

    //---------------------------------------------------------------------------------------------

    //Convierte los datos de potencia normal y reversa a punto flotante
    NuevoDato.kwhTotal = lista1[2].toFloat();
    NuevoDato.kwhTotalRev = lista1[3].toFloat();

    //---------------------------------------------------------------------------------------------

    //Convierte los datos de tension de las 3 fases a punto flotante
    NuevoDato.v1 = lista1[4].toFloat();
    NuevoDato.v2 = lista1[5].toFloat();
    NuevoDato.v3 = lista1[6].toFloat();

    //---------------------------------------------------------------------------------------------

    //Convierte los datos de corriente de las 3 fases a punto flotante
    NuevoDato.a1 = lista1[7].toFloat();
    NuevoDato.a2 = lista1[8].toFloat();
    NuevoDato.a3 = lista1[9].toFloat();

    //---------------------------------------------------------------------------------------------

    //Convierte los datos de potencia de las 3 fases a punto flotante
    NuevoDato.p1 = lista1[10].toFloat();
    NuevoDato.p2 = lista1[11].toFloat();
    NuevoDato.p3 = lista1[12].toFloat();

    //---------------------------------------------------------------------------------------------

    //Convierte el dato de potencia total consumida a punto flotante
    NuevoDato.pTot = lista1[13].toFloat();

    //---------------------------------------------------------------------------------------------

    //Lee el factor de potencia de la primera fase
    cadena = lista1[14];

    //Toma el caracter reactivo de la carga (primera letra) y la convierte a caracter, si existe
    if (cadena.toUtf8()[0] == 'L' || cadena.toUtf8()[0] == 'C') NuevoDato.pf1lc = cadena.toUtf8()[0];
    else NuevoDato.pf1lc = 0;

    //Remueve el caracter reactivo (letra L o C) para solo dejar el numero y almacenarlo
    cadena.remove(QRegExp("[LC]"));
    NuevoDato.pf1 = cadena.toFloat();

    //Reliza las mismas acciones con los factores de potencia de las otras fases
    cadena = lista1[15];
    if (cadena.toUtf8()[0] == 'L' || cadena.toUtf8()[0] == 'C') NuevoDato.pf2lc = cadena.toUtf8()[0];
    else NuevoDato.pf2lc = 0;
    cadena.remove(QRegExp("[LC]"));
    NuevoDato.pf2 = cadena.toFloat();

    cadena = lista1[16];
    if (cadena.toUtf8()[0] == 'L' || cadena.toUtf8()[0] == 'C') NuevoDato.pf3lc = cadena.toUtf8()[0];
    else NuevoDato.pf3lc = 0;
    cadena.remove(QRegExp("[LC]"));
    NuevoDato.pf3 = cadena.toFloat();

    //---------------------------------------------------------------------------------------------

    NuevoDato.maxDmd = lista1[17].toFloat();

    //---------------------------------------------------------------------------------------------

    //Agrega el elemento recien creado a la lista
    Lecturas.push_back(NuevoDato);
  }

  cadena.sprintf("Se leyeron %i entradas", i);
  QMessageBox::information(pVentanaPrincipal, "Apertura exitosa", cadena);

  fclose(pArchivo);
  ModificandoLecturas = false;
  return true;
}

void GenerarDatosGrafica(unsigned int CantidadPuntos, TIPO_GRAFICA TipoGrafica, float & EscalaMaxima) {
  unsigned int i, j;
  unsigned int InicioPaquete, FinPaquete;
  float ValorActual1;
  float ValorActual2;
  float ValorActual3;
  float ValorPromedio1;
  float ValorPromedio2;
  float ValorPromedio3;
  float ValorMaximo1;
  float ValorMaximo2;
  float ValorMaximo3;
  float ValorMinimo1;
  float ValorMinimo2;
  float ValorMinimo3;
  float MaximoDatos = 0.0;
  float magnitud;
  DATO_GRAFICA DatoNuevo;

  //Si se estan modificando las lecturas no se hara nada (impide el acceso concurrente)
  if (ModificandoLecturas) return;

  //Limita la cantidad de puntos a procesar a los que de por si existen en las lecturas
  //(No se desea sobre muestrear, solo sub muestrear)
  if (CantidadPuntos > Lecturas.size())
    CantidadPuntos = Lecturas.size();

  //Limpia los datos de grafica antes de comenzar
  DatosGrafica1.clear();
  DatosGrafica2.clear();
  DatosGrafica3.clear();

  //Establece el offset inicial al comienzo de las lecturas
  InicioPaquete = 0;

  switch(TipoGrafica) {
  case TG_Tensiones:
    //Inicializa la escala maxima con el primer valor de las lecturas
    MaximoDatos  = Lecturas[0].v1;
    for (i=1; i <= CantidadPuntos; i++) {
      //Determina el indice del final del paquete
      FinPaquete = i*Lecturas.size()/CantidadPuntos;

      //Inicializa los valores minimo, maximo y promedio con el primer dato del paquete
      ValorActual1 = Lecturas[InicioPaquete].v1;
      ValorActual2 = Lecturas[InicioPaquete].v2;
      ValorActual3 = Lecturas[InicioPaquete].v3;

      ValorMinimo1 = ValorActual1;
      ValorMinimo2 = ValorActual2;
      ValorMinimo3 = ValorActual3;

      ValorMaximo1 = ValorActual1;
      ValorMaximo2 = ValorActual2;
      ValorMaximo3 = ValorActual3;

      ValorPromedio1 = ValorActual1;
      ValorPromedio2 = ValorActual2;
      ValorPromedio3 = ValorActual3;

      //Estima los valores maximo, minimo y promedio de todos los demas datos que componen el
      //paquete (si es que existen datos adicionales)
      for (j=InicioPaquete+1; j<FinPaquete; j++) {
        ValorActual1 = Lecturas[j].v1;
        ValorActual2 = Lecturas[j].v2;
        ValorActual3 = Lecturas[j].v3;

        if (ValorActual1 < ValorMinimo1) ValorMinimo1 = ValorActual1;
        if (ValorActual2 < ValorMinimo2) ValorMinimo2 = ValorActual2;
        if (ValorActual3 < ValorMinimo3) ValorMinimo3 = ValorActual3;

        if (ValorActual1 > ValorMaximo1) ValorMaximo1 = ValorActual1;
        if (ValorActual2 > ValorMaximo2) ValorMaximo2 = ValorActual2;
        if (ValorActual3 > ValorMaximo3) ValorMaximo3 = ValorActual3;

        ValorPromedio1 += ValorActual1;
        ValorPromedio2 += ValorActual2;
        ValorPromedio3 += ValorActual3;
      }
      ValorPromedio1 /= (FinPaquete - InicioPaquete);
      ValorPromedio2 /= (FinPaquete - InicioPaquete);
      ValorPromedio3 /= (FinPaquete - InicioPaquete);

      //Determina el nuevo valor maximo detectado para todos los datos
      if (ValorMaximo1 > MaximoDatos) MaximoDatos = ValorMaximo1;
      if (ValorMaximo2 > MaximoDatos) MaximoDatos = ValorMaximo2;
      if (ValorMaximo3 > MaximoDatos) MaximoDatos = ValorMaximo3;

      //Almacena el dato del grafico en el vector correspondiente
      DatoNuevo.tiempo = Lecturas[InicioPaquete].tiempo;

      DatoNuevo.minimo = ValorMinimo1;
      DatoNuevo.maximo = ValorMaximo1;
      DatoNuevo.promedio = ValorPromedio1;
      DatosGrafica1.push_back(DatoNuevo);

      DatoNuevo.minimo = ValorMinimo2;
      DatoNuevo.maximo = ValorMaximo2;
      DatoNuevo.promedio = ValorPromedio2;
      DatosGrafica2.push_back(DatoNuevo);

      DatoNuevo.minimo = ValorMinimo3;
      DatoNuevo.maximo = ValorMaximo3;
      DatoNuevo.promedio = ValorPromedio3;
      DatosGrafica3.push_back(DatoNuevo);

      //El inicio del proximo paquete sera el limite final del anterior
      InicioPaquete = FinPaquete;
    }
    break;
  default:
    break;
  }

  //A continuacion se determinara la maxima escala que puede representar los numeros. Esta escala
  //es un numero redondo (un digito significativo precedido o seguido de cualquier cantidad
  //de ceros), que facilita el dibujado de escalas en el eje vertical
  EscalaMaxima = 0.1;
  magnitud = 0.1;
  i = 0;

  //El proceso persiste siempre que la escala maxima sea menor que el maximo de los datos
  while (EscalaMaxima < MaximoDatos) {
    //Con cada iteracion incrementa la escala maxima sumando la magnitud actual
    EscalaMaxima += magnitud;

    //Lleva una cuenta de las iteraciones, y cada 9 iteraciones incrementa la magnitud en 1 orden
    i++;
    if (i % 9 == 0) magnitud *= 10;

  }
}
