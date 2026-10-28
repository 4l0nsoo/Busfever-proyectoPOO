# Bus Fever Party (versión en consola)

Abstracción del juego **Bus Fever Party** en C++ con interfaz ASCII.
El usuario ve el tablero impreso y escribe comandos de números enteros asociados al ID de un bus.

## Documentación

| Archivo | Qué es |
|---|---|
| `docs/arquitectura.puml` | Diagrama de clases |
| `docs/secuencia.puml` | Diagrama de secuencia (un turno de juego) |
| `docs/flujo.puml` | Diagrama de actividades / flujo clásico |

Para verlos: extensión **PlantUML** en VS Code (`Alt+D` para la vista previa).

## Código

```
src/        clases (.h y .cpp) y main.cpp\
```

## Clases

- **Bus**: tiene `id`, color, capacidad, dirección y posición (x, y). Su largo se calcula desde la capacidad.
- **Passager**: grupo de pasajeros del mismo color. `split()` crea un grupo más pequeño.
- **Queue**: cola de grupos que abastece a la sala de espera.
- **WaitingRoom**: sala de espera (cupo limitado) que empareja grupos con buses por color.
- **Parking**: lugares donde se estacionan los buses que salieron del tráfico.
- **Traffic**: tablero de buses. Decide si un bus puede salir (`canExit`).
- **Controller**: interpreta el comando del usuario y llama a las demás clases.
- **Console**: imprime el tablero en ASCII y pide la entrada.

## Convenciones

- `Traffic.grid` es una matriz con `grid[y][x]`. Cada celda guarda el **color** del bus (0 = libre).
- Ejes: `y+1` = arriba, `x+1` = derecha, `y-1` = abajo, `x-1` = izquierda.
- Los getters y setters triviales **no** se dibujan en los diagramas (se asumen).
- Atributos privados; públicos solo los métodos que otras clases necesitan.

## Pendientes de diseño

- [ ] Completar tipos: `<vector>` → `vector<Bus>`, `vector<Passager>`, etc.
- [ ] Qué pasa cuando el bus está bloqueado: ¿termina el turno o se pide otro comando?
- [ ] Qué pasa cuando el parking está lleno (¿derrota?).
- [ ] Qué devuelve `getBus` si el id no existe.
- [ ] Condiciones de ganar y perder.

## Cómo colaborar

1. Crea una rama por tarea: `git checkout -b feature/nombre-de-la-clase`
2. Si cambias el diseño, actualiza también el `.puml` correspondiente.
3. Haz commits pequeños y abre un pull request para revisión.
