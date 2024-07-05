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

    #pragma omp parallel for num_threads(16)
    for (int i = 0; i < M; i+=4) {
        double c0 = 0;
        double c1 = 0;
        double c2 = 0;
        double c3 = 0;
        for (int j = 0; j < N; j+=4) {
            double b0 = b_ptr[j];
            double b1 = b_ptr[j+1];
            double b2 = b_ptr[j+2];
            double b3 = b_ptr[j+3];

            c0 += a_ptr[i * N + j] * b0;
            c0 += a_ptr[i * N + j+1] * b1;
            c0 += a_ptr[i * N + j+2] * b2;
            c0 += a_ptr[i * N + j+3] * b3;

            c1 += a_ptr[(i+1) * N + j] * b0;
            c1 += a_ptr[(i+1) * N + j+1] * b1;
            c1 += a_ptr[(i+1) * N + j+2] * b2;
            c1 += a_ptr[(i+1) * N + j+3] * b3;

            c2 += a_ptr[(i+2) * N + j] * b0;
            c2 += a_ptr[(i+2) * N + j+1] * b1;
            c2 += a_ptr[(i+2) * N + j+2] * b2;
            c2 += a_ptr[(i+2) * N + j+3] * b3;

            c3 += a_ptr[(i+3) * N + j] * b0;
            c3 += a_ptr[(i+3) * N + j+1] * b1;
            c3 += a_ptr[(i+3) * N + j+2] * b2;
            c3 += a_ptr[(i+3) * N + j+3] * b3;
        }
        c_ptr[i] = c0;
        c_ptr[i+1] = c1;
        c_ptr[i+2] = c2;
        c_ptr[i+3] = c3;
    }

    return c;
}

PYBIND11_MODULE(mv, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("kernel", &kernel, "A function that adds two numbers");
}