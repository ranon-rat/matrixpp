#include <iostream>
#include <functional>
#include <iomanip>
#include <tuple>
#include <future>
#include "matrixpp.h++"

// stressen algorithm mfs i will kill you
Matrix Matrix::dot(Matrix &m) const
{
    return this->stressen_dot(m);
}

Matrix Matrix::brute_force_dot(const Matrix &m) const
{
    if (this->width != m.height)
    {

        throw "Invalid matrix dimensions";
    }

    Matrix new_matrix(this->height, m.width);
    for (size_t i = 0; i < this->height; i++)
    {
        for (size_t j = 0; j < m.width; j++)
        {
            float sum = 0;
            for (size_t k = 0; k < this->width; k++)
            {
                sum += this->get(i,k) * m.get(k ,j);
            }
            new_matrix.data[i * new_matrix.width + j] = sum;
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
    const auto new_height = this->height;
    const auto new_width = m.width;
    // this is for the product matrix that we are gonna be using

    const auto padding_height = new_height + (new_height % 2 == 0 ? 0 : 1);
    const auto padding_width = new_width + (new_width % 2 == 0 ? 0 : 1);

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
    STRASSEN_SPLITPP(a11, a12, a21, a22, dot_matrix1, half_height_M, half_width_M);
    half_width_M = dot_matrix2.width / 2;

    half_height_M = dot_matrix2.height / 2;
    STRASSEN_SPLITPP(b11, b12, b21, b22, dot_matrix2, half_height_M, half_width_M);

    std::function<Matrix()> p1 = [&a11, &a22, &b11, &b22]()
    { return (a11 + a22) * (b11 + b22); };
    std::function<Matrix()> p2 = [&a21, &a22, &b11]()
    { return (a21 + a22) * b11; };
    std::function<Matrix()> p3 = [&a11, &b12, &b22]()
    { return a11 * (b12 - b22); };
    std::function<Matrix()> p4 = [&a22, &b21, &b11]()
    { return a22 * (b21 - b11); };
    std::function<Matrix()> p5 = [&a11, &a12, &b22]()
    { return (a11 + a12) * b22; };
    std::function<Matrix()> p6 = [&a21, &a11, &b11, &b21]()
    { return (a21 - a11) * (b11 + b21); };
    std::function<Matrix()> p7 = [&a12, &a22, &b21, &b22]()
    { return (a12 - a22) * (b21 + b22); };
    // then we need to calculate the 4 additions
    // a*e+b*g

    std::future<Matrix> c11_fut = std::async(std::launch::async, [p1, p4, p5, p7]()
                                             { return p1() + p4() - p5() + p7(); });
    std::future<Matrix> c12_fut = std::async(std::launch::async, [p3, p5]()
                                             { return p3() + p5(); });
    std::future<Matrix> c21_fut = std::async(std::launch::async, [p2, p4]()
                                             { return p2() + p4(); });
    std::future<Matrix> c22_fut = std::async(std::launch::async, [p1, p2, p3, p6]()
                                             { return p1() - p2() + p3() + p6(); });

    Matrix c11 = c11_fut.get();
    // a*f+b*h
    Matrix c12 = c12_fut.get();
    // c*e+d*g
    Matrix c21 = c21_fut.get();
    // c*f+d*h
    Matrix c22 = c22_fut.get();
    // then we need to merge the 4 parts into the final matrix
    const auto half_height = padding_height / 2;
    const auto half_width = padding_width / 2;

    for (size_t i = 0; i < new_height; i++)
    {
        for (size_t j = 0; j < new_width; j++)
        {
            if (i < half_height && j < half_width)
            {

                product_matrix.data[i * product_matrix.width + j] = c11.get(i, j);
            }
            else if (i < half_height && j >= half_width)
            {

                product_matrix.data[i * product_matrix.width + j] = c12.get(i, j - half_width);
            }
            else if (i >= half_height && j < half_width)
            {

                product_matrix.data[i * product_matrix.width + j] = c21.get(i - half_height, j);
            }
            else
            {

                product_matrix.data[i * product_matrix.width + j] = c22.get(i - half_height, j - half_width);
            }
        }
    }
    if (product_matrix.height == new_height && product_matrix.width == new_width)
    {
        return product_matrix;
    }
    return product_matrix.slice(0, new_height, 0, new_width);
}