

#include "Affine.h"

Hcoord::Hcoord(float X, float Y, float Z, float W) : x(X),y(Y),z(Z),w(W) {}

Point::Point(float X, float Y, float Z) : Hcoord(X,Y,Z,1) {}

Vector::Vector(float X, float Y, float Z) :Hcoord(X,Y,Z,0){}

Affine::Affine()
{
	row[0] = Hcoord{ 1,0,0,0 };
	row[1] = Hcoord{ 0,1,0,0 };
	row[2] = Hcoord{ 0,0,1,0 };
	row[3] = Hcoord{ 0,0,0,1 };
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
	row[0] = Hcoord{ Lx.x,Ly.x,Lz.x,D.x };
	row[1] = Hcoord{ Lx.y,Ly.y,Lz.y,D.y };
	row[2] = Hcoord{ Lx.z,Ly.z,Lz.z,D.z };
	row[3] = Hcoord{ Lx.w,Ly.w,Lz.w,D.w };
}

Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
	Hcoord result;
	for(int elem =0; elem <4 ; elem++)
	{
		result[elem] = u[elem] + v[elem];  
	}
	return result;
}

Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
	Hcoord result;
	for (int elem = 0; elem < 4; elem++)
	{
		result[elem] = u[elem] - v[elem];
	}
	return result;
}

Hcoord operator-(const Hcoord& v)
{
	Hcoord result;
	for (int elem = 0; elem < 4; elem++)
	{
		result[elem] = - v[elem];
	}
	return result;
}

Hcoord operator*(float r, const Hcoord& v)
{
	Hcoord result;
	for (int elem = 0; elem < 4; elem++)
	{
		result[elem] = r * v[elem];
	}
	return result;
}

Hcoord operator*(const Matrix& A, const Hcoord& v)
{
	Hcoord result;
	for (int row = 0; row < 4; row++)
	{
		for (int elem = 0; elem < 4; elem++)
		{
			//result[row] is x,y,z,w
			result[row] += A[row][elem] * v[elem];
		}
	}
	return result;
}

Matrix operator*(const float r, const Matrix& A)
{
	Matrix result;
	for (int row = 0; row < 4; row++)
	{
		result[row] = r * A[row];
	}
	return result;
}

Matrix operator+(const Matrix& A, const Matrix& B)
{
	Matrix result;
	for (int row = 0; row < 4; row++)
	{
		result[row] = A[row] + B[row];
	}
	return result;
}


Matrix operator*(const Matrix& A, const Matrix& B)
{
	
	Matrix result;

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			for (int inside = 0; inside < 4; inside++)
			{
				result[row][col] += A[row][inside] * B[inside][col];
			}
		}
	}
	return result;
}

float dot(const Vector& u, const Vector& v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z );
}

float abs(const Vector& v)
{
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

Vector normalize(const Vector& v)
{
	float magnitude = abs(v);
	return Vector{ v.x / magnitude , v.y / magnitude, v.z / magnitude };
}

Vector cross(const Vector& u, const Vector& v)
{
	Vector result;
	result.x = u.y * v.z - u.z * v.y;
	result.y = u.z * v.x - u.x * v.z;
	result.z = u.x * v.y - u.y * v.x;
	result.w = 0;
	return result;
}

Affine rotate(float t, const Vector& v)
{

	const float cost = cos(t);
	const float sint = sin(t);

	//unitmatrix
	Matrix IdentiyMatrix;
	IdentiyMatrix[0] = Hcoord(1,0,0,0);
	IdentiyMatrix[1] = Hcoord(0, 1, 0, 0);
	IdentiyMatrix[2] = Hcoord(0, 0, 1, 0);
	IdentiyMatrix[3] = Hcoord(0, 0, 0, 0);
	Matrix A = cost * IdentiyMatrix;
	
	const float powU = abs(v)*abs(v);
	const float one_minus_cost = 1 - cost;
	
	Matrix outproductMatrix;
	outproductMatrix[0] = Hcoord(v.x * v.x, v.x * v.y, v.x * v.z, 0);
	outproductMatrix[1] = Hcoord(v.y * v.x, v.y * v.y, v.y * v.z, 0);
	outproductMatrix[2] = Hcoord(v.z * v.x, v.z * v.y, v.z * v.z, 0);
	outproductMatrix[3] = Hcoord(0,0,0, 0);
	
	Matrix CrossProductMatrix;
	CrossProductMatrix[0] = Hcoord(0, -v.z, v.y, 0);
	CrossProductMatrix[1] = Hcoord(v.z,0, -v.x, 0);
	CrossProductMatrix[2] = Hcoord(-v.y,v.x, 0, 0);
	CrossProductMatrix[3] = Hcoord(0,0, 0, 0);
	const float Coefficient2 = (one_minus_cost / powU);
	const float square = abs(v);

	
	Matrix Result = A + Coefficient2 * outproductMatrix + (sint / square) * CrossProductMatrix;
	Result[3] = Hcoord(0, 0, 0, 1);

	return Result;
}
	


Affine translate(const Vector& v)
{
	Affine TranslateMatrix;
	TranslateMatrix[0][3] = v.x;
	TranslateMatrix[1][3] = v.y;
	TranslateMatrix[2][3] = v.z;

	return TranslateMatrix;
}

Affine scale(float r)
{
	Affine ScaleMatrix;
		ScaleMatrix[0][0] = r;
		ScaleMatrix[1][1] = r;
		ScaleMatrix[2][2] = r;
		ScaleMatrix[3][3] = 1;
		return ScaleMatrix;
}

Affine scale(float rx, float ry, float rz)
{
	Affine ScaleMatrix;
	ScaleMatrix[0][0] = rx;
	ScaleMatrix[1][1] = ry;
	ScaleMatrix[2][2] = rz;
	ScaleMatrix[3][3] = 1;
	return ScaleMatrix;
}

Affine inverse(const Affine& A)
{
	Affine M;

	M[0][0] =  A[1][1]*A[2][2] - A[1][2]*A[2][1];    //M[0][0]
	M[0][1] = -A[1][0]*A[2][2] + A[1][2]*A[2][0];    //M[0][1]
	M[0][2] =  A[1][0]*A[2][1] - A[1][1]*A[2][0];	 //M[0][2]
	M[1][0] = -A[0][1]*A[2][2] + A[0][2]*A[2][1];    //M[1][0]
	M[1][1] =  A[0][0]*A[2][2] - A[0][2]*A[2][0];    //M[1][1]
	M[1][2] = -A[0][0]*A[2][1] + A[0][1]*A[2][0];    //M[1][2]
	M[2][0] =  A[0][1]*A[1][2] - A[0][2]*A[1][1];    //M[2][0]
	M[2][1] = -A[0][0]*A[1][2] + A[0][2]*A[1][0];    //M[2][1]
	M[2][2] =  A[0][0]*A[1][1] - A[0][1]*A[1][0];    //M[2][2]
	M[3] = Hcoord{ 0,0,0,1 };
	const float det = A[0][0] * M[0][0] + A[0][1] * M[0][1] + A[0][2] * M[0][2];

		Affine inverseL;
		//Transpose M
		for(int i =0; i <3 ; i++)
		{
			for(int j =0; j<3 ;j++)
			{
				inverseL[i][j] = M[j][i];
			}
		}

		inverseL[0] = (1 / det) * inverseL[0];
		inverseL[1] = (1 / det) * inverseL[1];
		inverseL[2] = (1 / det) * inverseL[2];
		inverseL[3] = Hcoord{ 0,0 ,0,1 };

	    //T Inverse
		Affine V;
		V[0][3] = -A[0][3]; V[1][3] = -A[1][3]; V[2][3] = -A[2][3];

		return inverseL * V;
}
