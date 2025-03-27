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

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {
       
        DELETE_DATA_MATRIXPP(this); 
      

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

Matrix Matrix::randomized(int h, int w)
{
    Matrix m(h, w);
    m.fill_rand();
    return m;
}