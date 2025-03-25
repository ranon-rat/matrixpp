#include <iostream>
#include <functional>
#include <iomanip>
#include <tuple>
#include "matrixpp.h++"

Matrix::Matrix(int h, int w)
{
    this->width = w;
    this->height = h;
    this->data = new float *[h];
    for (int i = 0; i < h; i++)
    {
        this->data[i] = new float[w]{0};
    }
}
Matrix::Matrix(const Matrix &other) : height(other.height), width(other.width)
{
    data = new float *[height];
    for (int i = 0; i < height; i++)
    {
        data[i] = new float[width]{0};
        for (int j = 0; j < width; j++)
        {
            data[i][j] = other.data[i][j]; // Copia profunda
        }
    }
}
void Matrix::delete_data()
{
    for (int j = 0; j < this->height; j++)
    {
        delete[] this->data[j];
    }
    delete[] this->data;
}

Matrix Matrix::randomized(int h, int w)
{
    Matrix m(h, w);
    m.fill_rand();
    return m;
}
void Matrix::fill_rand()
{
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            this->data[i][j] = (rand() % 100) / 100.0;
        }
    }
}

Matrix Matrix::transpose()
{
    Matrix m(this->width, this->height);
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            m.data[j][i] = this->data[i][j];
        }
    }
    return m;
}
Matrix Matrix::transpose(Matrix m)
{
    Matrix m2(m.width, m.height);
    for (int i = 0; i < m.height; i++)
    {
        for (int j = 0; j < m.width; j++)
        {
            m2.data[j][i] = m.data[i][j];
        }
    }
    return m2;
}
Matrix Matrix::iter_through_matrix(std::function<float(float x, float y)> f, Matrix &m2)
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

// stressen algorithm mfs i will kill you
Matrix Matrix::dot(Matrix &m)
{
    return this->stressen_dot(m);
}
Matrix Matrix::operator+(Matrix &m)
{
    return this->iter_through_matrix([](float x, float y)
                                     { return x + y; }, m);
}
Matrix Matrix::operator-(Matrix &m)
{
    return this->iter_through_matrix([](float x, float y)
                                     { return x - y; }, m);
}
Matrix Matrix::operator*(Matrix &m)
{
    return this->stressen_dot(m);
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
std::ostream &operator<<(std::ostream &os, Matrix m)
{
    m.show(os);
    return os;
}

Matrix Matrix::slice(int y1, int y2, int x1, int x2)
{
    if (y1 < 0 || y1 > y2 || y2 > this->height || x1 < 0 || x1 > x2 || x2 > this->width)
    {
        throw "Invalid slice";
    }
    const int new_height = y2 - y1;
    const int new_width = x2 - x1;
    Matrix m(new_height, new_width);
    for (int i = 0; i < new_height; i++)
    {
        for (int j = 0; j < new_width; j++)
        {
            m.data[i][j] = this->data[i + y1][j + x1];
        }
    }
    return m;
}

// this will
Matrix Matrix::padding(int h_pad, int w_pad)
{

    Matrix m(this->height + h_pad, this->width + w_pad);
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            m.data[i][j] = this->data[i][j];
        }
    }
    return m;
}

Matrix Matrix::brute_force_dot(Matrix &m)
{
    if (this->width != m.height)
    {
        throw "Invalid matrix dimensions";
    }
    Matrix new_matrix(this->height, m.width);
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < m.width; j++)
        {
            float sum = 0;
            for (int k = 0; k < this->width; k++)
            {
                sum += this->data[i][k] * m.data[k][j];
            }
            new_matrix.data[i][j] = sum;
        }
    }
    return new_matrix;
}

std::tuple<Matrix, Matrix, Matrix, Matrix> Matrix::stressen_split()
{
    if (this->height % 2 != 0 || this->width % 2 != 0)
    {
        throw "Invalid matrix dimensions";
    }
    const int half_height = this->height / 2;
    const int half_width = this->width / 2;
    Matrix a = this->slice(0, half_height, 0, half_width);
    Matrix b = this->slice(0, half_height, half_width, this->width);
    Matrix c = this->slice(half_height, this->height, 0, half_width);
    Matrix d = this->slice(half_height, this->height, half_width, this->width);
    return {a, b, c, d};
}
Matrix Matrix::stressen_dot(Matrix &m)
{   // in case the matrix is too small we can just use brute force
    if (this->height <= 2 || this->width <= 2)
    {
        return this->brute_force_dot(m);
    }
    const int new_height = this->height;
    const int new_width = m.width;
    // this is for the product matrix that we are gonna be using
    const int padding_height = new_height + (new_height % 2 == 0 ? 0 : 1);
    const int padding_width = new_width + (new_width % 2 == 0 ? 0 : 1);
    Matrix product_matrix(padding_height, padding_width);
    // in case our matrices is not divisible by 2 we need to pad it
    auto dot_matrix1 = this->padding(this->height % 2 == 0 ? 0 : 1, this->width % 2 == 0 ? 0 : 1);
    auto dot_matrix2 = m.padding(m.height % 2 == 0 ? 0 : 1, m.width % 2 == 0 ? 0 : 1);
    // first we need to divide the matrix into 4 parts
    auto [a, b, c, d] = dot_matrix1.stressen_split();
    auto [e, f, g, h] = dot_matrix2.stressen_split();
    // then we need to calculate the 7 products
    auto ae = a.stressen_dot(e);
    auto bg = b.stressen_dot(g);
    auto af = a.stressen_dot(f);
    auto bh = b.stressen_dot(h);
    auto ce = c.stressen_dot(e);
    auto dg = d.stressen_dot(g);
    auto cf = c.stressen_dot(f);
    auto dh = d.stressen_dot(h);
    // then we need to calculate the 4 additions
    // a*e+b*g

    Matrix c11 = ae + bg;
    // a*f+b*h
    Matrix c12 = af + bh;
    // c*e+d*g
    Matrix c21 = ce + dg;
    // c*f+d*h
    Matrix c22 = cf + dh;
    // then we need to merge the 4 parts into the final matrix
    const int half_height = padding_height / 2;
    const int half_width = padding_width / 2;

    for (int i = 0; i < new_height; i++)
    {
        for (int j = 0; j < new_width; j++)
        {
            if (i < half_height && j < half_width)
            {

                product_matrix.data[i][j] = c11.data[i][j];
            }
            else if (i < half_height && j >= half_width)
            {

                product_matrix.data[i][j] = c12.data[i][j - half_width];
            }
            else if (i >= half_height && j < half_width)
            {

                product_matrix.data[i][j] = c21.data[i - half_height][j];
            }
            else
            {

                product_matrix.data[i][j] = c22.data[i - half_height][j - half_width];
            }
        }
    
    }
    return product_matrix.slice(0, new_height, 0, new_width);
}

Matrix Matrix::operator*(float scalar){
    return this->iter_through_matrix([scalar](float x, float y)
                                     { return x * scalar; }, *this);
}
Matrix Matrix::operator /(float scalar){
    return this -> iter_through_matrix([scalar](float x, float y)
                                     { return x / scalar; }, *this);
};
Matrix Matrix::operator+(float scalar){
    return this->iter_through_matrix([scalar](float x, float y)
                                     { return x + scalar; }, *this);
}
Matrix Matrix::operator-(float scalar){
    return this->iter_through_matrix([scalar](float x, float y)
                                     { return x - scalar; }, *this);
}