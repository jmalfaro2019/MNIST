// clasifier.cpp
#include <fstream>
#include <iostream>
#include <onnxruntime_cxx_api.h> // CAMBIO: añadido include de ONNX Runtime
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// CAMBIO: cambiar el tipo del segundo valor del pair a float (para devolver el
// logit máximo)
std::pair<int, float> clasifier(const std::vector<float> &v) {

  // Verificar tamaño esperado (1x1x28x28 = 784)
  const size_t expected_size = 1 * 1 * 28 * 28;
  if (v.size() != expected_size) {
    std::cerr << "Error: el vector de entrada debe tener tamaño "
              << expected_size << " pero tiene " << v.size() << std::endl;
    return std::make_pair(-1, -1.0f);
  }

  try {
    // 1. Configuración del Entorno
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "MinimAI_Test");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    // 2. Cargar el Modelo (.onnx)
    // En Windows, ONNX Runtime requiere wchar_t*
    const wchar_t *model_path = L"mnist_cnn.onnx";

    // Crear la sesión (lanza excepción si no encuentra el modelo)
    Ort::Session session(env, model_path, session_options);

    // MNIST es 1x1x28x28 = 784 float32
    const int64_t batch_size = 1;
    const int64_t channels = 1;
    const int64_t height = 28;
    const int64_t width = 28;
    const int64_t input_tensor_size = batch_size * channels * height * width;

    std::vector<int64_t> input_node_dims = {batch_size, channels, height,
                                            width};

    // Crear el tensor de ONNX en memoria
    auto memory_info =
        Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    // CAMBIO: usar los datos del vector 'v' (v.data()) en lugar de
    // input_tensor_values inexistente
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info, const_cast<float *>(v.data()), input_tensor_size,
        input_node_dims.data(),
        input_node_dims.size()); // CAMBIO: añadido const_cast para CreateTensor

    // Nombres de los nodos (tal cual los definimos en Python)
    const char *input_names[] = {"input"};
    const char *output_names[] = {"output"};

    // 4. EJECUTAR INFERENCIA (Run)
    std::cout << "Ejecutando inferencia..." << std::endl;

    auto output_tensors =
        session.Run(Ort::RunOptions{nullptr}, input_names, &input_tensor,
                    1, // Número de inputs
                    output_names,
                    1 // Número de outputs
        );

    // 5. Procesar Salida
    // Obtenemos el puntero a los datos flotantes de salida (los logits)
    float *floatarr = output_tensors.front().GetTensorMutableData<float>();

    // Buscamos cuál es el valor más alto (Argmax)
    // La salida son 10 valores (probabilidad para 0, 1, 2... 9)
    int prediccion = 0;
    float max_val = floatarr[0];

    std::cout << "Resultados (Logits): [ ";
    for (int i = 0; i < 10; i++) {
      std::cout << floatarr[i] << " ";
      if (floatarr[i] > max_val) {
        max_val = floatarr[i];
        prediccion = i;
      }
    }
    std::cout << "]" << std::endl;

    return std::make_pair(prediccion, max_val);
  } catch (const Ort::Exception &exception) {
    std::cerr << "ERROR ONNX: " << exception.what() << std::endl;
    return std::make_pair(-1, -1.0f);
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return std::make_pair(-1, -1.0f);
  }
}
