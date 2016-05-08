#ifndef MATRIX_H_
#define MATRIX_H_

#include "vector.h"
#include <cmath>

class Matrix
{
public:

	Matrix()
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m_v[i][j] = 0;
	}

	void make_identity();

	Matrix operator*(const Matrix &other) const;
	Matrix &operator*=(const Matrix &other);

	void Multiply(const Matrix *other, Matrix *result) const
	{
		double num1 = (double) ((double) m_v[0][0] * (double) other->m_v[0][0] + (double) m_v[0][1] * (double) other->m_v[1][0] + (double) m_v[0][2] * (double) other->m_v[2][0] + (double) m_v[0][3] * (double) other->m_v[3][0]);
		double num2 = (double) ((double) m_v[0][0] * (double) other->m_v[0][1] + (double) m_v[0][1] * (double) other->m_v[1][1] + (double) m_v[0][2] * (double) other->m_v[2][1] + (double) m_v[0][3] * (double) other->m_v[3][1]);
		double num3 = (double) ((double) m_v[0][0] * (double) other->m_v[0][2] + (double) m_v[0][1] * (double) other->m_v[1][2] + (double) m_v[0][2] * (double) other->m_v[2][2] + (double) m_v[0][3] * (double) other->m_v[3][2]);
		double num4 = (double) ((double) m_v[0][0] * (double) other->m_v[0][3] + (double) m_v[0][1] * (double) other->m_v[1][3] + (double) m_v[0][2] * (double) other->m_v[2][3] + (double) m_v[0][3] * (double) other->m_v[3][3]);
		double num5 = (double) ((double) m_v[1][0] * (double) other->m_v[0][0] + (double) m_v[1][1] * (double) other->m_v[1][0] + (double) m_v[1][2] * (double) other->m_v[2][0] + (double) m_v[1][3] * (double) other->m_v[3][0]);
		double num6 = (double) ((double) m_v[1][0] * (double) other->m_v[0][1] + (double) m_v[1][1] * (double) other->m_v[1][1] + (double) m_v[1][2] * (double) other->m_v[2][1] + (double) m_v[1][3] * (double) other->m_v[3][1]);
		double num7 = (double) ((double) m_v[1][0] * (double) other->m_v[0][2] + (double) m_v[1][1] * (double) other->m_v[1][2] + (double) m_v[1][2] * (double) other->m_v[2][2] + (double) m_v[1][3] * (double) other->m_v[3][2]);
		double num8 = (double) ((double) m_v[1][0] * (double) other->m_v[0][3] + (double) m_v[1][1] * (double) other->m_v[1][3] + (double) m_v[1][2] * (double) other->m_v[2][3] + (double) m_v[1][3] * (double) other->m_v[3][3]);
		double num9 = (double) ((double) m_v[2][0] * (double) other->m_v[0][0] + (double) m_v[2][1] * (double) other->m_v[1][0] + (double) m_v[2][2] * (double) other->m_v[2][0] + (double) m_v[2][3] * (double) other->m_v[3][0]);
		double num10 = (double) ((double) m_v[2][0] * (double) other->m_v[0][1] + (double) m_v[2][1] * (double) other->m_v[1][1] + (double) m_v[2][2] * (double) other->m_v[2][1] + (double) m_v[2][3] * (double) other->m_v[3][1]);
		double num11 = (double) ((double) m_v[2][0] * (double) other->m_v[0][2] + (double) m_v[2][1] * (double) other->m_v[1][2] + (double) m_v[2][2] * (double) other->m_v[2][2] + (double) m_v[2][3] * (double) other->m_v[3][2]);
		double num12 = (double) ((double) m_v[2][0] * (double) other->m_v[0][3] + (double) m_v[2][1] * (double) other->m_v[1][3] + (double) m_v[2][2] * (double) other->m_v[2][3] + (double) m_v[2][3] * (double) other->m_v[3][3]);
		double num13 = (double) ((double) m_v[3][0] * (double) other->m_v[0][0] + (double) m_v[3][1] * (double) other->m_v[1][0] + (double) m_v[3][2] * (double) other->m_v[2][0] + (double) m_v[3][3] * (double) other->m_v[3][0]);
		double num14 = (double) ((double) m_v[3][0] * (double) other->m_v[0][1] + (double) m_v[3][1] * (double) other->m_v[1][1] + (double) m_v[3][2] * (double) other->m_v[2][1] + (double) m_v[3][3] * (double) other->m_v[3][1]);
		double num15 = (double) ((double) m_v[3][0] * (double) other->m_v[0][2] + (double) m_v[3][1] * (double) other->m_v[1][2] + (double) m_v[3][2] * (double) other->m_v[2][2] + (double) m_v[3][3] * (double) other->m_v[3][2]);
		double num16 = (double) ((double) m_v[3][0] * (double) other->m_v[0][3] + (double) m_v[3][1] * (double) other->m_v[1][3] + (double) m_v[3][2] * (double) other->m_v[2][3] + (double) m_v[3][3] * (double) other->m_v[3][3]);
		result->m_v[0][0] = num1;
		result->m_v[0][1] = num2;
		result->m_v[0][2] = num3;
		result->m_v[0][3] = num4;
		result->m_v[1][0] = num5;
		result->m_v[1][1] = num6;
		result->m_v[1][2] = num7;
		result->m_v[1][3] = num8;
		result->m_v[2][0] = num9;
		result->m_v[2][1] = num10;
		result->m_v[2][2] = num11;
		result->m_v[2][3] = num12;
		result->m_v[3][0] = num13;
		result->m_v[3][1] = num14;
		result->m_v[3][2] = num15;
		result->m_v[3][3] = num16;
	}
	Matrix Invert() const;
	void Invert();

	static void Invert(const Matrix *matrix, Matrix *result)
	{
		double num1 = matrix->m_v[0][0];
		double num2 = matrix->m_v[0][1];
		double num3 = matrix->m_v[0][2];
		double num4 = matrix->m_v[0][3];
		double num5 = matrix->m_v[1][0];
		double num6 = matrix->m_v[1][1];
		double num7 = matrix->m_v[1][2];
		double num8 = matrix->m_v[1][3];
		double num9 = matrix->m_v[2][0];
		double num10 = matrix->m_v[2][1];
		double num11 = matrix->m_v[2][2];
		double num12 = matrix->m_v[2][3];
		double num13 = matrix->m_v[3][0];
		double num14 = matrix->m_v[3][1];
		double num15 = matrix->m_v[3][2];
		double num16 = matrix->m_v[3][3];
		double num17 = (double) ((double) num11 * (double) num16 - (double) num12 * (double) num15);
		double num18 = (double) ((double) num10 * (double) num16 - (double) num12 * (double) num14);
		double num19 = (double) ((double) num10 * (double) num15 - (double) num11 * (double) num14);
		double num20 = (double) ((double) num9 * (double) num16 - (double) num12 * (double) num13);
		double num21 = (double) ((double) num9 * (double) num15 - (double) num11 * (double) num13);
		double num22 = (double) ((double) num9 * (double) num14 - (double) num10 * (double) num13);
		double num23 = (double) ((double) num6 * (double) num17 - (double) num7 * (double) num18 + (double) num8 * (double) num19);
		double num24 = (double) -((double) num5 * (double) num17 - (double) num7 * (double) num20 + (double) num8 * (double) num21);
		double num25 = (double) ((double) num5 * (double) num18 - (double) num6 * (double) num20 + (double) num8 * (double) num22);
		double num26 = (double) -((double) num5 * (double) num19 - (double) num6 * (double) num21 + (double) num7 * (double) num22);
		double num27 = (double) (1.0 / ((double) num1 * (double) num23 + (double) num2 * (double) num24 + (double) num3 * (double) num25 + (double) num4 * (double) num26));

		result->m_v[0][0] = num23 * num27;
		result->m_v[1][0] = num24 * num27;
		result->m_v[2][0] = num25 * num27;
		result->m_v[3][0] = num26 * num27;
		result->m_v[0][1] = (double) -((double) num2 * (double) num17 - (double) num3 * (double) num18 + (double) num4 * (double) num19) * num27;
		result->m_v[1][1] = (double) ((double) num1 * (double) num17 - (double) num3 * (double) num20 + (double) num4 * (double) num21) * num27;
		result->m_v[2][1] = (double) -((double) num1 * (double) num18 - (double) num2 * (double) num20 + (double) num4 * (double) num22) * num27;
		result->m_v[3][1] = (double) ((double) num1 * (double) num19 - (double) num2 * (double) num21 + (double) num3 * (double) num22) * num27;
		double num28 = (double) ((double) num7 * (double) num16 - (double) num8 * (double) num15);
		double num29 = (double) ((double) num6 * (double) num16 - (double) num8 * (double) num14);
		double num30 = (double) ((double) num6 * (double) num15 - (double) num7 * (double) num14);
		double num31 = (double) ((double) num5 * (double) num16 - (double) num8 * (double) num13);
		double num32 = (double) ((double) num5 * (double) num15 - (double) num7 * (double) num13);
		double num33 = (double) ((double) num5 * (double) num14 - (double) num6 * (double) num13);
		result->m_v[0][2] = (double) ((double) num2 * (double) num28 - (double) num3 * (double) num29 + (double) num4 * (double) num30) * num27;
		result->m_v[1][2] = (double) -((double) num1 * (double) num28 - (double) num3 * (double) num31 + (double) num4 * (double) num32) * num27;
		result->m_v[2][2] = (double) ((double) num1 * (double) num29 - (double) num2 * (double) num31 + (double) num4 * (double) num33) * num27;
		result->m_v[3][2] = (double) -((double) num1 * (double) num30 - (double) num2 * (double) num32 + (double) num3 * (double) num33) * num27;
		double num34 = (double) ((double) num7 * (double) num12 - (double) num8 * (double) num11);
		double num35 = (double) ((double) num6 * (double) num12 - (double) num8 * (double) num10);
		double num36 = (double) ((double) num6 * (double) num11 - (double) num7 * (double) num10);
		double num37 = (double) ((double) num5 * (double) num12 - (double) num8 * (double) num9);
		double num38 = (double) ((double) num5 * (double) num11 - (double) num7 * (double) num9);
		double num39 = (double) ((double) num5 * (double) num10 - (double) num6 * (double) num9);
		result->m_v[0][3] = (double) -((double) num2 * (double) num34 - (double) num3 * (double) num35 + (double) num4 * (double) num36) * num27;
		result->m_v[1][3] = (double) ((double) num1 * (double) num34 - (double) num3 * (double) num37 + (double) num4 * (double) num38) * num27;
		result->m_v[2][3] = (double) -((double) num1 * (double) num35 - (double) num2 * (double) num37 + (double) num4 * (double) num39) * num27;
		result->m_v[3][3] = (double) ((double) num1 * (double) num36 - (double) num2 * (double) num38 + (double) num3 * (double) num39) * num27;
	}

	// static methods for world matrix creation
	static void CreateTranslation(const Vector3d &translation, Matrix *matrix)
	{
		matrix->m_v[0][0] = 1.0f;
		matrix->m_v[0][1] = 0.0f;
		matrix->m_v[0][2] = 0.0f;
		matrix->m_v[0][3] = 0.0f;

		matrix->m_v[1][0] = 0.0f;
		matrix->m_v[1][1] = 1.0f;
		matrix->m_v[1][2] = 0.0f;
		matrix->m_v[1][3] = 0.0f;

		matrix->m_v[2][0] = 0.0f;
		matrix->m_v[2][1] = 0.0f;
		matrix->m_v[2][2] = 1.0f;
		matrix->m_v[2][3] = 0.0f;

		matrix->m_v[3][0] = translation.x();
		matrix->m_v[3][1] = translation.y();
		matrix->m_v[3][2] = translation.z();
		matrix->m_v[3][3] = 1.0f;
	}

	static void CreateRotationX(double rad, Matrix *matrix)
	{
		double cosinus = cos(rad);
		double sinus = sin(rad);

		matrix->m_v[0][0] = 1.0f;
		matrix->m_v[0][1] = 0.0f;
		matrix->m_v[0][2] = 0.0f;
		matrix->m_v[0][3] = 0.0f;

		matrix->m_v[1][0] = 0.0f;
		matrix->m_v[1][1] = cosinus;
		matrix->m_v[1][2] = sinus;
		matrix->m_v[1][3] = 0.0f;

		matrix->m_v[2][0] = 0.0f;
		matrix->m_v[2][1] = -sinus;
		matrix->m_v[2][2] = cosinus;
		matrix->m_v[2][3] = 0.0f;

		matrix->m_v[3][0] = 0.0f;
		matrix->m_v[3][1] = 0.0f;
		matrix->m_v[3][2] = 0.0f;
		matrix->m_v[3][3] = 1.0f;

	}

	static void CreateRotationY(double rad, Matrix *matrix)
	{
		double cosinus = cos(rad);
		double sinus = sin(rad);

		matrix->m_v[0][0] = cosinus;
		matrix->m_v[0][1] = 0.0f;
		matrix->m_v[0][2] = -sinus;
		matrix->m_v[0][3] = 0.0f;

		matrix->m_v[1][0] = 0.0f;
		matrix->m_v[1][1] = 1.0f;
		matrix->m_v[1][2] = 0.0;
		matrix->m_v[1][3] = 0.0f;

		matrix->m_v[2][0] = sinus;
		matrix->m_v[2][1] = 0.0f;
		matrix->m_v[2][2] = cosinus;
		matrix->m_v[2][3] = 0.0f;

		matrix->m_v[3][0] = 0.0f;
		matrix->m_v[3][1] = 0.0f;
		matrix->m_v[3][2] = 0.0f;
		matrix->m_v[3][3] = 1.0f;
	}

	static void CreateRotationZ(double rad, Matrix *matrix)
	{
		double cosinus = cos(rad);
		double sinus = sin(rad);

		matrix->m_v[0][0] = cosinus;
		matrix->m_v[0][1] = sinus;
		matrix->m_v[0][2] = 0.0f;
		matrix->m_v[0][3] = 0.0f;

		matrix->m_v[1][0] = -sinus;
		matrix->m_v[1][1] = cosinus;
		matrix->m_v[1][2] = 0.0;
		matrix->m_v[1][3] = 0.0f;

		matrix->m_v[2][0] = 0.0f;
		matrix->m_v[2][1] = 0.0f;
		matrix->m_v[2][2] = 1.0f;
		matrix->m_v[2][3] = 0.0f;

		matrix->m_v[3][0] = 0.0f;
		matrix->m_v[3][1] = 0.0f;
		matrix->m_v[3][2] = 0.0f;
		matrix->m_v[3][3] = 1.0f;
	}

	static void CreateScale(double x, double y, double z, Matrix *matrix)
	{
		matrix->m_v[0][0] = x;
		matrix->m_v[0][1] = 0.0f;
		matrix->m_v[0][2] = 0.0f;
		matrix->m_v[0][3] = 0.0f;

		matrix->m_v[1][0] = 0.0f;
		matrix->m_v[1][1] = y;
		matrix->m_v[1][2] = 0.0;
		matrix->m_v[1][3] = 0.0f;

		matrix->m_v[2][0] = 0.0f;
		matrix->m_v[2][1] = 0.0f;
		matrix->m_v[2][2] = z;
		matrix->m_v[2][3] = 0.0f;

		matrix->m_v[3][0] = 0.0f;
		matrix->m_v[3][1] = 0.0f;
		matrix->m_v[3][2] = 0.0f;
		matrix->m_v[3][3] = 1.0f;
	}

	static void transform(const Vector3d &v, const Matrix &matrix, Vector3d &result)
	{
		double num1 =  ((double) v.x() * (double) matrix.m_v[0][0] + (double) v.y() * (double) matrix.m_v[1][0] + (double) v.z() * (double) matrix.m_v[2][0]) + matrix.m_v[3][0];
		double num2 = ((double) v.x() * (double) matrix.m_v[0][1] + (double) v.y() * (double) matrix.m_v[1][1] + (double) v.z() * (double) matrix.m_v[2][1]) + matrix.m_v[3][1];
		double num3 = ((double) v.x() * (double) matrix.m_v[0][2] + (double) v.y() * (double) matrix.m_v[1][2] + (double) v.z() * (double) matrix.m_v[2][2]) + matrix.m_v[3][2];
		result.x() = num1;
		result.y() = num2;
		result.z() = num3;
	}

	void copy_from(const Matrix &matrix);
private:
    double m_v[4][4];
};


#endif