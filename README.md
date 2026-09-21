# Pacman

## Denumirea proiectului
**Pacman** — recreare a jocului arcade clasic Pacman, implementată în C++.

## Descrierea proiectului / regulile jocului
Jucătorul controlează personajul Pacman, care se deplasează într-un labirint format din pereți, coridoare și puncte (pellets). Scopul este de a colecta toate punctele de pe hartă, evitând în același timp fantomele (ghosts) care patrulează labirintul.

Reguli de bază:
- Pacman se deplasează pe grilă în 4 direcții: sus, jos, stânga, dreapta.
- Colectarea unui punct obișnuit (`pellet`) adaugă puncte la scor.
- Colectarea unui punct mare (`power pellet`) face fantomele vulnerabile pentru o perioadă limitată de timp — Pacman le poate "mânca", câștigând puncte suplimentare.
- Dacă o fantomă atinge Pacman în stare normală (nevulnerabilă), jucătorul pierde o viață.
- Jocul se termină cu victorie când toate punctele au fost colectate, sau cu înfrângere când jucătorul rămâne fără vieți.
- Fantomele au comportamente diferite de urmărire (agresiv, aleatoriu, ambuscadă etc.), în funcție de implementare.

## Structuri de date și descrierea lor

| Structură | Descriere |
|---|---|
| `Vector2i` | Reprezintă o poziție/coordonată discretă pe grilă (`x`, `y`). |
| `Direction` (enum) | Direcțiile posibile de mișcare: `Up`, `Down`, `Left`, `Right`, `None`. |
| `CellType` (enum) | Tipul unei celule din labirint: `Wall`, `Empty`, `Pellet`, `PowerPellet`, `GhostHouse`. |
| `GameState` (enum) | Starea curentă a jocului: `Menu`, `Playing`, `Paused`, `Won`, `Lost`. |
| `GhostMode` (enum) | Comportamentul curent al unei fantome: `Chase`, `Scatter`, `Frightened`, `Eaten`. |
| `Entity` | Structură de bază pentru un obiect din joc: poziție, direcție curentă, viteză. |
| `Player` | Extinde `Entity`; conține scor, număr de vieți, stare (normal/power-up activ). |
| `Ghost` | Extinde `Entity`; conține modul curent (`GhostMode`), culoare/tip, poziție "acasă". |
| `Grid` / `Maze` | Matrice bidimensională de `CellType`, reprezentând labirintul. |
| `GameEngine` | Clasa centrală ce gestionează starea jocului, actualizarea entităților și logica regulilor. |
| `Renderer` | Componenta responsabilă de desenarea grilei, a entităților și a interfeței (scor, vieți). |
| `Listener` | Componenta responsabilă de captarea input-ului de la utilizator (tastatură) și traducerea lui în comenzi de joc. |

## Fișiere principale
- `Types.hpp` — tipuri și structuri de date comune, folosite de toate celelalte module.
- `GameEngine.hpp` — motorul de joc: bucla principală, logica de actualizare, gestionarea stării.
- `Renderer.hpp` — desenarea/afișarea stării jocului.
- `Listener.hpp` — captarea și procesarea input-ului utilizatorului.

## Compilare
```bash
g++ -std=c++17 main.cpp -o pacman
```