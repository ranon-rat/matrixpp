#include <iostream>
#include <functional>
#include <iomanip>
#include <tuple>
#include <future>

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
void Matrix::delete_data(std::string message)
{

    std::cout << "deleting data from\n"
              << message << std::endl;
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

Matrix Matrix::transpose() const
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

Matrix Matrix::padding(int h_pad, int w_pad) const
{
    if (h_pad < 0 || w_pad < 0)
    {
        throw "Invalid padding";
    }
    if (h_pad == 0 && w_pad == 0)
    {
        return *this;
    }
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
float Matrix::sum() const
{
    float sum = 0;
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            sum += this->data[i][j];
        }
    }
    return sum;
}
// stressen algorithm mfs i will kill you
Matrix Matrix::dot(Matrix &m) const
{
    return this->stressen_dot(m);
}

Matrix Matrix::slice(int y1, int y2, int x1, int x2) const
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

Matrix Matrix::brute_force_dot(const Matrix &m) const
{
    if (this->width != m.height)
    {
        std::cout << "brute force" << "\n";
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

Matrix Matrix::stressen_dot(const Matrix &m) const
{
    // in case the matrix is too small we can just use brute force
    if (this->height <= 256 || this->width <= 256)
    {
        return this->brute_force_dot(m);
    }
    if (this->width != m.height)
    {
        throw "Invalid matrix dimensions";
    }
    const int new_height = this->height;
    const int new_width = m.width;
    // this is for the product matrix that we are gonna be using

    const int padding_height = new_height + (new_height % 2 == 0 ? 0 : 1);
    const int padding_width = new_width + (new_width % 2 == 0 ? 0 : 1);

    // we will be using this for the result :D
    Matrix product_matrix(padding_height, padding_width);
    // in case our matrices is not divisible by 2 we need to pad it
    auto dot_matrix1 = this->padding(this->height % 2 == 0 ? 0 : 1, this->width % 2 == 0 ? 0 : 1);
    auto dot_matrix2 = m.padding(m.height % 2 == 0 ? 0 : 1, m.width % 2 == 0 ? 0 : 1);
    // first we need to divide the matrix into 4 parts
    Matrix a11(0, 0), a12(0, 0), a21(0, 0), a22(0, 0);
    Matrix b11(0, 0), b12(0, 0), b21(0, 0), b22(0, 0);

    int half_width_M = dot_matrix1.width / 2;
    int half_height_M = dot_matrix1.height / 2;
    STRASSEN_SPLIT(a11, a12, a21, a22, dot_matrix1, half_height_M, half_width_M);
    half_width_M = dot_matrix2.width / 2;

    half_height_M = dot_matrix2.height / 2;
    STRASSEN_SPLIT(b11, b12, b21, b22, dot_matrix2, half_height_M, half_width_M);

    // then we need to calculate the 7 products
    std::future<Matrix> p1_fut = std::async(std::launch::async, [&a11, &a22, &b11, &b22]()
                                            { return (a11 + a22) * (b11 + b22); });
    std::future<Matrix> p2_fut = std::async(std::launch::async, [&a21, &a22, &b11]()
                                            { return (a21 + a22) * b11; });
    std::future<Matrix> p3_fut = std::async(std::launch::async, [&a11, &b12, &b22]()
                                            { return a11 * (b12 - b22); });
    std::future<Matrix> p4_fut = std::async(std::launch::async, [&a22, &b21, &b11]()
                                            { return a22 * (b21 - b11); });
    std::future<Matrix> p5_fut = std::async(std::launch::async, [&a11, &a12, &b22]()
                                            { return (a11 + a12) * b22; });
    std::future<Matrix> p6_fut = std::async(std::launch::async, [&a21, &a11, &b11, &b21]()
                                            { return (a21 - a11) * (b11 + b21); });
    std::future<Matrix> p7_fut = std::async(std::launch::async, [&a12, &a22, &b21, &b22]()
                                            { return (a12 - a22) * (b21 + b22); });

    Matrix p1 = p1_fut.get();
    Matrix p2 = p2_fut.get();
    Matrix p3 = p3_fut.get();
    Matrix p4 = p4_fut.get();
    Matrix p5 = p5_fut.get();
    Matrix p6 = p6_fut.get();
    Matrix p7 = p7_fut.get();
    // then we need to calculate the 4 additions
    // a*e+b*g
    Matrix c11 = p1 + p4 - p5 + p7;
    // a*f+b*h
    Matrix c12 = p3 + p5;
    // c*e+d*g
    Matrix c21 = p2 + p4;
    // c*f+d*h
    Matrix c22 = p1 - p2 + p3 + p6;
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
    if (product_matrix.height == new_height && product_matrix.width == new_width)
    {
        return product_matrix;
    }
    return product_matrix.slice(0, new_height, 0, new_width);
}