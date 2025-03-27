#pragma once

#ifndef matrixpp_h
#define matrixpp_h
#include <iostream>
#include <format>
#include <tuple>
#include <functional>
#define ITER_THROUGH_MATRIXPP(result, A, B, func)                       \
    for (size_t i = 0; i < (A).height; i++)                             \
    {                                                                   \
        for (size_t j = 0; j < (A).width; j++)                          \
        {                                                               \
            (result).data[i][j] = (func)((A).get(i, j), (B).get(i, j)); \
        }                                                               \
    }

#define STRASSEN_SPLITPP(a11, a12, a21, a22, matrix, half_height, half_width) \
    if ((matrix).height % 2 != 0 || (matrix).width % 2 != 0)                  \
    {                                                                         \
        throw "Invalid matrix dimensions";                                    \
    }                                                                         \
    (a11) = (matrix).slice(0, (half_height), 0, (half_width));                \
    (a12) = (matrix).slice(0, (half_height), (half_width), (matrix).width);   \
    (a21) = (matrix).slice((half_height), (matrix).height, 0, (half_width));  \
    (a22) = (matrix).slice((half_height), (matrix).height, (half_width), (matrix).width);

#define DELETE_DATA_MATRIXPP(matrix)                      \
    do                                                    \
    {                                                     \
        if (data)                                         \
        {                                                 \
            for (size_t i = 0; i < (matrix)->height; i++) \
            {                                             \
                                                          \
                delete[] (matrix)->data[i];               \
                (matrix)->data[i] = nullptr;              \
            }                                             \
            delete[] (matrix)->data;                      \
            data = nullptr;                               \
        }                                                 \
    } while (0)

class Matrix
{
public:
    size_t height = 0;
    size_t width = 0;
    float **data = nullptr;

public:
    Matrix() {};
    Matrix(size_t h, size_t w);
    // copy constructor
    Matrix(const Matrix &other);

    ~Matrix()
    {
        DELETE_DATA_MATRIXPP(this);
    };
    Matrix randomized(size_t h, size_t w);
    void fill_rand();
    std::ostream &show(std::ostream &os)
    {
        for (size_t i = 0; i < this->height; i++)
        {
            os << "| ";
            for (size_t j = 0; j < this->width; j++)
            {
                os << std::format("{0:.2f} | ", this->data[i][j]);
            }
            os << "\n";
        }
        os << "\n";
        return os;
    }
    void show()
    {
        this->show(std::cout);
    };
    float get(size_t y, size_t x) const
    {
        return this->data[y][x];
    }

    Matrix slice(size_t y1, size_t y2, size_t x1, size_t x2) const;
    // this will add a new column or row depending of what you want :)
    Matrix padding(size_t h_pad, size_t w_pad) const;
    Matrix transpose() const;
    // so these are matrix multiplication functions
    Matrix brute_force_dot(const Matrix &m) const;
    Matrix stressen_dot(const Matrix &m) const;
    Matrix dot(Matrix &m) const;
    float sum() const;

public:
    bool operator==(const Matrix &m) const;
    bool operator!=(const Matrix &m) const;
    // copying
    Matrix &operator=(const Matrix &other);
    // scalars
    Matrix operator*(const float scalar) const;
    Matrix operator/(const float scalar) const;
    Matrix operator+(const float scalar) const;
    Matrix operator-(const float scalar) const;

    // matrix operations
    Matrix operator+(const Matrix &m) const;
    Matrix operator-(const Matrix &m) const;
    Matrix operator*(const Matrix &m) const;
    // some extra functions :)
    Matrix iter_through_matrix(std::function<float(float x, float y)> f, const Matrix &m2) const;
    friend std::ostream &operator<<(std::ostream &os, Matrix m);
};

#endif