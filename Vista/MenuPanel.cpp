#include "MenuPanel.h"
#include "SceneLayout.h"

// Módulo de fuentes .ttf.
// Ej: al_load_ttf_font("arialbd.ttf", 44, 0).
#include <allegro5/allegro_ttf.h>

// Módulo de figuras: rectángulos redondeados del panel y los botones.
// Ej: al_draw_filled_rounded_rectangle.
#include <allegro5/allegro_primitives.h>

// Trae sin() para el brillo que palpita en el botón señalado.
// Ej: sin(0) = 0 y sin(PI / 2) = 1.
#include <cmath>

// Espacio de nombres anónimo: medidas que solo usa este archivo.
// Ej: otro .cpp no puede cambiar BUTTON_RADIUS.
namespace {

// Redondeo de las esquinas de los botones y altura del "costado" que da relieve.
// Ej: 6 px de color oscuro debajo de cada botón.
const float BUTTON_RADIUS = 18.0f;
const float BUTTON_DEPTH = 6.0f;

// Redondeo de las esquinas del panel blanco.
// Ej: 28 px, más redondo que los botones.
const float PANEL_RADIUS = 28.0f;

// Tamaños de las tres fuentes.
// Ej: el título es el más grande.
const int TITLE_SIZE = 44;
const int BUTTON_SIZE = 30;
const int SMALL_SIZE = 20;

}

// Constructor: guarda el rectángulo del panel; todavía no hay botones ni fuentes.
// Ej: MenuPanel(88, 330, 488, 720) para el menú de pausa.
MenuPanel::MenuPanel(float panelLeft, float panelTop, float panelRight, float panelBottom)
    : panelLeft(panelLeft), panelTop(panelTop), panelRight(panelRight), panelBottom(panelBottom),
      hovered(MenuOption::None), elapsed(0.0),
      titleFont(nullptr), buttonFont(nullptr), smallFont(nullptr) {}

MenuPanel::~MenuPanel() {
    // Libera cada fuente que se haya cargado.
    // Ej: si loadFonts() nunca se llamó, siguen en nullptr y no se tocan.
    if (titleFont != nullptr) al_destroy_font(titleFont);
    if (buttonFont != nullptr) al_destroy_font(buttonFont);
    if (smallFont != nullptr) al_destroy_font(smallFont);
}

void MenuPanel::addButton(MenuOption option, const std::string& label, const std::string& detail,
                          ALLEGRO_COLOR face, ALLEGRO_COLOR side,
                          float left, float top, float right, float bottom) {
    // Agrega el botón al final de la lista.
    // Ej: los botones se dibujan en el orden en que se agregan.
    buttons.push_back({option, label, detail, face, side, left, top, right, bottom});
}

ALLEGRO_FONT* MenuPanel::loadFont(const std::string& fontPath, int size) {
    // Intenta cargar la fuente .ttf con el tamaño pedido.
    // Ej: Arial negrita de 30 px.
    ALLEGRO_FONT* font = al_load_ttf_font(fontPath.c_str(), size, 0);

    // Si no existe, usa la letra incluida en Allegro (más pequeña, pero se lee).
    // Ej: en un computador sin Arial.
    if (font == nullptr) {
        font = al_create_builtin_font();
    }
    return font;
}

void MenuPanel::loadFonts(const std::string& fontPath) {
    // Carga la fuente de cada tamaño.
    // Ej: 44 px para el título, 30 px para los botones y 20 px para los detalles.
    titleFont = loadFont(fontPath, TITLE_SIZE);
    buttonFont = loadFont(fontPath, BUTTON_SIZE);
    smallFont = loadFont(fontPath, SMALL_SIZE);
}

void MenuPanel::setMousePosition(float x, float y) {
    // Guarda qué botón está debajo del mouse.
    // Ej: si el mouse sale de los botones, queda en None y ninguno brilla.
    hovered = optionAt(x, y);
}

MenuOption MenuPanel::optionAt(float x, float y) const {
    // Revisa los botones uno por uno.
    // Ej: primero el de arriba, luego el siguiente...
    for (const MenuButton& button : buttons) {
        // Si el punto está dentro del rectángulo del botón, esa es la opción.
        // Ej: (288, 520) está dentro de un botón que va de (128, 490) a (448, 560).
        if (x >= button.left && x <= button.right && y >= button.top && y <= button.bottom) {
            return button.option;
        }
    }

    // El punto no cae en ningún botón.
    // Ej: un clic en el título del menú.
    return MenuOption::None;
}

void MenuPanel::update(double seconds) {
    // Suma el tiempo para que el brillo del botón señalado palpite.
    // Ej: 1.0 + 0.016 = 1.016 segundos.
    elapsed += seconds;
}

void MenuPanel::drawFooter() const {
    // Por defecto el panel no tiene texto al pie.
    // Ej: una hija puede sobrescribir este método para mostrar ayudas.
}

void MenuPanel::drawCenteredText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float y,
                                 const std::string& text) const {
    // Escribe el texto centrado en el ancho de la ventana.
    // Ej: x = 576 / 2 = 288.
    al_draw_text(font, color, SceneLayout::WIDTH / 2.0f, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
}

void MenuPanel::drawSeparator(float y) const {
    // Línea fina gris clara, con 60 px de margen a cada lado del panel.
    // Ej: al_draw_line(x1, y1, x2, y2, color, grosor).
    al_draw_line(panelLeft + 60, y, panelRight - 60, y, al_map_rgb(222, 226, 240), 2.0f);
}

ALLEGRO_FONT* MenuPanel::getTitleFont() const {
    // Fuente grande.
    // Ej: para "Pausa".
    return titleFont;
}

ALLEGRO_FONT* MenuPanel::getButtonFont() const {
    // Fuente mediana.
    // Ej: para "Elige un nivel".
    return buttonFont;
}

ALLEGRO_FONT* MenuPanel::getSmallFont() const {
    // Fuente pequeña.
    // Ej: para "FPOO-C++ Edition".
    return smallFont;
}

float MenuPanel::getPanelTop() const {
    // Borde de arriba del panel.
    // Ej: 240 en el menú principal.
    return panelTop;
}

float MenuPanel::getPanelBottom() const {
    // Borde de abajo del panel.
    // Ej: 840 en el menú principal.
    return panelBottom;
}

void MenuPanel::drawButton(const MenuButton& button) const {
    // Indica si este botón es el que tiene el mouse encima.
    // Ej: true para "Continuar" si el mouse está sobre él.
    const bool isHovered = (button.option == hovered);

    // El botón señalado se "hunde" un poco: su cara baja 3 px.
    // Ej: parece que se está presionando.
    const float press = isHovered ? 3.0f : 0.0f;

    // Dibuja el costado oscuro (relieve) debajo del botón.
    // Ej: se ve como el borde de una tecla.
    al_draw_filled_rounded_rectangle(button.left, button.top + BUTTON_DEPTH,
                                     button.right, button.bottom + BUTTON_DEPTH,
                                     BUTTON_RADIUS, BUTTON_RADIUS, button.side);

    // Dibuja la cara de color del botón.
    // Ej: verde para "Continuar".
    al_draw_filled_rounded_rectangle(button.left, button.top + press,
                                     button.right, button.bottom + press,
                                     BUTTON_RADIUS, BUTTON_RADIUS, button.face);

    // Brillo blanco suave en la mitad de arriba para dar volumen.
    // Ej: al_premul_rgba(255, 255, 255, 40) es blanco casi transparente.
    al_draw_filled_rounded_rectangle(button.left + 8, button.top + press + 5,
                                     button.right - 8, (button.top + button.bottom) / 2.0f + press,
                                     BUTTON_RADIUS - 6, BUTTON_RADIUS - 6,
                                     al_premul_rgba(255, 255, 255, 40));

    // Si el mouse está encima, dibuja un borde blanco que palpita.
    // Ej: la transparencia sube y baja con sin(tiempo).
    if (isHovered) {
        // Transparencia entre 120 y 240 que cambia con el tiempo.
        // Ej: sin() va de -1 a 1 -> 180 + 60 * sin() va de 120 a 240.
        const int alpha = static_cast<int>(180 + 60 * std::sin(elapsed * 6.0));

        // Borde blanco de 4 px alrededor de la cara del botón.
        // Ej: al_draw_rounded_rectangle dibuja solo el contorno.
        al_draw_rounded_rectangle(button.left, button.top + press, button.right, button.bottom + press,
                                  BUTTON_RADIUS, BUTTON_RADIUS, al_premul_rgba(255, 255, 255, alpha), 4.0f);
    }

    // Centro vertical del botón, para centrar los textos.
    // Ej: (490 + 560) / 2 = 525.
    const float centerY = (button.top + button.bottom) / 2.0f + press;

    // Color blanco de los textos.
    // Ej: blanco sobre verde se lee bien.
    const ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    // Si el botón no tiene detalle, el texto va centrado.
    // Ej: "Continuar" en el centro del botón.
    if (button.detail.empty()) {
        // Escribe el texto centrado (se resta media altura de letra para centrarlo en y).
        // Ej: al_get_font_line_height da el alto de una línea de texto.
        al_draw_text(buttonFont, white, (button.left + button.right) / 2.0f,
                     centerY - al_get_font_line_height(buttonFont) / 2.0f,
                     ALLEGRO_ALIGN_CENTRE, button.label.c_str());
        return;
    }

    // Escribe el texto principal a la izquierda.
    // Ej: "Nivel 1".
    al_draw_text(buttonFont, white, button.left + 26.0f,
                 centerY - al_get_font_line_height(buttonFont) / 2.0f,
                 ALLEGRO_ALIGN_LEFT, button.label.c_str());

    // Escribe el detalle a la derecha, un poco transparente.
    // Ej: "Fácil".
    al_draw_text(smallFont, al_premul_rgba(255, 255, 255, 225), button.right - 26.0f,
                 centerY - al_get_font_line_height(smallFont) / 2.0f,
                 ALLEGRO_ALIGN_RIGHT, button.detail.c_str());
}

void MenuPanel::draw() const {
    // Capa oscura semitransparente sobre todo lo de atrás para que resalte el panel.
    // Ej: el juego congelado se ve, pero más oscuro.
    al_draw_filled_rectangle(0, 0, SceneLayout::WIDTH, SceneLayout::HEIGHT,
                             al_premul_rgba(18, 22, 38, 150));

    // Sombra del panel, corrida 8 px hacia abajo.
    // Ej: hace que el panel parezca flotar.
    al_draw_filled_rounded_rectangle(panelLeft, panelTop + 8, panelRight, panelBottom + 8,
                                     PANEL_RADIUS, PANEL_RADIUS, al_premul_rgba(0, 0, 0, 70));

    // Panel blanco del menú.
    // Ej: un rectángulo redondeado casi blanco.
    al_draw_filled_rounded_rectangle(panelLeft, panelTop, panelRight, panelBottom,
                                     PANEL_RADIUS, PANEL_RADIUS, al_map_rgb(246, 247, 253));

    // Encabezado propio de cada menú (polimorfismo: se llama al drawHeader de la hija).
    // Ej: en MainMenu escribe "Bus Fever Party!"; en PauseMenu escribe "Pausa".
    drawHeader();

    // Dibuja cada botón.
    // Ej: "Continuar" y "Retirarse" en el menú de pausa.
    for (const MenuButton& button : buttons) {
        drawButton(button);
    }

    // Pie de página propio de cada menú (si la hija lo definió).
    // Ej: "ESC = continuar".
    drawFooter();
}
