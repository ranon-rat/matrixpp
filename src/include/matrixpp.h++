#pragma once

#ifndef matrixpp_h
#define matrixpp_h
#include <iostream>
#include <format>
#include <functional>
#include <tuple>
#include <string>

#define ITER_THROUGH_MATRIX(result, A, B, func)                 \
    for (int i = 0; i < (A).height; i++) {                      \
        for (int j = 0; j < (A).width; j++) {                   \
            (result).data[i][j] = (func)((A).get(i, j), (B).get(i, j)); \
        }                                                       \
    }                                                           \

#define STRASSEN_SPLIT(a11, a12, a21, a22, matrix, half_height, half_width) \
    if ((matrix).height % 2 != 0 || (matrix).width % 2 != 0) {              \
        throw "Invalid matrix dimensions";                                  \
    }                                                                       \
    (a11) = (matrix).slice(0, (half_height), 0, (half_width));              \
    (a12) = (matrix).slice(0, (half_height), (half_width), (matrix).width); \
    (a21) = (matrix).slice((half_height), (matrix).height, 0, (half_width));\
    (a22) = (matrix).slice((half_height), (matrix).height, (half_width), (matrix).width);\
    \
    

class Matrix
{
public:
    int height=0;
    int width=0;
    float **data;

public:
    Matrix(){};
    Matrix(int h, int w);
    // copy constructor
    Matrix(const Matrix &other);

    ~Matrix()
    {
        for (int j = 0; j < this->height; j++)
        {
            delete[] this->data[j];
        }
        delete[] this->data;
    };
    void delete_data(std::string message);
    Matrix randomized(int h, int w);
    void fill_rand();
    std::ostream &show(std::ostream &os)
    {
        for (int i = 0; i < this->height; i++)
        {
            os << "| ";
            for (int j = 0; j < this->width; j++)
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
    float get(int y, int x) const
    {
        return this->data[y][x];
    }

   
    Matrix slice(int y1, int y2, int x1, int x2) const;
    // this will add a new column or row depending of what you want :)
     Matrix padding(int h_pad, int w_pad) const; 
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