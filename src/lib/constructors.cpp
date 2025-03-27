#include "matrixpp.h++"
#include <memory>

Matrix::Matrix(size_t h, size_t w) : height(h), width(w)
{
    this->data = std::make_unique<Matrix::MatrixppDataRow[]>(h);
    for (size_t i = 0; i < h; i++)
    {
        this->data[i] = std::make_unique<float[]>(w);
    }
}

Matrix::Matrix(const Matrix &other) : height(other.height), width(other.width)
{
    this->data = std::make_unique<Matrix::MatrixppDataRow[]>(other.height); 
    for (size_t i = 0; i < height; i++)
    {
        this->data[i] = std::make_unique<float[]>(width);
        for (size_t j = 0; j < width; j++)
        {
            this->data[i][j] = other.data[i][j]; // Copia profunda
        }
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
        this->data = std::make_unique<Matrix::MatrixppDataRow[]>(height);
        for (size_t i = 0; i < this->height; i++)
        {
            this->data[i] = std::make_unique<float[]> (width);
            for (size_t j = 0; j < width; j++)
            {
                this->data[i][j] = other.data[i][j]; // Copia profunda
            }
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