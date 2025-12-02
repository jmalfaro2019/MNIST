// input.cpp
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<float> read_image_as_mnist_vector(bool normalize01 = false) {
    std::string path;
    cv::Mat img;

    // Keep asking until a valid image is loaded
    while (true) {
        std::cout << "Introduce la ruta de la imagen a clasificar: ";
        if (!std::getline(std::cin, path)) {
            // Handle EOF or input error
            std::cerr << "Error leyendo la entrada. Intenta de nuevo.\n";
            continue;
        }
        if (path.empty()) {
            std::cout << "Ruta vacía. Intenta de nuevo.\n";
            continue;
        }

        img = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (img.empty()) {
            std::cout << "No se pudo abrir la imagen en: " << path << "\n";
            std::cout << "Verifica la ruta y vuelve a intentarlo.\n";
            continue;
        }
        break;
    }

    // Convert to grayscale if necessary
    cv::Mat gray;
    if (img.channels() == 3) {
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    } else if (img.channels() == 4) {
        cv::cvtColor(img, gray, cv::COLOR_BGRA2GRAY);
    } else {
        // already single channel
        gray = img;
    }

    // Resize to 28x28 (MNIST)
    cv::Mat mnist_img;
    cv::resize(gray, mnist_img, cv::Size(28, 28), 0, 0, cv::INTER_AREA);

    // Optionally show the image
    // Optionally show the image (scaled up for visibility)
cv::Mat display_img;
cv::resize(mnist_img, display_img, cv::Size(), 10.0, 10.0, cv::INTER_NEAREST);

cv::imshow("Imagen 28x28 (agrandada x10)", display_img);
std::cout << "Ventana con la imagen 28x28 (x10) mostrada. Presiona cualquier tecla en la ventana para continuar...\n";
cv::waitKey(0);
cv::destroyAllWindows();


    // Flatten to a vector row-major
    std::vector<float> vec;
    vec.reserve(28 * 28);
    for (int r = 0; r < mnist_img.rows; ++r) {
        const uchar* row_ptr = mnist_img.ptr<uchar>(r);
        for (int c = 0; c < mnist_img.cols; ++c) {
            float v = static_cast<float>(row_ptr[c]);
            if (normalize01) {
                v /= 255.0f;
            }
            vec.push_back(v);
        }
    }

    // Log and print length (and a small preview)
    const std::string log_file = "input.log";
    std::ofstream log(log_file, std::ios::app);
    if (log.is_open()) {
        log << "Image loaded: length = " << vec.size() << "\n";
    }
    std::cout << "Vector length: " << vec.size() << "\n";
    std::cout << "First 10 values: ";
    for (size_t i = 0; i < std::min<size_t>(10, vec.size()); ++i) {
        std::cout << vec[i] << (i + 1 < 10 ? ", " : "\n");
    }

    return vec;
}
