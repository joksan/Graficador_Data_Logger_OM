#include <stdio.h>
#include <QtGui>

#include "Principal.h"
#include "ManejoDatos.h"

#include <unistd.h>

vector<DATO_LECTURA> Lecturas;
vector<DATO_GRAFICA> DatosGrafica;

bool LeerArchivo(const char *NombreArchivo)
{
  FILE *pArchivo;
  char *PunteroRetorno;
  char LineaTexto[256];
  QString cadena;
  QStringList lista1;
  QStringList lista2;
  tm tmLectura;
  time_t TiempoLectura;
  DATO_LECTURA NuevoDato;

//  QProgressBar BarraProgreso(pVentanaPrincipal);
//  BarraProgreso.show();
//  sleep(5);

  pArchivo = fopen(NombreArchivo, "r");
  if (!pArchivo) {
    QMessageBox::critical(pVentanaPrincipal, "Error", "No se pudo abrir el archivo");
    return false;
  }

  //Lee la primera linea de texto del archivo
  PunteroRetorno = fgets(LineaTexto, sizeof(LineaTexto), pArchivo);
  //Verifica que la lectura haya sido exitosa
  if (!PunteroRetorno || feof(pArchivo)) return false;

  for (int i=0; i<10; i++) {
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

//    cadena.sprintf("%.2f", NuevoDato.pf1);
//    QMessageBox::information(pVentanaPrincipal, NULL, cadena);
    //QMessageBox::information(pVentanaPrincipal, NULL, QString(asctime(localtime(&TiempoLectura))));

    //Agrega el elemento recien creado a la lista
    Lecturas.push_back(NuevoDato);
  }

  fclose(pArchivo);
  return true;
}
