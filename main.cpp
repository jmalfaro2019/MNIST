// main.cpp
#include <iostream>
#include <vector>

// declara la función extern
std::vector<float> read_image_as_mnist_vector(bool normalize01 = false);

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
  // Aquí iría la llamada al clasificador usando 'v'

  return 0;
}
