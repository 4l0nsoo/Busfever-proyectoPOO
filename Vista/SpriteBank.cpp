#include "SpriteBank.h"

// Módulo de imágenes: permite cargar archivos .png.
// Ej: al_load_bitmap("caminar_0.png").
#include <allegro5/allegro_image.h>

// Espacio de nombres anónimo: lo de adentro solo se usa en este archivo.
// Ej: otro .cpp no puede usar PERSON_FOLDERS por accidente.
namespace {

// Carpeta de las personas de cada color, en el orden de los colores (1 a 5).
// Ej: el color 1 (rojo) está en "personas/rojos".
const char* PERSON_FOLDERS[] = {"rojos", "azules", "amarilla", "verde", "rosado"};

// Carpeta de los vehículos de cada color, en el mismo orden.
// Ej: el color 4 (verde) está en "buses/busesverdes".
const char* VEHICLE_FOLDERS[] = {"busesrojos", "busesazules", "busesamarillos", "busesverdes", "busesrosados"};

// Nombre del archivo de cada tamaño de vehículo (largo 1, 2 y 3).
// Ej: largo 2 -> "busmediano_0.png" y "busmediano_1.png".
const char* VEHICLE_NAMES[] = {"carro", "busmediano", "busgrande"};

}

SpriteBank::~SpriteBank() {
    // Recorre los cuadros de cada color de persona.
    // Ej: 5 colores x 4 cuadros = 20 imágenes.
    for (const auto& frames : personFrames) {
        // Libera cada imagen de ese color.
        // Ej: caminar_0.png, caminar_1.png...
        for (ALLEGRO_BITMAP* bitmap : frames) {
            al_destroy_bitmap(bitmap);
        }
    }

    // Recorre los vehículos de cada color y cada tamaño.
    // Ej: 5 colores x 3 tamaños x 2 cuadros = 30 imágenes.
    for (const auto& sizes : vehicleFrames) {
        for (const auto& frames : sizes) {
            // Libera cada imagen de ese vehículo.
            // Ej: carro_0.png y carro_1.png.
            for (ALLEGRO_BITMAP* bitmap : frames) {
                al_destroy_bitmap(bitmap);
            }
        }
    }
}

bool SpriteBank::loadInto(const std::string& path, std::vector<ALLEGRO_BITMAP*>& frames) {
    // Carga la imagen desde el archivo.
    // Ej: path = "assets/sprites/personas/verde/caminar_2.png".
    ALLEGRO_BITMAP* bitmap = al_load_bitmap(path.c_str());

    // Si no existe el archivo, avisa con false.
    // Ej: pasa si se borró o se renombró un sprite.
    if (bitmap == nullptr) {
        return false;
    }

    // Guarda la imagen al final de la lista.
    // Ej: la lista de cuadros queda [cuadro0, cuadro1, ...].
    frames.push_back(bitmap);
    return true;
}

bool SpriteBank::load(const std::string& spritesFolder) {
    // Guarda la configuración actual de cómo se crean las imágenes.
    // Ej: para dejarla igual al terminar.
    const int previousFlags = al_get_new_bitmap_flags();

    // Las imágenes se verán suaves al reducirlas (MIPMAP guarda copias más pequeñas).
    // Ej: un bus de 192 px dibujado a 93 px no se ve pixelado.
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);

    // Supone que todo va a cargar bien hasta que algo falle.
    // Ej: si falta un archivo, ok pasa a false.
    bool ok = true;

    // Prepara una lista vacía de cuadros para cada color de persona.
    // Ej: personFrames queda con 5 listas vacías.
    personFrames.assign(COLOR_COUNT, std::vector<ALLEGRO_BITMAP*>());

    // Prepara, para cada color, una lista por cada tamaño de vehículo.
    // Ej: vehicleFrames queda con 5 colores x 3 tamaños, todas vacías.
    vehicleFrames.assign(COLOR_COUNT, std::vector<std::vector<ALLEGRO_BITMAP*>>(VEHICLE_SIZES));

    // Recorre los 5 colores.
    // Ej: c = 0 es rojo y c = 4 es rosado.
    for (int c = 0; c < COLOR_COUNT; c++) {
        // Carga los 4 cuadros de la persona caminando de este color.
        // Ej: personas/rojos/caminar_0.png ... caminar_3.png.
        for (int f = 0; f < PERSON_FRAMES; f++) {
            // Arma la ruta completa del archivo.
            // Ej: "assets/sprites" + "/personas/" + "rojos" + "/caminar_" + "0" + ".png".
            const std::string path = spritesFolder + "/personas/" + PERSON_FOLDERS[c] +
                                     "/caminar_" + std::to_string(f) + ".png";

            // Carga el cuadro; si falla, se recuerda el error con &&.
            // Ej: ok = true && false -> false.
            ok = loadInto(path, personFrames[c]) && ok;
        }

        // Recorre los 3 tamaños de vehículo de este color.
        // Ej: s = 0 carro, s = 1 bus mediano, s = 2 bus grande.
        for (int s = 0; s < VEHICLE_SIZES; s++) {
            // Carga los 2 cuadros de ese vehículo.
            // Ej: buses/busesrojos/carro_0.png y carro_1.png.
            for (int f = 0; f < VEHICLE_FRAMES; f++) {
                // Arma la ruta completa del archivo.
                // Ej: "assets/sprites/buses/busesazules/busgrande_1.png".
                const std::string path = spritesFolder + "/buses/" + VEHICLE_FOLDERS[c] + "/" +
                                         VEHICLE_NAMES[s] + "_" + std::to_string(f) + ".png";

                // Carga el cuadro y recuerda si hubo error.
                // Ej: si falta busgrande_1.png, ok queda en false.
                ok = loadInto(path, vehicleFrames[c][s]) && ok;
            }
        }
    }

    // Deja la configuración de imágenes como estaba.
    // Ej: el fondo y otras imágenes no se ven afectados.
    al_set_new_bitmap_flags(previousFlags);

    // Retorna true solo si cargaron los 50 sprites.
    // Ej: false si faltó alguno.
    return ok;
}

ALLEGRO_BITMAP* SpriteBank::getPerson(int color, int frame) const {
    // Convierte el color (1 a 5) en posición de la lista (0 a 4).
    // Ej: color 3 (amarillo) -> índice 2.
    const int index = color - 1;

    // Si el color no existe o no se cargó, no hay imagen.
    // Ej: color 9 -> nullptr.
    if (index < 0 || index >= static_cast<int>(personFrames.size())) {
        return nullptr;
    }

    // Si el cuadro no existe, no hay imagen.
    // Ej: cuadro 7 con solo 4 cuadros -> nullptr.
    if (frame < 0 || frame >= static_cast<int>(personFrames[index].size())) {
        return nullptr;
    }

    // Retorna la imagen pedida.
    // Ej: persona amarilla en el cuadro 0.
    return personFrames[index][frame];
}

ALLEGRO_BITMAP* SpriteBank::getVehicle(int color, int length, int frame) const {
    // Convierte el color (1 a 5) en posición de la lista (0 a 4).
    // Ej: color 1 (rojo) -> índice 0.
    const int index = color - 1;

    // Convierte el largo (1 a 3) en posición de la lista (0 a 2).
    // Ej: largo 3 (bus grande) -> índice 2.
    const int size = length - 1;

    // Si el color no existe, no hay imagen.
    // Ej: color 0 -> nullptr.
    if (index < 0 || index >= static_cast<int>(vehicleFrames.size())) {
        return nullptr;
    }

    // Si el tamaño no existe, no hay imagen.
    // Ej: largo 4 -> nullptr.
    if (size < 0 || size >= static_cast<int>(vehicleFrames[index].size())) {
        return nullptr;
    }

    // Si el cuadro no existe, no hay imagen.
    // Ej: cuadro 2 con solo 2 cuadros (0 y 1) -> nullptr.
    if (frame < 0 || frame >= static_cast<int>(vehicleFrames[index][size].size())) {
        return nullptr;
    }

    // Retorna la imagen pedida.
    // Ej: bus grande rojo en el cuadro 1.
    return vehicleFrames[index][size][frame];
}
