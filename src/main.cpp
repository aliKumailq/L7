#include <iostream>

#include "entity_manager.h"

int main() {
    EntityManager<int, float, bool> entityManager;

    EntityID e = entityManager.createEntity();
    entityManager.addComponent(e, 5); // template argument deduction can tell this is addComponent<int>
    std::cout << entityManager.hasComponent<int>(e) << '\n';   // prints '1'
    std::cout << entityManager.hasComponent<float>(e) << '\n'; // prints '0'
    std::cout << entityManager.hasComponent<bool>(e) << '\n';  // prints '0'
    entityManager.addComponent<bool>(e, false); // template argument deduction can tell this is addComponent<int>
    std::cout << entityManager.getComponent<bool>(e) << '\n'; // prints 0
    std::cout << (entityManager.getComponent<bool>(e) = true) << '\n'; // prints true
    std::cout << entityManager.getComponent<int>(e) << '\n';     // prints '5'
    std::cout << (entityManager.getComponent<int>(e) = 7) << '\n';     // prints '7'
    return 0;
}
