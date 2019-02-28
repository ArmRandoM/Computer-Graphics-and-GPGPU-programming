#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

//Class that defines a reader for a data set configuration file
class DataFileReader
{
    private:
        //The minimum z coordinate
        float z_min;
        //The maximum z coordinate
        float z_max;
        //An array that contains the parameters stored in the data set configuration file
        float* parameters;
        //A matrix that contains all vertices that are suggested by the configuration file
        float** vertices_matrix;
    public:
        //constructor with parameters
        DataFileReader(string data_file_name);

        //set functions

        //function that sets the number of the columns
        void setNumColumns(float nCols);
        //function that sets the number of the rows
        void setNumRows(float nrows);
        //function that sets the X coordinate for the lower left corner
        void setXllCorner(float xllcorner);
        //function that sets the Y coordinate for the lower left corner
        void setYllCorner(float yllcorner);
        //function that sets the cell size value
        void setCellSize(float cellsize);
        //function that sets the NODATA value
        void setNoDataValue(float NODATA_value);
        //function that sets the matrix that contains all vertices
        void setVerticesMatrix(float** vertices_matrix);
        //function that sets the z_min

        //get functions

        //function that returns the number of the columns
        int getNumColumns();
        //function that returns the number of the rows
        int getNumRows();
        //function that returns the X coordinate for the lower left corner
        float getXllCorner();
        //function that returns the Y coordinate for the lower left corner
        float getYllCorner();
        //function that returns the cell size value
        float getCellSize();
        //function that returns the NODATA value
        int getNoDataValue();
        //function that returns the matrix that contains all vertices
        float** getVerticesMatrix();
        //fuction that returns the minimum z coordinate
        float get_z_min();
        //function that returns the maximum z coordinate
        float get_z_max();
};

DataFileReader::DataFileReader(string data_file_name)
{
    ifstream myfile;
    string parameter_name;
    string parameter_value;
    parameters = new float[6];
    bool matrix = false;
    bool create_matrix = true;
    int index = 0;
    myfile.open(data_file_name.c_str(), ios::in | ios::binary);

    while (!myfile.eof())
    {
        z_max = 0;
        if(!matrix)
        {
            myfile >> parameter_name >> parameter_value;
            parameters[index] = stof(parameter_value);
            index++;
            if( parameter_name == "NODATA_value")
                matrix = true;
        }
        else
        {
            int cols = static_cast<int> (parameters[0]);
            int rows = static_cast<int> (parameters[1]);
          
            if(create_matrix)
            {
                vertices_matrix = new float* [rows];
                for( int i = 0; i < rows; i++ )
                    vertices_matrix[i] = new float [cols];
                create_matrix = false;
            }

            for( int i = 0; i < rows; i++ )
            {
                for( int j = 0; j < cols; j++ )
                {
                    myfile >> vertices_matrix[i][j];
                 
                    if( i == 0 && j == 0)
                        z_min = vertices_matrix[i][j];
                    
                    if( vertices_matrix[i][j] > z_max )
                        z_max = vertices_matrix[i][j];

                    if( vertices_matrix[i][j] < z_min )
                        z_min = vertices_matrix[i][j];
                }
            }   
                
            break;
        }
    }
    myfile.close();
}

void DataFileReader::setNumColumns(float ncols)
{
    parameters[0] = ncols;
}

void DataFileReader::setNumRows(float nrows)
{
    parameters[1] = nrows;
}

void DataFileReader::setXllCorner(float xllcorner)
{
    parameters[2] = xllcorner;
}

void DataFileReader::setYllCorner(float yllcorner)
{
    parameters[3] = yllcorner;
}

void DataFileReader::setCellSize(float cellsize)
{
    parameters[4] = cellsize;
}

void DataFileReader::setNoDataValue(float NODATA_value)
{
   parameters[5] = NODATA_value;
}

void DataFileReader::setVerticesMatrix(float** vertices_matrix)
{
    this->vertices_matrix = vertices_matrix;
}

int DataFileReader::getNumColumns()
{
    return (int) parameters[0];
}

int DataFileReader::getNumRows()
{
    return (int) parameters[1];
}

float DataFileReader::getXllCorner()
{
    return parameters[2];
}

float DataFileReader::getYllCorner()
{
    return parameters[3];
}

float DataFileReader::getCellSize()
{
    return parameters[4];
}

int DataFileReader::getNoDataValue()
{
   return (int) parameters[5];
}

float** DataFileReader::getVerticesMatrix()
{
    return this->vertices_matrix;
}    

float DataFileReader::get_z_min()
{
    return this->z_min;
} 

float DataFileReader::get_z_max()
{
    return this->z_max;
} 