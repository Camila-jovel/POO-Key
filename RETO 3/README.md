Pequeño sistema de gestión de biblioteca escolar implementado en C++ usando:

struct para tablas (Autor, Libro, Estudiante, Prestamo)
vector<> como “tablas” en memoria
Archivos .txt (formato CSV simple) para persistencia
Menú por entidad con operaciones CRUD (Create, Read, Update, Delete)

Para compilar: 
En windows usar(PowerShell): g++ -std=c++17 -O2 -o biblioteca.exe main.cpp
Para ejecutar (siempre en Windows): 
.\biblioteca.exe

¿Cómo usarlo? 
Al inicio se verá el menú principal: 
===== BIBLIOTECA =====
1) Autores
2) Estudiantes
3) Prestamos
4) Libros
9) Guardar
10) Cargar
11) Ver Base de Datos
0) Salir
Opcion:

Dentro de Cada uno hay opciones para asegurar lo del CRUD: 
1) Crear
2) Listar
3) Actualizar
4) Eliminar
0) Volver (para regresar al menú principal)

Acciones globales:
9) Guardar: escribe todos los vectores a los .txt.
10) Cargar: carga desde los .txt a memoria (sobrescribe lo actual en memoria).
11) Ver Base de Datos: imprime todo (autores, libros, estudiantes, prestamos) con “joins” por nombre.
Al salir con 0), el programa también guarda automáticamente.
