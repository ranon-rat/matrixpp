#include "matrixpp.h++"
#include <memory>

Matrix::Matrix(size_t h, size_t w) : height(h), width(w)
{
    this->data = std::make_unique<float[]>(h * w);
    std::fill_n(this->data.get(), h * w, 0);
}

Matrix::Matrix(const Matrix &other) : height(other.height), width(other.width)
{
    this->data = std::make_unique<float[]>(other.height * other.width);
    for (size_t i = 0; i < height * width; i++)
    {

        this->data[i] = other.data[i]; // Copia profunda
    }
}

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {

        DELETE_DATA_MATRIXPP(this);
        // Copiar nuevas dimensiones
        this->height = other.height;
        this->width = other.width;
        this->data = std::make_unique<float[]>(this->height * this->width);
        for (size_t i = 0; i < this->height * this->width; i++)
        {

            this->data[i] = other.data[i]; // Copia profunda
        }
    }

    return *this;
}
Matrix Matrix::randomized(size_t h, size_t w)
{
    Matrix m(h, w);
    m.fill_rand();
    return m;
}