#include "PauseMenu.h"

// Espacio de nombres anónimo: medidas que solo usa este archivo.
// Ej: otro .cpp no puede cambiar PANEL_TOP.
namespace {

// Rectángulo del panel del menú de pausa (más pequeño que el principal).
// Ej: va de x = 88 a x = 488 y de y = 330 a y = 720.
const float PANEL_LEFT = 88.0f;
const float PANEL_TOP = 330.0f;
const float PANEL_RIGHT = 488.0f;
const float PANEL_BOTTOM = 720.0f;

// Bordes izquierdo y derecho de los dos botones.
// Ej: botones de 320 px de ancho.
const float BUTTON_LEFT = 128.0f;
const float BUTTON_RIGHT = 448.0f;

// Alto de los botones y altura donde empieza cada uno.
// Ej: "Continuar" de y = 490 a 560 y "Retirarse" de y = 584 a 654.
const float BUTTON_HEIGHT = 70.0f;
const float RESUME_TOP = 490.0f;
const float RETIRE_TOP = 584.0f;

}

// Constructor: crea el panel (en la clase base) y agrega los dos botones.
// Ej: "Continuar" en verde y "Retirarse" en rojo.
PauseMenu::PauseMenu() : MenuPanel(PANEL_LEFT, PANEL_TOP, PANEL_RIGHT, PANEL_BOTTOM), levelName("") {
    // Botón verde para volver a la partida.
    // Ej: la partida sigue justo donde quedó.
    addButton(MenuOption::Resume, "Continuar", "", al_map_rgb(67, 184, 90), al_map_rgb(42, 133, 64),
              BUTTON_LEFT, RESUME_TOP, BUTTON_RIGHT, RESUME_TOP + BUTTON_HEIGHT);

    // Botón rojo para abandonar la partida y volver al menú principal.
    // Ej: allí hay que elegir otra vez un nivel.
    addButton(MenuOption::Retire, "Retirarse", "", al_map_rgb(234, 75, 67), al_map_rgb(178, 46, 40),
              BUTTON_LEFT, RETIRE_TOP, BUTTON_RIGHT, RETIRE_TOP + BUTTON_HEIGHT);
}

void PauseMenu::setLevelName(const std::string& name) {
    // Guarda el nombre del nivel.
    // Ej: "Nivel 2 (Normal)".
    levelName = name;
}

void PauseMenu::drawHeader() const {
    // Escribe "Pausa" en azul oscuro con la fuente grande.
    // Ej: a 34 px del borde de arriba del panel.
    drawCenteredText(getTitleFont(), al_map_rgb(40, 52, 82), getPanelTop() + 34, "Pausa");

    // Escribe el nivel que se está jugando, en gris.
    // Ej: "Nivel 1 (Fácil)".
    drawCenteredText(getSmallFont(), al_map_rgb(112, 120, 150), getPanelTop() + 92, levelName);

    // Línea fina que separa el encabezado de los botones.
    // Ej: 132 px debajo del borde de arriba del panel.
    drawSeparator(getPanelTop() + 132);
}

void PauseMenu::drawFooter() const {
    // Escribe la ayuda de teclado al pie del panel, en gris.
    // Ej: ESC o P cierran el menú de pausa.
    drawCenteredText(getSmallFont(), al_map_rgb(140, 147, 175),
                     getPanelBottom() - 16 - al_get_font_line_height(getSmallFont()),
                     "ESC o P = continuar");
}
