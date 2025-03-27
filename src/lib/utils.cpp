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
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            new_matrix.data[i][j] = f(this->get(i, j), m2.get(i, j));
        }
    }

    return new_matrix; 
}
// this will fill the matrix with random numbers
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
// this will add new columns and rows to the matrix
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

// this will slice the matrix
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
// transpose the matrix
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
// this will sum all the elements of the matrix
float Matrix::sum()const{
    float sum=0;
    for(int i=0;i<this->height;i++){
        for(int j=0;j<this->width;j++){
            sum+=this->data[i][j];
        }
    }
    return sum;

}