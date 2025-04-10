#include <functional>
#include "matrixpp.h++"

// this will iter through 2 matrices and apply a function to each element
Matrix Matrix::iter_through_matrix(std::function<float(float x, float y)> f, const Matrix &m2) const
{
    if (this->height != m2.height || this->width != m2.width)
    {

        throw "Matrix dimensions must be equal";
    }

    Matrix new_matrix(this->height, this->width);
    for (size_t i = 0; i < this->height * this->width; i++)
    {
        new_matrix.data[i] = f(this->data[i], m2.data[i]);
    }

    return new_matrix;
}
// this will fill the matrix with random numbers
void Matrix::fill_rand()
{
    for (size_t i = 0; i < this->height; i++)
    {
        for (size_t j = 0; j < this->width; j++)
        {
            this->data[i * this->width + j] = (rand() % 100) / 100.0;
        }
    }
}
// this will add new columns and rows to the matrix
Matrix Matrix::padding(size_t h_pad, size_t w_pad) const
{

    if (h_pad == 0 && w_pad == 0)
    {
        return *this;
    }
    Matrix m(this->height + h_pad, this->width + w_pad);
    for (size_t i = 0; i < this->height; i++)
    {
        for (size_t j = 0; j < this->width; j++)
        {
            m.data[i * m.width + j] = this->get(i, j);
        }
    }
    return m;
}

// this will slice the matrix
Matrix Matrix::slice(size_t y1, size_t y2, size_t x1, size_t x2) const
{
    if (y1 > y2 || y2 > this->height || x1 > x2 || x2 > this->width)
    {
        throw "Invalid slice";
    }
    const auto new_height = y2 - y1;
    const auto new_width = x2 - x1;
    Matrix m(new_height, new_width);
    for (size_t i = 0; i < new_height; i++)
    {
        for (size_t j = 0; j < new_width; j++)
        {
            m.data[i * m.width + j] = this->get(i + y1, j + x1);
        }
    }
    return m;
}
// transpose the matrix
Matrix Matrix::transpose() const
{
    Matrix m(this->width, this->height);
    for (size_t i = 0; i < this->height; i++)
    {
        for (size_t j = 0; j < this->width; j++)
        {
            m.data[j * m.width + i] = this->get(i, j);
        }
    }
    return m;
}
// this will sum all the elements of the matrix
float Matrix::sum() const
{
    float sum = 0;
    for (size_t i = 0; i < this->height; i++)
    {
        for (size_t j = 0; j < this->width; j++)
        {
            sum += this->get(i, j);
        }
    }
    return sum;
}