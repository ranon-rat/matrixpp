#include <functional>
#include <iostream>
#include "matrixpp.h++"
Matrix Matrix::iter_through_matrix(std::function<float(float x, float y)> f, const Matrix &m2) const
{
    if (this->height != m2.height || this->width != m2.width)
    {

        throw "Matrix dimensions must be equal";
    }

    Matrix new_matrix(this->height, this->width); // Asignar la memoria para la nueva matriz

    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            new_matrix.data[i][j] = f(this->get(i, j), m2.get(i, j));
        }
    }

    return new_matrix; // Regresar la nueva matriz con los resultados
}


Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {

        this->delete_data();
        // Copiar nuevas dimensiones
        this->height = other.height;
        this->width = other.width;
        this->data = new float *[height];
        for (int i = 0; i < height; i++)
        {
            this->data[i] = new float[width]{0};
            for (int j = 0; j < width; j++)
            {
                this->data[i][j] = other.data[i][j]; // Copia profunda
            }
        }
    }
    return *this;
}
Matrix Matrix::operator*(const float scalar) const
{
    return this->iter_through_matrix([scalar](float x, float)
                                     { return x * scalar; }, *this);
}
Matrix Matrix::operator/(const float scalar) const
{
    return this->iter_through_matrix([scalar](float x, float)
                                     { return x / scalar; }, *this);
};
Matrix Matrix::operator+(const float scalar) const
{
    return this->iter_through_matrix([scalar](float x, float)
                                     { return x + scalar; }, *this);
}
Matrix Matrix::operator-(const float scalar) const
{
    return this->iter_through_matrix([scalar](float x, float)
                                     { return x - scalar; }, *this);
}
Matrix Matrix::operator+(const Matrix &m) const
{
    return this->iter_through_matrix([](float x, float y)
                                     { return x + y; }, m);
}

Matrix Matrix::operator-(const Matrix &m) const
{
    return this->iter_through_matrix([](float x, float y)
                                     { return x - y; }, m);
}
Matrix Matrix::operator*(const Matrix &m) const
{
    return this->stressen_dot(m);
}



std::ostream &operator<<(std::ostream &os, Matrix m)
{
    m.show(os);
    return os;
}
