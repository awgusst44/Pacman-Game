#include "GameEngine.hpp"
#include "Listener.hpp"
#include "Renderer.hpp"

int main() {
    pacman::GameEngine engine;
    pacman::Renderer renderer;
    pacman::Listener listener;

    engine.initialize();
    renderer.initialize();

    // Joc pe ture: fiecare comanda introdusa = un pas de simulare (1 secunda).
    while (engine.isRunning()) {
        renderer.render(engine);

        listener.poll();
        if (listener.wasQuitRequested()) {
            break;
        }

        engine.handleInput(listener.getRequestedDirection());
        engine.update(1.0f);
    }

    renderer.render(engine); // ultimul cadru (Won / Lost)
    renderer.shutdown();
    return 0;
}
