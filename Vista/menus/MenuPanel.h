/**
 * @file MenuPanel.h
 * @brief Definición de la clase base MenuPanel y del enum MenuOption
 *
 * @details
 * Clase base de todos los menús del juego (el menú principal y el menú de
 * pausa). Sabe dibujar la capa oscura, el panel blanco y los botones con
 * relieve, iluminar el botón que está debajo del mouse y decir qué botón se
 * presionó. Cada menú hijo solo agrega sus botones y dibuja su encabezado
 * (título y textos), así no se repite el código (regla DRY).
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   MenuPanel (clase base abstracta, implementa IRenderable)
 *
 * Responsabilidades:
 *  - Guardar el rectángulo del panel y la lista de botones.
 *  - Cargar y liberar las fuentes de los textos.
 *  - Dibujar la capa oscura, el panel y los botones.
 *  - Iluminar el botón señalado por el mouse y decir qué opción hay en un punto.
 *  - Obligar a cada hijo a dibujar su encabezado (método virtual puro drawHeader).
 *
 * Colaboradores:
 *  - MainMenu, PauseMenu:
 *      * Heredan de MenuPanel y agregan sus propios botones y textos.
 *  - IRenderable:
 *      * Es la interfaz que implementa para dibujarse como una capa más.
 *  - SceneryWindow:
 *      * Le pasa la posición del mouse y le pregunta qué opción se eligió.
 *
 * Herencia y polimorfismo: SceneryWindow puede tratar a MainMenu y a PauseMenu
 * como MenuPanel, y draw() llama al drawHeader() de cada hijo.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
// Interfaz de todo lo que se dibuja; está en Vista/, una carpeta arriba de Vista/menus/.
// Ej: "../" significa "subir a la carpeta de arriba".
#include "../IRenderable.h"

// Trae ALLEGRO_COLOR y los tipos básicos de Allegro.
// Ej: el color verde de un botón.
#include <allegro5/allegro.h>

// Trae ALLEGRO_FONT, el tipo de una fuente cargada.
// Ej: la letra del título de un menú.
#include <allegro5/allegro_font.h>

// Trae std::string y std::vector para los textos y la lista de botones.
// Ej: {"Continuar", "Retirarse"}.
#include <string>
#include <vector>

// Opciones de todos los menús; los niveles valen lo mismo que su dificultad.
// Ej: MenuOption::Level2 vale 2 (Difficulty::Normal); MenuOption::Retire abandona la partida.
enum class MenuOption {
    None = 0,
    Level1 = 1,
    Level2 = 2,
    Level3 = 3,
    Exit = 4,
    Resume = 5,
    Retire = 6
};

class MenuPanel : public IRenderable {
private:
    // Datos de un botón: qué opción es, sus textos, sus colores y su rectángulo.
    // Ej: {Resume, "Continuar", "", verde, verde oscuro, 128, 490, 448, 560}.
    struct MenuButton {
        MenuOption option;
        std::string label;
        std::string detail;
        ALLEGRO_COLOR face;
        ALLEGRO_COLOR side;
        float left;
        float top;
        float right;
        float bottom;
    };

    // Rectángulo del panel blanco.
    // Ej: el menú principal va de (60, 240) a (516, 840).
    float panelLeft;
    float panelTop;
    float panelRight;
    float panelBottom;

    // Lista de los botones del menú, de arriba a abajo.
    // Ej: 4 botones en el menú principal y 2 en el de pausa.
    std::vector<MenuButton> buttons;

    // Opción que está debajo del mouse (None si ninguna).
    // Ej: si el mouse está sobre "Retirarse", vale MenuOption::Retire.
    MenuOption hovered;

    // Tiempo que lleva abierto el menú, para el brillo que palpita en el botón señalado.
    // Ej: 2.5 segundos.
    double elapsed;

    // Fuentes grande (títulos), mediana (botones) y pequeña (textos secundarios).
    // Ej: 44, 30 y 20 píxeles.
    ALLEGRO_FONT* titleFont;
    ALLEGRO_FONT* buttonFont;
    ALLEGRO_FONT* smallFont;

    // Carga una fuente .ttf; si no existe, usa la letra incluida en Allegro.
    // Ej: loadFont("C:/Windows/Fonts/arialbd.ttf", 44).
    static ALLEGRO_FONT* loadFont(const std::string& fontPath, int size);

    // Dibuja un botón con su relieve, su color y sus textos.
    // Ej: drawButton(buttons[0]) dibuja el primer botón.
    void drawButton(const MenuButton& button) const;

protected:
    // Constructor protegido: solo las clases hijas pueden crear un MenuPanel.
    // Ej: MainMenu() : MenuPanel(60, 240, 516, 840) {}
    MenuPanel(float panelLeft, float panelTop, float panelRight, float panelBottom);

    // Agrega un botón al menú (lo usan los constructores de las clases hijas).
    // Ej: addButton(MenuOption::Resume, "Continuar", "", verde, verdeOscuro, 128, 490, 448, 560);
    void addButton(MenuOption option, const std::string& label, const std::string& detail,
                   ALLEGRO_COLOR face, ALLEGRO_COLOR side,
                   float left, float top, float right, float bottom);

    // Cada menú dibuja su propio encabezado (título y textos): método virtual puro.
    // Ej: MainMenu escribe "Bus Fever Party!"; PauseMenu escribe "Pausa".
    virtual void drawHeader() const = 0;

    // Texto de ayuda al pie del panel; por defecto no hay ninguno (las hijas lo pueden cambiar).
    // Ej: MainMenu muestra "Teclas 1, 2, 3 = nivel".
    virtual void drawFooter() const;

    // Escribe un texto centrado en x, a la altura y, con la fuente y el color dados.
    // Ej: drawCenteredText(getTitleFont(), azul, 300, "Pausa");
    void drawCenteredText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float y, const std::string& text) const;

    // Dibuja una línea fina horizontal dentro del panel (para separar secciones).
    // Ej: drawSeparator(372) dibuja la línea debajo del título.
    void drawSeparator(float y) const;

    // Getters de las fuentes y del panel para que las hijas dibujen sus textos.
    // Ej: getTitleFont() para el título; getPanelTop() para ubicarlo dentro del panel.
    ALLEGRO_FONT* getTitleFont() const;
    ALLEGRO_FONT* getButtonFont() const;
    ALLEGRO_FONT* getSmallFont() const;
    float getPanelTop() const;
    float getPanelBottom() const;

public:
    // Destructor virtual: libera las fuentes y destruye bien a las hijas.
    // Ej: borrar un MenuPanel* que es un PauseMenu llama también al destructor de PauseMenu.
    virtual ~MenuPanel();

    // Se prohíbe copiar: dos menús liberarían las mismas fuentes.
    // Ej: PauseMenu otro = pauseMenu; da error de compilación.
    MenuPanel(const MenuPanel&) = delete;

    // Se prohíbe asignar por la misma razón.
    // Ej: otro = pauseMenu; da error de compilación.
    MenuPanel& operator=(const MenuPanel&) = delete;

    // Carga las tres fuentes del menú desde un archivo .ttf.
    // Ej: menu.loadFonts("C:/Windows/Fonts/arialbd.ttf");
    // Debe llamarse DESPUÉS de crear la ventana (AllegroWindow::initialize).
    void loadFonts(const std::string& fontPath);

    // Recibe la posición del mouse (en medidas del diseño) para iluminar el botón señalado.
    // Ej: menu.setMousePosition(288, 520);
    void setMousePosition(float x, float y);

    // Retorna la opción que hay en el punto (x, y), o None si no hay botón ahí.
    // Ej: optionAt(288, 620) -> MenuOption::Retire en el menú de pausa.
    MenuOption optionAt(float x, float y) const;

    // Avanza el tiempo del brillo del botón señalado.
    // Ej: menu.update(1.0 / 60.0);
    void update(double seconds) override;

    // Dibuja la capa oscura, el panel, el encabezado de la hija, los botones y el pie.
    // Ej: pauseMenu.draw(); encima del juego congelado.
    void draw() const override;
};
