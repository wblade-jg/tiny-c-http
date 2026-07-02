# tiny-c-http

Servidor HTTP minimalista en C con enrutado basado en árboles.

Actualmente solo soporta el método **GET** con enrutado basado en árboles.
Las rutas se registran con `addGet()` y se asocian a funciones manejadoras
definidas por el usuario.

## Compilar

    make

## Usar

    ./bin/servidor

## Backends de mapa

- **Treemap** — Red-Black Tree (balanceado)
- **RadixTree** — Árbol de prefijos para routing eficiente
