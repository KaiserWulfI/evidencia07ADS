/*
 * Autores:
 *  Alejandro Longoria Gonzalez
 *  Elihu Orlando Osuna Maltos
 *  Humberto Alejandro Vargas Sanchez
 *  Carlos Enrique Aguilar Sanchez
 * Programa: Cruce De Archivos
 * Objetivo: Actualizar el archivo de Personal con Movimientos (altas, bajas,
 * cambios).
 * Fecha: 21 de Marzo del 2026
 */
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Estructura para Manejar el Personal
struct Personal {
  string noTrabajador;
  string grupo;
  string empresa;
  string planta;
  string departamento;
  char claveOE;
  string nombre;
  double salarioBase;
  int fechaIngreso;
};

// Estructura para Manejar los Movimientos
struct Movimiento {
  char claveMovimiento; // A, B, C
  Personal datos;
};

// Variables para guardar moviemiento y personal
Personal regPersonal;
Movimiento regMovimientos;

// Variables de fin de Archivo
bool finPersonal = false;
bool finMovimientos = false;

// Variables para contar lineas de Reporte
int numLinea = 0;
int numHoja = 0;

// Declaracion del archivo de Personal
ifstream inArchivoPersonal("archivoPersonal.txt", ios::in);
// Declaracion del archivo Movimientos de Personal
ifstream inArchivoMovimientos("archivoMovimientos.txt", ios::in);
// Declaracion del archivo de Personal Actualizado
ofstream outArchivoNuevoPersonal("archivoNuevoPersonal.txt", ios::out);
// Declaracion del archivo de Reporte
ofstream outArchivoReporte("archivoReporte.txt", ios::out);

/* Funcion: escribirEncabezado
 * Objetivo: Desplegar el encabezado del reporte
 */
void escribirEncabezado() {
  outArchivoReporte
      << "  NOP120                  ACTUALIZACION DEL ARCHIVO DE PERSONAL     "
         "             HOJA "
      << numHoja << endl
      << endl;
  outArchivoReporte << "ACME - DIV. BANCOS             MOVIMIENTOS EFECTUADOS"
                    << endl;
  outArchivoReporte << "PERSONAL" << endl << endl;
  outArchivoReporte << "        TRABAJADOR        MOVIMIENTO EFECTUADO" << endl;
  outArchivoReporte << "        ----------        --------------------" << endl;
}

/* Funcion: escribirReporte
 * Objetivo: Agregar moviemientos al reporte y cada 20 lineas agregar una nueva
 * hoja con su respectivo encabezado.
 */
void escribirReporte(string mensaje) {
  if (numLinea < 1) {
    outArchivoReporte << "\n";
    numLinea = 20;
    numHoja++;
    escribirEncabezado();
  }
  outArchivoReporte << "        " << regMovimientos.datos.noTrabajador
                    << "                " << mensaje << endl;
  numLinea--;
}

/* Funcion: abrirArchivos
 * Objetivo: Abrir archivos.
 */
void abrirArchivos() {
  int archivosNoAbiertos = 0;

  if (!inArchivoPersonal) {
    cerr << "Archivo Personal no pudo ser abierto" << endl;
    archivosNoAbiertos++;
  }
  if (!inArchivoMovimientos) {
    cerr << "Archivo Movimientos no pudo ser abierto" << endl;
    archivosNoAbiertos++;
  }
  if (!outArchivoNuevoPersonal) {
    cerr << "Archivo Nuevo Personal no pudo ser abierto" << endl;
    archivosNoAbiertos++;
  }
  if (!outArchivoReporte) {
    cerr << "Archivo Reporte no pudo ser abierto" << endl;
    archivosNoAbiertos++;
  }
  if (archivosNoAbiertos > 0) {
    exit(EXIT_FAILURE);
  }
}

/* Funcion: cerrarArchivos
 * Objetivo: Cerrar los Archivos y agregar la terminacion al archivo que se
 * creo.
 */
void cerrarArchivos() {
  outArchivoNuevoPersonal << "XXX" << endl;

  inArchivoPersonal.close();
  inArchivoMovimientos.close();
  outArchivoNuevoPersonal.close();
  outArchivoReporte.close();
}

/* Funcion: leerPersonal
 * Objetivo: Leer un registro del archivo de Personal
 */
void leerPersonal() {
  string linea;
  getline(inArchivoPersonal, linea);
  if (linea == "XXX") {
    finPersonal = true;
    return;
  }

  stringstream personal(linea);
  personal >> regPersonal.noTrabajador >> regPersonal.grupo >>
      regPersonal.empresa >> regPersonal.planta >> regPersonal.departamento >>
      regPersonal.claveOE >> regPersonal.nombre >> regPersonal.salarioBase >>
      regPersonal.fechaIngreso;
}

/* Funcion leerMovimiento
 * Objetivo: Leer un registro del archivo de Movimientos
 */
void leerMovimiento() {
  string linea;
  getline(inArchivoMovimientos, linea);
  if (linea == "XXX") {
    finMovimientos = true;
    return;
  }
  stringstream movimiento(linea);
  movimiento >> regMovimientos.claveMovimiento >>
      regMovimientos.datos.noTrabajador >> regMovimientos.datos.grupo >>
      regMovimientos.datos.empresa >> regMovimientos.datos.planta >>
      regMovimientos.datos.departamento >> regMovimientos.datos.claveOE >>
      regMovimientos.datos.nombre >> regMovimientos.datos.salarioBase >>
      regMovimientos.datos.fechaIngreso;
}

/* Funcion: copiarAlNuevoArchivo
 * Objetivo: Copiar directamente un regsitro de personal al archivo nuevo.
 */
void copiarAlNuevoArchivo() {
  outArchivoNuevoPersonal << regPersonal.noTrabajador << " "
                          << regPersonal.grupo << " " << regPersonal.empresa
                          << " " << regPersonal.planta << " "
                          << regPersonal.departamento << " "
                          << regPersonal.claveOE << " " << regPersonal.nombre
                          << " " << regPersonal.salarioBase << " "
                          << regPersonal.fechaIngreso << endl;
}

/* Funcion: darDeAlta
 * Objetivo: Ejecutar una Alta del archivo de Movimientos
 */
void darDeAlta() {
  Personal trabajador = regMovimientos.datos;

  if (trabajador.grupo == "0") {
    trabajador.grupo = "000";
  }
  if (trabajador.empresa == "0") {
    trabajador.empresa = "000";
  }
  if (trabajador.planta == "0") {
    trabajador.planta = "000";
  }
  if (trabajador.departamento == "0") {
    trabajador.departamento = "000000";
  }
  if (trabajador.nombre == "0") {
    trabajador.nombre = "       ";
  }
  if (trabajador.salarioBase == 0) {
    trabajador.salarioBase = 0;
  }
  if (trabajador.fechaIngreso == 0) {
    auto now = std::chrono::system_clock::now();
    auto const today = chrono::year_month_day{chrono::floor<chrono::days>(now)};
    int date_yyyymmdd = static_cast<int>(today.year()) * 10000 +
                        static_cast<unsigned>(today.month()) * 100 +
                        static_cast<unsigned>(today.day());
    trabajador.fechaIngreso = date_yyyymmdd;
  }
  outArchivoNuevoPersonal << trabajador.noTrabajador << " " << trabajador.grupo
                          << " " << trabajador.empresa << " "
                          << trabajador.planta << " " << trabajador.departamento
                          << " " << trabajador.claveOE << " "
                          << trabajador.nombre << " " << trabajador.salarioBase
                          << " " << trabajador.fechaIngreso << endl;
  escribirReporte("A L T A");
}

/* Funcion: darDeBaja
 * Objetivo: Ejecutar una Baja del archivo de Movimientos
 */
void darDeBaja() { escribirReporte("B A J A"); }

/* Funcion: aplicarCambio
 * Objetivo: Ejecutar un Cambio a un registro de Personal al nuevo registro.
 */
void aplicarCambio() {
  Personal trabajador = regMovimientos.datos;

  if (trabajador.grupo == "0") {
    trabajador.grupo = regPersonal.grupo;
  }

  if (trabajador.empresa == "0") {
    trabajador.empresa = regPersonal.empresa;
  }

  if (trabajador.planta == "0") {
    trabajador.planta = regPersonal.planta;
  }

  if (trabajador.departamento == "0") {
    trabajador.departamento = regPersonal.departamento;
  }

  if (trabajador.nombre == "0") {
    trabajador.nombre = regPersonal.nombre;
  }

  if (trabajador.salarioBase == 0) {
    trabajador.salarioBase = regPersonal.salarioBase;
  }

  if (trabajador.fechaIngreso == 0) {
    trabajador.fechaIngreso = regPersonal.fechaIngreso;
  }
  outArchivoNuevoPersonal << trabajador.noTrabajador << " " << trabajador.grupo
                          << " " << trabajador.empresa << " "
                          << trabajador.planta << " " << trabajador.departamento
                          << " " << trabajador.claveOE << " "
                          << trabajador.nombre << " " << trabajador.salarioBase
                          << " " << trabajador.fechaIngreso << endl;
  escribirReporte("C A M B I O");
}

/* Funcion: ejecutarCruze
 * Objetivo: Funcion que decide que se tiene que llevar a cabo
 */
void ejecutarCruze() {
  if (regPersonal.noTrabajador == regMovimientos.datos.noTrabajador) {
    switch (regMovimientos.claveMovimiento) {
    case 'A':
      escribirReporte("ALTA NO EXITOSA");
      copiarAlNuevoArchivo();
      break;
    case 'B':
      darDeBaja();
      break;
    case 'C':
      aplicarCambio();
      break;
    default:
      break;
    }
    leerPersonal();
    leerMovimiento();
  } else if (regPersonal.noTrabajador > regMovimientos.datos.noTrabajador) {
    switch (regMovimientos.claveMovimiento) {
    case 'A':
      darDeAlta();
      break;
    case 'B':
      escribirReporte("BAJA NO EXITOSA");
      break;
    case 'C':
      escribirReporte("CAMBIO NO EXITOSO");
      break;
    default:
      break;
    }
    leerMovimiento();
  } else if (regPersonal.noTrabajador < regMovimientos.datos.noTrabajador) {
    copiarAlNuevoArchivo();
    leerPersonal();
  }
}

/* Funcion: main
 * Objetivo: Control General de la Aplicacion
 */
int main() {
  abrirArchivos();
  leerPersonal();
  leerMovimiento();
  cout << "Inicia Ejecucion del Programa" << endl;
  while (!(finPersonal && finMovimientos)) {
    ejecutarCruze();
  }
  cerrarArchivos();
  cout << "Termina Ejecucion del Programa" << endl;
  return 0;
}
