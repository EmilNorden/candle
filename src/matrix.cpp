#include "matrix.h"
#include "vector.h"
#include <cstring>
#include <cmath>

void Matrix::make_identity()
{
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			m_v[i][j] = 0;

	m_v[0][0] = 1;
	m_v[1][1] = 1;
	m_v[2][2] = 1;
	m_v[3][3] = 1;
}

Matrix Matrix::operator*(const Matrix &other) const
{
	Matrix result;

	result.m_v[0][0] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][0] + (double) this->m_v[0][1] * (double) other.m_v[1][0] + (double) this->m_v[0][2] * (double) other.m_v[2][0] + (double) this->m_v[0][3] * (double) other.m_v[3][0]);
	result.m_v[0][1] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][1] + (double) this->m_v[0][1] * (double) other.m_v[1][1] + (double) this->m_v[0][2] * (double) other.m_v[2][1] + (double) this->m_v[0][3] * (double) other.m_v[3][1]);
	result.m_v[0][2] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][2] + (double) this->m_v[0][1] * (double) other.m_v[1][2] + (double) this->m_v[0][2] * (double) other.m_v[2][2] + (double) this->m_v[0][3] * (double) other.m_v[3][2]);
	result.m_v[0][3] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][3] + (double) this->m_v[0][1] * (double) other.m_v[1][3] + (double) this->m_v[0][2] * (double) other.m_v[2][3] + (double) this->m_v[0][3] * (double) other.m_v[3][3]);
	result.m_v[1][0] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][0] + (double) this->m_v[1][1] * (double) other.m_v[1][0] + (double) this->m_v[1][2] * (double) other.m_v[2][0] + (double) this->m_v[1][3] * (double) other.m_v[3][0]);
	result.m_v[1][1] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][1] + (double) this->m_v[1][1] * (double) other.m_v[1][1] + (double) this->m_v[1][2] * (double) other.m_v[2][1] + (double) this->m_v[1][3] * (double) other.m_v[3][1]);
	result.m_v[1][2] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][2] + (double) this->m_v[1][1] * (double) other.m_v[1][2] + (double) this->m_v[1][2] * (double) other.m_v[2][2] + (double) this->m_v[1][3] * (double) other.m_v[3][2]);
	result.m_v[1][3] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][3] + (double) this->m_v[1][1] * (double) other.m_v[1][3] + (double) this->m_v[1][2] * (double) other.m_v[2][3] + (double) this->m_v[1][3] * (double) other.m_v[3][3]);
	result.m_v[2][0] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][0] + (double) this->m_v[2][1] * (double) other.m_v[1][0] + (double) this->m_v[2][2] * (double) other.m_v[2][0] + (double) this->m_v[2][3] * (double) other.m_v[3][0]);
	result.m_v[2][1] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][1] + (double) this->m_v[2][1] * (double) other.m_v[1][1] + (double) this->m_v[2][2] * (double) other.m_v[2][1] + (double) this->m_v[2][3] * (double) other.m_v[3][1]);
	result.m_v[2][2] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][2] + (double) this->m_v[2][1] * (double) other.m_v[1][2] + (double) this->m_v[2][2] * (double) other.m_v[2][2] + (double) this->m_v[2][3] * (double) other.m_v[3][2]);
	result.m_v[2][3] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][3] + (double) this->m_v[2][1] * (double) other.m_v[1][3] + (double) this->m_v[2][2] * (double) other.m_v[2][3] + (double) this->m_v[2][3] * (double) other.m_v[3][3]);
	result.m_v[3][0] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][0] + (double) this->m_v[3][1] * (double) other.m_v[1][0] + (double) this->m_v[3][2] * (double) other.m_v[2][0] + (double) this->m_v[3][3] * (double) other.m_v[3][0]);
	result.m_v[3][1] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][1] + (double) this->m_v[3][1] * (double) other.m_v[1][1] + (double) this->m_v[3][2] * (double) other.m_v[2][1] + (double) this->m_v[3][3] * (double) other.m_v[3][1]);
	result.m_v[3][2] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][2] + (double) this->m_v[3][1] * (double) other.m_v[1][2] + (double) this->m_v[3][2] * (double) other.m_v[2][2] + (double) this->m_v[3][3] * (double) other.m_v[3][2]);
	result.m_v[3][3] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][3] + (double) this->m_v[3][1] * (double) other.m_v[1][3] + (double) this->m_v[3][2] * (double) other.m_v[2][3] + (double) this->m_v[3][3] * (double) other.m_v[3][3]);

	return result;
}

Matrix &Matrix::operator*=(const Matrix &other)
{
	double v[4][4];

	v[0][0] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][0] + (double) this->m_v[0][1] * (double) other.m_v[1][0] + (double) this->m_v[0][2] * (double) other.m_v[2][0] + (double) this->m_v[0][3] * (double) other.m_v[3][0]);
	v[0][1] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][1] + (double) this->m_v[0][1] * (double) other.m_v[1][1] + (double) this->m_v[0][2] * (double) other.m_v[2][1] + (double) this->m_v[0][3] * (double) other.m_v[3][1]);
	v[0][2] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][2] + (double) this->m_v[0][1] * (double) other.m_v[1][2] + (double) this->m_v[0][2] * (double) other.m_v[2][2] + (double) this->m_v[0][3] * (double) other.m_v[3][2]);
	v[0][3] = (double) ((double) this->m_v[0][0] * (double) other.m_v[0][3] + (double) this->m_v[0][1] * (double) other.m_v[1][3] + (double) this->m_v[0][2] * (double) other.m_v[2][3] + (double) this->m_v[0][3] * (double) other.m_v[3][3]);
	v[1][0] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][0] + (double) this->m_v[1][1] * (double) other.m_v[1][0] + (double) this->m_v[1][2] * (double) other.m_v[2][0] + (double) this->m_v[1][3] * (double) other.m_v[3][0]);
	v[1][1] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][1] + (double) this->m_v[1][1] * (double) other.m_v[1][1] + (double) this->m_v[1][2] * (double) other.m_v[2][1] + (double) this->m_v[1][3] * (double) other.m_v[3][1]);
	v[1][2] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][2] + (double) this->m_v[1][1] * (double) other.m_v[1][2] + (double) this->m_v[1][2] * (double) other.m_v[2][2] + (double) this->m_v[1][3] * (double) other.m_v[3][2]);
	v[1][3] = (double) ((double) this->m_v[1][0] * (double) other.m_v[0][3] + (double) this->m_v[1][1] * (double) other.m_v[1][3] + (double) this->m_v[1][2] * (double) other.m_v[2][3] + (double) this->m_v[1][3] * (double) other.m_v[3][3]);
	v[2][0] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][0] + (double) this->m_v[2][1] * (double) other.m_v[1][0] + (double) this->m_v[2][2] * (double) other.m_v[2][0] + (double) this->m_v[2][3] * (double) other.m_v[3][0]);
	v[2][1] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][1] + (double) this->m_v[2][1] * (double) other.m_v[1][1] + (double) this->m_v[2][2] * (double) other.m_v[2][1] + (double) this->m_v[2][3] * (double) other.m_v[3][1]);
	v[2][2] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][2] + (double) this->m_v[2][1] * (double) other.m_v[1][2] + (double) this->m_v[2][2] * (double) other.m_v[2][2] + (double) this->m_v[2][3] * (double) other.m_v[3][2]);
	v[2][3] = (double) ((double) this->m_v[2][0] * (double) other.m_v[0][3] + (double) this->m_v[2][1] * (double) other.m_v[1][3] + (double) this->m_v[2][2] * (double) other.m_v[2][3] + (double) this->m_v[2][3] * (double) other.m_v[3][3]);
	v[3][0] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][0] + (double) this->m_v[3][1] * (double) other.m_v[1][0] + (double) this->m_v[3][2] * (double) other.m_v[2][0] + (double) this->m_v[3][3] * (double) other.m_v[3][0]);
	v[3][1] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][1] + (double) this->m_v[3][1] * (double) other.m_v[1][1] + (double) this->m_v[3][2] * (double) other.m_v[2][1] + (double) this->m_v[3][3] * (double) other.m_v[3][1]);
	v[3][2] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][2] + (double) this->m_v[3][1] * (double) other.m_v[1][2] + (double) this->m_v[3][2] * (double) other.m_v[2][2] + (double) this->m_v[3][3] * (double) other.m_v[3][2]);
	v[3][3] = (double) ((double) this->m_v[3][0] * (double) other.m_v[0][3] + (double) this->m_v[3][1] * (double) other.m_v[1][3] + (double) this->m_v[3][2] * (double) other.m_v[2][3] + (double) this->m_v[3][3] * (double) other.m_v[3][3]);

	memcpy(this->m_v, v, sizeof(v));

	return *this;
}

Matrix Matrix::Invert() const
{
	Matrix result;

	double num1 = m_v[0][0];
      double num2 = m_v[0][1];
      double num3 = m_v[0][2];
      double num4 = m_v[0][3];
      double num5 = m_v[1][0];
      double num6 = m_v[1][1];
      double num7 = m_v[1][2];
      double num8 = m_v[1][3];
      double num9 = m_v[2][0];
      double num10 = m_v[2][1];
      double num11 = m_v[2][2];
      double num12 = m_v[2][3];
      double num13 = m_v[3][0];
      double num14 = m_v[3][1];
      double num15 = m_v[3][2];
      double num16 = m_v[3][3];
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
      
      result.m_v[0][0] = num23 * num27;
      result.m_v[1][0] = num24 * num27;
      result.m_v[2][0] = num25 * num27;
      result.m_v[3][0] = num26 * num27;
      result.m_v[0][1] = (double) -((double) num2 * (double) num17 - (double) num3 * (double) num18 + (double) num4 * (double) num19) * num27;
      result.m_v[1][1] = (double) ((double) num1 * (double) num17 - (double) num3 * (double) num20 + (double) num4 * (double) num21) * num27;
      result.m_v[2][1] = (double) -((double) num1 * (double) num18 - (double) num2 * (double) num20 + (double) num4 * (double) num22) * num27;
      result.m_v[3][1] = (double) ((double) num1 * (double) num19 - (double) num2 * (double) num21 + (double) num3 * (double) num22) * num27;
      double num28 = (double) ((double) num7 * (double) num16 - (double) num8 * (double) num15);
      double num29 = (double) ((double) num6 * (double) num16 - (double) num8 * (double) num14);
      double num30 = (double) ((double) num6 * (double) num15 - (double) num7 * (double) num14);
      double num31 = (double) ((double) num5 * (double) num16 - (double) num8 * (double) num13);
      double num32 = (double) ((double) num5 * (double) num15 - (double) num7 * (double) num13);
      double num33 = (double) ((double) num5 * (double) num14 - (double) num6 * (double) num13);
      result.m_v[0][2] = (double) ((double) num2 * (double) num28 - (double) num3 * (double) num29 + (double) num4 * (double) num30) * num27;
      result.m_v[1][2] = (double) -((double) num1 * (double) num28 - (double) num3 * (double) num31 + (double) num4 * (double) num32) * num27;
      result.m_v[2][2] = (double) ((double) num1 * (double) num29 - (double) num2 * (double) num31 + (double) num4 * (double) num33) * num27;
      result.m_v[3][2] = (double) -((double) num1 * (double) num30 - (double) num2 * (double) num32 + (double) num3 * (double) num33) * num27;
      double num34 = (double) ((double) num7 * (double) num12 - (double) num8 * (double) num11);
      double num35 = (double) ((double) num6 * (double) num12 - (double) num8 * (double) num10);
      double num36 = (double) ((double) num6 * (double) num11 - (double) num7 * (double) num10);
      double num37 = (double) ((double) num5 * (double) num12 - (double) num8 * (double) num9);
      double num38 = (double) ((double) num5 * (double) num11 - (double) num7 * (double) num9);
      double num39 = (double) ((double) num5 * (double) num10 - (double) num6 * (double) num9);
      result.m_v[0][3] = (double) -((double) num2 * (double) num34 - (double) num3 * (double) num35 + (double) num4 * (double) num36) * num27;
      result.m_v[1][3] = (double) ((double) num1 * (double) num34 - (double) num3 * (double) num37 + (double) num4 * (double) num38) * num27;
      result.m_v[2][3] = (double) -((double) num1 * (double) num35 - (double) num2 * (double) num37 + (double) num4 * (double) num39) * num27;
      result.m_v[3][3] = (double) ((double) num1 * (double) num36 - (double) num2 * (double) num38 + (double) num3 * (double) num39) * num27;
      return result;
}

void Matrix::Invert()
{
	double num1 = m_v[0][0];
      double num2 = m_v[0][1];
      double num3 = m_v[0][2];
      double num4 = m_v[0][3];
      double num5 = m_v[1][0];
      double num6 = m_v[1][1];
      double num7 = m_v[1][2];
      double num8 = m_v[1][3];
      double num9 = m_v[2][0];
      double num10 = m_v[2][1];
      double num11 = m_v[2][2];
      double num12 = m_v[2][3];
      double num13 = m_v[3][0];
      double num14 = m_v[3][1];
      double num15 = m_v[3][2];
      double num16 = m_v[3][3];
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
      
      m_v[0][0] = num23 * num27;
      m_v[1][0] = num24 * num27;
      m_v[2][0] = num25 * num27;
      m_v[3][0] = num26 * num27;
      m_v[0][1] = (double) -((double) num2 * (double) num17 - (double) num3 * (double) num18 + (double) num4 * (double) num19) * num27;
      m_v[1][1] = (double) ((double) num1 * (double) num17 - (double) num3 * (double) num20 + (double) num4 * (double) num21) * num27;
      m_v[2][1] = (double) -((double) num1 * (double) num18 - (double) num2 * (double) num20 + (double) num4 * (double) num22) * num27;
      m_v[3][1] = (double) ((double) num1 * (double) num19 - (double) num2 * (double) num21 + (double) num3 * (double) num22) * num27;
      double num28 = (double) ((double) num7 * (double) num16 - (double) num8 * (double) num15);
      double num29 = (double) ((double) num6 * (double) num16 - (double) num8 * (double) num14);
      double num30 = (double) ((double) num6 * (double) num15 - (double) num7 * (double) num14);
      double num31 = (double) ((double) num5 * (double) num16 - (double) num8 * (double) num13);
      double num32 = (double) ((double) num5 * (double) num15 - (double) num7 * (double) num13);
      double num33 = (double) ((double) num5 * (double) num14 - (double) num6 * (double) num13);
      m_v[0][2] = (double) ((double) num2 * (double) num28 - (double) num3 * (double) num29 + (double) num4 * (double) num30) * num27;
      m_v[1][2] = (double) -((double) num1 * (double) num28 - (double) num3 * (double) num31 + (double) num4 * (double) num32) * num27;
      m_v[2][2] = (double) ((double) num1 * (double) num29 - (double) num2 * (double) num31 + (double) num4 * (double) num33) * num27;
      m_v[3][2] = (double) -((double) num1 * (double) num30 - (double) num2 * (double) num32 + (double) num3 * (double) num33) * num27;
      double num34 = (double) ((double) num7 * (double) num12 - (double) num8 * (double) num11);
      double num35 = (double) ((double) num6 * (double) num12 - (double) num8 * (double) num10);
      double num36 = (double) ((double) num6 * (double) num11 - (double) num7 * (double) num10);
      double num37 = (double) ((double) num5 * (double) num12 - (double) num8 * (double) num9);
      double num38 = (double) ((double) num5 * (double) num11 - (double) num7 * (double) num9);
      double num39 = (double) ((double) num5 * (double) num10 - (double) num6 * (double) num9);
      m_v[0][3] = (double) -((double) num2 * (double) num34 - (double) num3 * (double) num35 + (double) num4 * (double) num36) * num27;
      m_v[1][3] = (double) ((double) num1 * (double) num34 - (double) num3 * (double) num37 + (double) num4 * (double) num38) * num27;
      m_v[2][3] = (double) -((double) num1 * (double) num35 - (double) num2 * (double) num37 + (double) num4 * (double) num39) * num27;
      m_v[3][3] = (double) ((double) num1 * (double) num36 - (double) num2 * (double) num38 + (double) num3 * (double) num39) * num27;
}

void Matrix::copy_from(const Matrix &matrix)
{
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			m_v[i][j] = matrix.m_v[i][j];
}