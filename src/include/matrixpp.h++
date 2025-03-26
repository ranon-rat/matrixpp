#pragma once

#ifndef matrixpp_h
#define matrixpp_h
#include <iostream>
#include <format>
#include <functional>
#include <tuple>
class Matrix
{
public:
    int height;
    int width;
    float **data;

public:
    Matrix(int h, int w);
    Matrix(const Matrix &other);

    ~Matrix()
    {
        for (int j = 0; j < this->height; j++)
        {
            delete[] this->data[j];
        }
        delete[] this->data;
    };
    void delete_data();
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
    Matrix brute_force_dot(const Matrix &m) const;
    Matrix stressen_dot(const Matrix &m) const;
    Matrix slice(int y1, int y2, int x1, int x2) const;
    // this will add a new column or row depending of what you want :)
    Matrix padding(int h_pad, int w_pad) const;
    Matrix dot(Matrix &m) const;
    Matrix transpose() const;
    std::tuple<Matrix, Matrix, Matrix, Matrix> stressen_split() const;

public:
    // scalars
    Matrix operator*(const float scalar) const;
    Matrix operator/(const float scalar) const;
    Matrix operator+(const float scalar) const;
    Matrix operator-(const float scalar) const;

    Matrix &operator=(const Matrix &other);
    // matrix operations
    Matrix operator+(const Matrix &m) const;
    Matrix operator-(const Matrix &m) const;
    Matrix operator*(const Matrix &m) const;

    friend std::ostream &operator<<(std::ostream &os, Matrix m);

private:
    Matrix iter_through_matrix(std::function<float(float x, float y)> f, const Matrix &m2) const;
};

#endif