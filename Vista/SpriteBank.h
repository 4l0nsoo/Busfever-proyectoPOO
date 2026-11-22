/**
 * @file SpriteBank.h
 * @brief Definición de la clase SpriteBank
 *
 * @details
 * Carga una sola vez todas las imágenes (sprites) de la carpeta assets/sprites:
 * los 4 cuadros de las personas caminando de cada color y los 2 cuadros de cada
 * vehículo (carro, bus mediano y bus grande) de cada color. Luego las entrega a
 * quien las necesite para dibujar, y las libera al final.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   SpriteBank
 *
 * Responsabilidades:
 *  - Saber en qué carpeta y con qué nombre está cada sprite.
 *  - Cargar todos los sprites de personas y vehículos.
 *  - Entregar el sprite pedido según color, tamaño y cuadro de animación.
 *  - Liberar la memoria de todos los sprites al terminar.
 *
 * Colaboradores:
 *  - PassengerRenderer:
 *      * Le pide los sprites de las personas.
 *  - BoardRenderer:
 *      * Le pide los sprites de los vehículos.
 *
 * Colores (igual que en los archivos de nivel): 1 rojo, 2 azul, 3 amarillo,
 * 4 verde, 5 rosado.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

// Trae ALLEGRO_BITMAP, el tipo de una imagen cargada.
// Ej: cada sprite es un ALLEGRO_BITMAP*.
#include <allegro5/allegro.h>

// Trae std::string para armar las rutas de los archivos.
// Ej: "assets/sprites/personas/rojos/caminar_0.png".
#include <string>

// Trae std::vector para guardar los sprites en listas.
// Ej: una lista de cuadros por cada color.
#include <vector>

class SpriteBank {
private:
    // Sprites de personas: [color - 1][cuadro].
    // Ej: personFrames[1][3] es la persona azul en el cuadro 3.
    std::vector<std::vector<ALLEGRO_BITMAP*>> personFrames;

    // Sprites de vehículos: [color - 1][largo - 1][cuadro].
    // Ej: vehicleFrames[0][2][1] es el bus grande rojo en el cuadro 1.
    std::vector<std::vector<std::vector<ALLEGRO_BITMAP*>>> vehicleFrames;

    // Carga una imagen y la guarda en la lista; retorna false si no existe.
    // Ej: loadInto("assets/sprites/buses/busesrojos/carro_0.png", lista);
    bool loadInto(const std::string& path, std::vector<ALLEGRO_BITMAP*>& frames);

public:
    // Cantidad de colores del juego.
    // Ej: 5 (rojo, azul, amarillo, verde, rosado).
    static const int COLOR_COUNT = 5;

    // Cuadros de la animación de caminar.
    // Ej: caminar_0.png hasta caminar_3.png.
    static const int PERSON_FRAMES = 4;

    // Cuadros de la animación de cada vehículo.
    // Ej: carro_0.png y carro_1.png.
    static const int VEHICLE_FRAMES = 2;

    // Tamaños de vehículo (largo en celdas).
    // Ej: 1 = carro, 2 = bus mediano, 3 = bus grande.
    static const int VEHICLE_SIZES = 3;

    // Constructor: todavía no carga ninguna imagen.
    // Ej: SpriteBank sprites; y luego sprites.load("assets/sprites");
    SpriteBank() = default;

    // Destructor: libera todas las imágenes cargadas.
    // Ej: se llama solo al terminar SceneryWindow::run().
    ~SpriteBank();

    // Se prohíbe copiar: dos objetos liberarían las mismas imágenes.
    // Ej: SpriteBank otro = sprites; da error de compilación.
    SpriteBank(const SpriteBank&) = delete;

    // Se prohíbe asignar por la misma razón.
    // Ej: otro = sprites; da error de compilación.
    SpriteBank& operator=(const SpriteBank&) = delete;

    // Carga todos los sprites desde la carpeta indicada; retorna false si falta alguno.
    // Ej: sprites.load("assets/sprites");
    // Debe llamarse DESPUÉS de crear la ventana (AllegroWindow::initialize).
    bool load(const std::string& spritesFolder);

    // Retorna el sprite de una persona (nullptr si el color o el cuadro no existen).
    // Ej: getPerson(3, 0) -> persona amarilla en el cuadro 0.
    ALLEGRO_BITMAP* getPerson(int color, int frame) const;

    // Retorna el sprite de un vehículo (nullptr si no existe).
    // Ej: getVehicle(2, 3, 1) -> bus grande azul en el cuadro 1.
    ALLEGRO_BITMAP* getVehicle(int color, int length, int frame) const;
};
