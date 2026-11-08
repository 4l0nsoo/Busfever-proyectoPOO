/**
 * @file MainMenu.h
 * @brief Definición de la clase MainMenu
 *
 * @details
 * Menú principal que aparece antes de jugar y al retirarse de una partida.
 * Hereda de MenuPanel todo lo común (panel, botones, mouse) y solo agrega sus
 * cuatro botones (Nivel 1, Nivel 2, Nivel 3 y Salir) y su encabezado con el
 * nombre del juego. No carga niveles: eso lo hace el Controller.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   MainMenu (hereda de MenuPanel)
 *
 * Responsabilidades:
 *  - Crear los botones de los tres niveles y el de salir.
 *  - Dibujar el nombre del juego y la instrucción "Elige un nivel".
 *  - Mostrar al pie los atajos de teclado.
 *
 * Colaboradores:
 *  - MenuPanel:
 *      * Clase base de la que hereda el panel, los botones y el mouse.
 *  - LevelCatalog:
 *      * Le da el nombre de cada dificultad para los botones.
 *  - SceneryWindow:
 *      * Le pregunta qué opción se eligió.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
#include "MenuPanel.h"

class MainMenu : public MenuPanel {
protected:
    // Dibuja el nombre del juego, el subtítulo y la instrucción.
    // Ej: "Bus Fever Party!", "FPOO-C++ Edition" y "Elige un nivel".
    void drawHeader() const override;

    // Dibuja al pie los atajos de teclado.
    // Ej: "Teclas 1, 2, 3 = nivel | ESC = salir".
    void drawFooter() const override;

public:
    // Crea el panel y sus cuatro botones.
    // Ej: MainMenu menu; y luego menu.loadFonts("C:/Windows/Fonts/arialbd.ttf");
    MainMenu();
};
