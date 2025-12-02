// main.cpp
#include <iostream>
#include <vector>

// declara la función extern
std::vector<float> read_image_as_mnist_vector(bool normalize01 = false);
std::pair<int, float> clasifier(const std::vector<float> &v);

int main() {
  std::cout
      << "Programa de prueba para leer imagen y convertir a vector MNIST\n";

  // Llamar a la función de input
  std::vector<float> v = read_image_as_mnist_vector(false); // o true para [0,1]

  // Comprobación rápida
  if (v.size() != 28 * 28) {
    std::cerr << "Error: vector con tamaño inesperado: " << v.size() << "\n";
    return 1;
  }

  std::cout << "Vector recibido correctamente. Listo para clasificar.\n";

  // Llamar al clasificador
  std::pair<int, float> resultado = clasifier(v);

  if (resultado.first == -1) {
    std::cerr << "Error en la clasificación.\n";
    return 1;
  }

  std::cout << "\n=== RESULTADO ===\n";
  std::cout << "Dígito predicho: " << resultado.first << "\n";
  std::cout << "logit máximo: " << resultado.second << "\n";

  return 0;
}
