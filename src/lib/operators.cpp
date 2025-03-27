#include <functional>
#include <iostream>
#include "matrixpp.h++"

Matrix Matrix::operator*(const float scalar) const
{
    Matrix result(this->height, this->width);
    auto f = [scalar](float x, float)
    { return x * scalar; };
    ITER_THROUGH_MATRIXPP(result, *this, *this, f);
    return result;
}
Matrix Matrix::operator/(const float scalar) const
{
    Matrix result(this->height, this->width);
    auto f = [scalar](float x, float)
    { return x / scalar; };
    ITER_THROUGH_MATRIXPP(result, *this, *this, f);
    return result;
};
Matrix Matrix::operator+(const float scalar) const
{

    Matrix result(this->height, this->width);
    auto f = [scalar](float x, float)
    { return x + scalar; };
    ITER_THROUGH_MATRIXPP(result, *this, *this, f);
    return result;
}
Matrix Matrix::operator-(const float scalar) const
{

    Matrix result(this->height, this->width);
    auto f = [scalar](float x, float)
    { return x - scalar; };
    ITER_THROUGH_MATRIXPP(result, *this, *this, f);
    return result;
}
Matrix Matrix::operator+(const Matrix &m) const
{
    if (this->height != m.height || this->width != m.width)
    {
        throw "Matrix dimensions must be equal";
    }
    Matrix result(this->height, this->width);
    auto f = [](float x, float y)
    { return x + y; };
    ITER_THROUGH_MATRIXPP(result, *this, m, f);
    return result;
}

Matrix Matrix::operator-(const Matrix &m) const
{
    if (this->height != m.height || this->width != m.width)
    {
        throw "Matrix dimensions must be equal";
    }
    Matrix result(this->height, this->width);
    auto f = [](float x, float y)
    { return x - y; };
    ITER_THROUGH_MATRIXPP(result, *this, m, f);
    return result;
}
Matrix Matrix::operator*(const Matrix &m) const
{
    return this->stressen_dot(m);
}

bool Matrix::operator==(const Matrix &m) const
{
    if (this->height != m.height || this->width != m.width)
    {
        return false;
    }
    for (size_t i = 0; i < this->height; i++)
    {
        for (size_t j = 0; j < this->width; j++)
        {
            if (this->data[i][j] != m.data[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
bool Matrix::operator!=(const Matrix &m) const
{
    return !(*this == m);
}

std::ostream &operator<<(std::ostream &os, Matrix m)
{
    m.show(os);
    return os;
}
