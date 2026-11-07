#include "MainMenu.h"
#include "../Controlador/LevelCatalog.h"

// Espacio de nombres anónimo: medidas que solo usa este archivo.
// Ej: otro .cpp no puede cambiar PANEL_TOP.
namespace {

// Rectángulo del panel blanco del menú principal.
// Ej: va de x = 60 a x = 516 y de y = 240 a y = 840.
const float PANEL_LEFT = 60.0f;
const float PANEL_TOP = 240.0f;
const float PANEL_RIGHT = 516.0f;
const float PANEL_BOTTOM = 840.0f;

// Bordes izquierdo y derecho de los botones de nivel.
// Ej: botones de 340 px de ancho.
const float BUTTON_LEFT = 118.0f;
const float BUTTON_RIGHT = 458.0f;

// Alto de los botones de nivel y separación entre uno y otro.
// Ej: el botón 1 empieza en y = 440 y el botón 2 en 440 + 94 = 534.
const float BUTTON_HEIGHT = 76.0f;
const float BUTTON_GAP = 94.0f;
const float FIRST_BUTTON_TOP = 440.0f;

// Botón "Salir": más angosto y más bajo que los de nivel.
// Ej: va de x = 188 a x = 388.
const float EXIT_LEFT = 188.0f;
const float EXIT_RIGHT = 388.0f;
const float EXIT_TOP = 730.0f;
const float EXIT_HEIGHT = 54.0f;

}

// Constructor: crea el panel (en la clase base) y agrega los cuatro botones.
// Ej: el primer botón es "Nivel 1 - Fácil" en verde.
MainMenu::MainMenu() : MenuPanel(PANEL_LEFT, PANEL_TOP, PANEL_RIGHT, PANEL_BOTTOM) {
    // Opciones de nivel en orden.
    // Ej: Level1, Level2, Level3.
    const MenuOption levels[3] = {MenuOption::Level1, MenuOption::Level2, MenuOption::Level3};

    // Color de la cara de cada botón de nivel: verde, naranja y rojo (de fácil a difícil).
    // Ej: al_map_rgb(rojo, verde, azul).
    const ALLEGRO_COLOR faces[3] = {al_map_rgb(67, 184, 90), al_map_rgb(246, 168, 38),
                                    al_map_rgb(234, 75, 67)};

    // Color del costado (más oscuro) de cada botón.
    // Ej: verde oscuro debajo del botón verde.
    const ALLEGRO_COLOR sides[3] = {al_map_rgb(42, 133, 64), al_map_rgb(196, 120, 10),
                                    al_map_rgb(178, 46, 40)};

    // Crea los tres botones de nivel.
    // Ej: i = 0 -> Nivel 1, i = 2 -> Nivel 3.
    for (int i = 0; i < 3; i++) {
        // Número del nivel (1, 2 o 3).
        // Ej: i = 1 -> nivel 2.
        const int number = static_cast<int>(levels[i]);

        // Dificultad que corresponde a ese número, para pedir su nombre.
        // Ej: 2 -> Difficulty::Normal -> "Normal".
        Difficulty difficulty = Difficulty::Easy;
        LevelCatalog::fromNumber(number, difficulty);

        // Altura donde empieza este botón.
        // Ej: i = 2 -> 440 + 2 * 94 = 628.
        const float top = FIRST_BUTTON_TOP + i * BUTTON_GAP;

        // Agrega el botón (método heredado de MenuPanel).
        // Ej: "Nivel 3" a la izquierda y "Difícil" a la derecha, en rojo.
        addButton(levels[i], "Nivel " + std::to_string(number), LevelCatalog::getName(difficulty),
                  faces[i], sides[i], BUTTON_LEFT, top, BUTTON_RIGHT, top + BUTTON_HEIGHT);
    }

    // Agrega el botón "Salir" en gris al final.
    // Ej: cierra la ventana al hacerle clic.
    addButton(MenuOption::Exit, "Salir", "", al_map_rgb(122, 130, 154), al_map_rgb(84, 90, 112),
              EXIT_LEFT, EXIT_TOP, EXIT_RIGHT, EXIT_TOP + EXIT_HEIGHT);
}

void MainMenu::drawHeader() const {
    // Color azul oscuro de los textos del panel.
    // Ej: al_map_rgb(rojo, verde, azul).
    const ALLEGRO_COLOR dark = al_map_rgb(40, 52, 82);

    // Escribe el nombre del juego con la fuente grande.
    // Ej: "Bus Fever Party!" a 34 px del borde de arriba del panel.
    drawCenteredText(getTitleFont(), dark, getPanelTop() + 34, "Bus Fever Party!");

    // Escribe el subtítulo en gris con la fuente pequeña.
    // Ej: "FPOO-C++ Edition".
    drawCenteredText(getSmallFont(), al_map_rgb(112, 120, 150), getPanelTop() + 90, "FPOO-C++ Edition");

    // Línea fina que separa el título de los botones.
    // Ej: 132 px debajo del borde de arriba del panel.
    drawSeparator(getPanelTop() + 132);

    // Escribe la instrucción para el jugador.
    // Ej: "Elige un nivel".
    drawCenteredText(getButtonFont(), dark, getPanelTop() + 150, "Elige un nivel");
}

void MainMenu::drawFooter() const {
    // Escribe los atajos de teclado al pie del panel, en gris.
    // Ej: también se puede elegir con las teclas 1, 2 y 3.
    drawCenteredText(getSmallFont(), al_map_rgb(140, 147, 175),
                     getPanelBottom() - 16 - al_get_font_line_height(getSmallFont()),
                     "Teclas 1, 2, 3 = nivel   |   ESC = salir");
}
