#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <omp.h>

namespace py = pybind11;

py::array_t<double> kernel(py::array_t<double> a, py::array_t<double> b) {
    auto M = a.shape(0);
    auto N = a.shape(1);
    auto c = py::array_t<double>(M);
    auto a_ptr = a.mutable_data();
    auto b_ptr = b.mutable_data();
    auto c_ptr = c.mutable_data();

    #pragma omp parallel for num_threads(64)
    for (int i = 0; i < M; i++) {
        double sum = 0;
        for (int j = 0; j < N; j++) {
            sum += a_ptr[i * N + j] * b_ptr[j];
        }
        c_ptr[i] = sum;
    }

    return c;
}

PYBIND11_MODULE(mv, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("kernel", &kernel, "A function that adds two numbers");
}