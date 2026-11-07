/**
 * @file SceneLayout.h
 * @brief Medidas del escenario gráfico (ventana, parqueo, glorieta y tablero).
 *
 * @details
 * Guarda en un solo lugar las coordenadas de cada zona de la imagen de fondo
 * assets/escenario.png. Así, cuando más adelante se dibujen los vehículos y los
 * pasajeros, todas las clases de la Vista usan las mismas medidas y no hay
 * "números mágicos" repetidos por el código.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   SceneLayout (espacio de nombres con constantes, no se instancia)
 *
 * Responsabilidades:
 *  - Definir el tamaño lógico de la ventana (576 x 1024).
 *  - Definir la posición de los 4 puestos de parqueo.
 *  - Definir la glorieta (fila rotatoria) y la posición de embarque.
 *  - Definir el rectángulo donde se dibujará el tablero de tráfico.
 *
 * Colaboradores:
 *  - AllegroWindow:
 *      * Usa el tamaño lógico para escalar la ventana a la pantalla.
 *  - Scenery:
 *      * Dibuja la imagen de fondo que tiene exactamente estas medidas.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

// Agrupa las constantes bajo un nombre para no confundirlas con otras.
// Ej: se escribe SceneLayout::WIDTH en vez de solo WIDTH.
namespace SceneLayout {

    // ---------- Tamaño lógico de la ventana ----------

    // Ancho con el que se diseñó el escenario, en píxeles.
    // Ej: la imagen escenario.png mide exactamente 576 de ancho.
    const int WIDTH = 576;

    // Alto con el que se diseñó el escenario, en píxeles.
    // Ej: si la pantalla es pequeña, la ventana se reduce, pero el dibujo sigue pensado en 1024.
    const int HEIGHT = 1024;

    // ---------- Franja de parqueo (4 puestos) ----------

    // Cantidad de puestos de parqueo que pide el enunciado.
    // Ej: si los 4 están ocupados, ningún vehículo más puede salir del tablero.
    const int PARKING_SLOTS = 4;

    // Ancho de cada puesto de parqueo.
    // Ej: un carro dibujado de 60 px de ancho cabe con 10 px de margen a cada lado.
    const int SLOT_WIDTH = 80;

    // Alto de cada puesto de parqueo.
    // Ej: el puesto va desde y = 360 hasta y = 460.
    const int SLOT_HEIGHT = 100;

    // Coordenada y de la parte de arriba de los puestos.
    // Ej: todos los puestos están a la misma altura, solo cambia su x.
    const int SLOT_Y = 360;

    // Coordenada x de la esquina izquierda de cada puesto (de izquierda a derecha).
    // Ej: SLOT_X[0] = 83 es el puesto 1 y SLOT_X[3] = 413 es el puesto 4.
    const int SLOT_X[PARKING_SLOTS] = {83, 193, 303, 413};

    // ---------- Glorieta (fila rotatoria) ----------

    // Centro de la glorieta en x.
    // Ej: 288 es justo la mitad del ancho (576 / 2).
    const int RING_CENTER_X = 288;

    // Centro de la glorieta en y.
    // Ej: los pasajeros girarán alrededor del punto (288, 210).
    const int RING_CENTER_Y = 210;

    // Radio horizontal del carril donde caminan los pasajeros.
    // Ej: es la línea punteada ovalada que se ve en la glorieta.
    const int RING_RADIUS_X = 122;

    // Radio vertical del carril donde caminan los pasajeros.
    // Ej: es menor que el horizontal porque el carril es un óvalo, no un círculo.
    const int RING_RADIUS_Y = 65;

    // Posición de embarque: el paso peatonal que sale hacia el parqueo.
    // Ej: el pasajero que esté aquí es el que se revisa para subir a un vehículo.
    const int BOARDING_X = 291;

    // Altura del paso peatonal de embarque.
    // Ej: queda entre la glorieta (arriba) y la franja de parqueo (abajo).
    const int BOARDING_Y = 324;

    // ---------- Caminos de la fila estática (izquierdo; el derecho es su espejo) ----------

    // Línea central del camino izquierdo en x (tramo recto que baja desde arriba).
    // Ej: el camino derecho está en 576 - 78 = 498.
    const int LANE_X = 78;

    // Altura donde termina el tramo recto y empieza la curva hacia la glorieta.
    // Ej: de y = -40 hasta y = 120 el camino es una línea recta.
    const int LANE_BEND_Y = 120;

    // Primer punto de control de la curva (curva de Bézier).
    // Ej: jala la curva hacia abajo antes de girar.
    const int LANE_CONTROL1_X = 78;
    const int LANE_CONTROL1_Y = 185;

    // Segundo punto de control de la curva.
    // Ej: jala la curva hacia la derecha, hacia la glorieta.
    const int LANE_CONTROL2_X = 105;
    const int LANE_CONTROL2_Y = 212;

    // Punto donde el camino entra a la glorieta.
    // Ej: ahí la primera fila de personas espera entrar a la fila rotatoria.
    const int LANE_END_X = 160;
    const int LANE_END_Y = 216;

    // ---------- Zona del tablero (tráfico) ----------

    // Esquina izquierda del panel donde va el tablero.
    // Ej: deja 20 px de margen con el borde de la ventana.
    const int BOARD_X = 20;

    // Esquina de arriba del panel donde va el tablero.
    // Ej: empieza un poco debajo de la franja de parqueo.
    const int BOARD_Y = 496;

    // Ancho del panel del tablero.
    // Ej: en un nivel de 8 columnas, cada celda mediría 536 / 8 = 67 px.
    const int BOARD_WIDTH = 536;

    // Alto del panel del tablero.
    // Ej: en un nivel de 16 filas, cada celda mediría 508 / 16 = 31 px.
    const int BOARD_HEIGHT = 508;
}
