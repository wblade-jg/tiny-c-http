# tiny-c-http

Servidor HTTP minimalista en C con enrutado basado en árboles y soporte
de middlewares.

Soporta el método **GET**. Las rutas se registran con `addGet()` y se asocian
a funciones manejadoras definidas por el usuario.

## Compilar

    make

## Usar

    ./bin/servidor

## Backends de mapa

- **Treemap** — Red-Black Tree (balanceado)
- **RadixTree** — Árbol de prefijos para routing eficiente

Se seleccionan con `mapImplementation(builder, "Treemap")` o
`mapImplementation(builder, "RadixTree")` antes de crear el servidor.

## Middleware

Se registran con `addMiddleware("/ruta", handler)`. Se ejecutan en orden de
inserción para toda petición cuyo URI comience con el prefijo registrado.
Un middleware puede cortar la cadena asignando `req->isHandled = true`;
mientras sea `false`, el siguiente middleware en la lista se ejecuta.
Agotados los middlewares, se ejecuta el handler de ruta (si existe).

Ejemplo de middleware global que aplica a todas las rutas:

```c
addMiddleware("/", miMiddleware);
```

## Archivos estáticos

El middleware `static_router` del ejemplo (`main.c`) sirve archivos desde
el directorio `./public/`. Soporta múltiples extensiones (`.html`, `.css`,
`.js`, `.json`, `.png`, `.jpg`, `.svg`, `.ico`, etc.) con detección
automática del `Content-Type` a través del campo `res->contentType`.

```c
addMiddleware("/", static_router);
```

Si el archivo existe, se sirve con código 200; si no, responde 404.
Al marcar `req->isHandled = true` siempre, absorbe la petición y evita
que un handler de ruta se ejecute después.

## Ejemplo

```c
int main(){
    ServerBuilder* builder = createServerBuilder();
    port(builder, 8080);
    backlogSize(builder, 10);
    mapImplementation(builder, "Treemap");

    Server* server = createServer(builder);
    addMiddleware("/", static_router);
    addGet("/", hello);
    run(server);
}
```
