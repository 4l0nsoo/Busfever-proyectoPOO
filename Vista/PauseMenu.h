/**
 * @file PauseMenu.h
 * @brief Definición de la clase PauseMenu
 *
 * @details
 * Menú que aparece al pausar la partida. Hereda de MenuPanel todo lo común
 * (panel, botones, mouse) y solo agrega dos botones: "Continuar" (vuelve a la
 * partida) y "Retirarse" (abandona la partida y vuelve al menú principal), y su
 * encabezado con la palabra "Pausa" y el nivel que se está jugando.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   PauseMenu (hereda de MenuPanel)
 *
 * Responsabilidades:
 *  - Crear los botones "Continuar" y "Retirarse".
 *  - Guardar y mostrar el nombre del nivel que se está jugando.
 *  - Dibujar el encabezado "Pausa" y la ayuda de teclado.
 *
 * Colaboradores:
 *  - MenuPanel:
 *      * Clase base de la que hereda el panel, los botones y el mouse.
 *  - SceneryWindow:
 *      * Lo muestra al pausar y le pregunta qué opción se eligió.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
#include "MenuPanel.h"

class PauseMenu : public MenuPanel {
private:
    // Nombre del nivel que se está jugando, para mostrarlo debajo de "Pausa".
    // Ej: "Nivel 2 (Normal)".
    std::string levelName;

protected:
    // Dibuja "Pausa" y el nombre del nivel.
    // Ej: "Pausa" en grande y "Nivel 1 (Fácil)" en gris.
    void drawHeader() const override;

    // Dibuja al pie la ayuda de teclado.
    // Ej: "ESC o P = continuar".
    void drawFooter() const override;

public:
    // Crea el panel y sus dos botones.
    // Ej: PauseMenu pauseMenu; y luego pauseMenu.loadFonts("C:/Windows/Fonts/arialbd.ttf");
    PauseMenu();

    // Cambia el nombre del nivel que se muestra.
    // Ej: pauseMenu.setLevelName("Nivel 3 (Difícil)");
    void setLevelName(const std::string& name);
};
