# tiny-c-http

Servidor HTTP minimalista en C con enrutado basado en árboles.

Actualmente solo soporta el método **GET** y ejecuta lógica de negocio
definida en el router.

## Compilar

    make

## Usar

    ./bin/servidor

## Backends de mapa

- **Treemap** — Red-Black Tree (balanceado)
- **RadixTree** — Árbol de prefijos para routing eficiente
