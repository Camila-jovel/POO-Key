#include <bits/stdc++.h>
using namespace std;
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

/*** ======== MODELO ======== ***/
struct Autor {
    int id; string nombre; string nacionalidad;
};
struct Libro {
    int id; string titulo; string isbn; int anio; int id_autor; // FK Autor.id
};
struct Estudiante {
    int id; string nombre; string grado;
};
struct Prestamo {
    int id; int id_libro; int id_estudiante;
    string fecha_prestamo;   // "YYYY-MM-DD"
    string fecha_devolucion; // "" si activo
};

/***  DB EN MEMORIA + PERSISTENCIA  ***/
struct DB {
    vector<Autor> autores;
    vector<Libro> libros;
    vector<Estudiante> estudiantes;
    vector<Prestamo> prestamos;

    // --- utilidades CSV (muy simple: escapa , y \) ---
    static string esc(const string& s){ //esc() escapa texto antes de guardarlo en archivo
        string r; r.reserve(s.size());
        for(char c: s){ if(c=='\\') r+="\\\\"; else if(c==',') r+="\\,"; else r+=c; }
        return r;
    }
    static string unesc(const string& s){//unesc() recuperar texto original al leer
        string r; r.reserve(s.size());
        for(size_t i=0;i<s.size();++i){
            if(s[i]=='\\' && i+1<s.size() && (s[i+1]=='\\' || s[i+1]==',')){ r+=s[i+1]; ++i; }
            else r+=s[i];
        }
        return r;
    }
    static vector<string> splitCSV(const string& s){ //splitCSV convierte un línea CSV en un vector de strings
        vector<string> out; string cur; bool slash=false;
        for(char c: s){
            if(slash){ cur.push_back(c); slash=false; }
            else if(c=='\\'){ slash=true; }
            else if(c==','){ out.push_back(unesc(cur)); cur.clear(); }
            else cur.push_back(c);
        }
        out.push_back(unesc(cur));
        return out;
    }
    static string joinCSV(initializer_list<string> v){ //join convierte un vector ded strings en una línea CSV lista para escribir
        string r; bool first=true;
        for(const auto& x: v){ if(!first) r.push_back(','); first=false; r+=esc(x); }
        return r;
    }

    // --- cargar/guardar ---
    bool cargar(){ //Antes de cargar se limpian los vectores para evitar que los datos se dupliquen
        autores.clear(); libros.clear(); estudiantes.clear(); prestamos.clear();
        // autores - lo abre y lee linea por linea
        ifstream fa("autores.txt"); string line;
        while(getline(fa,line)){ auto v=splitCSV(line); if(v.size()<3) continue;
            autores.push_back({stoi(v[0]), v[1], v[2]}); }
        // libros
        ifstream fl("libros.txt");
        while(getline(fl,line)){ auto v=splitCSV(line); if(v.size()<5) continue;
            libros.push_back({stoi(v[0]), v[1], v[2], stoi(v[3]), stoi(v[4])}); }
        // estudiantes
        ifstream fe("estudiantes.txt");
        while(getline(fe,line)){ auto v=splitCSV(line); if(v.size()<3) continue;
            estudiantes.push_back({stoi(v[0]), v[1], v[2]}); }
        // prestamos
        ifstream fp("prestamos.txt");
        while(getline(fp,line)){ auto v=splitCSV(line); if(v.size()<5) continue;
            prestamos.push_back({stoi(v[0]), stoi(v[1]), stoi(v[2]), v[3], v[4]}); }
        return true;
    }
    bool guardar(){ //recorre todos los autores y los escribe como una línea CSV escapada (joinCSV)
        ofstream fa("autores.txt", ios::trunc);
        for(auto&a:autores) fa<<joinCSV({to_string(a.id),a.nombre,a.nacionalidad})<<"\n";
        ofstream fl("libros.txt", ios::trunc);
        for(auto&l:libros) fl<<joinCSV({to_string(l.id),l.titulo,l.isbn,to_string(l.anio),to_string(l.id_autor)})<<"\n";
        ofstream fe("estudiantes.txt", ios::trunc);
        for(auto&e:estudiantes) fe<<joinCSV({to_string(e.id),e.nombre,e.grado})<<"\n";
        ofstream fp("prestamos.txt", ios::trunc);
        for(auto&p:prestamos) fp<<joinCSV({to_string(p.id),to_string(p.id_libro),to_string(p.id_estudiante),p.fecha_prestamo,p.fecha_devolucion})<<"\n";
        return true;
    }

    // --- validaciones (IDs, FKs, reglas) ---
    bool existeAutor(int id) const { for(auto&a:autores) if(a.id==id) return true; return false; }
    bool existeLibro(int id) const { for(auto&l:libros) if(l.id==id) return true; return false; }
    bool existeEst(int id)  const { for(auto&e:estudiantes) if(e.id==id) return true; return false; }
    bool isbnUsado(const string& isbn,int except=-1) const {
        for(auto&l:libros) if(l.isbn==isbn && l.id!=except) return true; return false; }
    bool prestamoActivoDeLibro(int id_libro) const {
        for(auto&p:prestamos) if(p.id_libro==id_libro && p.fecha_devolucion.empty()) return true; return false; }
    bool idPrestamoExiste(int id) const { for(auto&p:prestamos) if(p.id==id) return true; return false; }

    // --- CRUD Autor ---
    bool addAutor(const Autor& a, string& err){
        if(existeAutor(a.id)){ err="ID de autor duplicado."; return false; }
        autores.push_back(a); return true;
    }
    // --- CRUD Libro ---
    bool addLibro(const Libro& l, string& err){
        if(existeLibro(l.id)){ err="ID de libro duplicado."; return false; }
        if(!existeAutor(l.id_autor)){ err="Autor inexistente (FK)."; return false; }
        if(isbnUsado(l.isbn)){ err="ISBN duplicado."; return false; }
        libros.push_back(l); return true;
    }
    // --- CRUD Estudiante ---
    bool addEst(const Estudiante& e, string& err){
        if(existeEst(e.id)){ err="ID de estudiante duplicado."; return false; }
        estudiantes.push_back(e); return true;
    }
    // --- Prestamos ---
    bool crearPrestamo(const Prestamo& p, string& err){
        if(idPrestamoExiste(p.id)){ err="ID de prestamo duplicado."; return false; }
        if(!existeLibro(p.id_libro)){ err="Libro inexistente (FK)."; return false; }
        if(!existeEst(p.id_estudiante)){ err="Estudiante inexistente (FK)."; return false; }
        if(prestamoActivoDeLibro(p.id_libro)){ err="El libro ya esta prestado (activo)."; return false; }
        prestamos.push_back(p); return true;
    }
    bool devolver(int id_prestamo, const string& fecha, string& err){
        for(auto& x: prestamos){
            if(x.id==id_prestamo){
                if(!x.fecha_devolucion.empty()){ err="Ese prestamo ya fue devuelto."; return false; }
                x.fecha_devolucion=fecha; return true;
            }
        }
        err="Prestamo no encontrado."; return false;
    }

    // ===== CRUD Autor (UPDATE/DELETE) =====
    bool updAutor(const Autor& a, string& err){
        for (auto& x : autores) if (x.id == a.id) { x = a; return true; }
        err = "Autor no encontrado."; return false;
    }
    bool delAutor(int id, string& err){
        // regla: no eliminar si hay libros que referencian al autor
        for (auto& l : libros) if (l.id_autor == id) {
            err = "No se puede eliminar: autor con libros asociados."; return false;
        }
        auto it = remove_if(autores.begin(), autores.end(), [&](const Autor& a){ return a.id==id; });
        if (it == autores.end()) { err="Autor no encontrado."; return false; }
        autores.erase(it, autores.end()); return true;
    }

    // ===== CRUD Libro (UPDATE/DELETE) =====
    bool updLibro(const Libro& l, string& err){//Actualizar el libro existente por ID
        for (auto& x : libros) if (x.id == l.id) {
            //Regla de integridad: el autor debe existir (FK)
            if (!existeAutor(l.id_autor)) { err="Autor inexistente (FK)."; return false; }
            // Unicidad de ISBN: no puede estar repetido en otro libro
            if (isbnUsado(l.isbn, l.id))   { err="ISBN duplicado.";     return false; }
            x = l; return true; 
        }
         // Si no encontró el ID
        err = "Libro no encontrado."; return false;
    }
    // Eliminar libro por ID
    bool delLibro(int id, string& err){
        // regla: no eliminar si hay prestamos que lo referencian
        for (auto& p : prestamos) if (p.id_libro == id) {
            err = "No se puede eliminar: existen prestamos con este libro."; return false;
        }
        auto it = remove_if(libros.begin(), libros.end(), [&](const Libro& l){ return l.id==id; });
        if (it == libros.end()) { err="Libro no encontrado."; return false; }
        libros.erase(it, libros.end()); return true;
    }

    // ===== CRUD Estudiante (UPDATE/DELETE) =====
    bool updEst(const Estudiante& e, string& err){
        for (auto& x : estudiantes) if (x.id == e.id) { x = e; return true; }
        err = "Estudiante no encontrado."; return false;
    }
    bool delEst(int id, string& err){
        // regla: no eliminar si tiene prestamos
        for (auto& p : prestamos) if (p.id_estudiante == id) {
            err = "No se puede eliminar: existen prestamos del estudiante."; return false;
        }
        auto it = remove_if(estudiantes.begin(), estudiantes.end(), [&](const Estudiante& e){ return e.id==id; });
        if (it == estudiantes.end()) { err="Estudiante no encontrado."; return false; }
        estudiantes.erase(it, estudiantes.end()); return true;
    }

    // ===== DELETE Prestamo =====
    bool delPrestamo(int id, string& err){
        auto it = remove_if(prestamos.begin(), prestamos.end(), [&](const Prestamo& p){ return p.id==id; });
        if (it == prestamos.end()) { err="Prestamo no encontrado."; return false; }
        prestamos.erase(it, prestamos.end()); return true;
    }

    // --- consultas (joins via bucles) ---
    //“Mini-joins” que retornan punteros al registro asociado
    const Autor* getAutor(int id) const { for(auto&a:autores) if(a.id==id) return &a; return nullptr; }
    const Libro* getLibro(int id) const { for(auto&l:libros) if(l.id==id) return &l; return nullptr; }
    const Estudiante* getEst(int id) const { for(auto&e:estudiantes) if(e.id==id) return &e; return nullptr; }

    vector<Prestamo> porEstudiante(int id_est) const { //Consultas que filtran la tabla de Prestamos por criterios comunes.
        vector<Prestamo> r; for(auto&p:prestamos) if(p.id_estudiante==id_est) r.push_back(p); return r;
    }
    vector<Prestamo> activos() const {
        vector<Prestamo> r; for(auto&p:prestamos) if(p.fecha_devolucion.empty()) r.push_back(p); return r;
    }
};

/*** ======== IO DE CONSOLA ======== ***/
int leerInt(const string& msg){
    cout<<msg; int x;
    while(!(cin>>x)){ cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); cout<<"Invalido. Intenta de nuevo: "; }
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); return x;
}
string leerStr(const string& msg){ cout<<msg; string s; getline(cin,s); return s; }

/* ================= SUBMENUS CRUD ================= */

// -------- Autores: Create, Read, Update, Delete --------
//Submenús por entidad que implementan CRUD y, en el caso de Prestamos y Libros, muestran joins
void menuAutores(DB& db) {
    cout << "\n[Autores] 1) Crear  2) Listar  3) Actualizar  4) Eliminar  0) Volver\n";
    int op = leerInt("Opcion: ");
    string err;
    if (op==1) { // CREATE
        Autor a;
        a.id = leerInt("ID Autor: ");
        a.nombre = leerStr("Nombre: ");
        a.nacionalidad = leerStr("Nacionalidad: ");
        if (!db.addAutor(a,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Autor agregado.\n";
    } else if (op==2) { // READ (listar)
        cout << "\n-- AUTORES --\n";
        for (const auto& a : db.autores)
            cout << a.id << " | " << a.nombre << " | " << a.nacionalidad << "\n";
    } else if (op==3) { // UPDATE
        Autor a;
        a.id = leerInt("ID Autor (existente): ");
        a.nombre = leerStr("Nuevo nombre: ");
        a.nacionalidad = leerStr("Nueva nacionalidad: ");
        if (!db.updAutor(a,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Autor actualizado.\n";
    } else if (op==4) { // DELETE
        int id = leerInt("ID Autor a eliminar: ");
        if (!db.delAutor(id,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Autor eliminado.\n";
    }
}

// -------- Estudiantes: Create, Read, Update, Delete --------
void menuEstudiantes(DB& db) {
    cout << "\n[Estudiantes] 1) Crear  2) Listar  3) Actualizar  4) Eliminar  0) Volver\n";
    int op = leerInt("Opcion: ");
    string err;
    if (op==1) { // CREATE
        Estudiante e;
        e.id = leerInt("ID Estudiante: ");
        e.nombre = leerStr("Nombre: ");
        e.grado  = leerStr("Grado: ");
        if (!db.addEst(e,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Estudiante agregado.\n";
    } else if (op==2) { // READ
        cout << "\n-- ESTUDIANTES --\n";
        for (const auto& e : db.estudiantes)
            cout << e.id << " | " << e.nombre << " | " << e.grado << "\n";
    } else if (op==3) { // UPDATE
        Estudiante e;
        e.id = leerInt("ID Estudiante (existente): ");
        e.nombre = leerStr("Nuevo nombre: ");
        e.grado  = leerStr("Nuevo grado: ");
        if (!db.updEst(e,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Estudiante actualizado.\n";
    } else if (op==4) { // DELETE
        int id = leerInt("ID Estudiante a eliminar: ");
        if (!db.delEst(id,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Estudiante eliminado.\n";
    }
}

// -------- Prestamos: Create, Read, Update(Devolver), Delete --------
void menuPrestamos(DB& db) {
    cout << "\n[Prestamos] 1) Crear  2) Listar  3) Devolver (Update)  4) Eliminar  0) Volver\n";
    int op = leerInt("Opcion: ");
    string err;
    if (op==1) { // CREATE
        Prestamo p;
        p.id = leerInt("ID Prestamo: ");
        p.id_libro = leerInt("ID Libro: ");
        p.id_estudiante = leerInt("ID Estudiante: ");
        p.fecha_prestamo = leerStr("Fecha prestamo (YYYY-MM-DD): ");
        p.fecha_devolucion = ""; // activo
        if (!db.crearPrestamo(p,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Prestamo creado.\n";
    } else if (op==2) { // READ (listar con join libro/est.)
        cout << "\n-- PRESTAMOS --\n";
        for (const auto& p : db.prestamos) {
            const Libro* l = db.getLibro(p.id_libro);
            const Estudiante* e = db.getEst(p.id_estudiante);
            cout << p.id
                 << " | Libro:" << (l?l->titulo:"?")
                 << " | Estudiante:" << (e?e->nombre:"?")
                 << " | Prestado:" << p.fecha_prestamo
                 << " | Devuelto:" << (p.fecha_devolucion.empty()?"(activo)":p.fecha_devolucion)
                 << "\n";
        }
    } else if (op==3) { // UPDATE (devolver)
        int idp = leerInt("ID Prestamo: ");
        string f = leerStr("Fecha devolucion (YYYY-MM-DD): ");
        if (!db.devolver(idp,f,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Prestamo devuelto.\n";
    } else if (op==4) { // DELETE
        int id = leerInt("ID Prestamo a eliminar: ");
        if (!db.delPrestamo(id,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Prestamo eliminado.\n";
    }
}

// -------- Libros: Create, Read, Update, Delete --------
void menuLibros(DB& db) {
    cout << "\n[Libros] 1) Crear  2) Listar  3) Actualizar  4) Eliminar  0) Volver\n";
    int op = leerInt("Opcion: ");
    string err;
    if (op==1) { // CREATE
        Libro l;
        l.id = leerInt("ID Libro: ");
        l.titulo = leerStr("Titulo: ");
        l.isbn = leerStr("ISBN: ");
        l.anio = leerInt("Anio de publicacion: ");
        l.id_autor = leerInt("ID Autor: ");
        if (!db.addLibro(l,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Libro agregado.\n";
    } else if (op==2) { // READ (listar con autor)
        cout << "\n-- LIBROS --\n";
        for (const auto& l : db.libros) {
            const Autor* a = db.getAutor(l.id_autor);
            cout << l.id << " | " << l.titulo
                 << " | ISBN:" << l.isbn
                 << " | Anio:" << l.anio
                 << " | Autor:" << (a? a->nombre : "(desconocido)") << "\n";
        }
    } else if (op==3) { // UPDATE
        Libro l;
        l.id = leerInt("ID Libro (existente): ");
        l.titulo = leerStr("Nuevo titulo: ");
        l.isbn = leerStr("Nuevo ISBN: ");
        l.anio = leerInt("Nuevo anio de publicacion: ");
        l.id_autor = leerInt("Nuevo ID Autor: ");
        if (!db.updLibro(l,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Libro actualizado.\n";
    } else if (op==4) { // DELETE
        int id = leerInt("ID Libro a eliminar: ");
        if (!db.delLibro(id,err)) cout<<"Error: "<<err<<"\n"; else cout<<"Libro eliminado.\n";
    }
}

// -------- VER TODA LA BASE DE DATOS --------
void verBaseDeDatos(DB& db) {
    cout << "\n================= BASE DE DATOS =================\n";

    cout << "\n[AUTORES]\n";
    if (db.autores.empty()) cout << "(vacio)\n";
    for (const auto& a : db.autores)
        cout << a.id << " | " << a.nombre << " | " << a.nacionalidad << "\n";

    cout << "\n[LIBROS]\n";
    if (db.libros.empty()) cout << "(vacio)\n";
    for (const auto& l : db.libros) {
        const Autor* a = db.getAutor(l.id_autor);
        cout << l.id << " | " << l.titulo << " | " << l.isbn
             << " | Anio:" << l.anio
             << " | Autor:" << (a? a->nombre : "(desconocido)") << "\n";
    }

    cout << "\n[ESTUDIANTES]\n";
    if (db.estudiantes.empty()) cout << "(vacio)\n";
    for (const auto& e : db.estudiantes)
        cout << e.id << " | " << e.nombre << " | " << e.grado << "\n";

    cout << "\n[PRESTAMOS]\n";
    if (db.prestamos.empty()) cout << "(vacio)\n";
    for (const auto& p : db.prestamos) {
        const Libro* l = db.getLibro(p.id_libro);
        const Estudiante* e = db.getEst(p.id_estudiante);
        cout << p.id
             << " | Libro:" << (l? l->titulo : "?")
             << " | Estudiante:" << (e? e->nombre : "?")
             << " | Prestado:" << p.fecha_prestamo
             << " | Devuelto:" << (p.fecha_devolucion.empty() ? "(activo)" : p.fecha_devolucion)
             << "\n";
    }
    cout << "=================================================\n";
}

void menuPrincipal(){
    cout << "\n===== BIBLIOTECA =====\n"
         << "1) Autores\n"
         << "2) Estudiantes\n"
         << "3) Prestamos\n"
         << "4) Libros\n"
         << "9) Guardar\n"
         << "10) Cargar\n"
         << "11) Ver Base de Datos\n"
         << "0) Salir\n"
         << "Opcion: ";
}

int main(){
    ios::sync_with_stdio(false);
    

    DB db;
    db.cargar(); // carga si existen .txt

    int op;
    while(true){
        menuPrincipal();
        if(!(cin>>op)){ cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        if(op==0){ db.guardar(); cout<<"Datos guardados. Adios!\n"; break; }
        else if(op==1) menuAutores(db);
        else if(op==2) menuEstudiantes(db);
        else if(op==3) menuPrestamos(db);
        else if(op==4) menuLibros(db);
        else if(op==9){ db.guardar(); cout<<"Guardado.\n"; }
        else if(op==10){ db.cargar(); cout<<"Cargado.\n"; }
        else if(op==11){ verBaseDeDatos(db); }
        else cout << "Opcion invalida.\n";
    }
    return 0;
}


