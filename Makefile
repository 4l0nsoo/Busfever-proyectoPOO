# ==============================================================================
# MAKEFILE - PROYECTO POO: BUS FEVER PARTY
# ==============================================================================
# USO (desde la terminal de VS Code, en la carpeta del proyecto):
#   mingw32-make          -> compila lo necesario y genera main.exe
#   mingw32-make run      -> compila y ejecuta el juego
#   mingw32-make clean    -> borra la carpeta build y main.exe (compilacion limpia)
#   mingw32-make info     -> muestra que archivos .cpp encontro el Makefile
#   mingw32-make check    -> muestra que hay en C:/allegro5/lib (para depurar)
# (Si en tu PC el comando "make" funciona, puedes usar "make" en vez de "mingw32-make")
# ==============================================================================

# Usamos el interprete de comandos de Windows (cmd) para que "if not exist",
# "mkdir", "del", etc. funcionen igual en cualquier PC con Windows.
SHELL := cmd

# ------------------------------------------------------------------------------
# 1) RUTAS DE TUS HERRAMIENTAS (cambialas aqui si instalas algo en otro lugar)
# ------------------------------------------------------------------------------
# Carpeta bin de MinGW. Es la MISMA que usa tu .vscode/tasks.json.
# Usar la ruta completa evita que make use por error otro g++ instalado en el PC
# (por ejemplo el de MSYS2), cuyas librerias NO son compatibles con tu Allegro.
MINGW_BIN := C:/mingw64/bin

# Carpeta donde instalaste Allegro (debe tener las subcarpetas include y lib).
ALLEGRO := C:/allegro5

# ------------------------------------------------------------------------------
# 2) OPCIONES DEL COMPILADOR Y DEL ENLAZADOR
# ------------------------------------------------------------------------------
# Compilador de C++.
CXX := $(MINGW_BIN)/g++.exe

# Opciones para COMPILAR cada .cpp:
#   -std=c++17       usa el estandar C++17
#   -Wall            muestra todas las advertencias utiles
#   -g               agrega informacion de depuracion (para el debugger de VS Code)
#   -I.              permite hacer #include "Modelo/Bus.h" desde la raiz del proyecto
#   -I$(ALLEGRO)/include   donde estan los .h de Allegro (<allegro5/allegro.h>)
#   -MMD -MP         genera archivos .d con las dependencias de cada .cpp, asi si
#                    cambias un .h, make recompila SOLO los .cpp que lo usan
CXXFLAGS := -std=c++17 -Wall -g -I. -I$(ALLEGRO)/include -MMD -MP

# Carpeta donde el enlazador busca las librerias de Allegro (archivos .a / .dll.a).
LDFLAGS := -L$(ALLEGRO)/lib

# Librerias de Allegro a enlazar. Este es el MISMO orden que ya te funcionaba en
# tu tasks.json. Son las unicas que tu codigo usa (allegro, image, primitives,
# font y ttf). Tu codigo NO usa audio, por eso NO se enlaza allegro_audio.
# Si algun dia usas sonido, agrega al final:  -lallegro_audio
LDLIBS := -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf

# ------------------------------------------------------------------------------
# 3) ARCHIVOS DEL PROYECTO (se detectan SOLOS, no hay que listarlos uno por uno)
# ------------------------------------------------------------------------------
# Nombre del ejecutable final.
TARGET := main.exe

# Carpeta donde se guardan los .o (archivos intermedios). Se crea sola.
OBJ_DIR := build

# Carpetas que contienen codigo (.cpp). >>> SI CREAS UNA CARPETA NUEVA CON CLASES,
# AGREGALA AQUI <<<. Ejemplo: SRC_DIRS := Modelo Controlador Vista Vista/menus Utils
SRC_DIRS := Modelo Controlador Vista Vista/menus

# Lista de todos los .cpp: main.cpp + todos los .cpp de cada carpeta de SRC_DIRS.
# "wildcard" busca archivos y "foreach" repite la busqueda por cada carpeta.
SRCS := main.cpp $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Lista de .o: cada Algo/Clase.cpp se convierte en build/Algo/Clase.o
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# Lista de archivos .d (dependencias generadas por -MMD), uno por cada .o
DEPS := $(OBJS:.o=.d)

# ------------------------------------------------------------------------------
# 4) REGLAS (que hace cada comando)
# ------------------------------------------------------------------------------
# Estos nombres son comandos, no archivos.
.PHONY: all run clean info check

# Regla por defecto: al escribir solo "mingw32-make" se construye el ejecutable.
all: $(TARGET)

# Enlazar: une todos los .o con las librerias de Allegro y crea main.exe.
# IMPORTANTE: las librerias (-l...) van DESPUES de los .o, si van antes MinGW
# no las encuentra y sale el error "undefined reference to al_...".
$(TARGET): $(OBJS)
	@echo Enlazando todo el proyecto para generar $(TARGET)...
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)
	@echo Compilacion exitosa!

# Compilar: convierte cada .cpp en su .o dentro de build/ (conservando subcarpetas).
#   $<  = el .cpp que se compila      $@ = el .o que se genera
#   $(@D) = carpeta donde va el .o (se crea si no existe; en cmd se usan "\")
$(OBJ_DIR)/%.o: %.cpp
	@if not exist "$(subst /,\,$(@D))" mkdir "$(subst /,\,$(@D))"
	@echo Compilando $<...
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar y ejecutar el juego.
run: all
	.\$(TARGET)

# Borrar todo lo compilado (util si algo queda raro o antes de entregar el proyecto).
clean:
	@echo Limpiando archivos compilados...
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(TARGET)" del /q "$(TARGET)"

# Muestra los .cpp detectados, para comprobar que tu clase nueva fue incluida.
info:
	@echo Compilador: $(CXX)
	@echo Archivos .cpp detectados:
	@for %%f in ($(subst /,\,$(SRCS))) do @echo    %%f

# Muestra el contenido de la carpeta de librerias de Allegro (para depurar errores
# de enlazado: aqui deben verse liballegro.dll.a, liballegro_font.dll.a, etc.).
check:
	@echo Librerias en $(ALLEGRO)/lib:
	@dir /b "$(subst /,\,$(ALLEGRO))\lib"

# Incluye las dependencias generadas (el "-" evita error la primera vez que no existen).
-include $(DEPS)