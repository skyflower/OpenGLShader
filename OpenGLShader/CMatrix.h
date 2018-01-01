#ifndef CMATRIX_H_INCLUDED
#define CMATRIX_H_INCLUDED

#include <iostream>
#include <cmath>
#include <algorithm>


template<size_t N, size_t M, typename type>
class CMatrix
{
public:
    CMatrix()
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] = type();
            }
        }
    }

    template<typename InputIterator>
    CMatrix(InputIterator &First, InputIterator &End)
    {
        InputIterator Begin = First;
        size_t i = 0;
        size_t j = 0;
        while(Begin != End)
        {
            m_data[i][j++] = *Begin++;
            if(j == M)
            {
                i++;
                j = 0;
            }
            if(i >= N)
            {
                break;
            }
        }
    }

    template<typename Input>
    CMatrix(Input &First, size_t n)
    {
        Input Begin = First;
        size_t i = 0;
        size_t j = 0;
        size_t k = 0;
        while(k < n)
        {
            m_data[i][j++] = *Begin++;
            if(j == M)
            {
                i++;
                j = 0;
            }
            if(i >= N)
            {
                break;
            }
            k++;
        }
    }

    template<typename Num>
    CMatrix(Num *First, Num *End)
    {
        size_t i = 0;
        size_t j = 0;
        while(First != End)
        {
            m_data[i][j++] = *First++;
            if(j == M)
            {
                i++;
                j = 0;
            }
            if(i >= N)
            {
                break;
            }
        }
    }

    template<typename Num>
    CMatrix(Num *First, size_t n)
    {
        size_t i = 0;
        size_t j = 0;
        size_t k = 0;
        while(k < n)
        {
            m_data[i][j++] = First[k++];
            if(j == M)
            {
                i++;
                j = 0;
            }
            if(i >= N)
            {
                break;
            }
        }
    }

    size_t GetRow()
    {
        return N;
    }
    size_t GetCol()
    {
        return M;
    }

    type *operator[](size_t x)
    {
        if((x >= N))
        {
            throw "Index out of range";
        }
        return &m_data[x][0];
    }
    const type* operator[](size_t x)const
    {
        if((x >= N))// || (y >= M))
        {
            throw "Index out of Range";
        }
        return &m_data[x][0];
    }

	
	CMatrix<M, N, type>  Transposition()
	{
		CMatrix<M, N, type> result;
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < M; ++j)
			{
				result[j][i] = this->m_data[i][j];
			}
		}
		return result;
	}



    decltype(type(0) * type(0)) GetValue() const
    {
        if(N != M)
        {
            throw "row is not equal col, no reverse Matrix";
            return type(0);
        }

        short int *tmpList = new short int[N];
        //memset(tmpList, 0, sizeof(short int) * (N + 1));
        for(size_t i = 0; i < N; ++i)
        {
            tmpList[i] = i;
        }
        using reType = decltype(type(0) * type(0));
        reType result = 0;
        reType tmpValue = 0;
        bool bNeg = false;
        while(std::next_permutation(tmpList, tmpList + N))
        {
            size_t i = 0;
            tmpValue = 1;
            for(size_t i = 0; i < N; ++i)
            {
                tmpValue *= m_data[i][tmpList[i]];
            }
            result += (bNeg == false ? tmpValue : -tmpValue);
            bNeg = !bNeg;
        }
		delete[]tmpList;
		tmpList = nullptr;
        return result;
    }
	CMatrix<N - 1, M - 1, type> reduceDimension()
	{
		if ((N < 2) || (M < 2))
		{
			throw "reduce dimension error";
		}
		CMatrix<N - 1, M - 1, type> result;
		for (size_t i = 0; i < N - 1; ++i)
		{
			for (size_t j = 0; j < M - 1; ++j)
			{
				result[i][j] = m_data[i][j];
			}
		}
		return result;
	}

    CMatrix<N, M, type> inverse()
    {
		if (GetValue() == type(0))
		{
			throw "row is not equal col, no reverse Matrix";
			return CMatrix<N, M, float>();
		}
        CMatrix<N, 2 * M, type> result;
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < M; ++j)
			{
				result[i][j] = m_data[i][j];
			}
			result[i][i + N] = type(1);
		}
		std::function<size_t(size_t Begin)> lambda = [&](size_t begin)->size_t
		{
			size_t maxIndex = begin;
			type maxValue = result[maxIndex][begin] > 0 ? result[maxIndex][begin] : -result[maxIndex][begin];

			for (size_t i = begin; i < N; ++i)
			{
				if ((result[i][begin] > maxValue) || (-result[i][begin] < -maxValue))
				{
					maxIndex = i;
					maxValue = result[maxIndex][begin] > 0 ? result[maxIndex][begin] : -result[maxIndex][begin];
				}
			}
			return maxIndex;
		};
		std::function<void(size_t first, size_t second)> RowExchange = \
			[&](size_t first, size_t second)->void
		{
			for (size_t i = 0; i < 2 * M; ++i)
			{
				type tmp = result[first][i];
				result[first][i] = result[second][i];
				result[second][i] = tmp;
			}
		};


		for (size_t i = 0; i < M; ++i)
		{
			size_t tmpIndex = lambda(i);
			if (i != tmpIndex)
			{
				RowExchange(i, tmpIndex);
			}
			for (size_t j = 0; j < N; ++j)
			{
				if ((result[j][i] != type(0)) && (i != j))
				{
					type factor = -result[j][i] / result[i][i];
					for (size_t k = i; k < 2 * M; ++k)
					{
						result[j][k] += factor * result[i][k];
					}
				}
			}
			for (size_t k = i + 1; k < 2 * M; ++k)
			{
				result[i][k] = result[i][k] / result[i][i];
			}
			result[i][i] = 1;
		}
		CMatrix<N, M, type> reValue;
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < M; ++j)
			{
				reValue[i][j] = result[i][j + M];
			}
		}
        return reValue;
    }

    static CMatrix<N, N, type> GetUnit()
    {
        CMatrix<N, N, type> result;
        for(size_t i = 0; i < N; ++i)
        {
            result[i][i] = type(1);
        }
        return result;
    }

	static CMatrix<4, 4, float> GetPerspective(CVector<4, float> &a)
	{
		return GetPerspective(a[0], a[1], a[2], a[3]);
	}

	//
	static CMatrix<4, 4, float> GetPerspective(float alpha, float ratio, float zNear, float zFar)
	{
		//double tmpAlpha = 90 - alpha / 2;
		//double tmpAlpha = alpha * 3.14159265 / 180;
		double cot_halfAlpha = 1 / std::tan(alpha/2);
		//std::tan(tmpAlpha) = zNear / (height / 2);
		//double height = 2 * zNear / cot_halfAlpha;
		//double width = height * ratio;
		CMatrix<4, 4, float> result;
		result[0][0] = cot_halfAlpha / ratio;// 2 * zNear / width;
		result[1][1] = cot_halfAlpha;// 2 * zNear / height;
		result[2][2] = -(zFar + zNear) / (zFar - zNear);
		result[2][3] = -2 * zFar * zNear / (zFar - zNear);
		result[3][2] = -1;
		return result;
	}

	static CMatrix<4, 4, float> GetOrtho(CVector<6, float> &rhs)
	{
		return GetOrtho(rhs[0], rhs[1], rhs[2], rhs[3], rhs[4], rhs[5]);
	}
	static CMatrix<4, 4, float> GetOrtho(float left, float right, float bottom, float top, float Near, float Far)
	{
		CMatrix<4, 4, float> result;// = CMatrix<4, 4, float>::GetUnit();
		result.m_data[0][0] = 2 / (double)(right - left);
		result.m_data[1][1] = 2 / (double)(top - bottom);
		result.m_data[2][2] = -2 / (double)(Far - Near);
		result.m_data[0][3] = -(right + left) / (double)(right - left);
		result.m_data[1][3] = (top + bottom) / (double)(top - bottom);
		result.m_data[2][3] = -(Far + Near) / (double)(Far - Near);
		result.m_data[3][3] = 1;
		
		return result;
	}

	//
	static CMatrix<4, 4, float> GetViewMatrix(float x, float y, float width, float height, float zNear, float zFar)
	{
		CMatrix<4, 4, float> result;
		result[0][0] = width / 2;
		result[1][1] = height / 2;
		result[0][3] = x + width / 2;
		result[1][3] = y + height / 2;
		result[2][2] = (zFar - zNear) / 2;
		result[2][3] = (zFar + zNear) / 2;
		result[3][3] = 1;
		return result;
	}

	//
    static CMatrix<4, 4, float> GetTranslate(const CVector<3, type> &pos)
    {
		return GetTranslate(pos[0], pos[1], pos[2]);
    }
	//
    static CMatrix<4, 4, float> GetTranslate(type x, type y, type z)
    {
        CMatrix<4, 4, float> result = CMatrix<4, 4, float>::GetUnit();
        result[3][0] = x;
        result[3][1] = y;
        result[3][2] = z;
        return result;
    }



    static CMatrix<4, 4, float> GetScale(const CVector<3, type> &scale)
    {
		CMatrix<4, 4, float> result;// = CMatrix<4, 4, float>::GetUnit();
        result[0][0] = scale[0];
        result[1][1] = scale[1];
        result[2][2] = scale[2];
		result[3][3] = 1;
        return result;
    }
    static CMatrix<4, 4, float> GetScale(type x, type y, type z)
    {
		CMatrix<4, 4, float> result;// = CMatrix<4, 4, float>::GetUnit();
        result[0][0] = x;
        result[1][1] = y;
        result[2][2] = z;
		result[3][3] = 1;
        return result;
    }


    static CMatrix<4, 4, float> GetRotate(int label, float rotate)
    {
		//rotate = rotate * 3.1415926 / 180;
		// 与数学库函数(std::sin, std::cos)保持一致
        CMatrix<4, 4, float> result = CMatrix<4, 4, float>::GetUnit();
        if(label == 1)
        {
            result[1][1] = result[2][2] = std::cos(rotate);
            result[2][1] = std::sin(rotate);
            result[1][2] = -result[2][1];
        }
        else if(label == 2)
        {
            result[0][0] = result[2][2] = std::cos(rotate);
            result[2][0] = std::sin(rotate);
            result[0][2] = -result[2][0];
        }
        else if(label == 3)
        {
            result[0][0] = result[1][1] = std::cos(rotate);
            result[0][1] = std::sin(rotate);
            result[1][0] = -result[0][1];
        }
        else
        {
            throw "illegal param label";
        }
        return result;
    }

	//
	static CMatrix<4, 4, float> GetRotate(float angle, float x, float y, float z)
	{
		CMatrix<4, 4, float> result;

		vec3f tmpLabel;
		tmpLabel[0] = x;
		tmpLabel[1] = y;
		tmpLabel[2] = z;
		tmpLabel = tmpLabel.Normalize();
		x = tmpLabel[0];
		y = tmpLabel[1];
		z = tmpLabel[2];

		float C = cosf(angle);
		float S = sinf(angle);

		vec3f tmpX;// (C + x * x * (1 - C), )
		result[0][0] = C + x * x * (1 - C);
		result[0][1] = x * y * (1 - C) - z * S;
		result[0][2] = x * z * (1 - C) + y * S;
		//newDirector[0] = tmpX.dotMultiply(viewDirector);
		//vec3f tmpY;
		result[1][0] = x * y * (1 - C) + z * S;
		result[1][1] = C + y * y * (1 - C);
		result[1][2] = y * z * (1 - C) - x * S;
		//newDirector[1] = tmpY.dotMultiply(viewDirector);

		//vec3f tmpZ;
		result[2][0] = x * z * (1 - C) - y * S;
		result[2][1] = y * z * (1 - C) + x * S;
		result[2][2] = C + z * z * (1 - C);
		//newDirector[2] = tmpZ.dotMultiply(viewDirector);
		result[3][3] = 1;
		return result;
	}

	type *GetData()
	{
		return &m_data[0][0];
	}

    template<typename Number>
    CMatrix<N, M, type>& operator=(const CMatrix<N, M, Number> &a)
    {
        if(&a == this)
        {
            return *this;
        }
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] = a.m_data[i][j];
            }
        }
        return *this;
    }

    template<typename Number>
    CMatrix<N, M, type>& operator*=(CMatrix<N, M, Number> &a)
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] = m_data[i][j] * a.m_data[i][j];
            }
        }
        return *this;
    }

    template<typename Number>
    CMatrix<N, M, type>& operator+=(CMatrix<N, M, Number> &a)
    {

        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] = m_data[i][j] + a.m_data[i][j];
            }
        }
        return *this;
    }

    template<typename Number>
    CMatrix<N, M, type>& operator-=(CMatrix<N, M, Number> &a)
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] = m_data[i][j] - a.m_data[i][j];
            }
        }
        return *this;
    }

    template<typename Number>
    CMatrix<N, M, type>& operator/=(CMatrix<N, M, Number> &a)
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] = m_data[i][j] / a.m_data[i][j];
            }
        }
        return *this;
    }

    //////////////////////////////////////////////

    template<typename Number>
    CMatrix<N, M, type>& operator*=(Number k)
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] *= k;
            }
        }
        return *this;
    }

    template<typename Number>
    CMatrix<N, M, type>& operator/=(Number k)
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] /= k;
            }
        }
        return *this;
    }

    template<typename Number>
    CMatrix<N, M, type>& operator+=(Number k)
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] += k;
            }
        }
        return *this;
    }

    template<typename Number>
    CMatrix<N, M, type>& operator-=(Number k)
    {
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                m_data[i][j] -= k;
            }
        }
        return *this;
    }

    ////////////////////////////////////////////////
//    template<size_t K, size_t L, typename Number>
//    friend bool operator==(CMatrix<N, M, type> &a, CMatrix<K, L, Number> *b);
//
//    template<size_t K, size_t L, typename Number>
//    friend bool operator!=(CMatrix<N, M, type> &a, CMatrix<K, L, Number> *b);


    template<size_t K, size_t L, typename Number>
    bool operator==(CMatrix<K, L, Number> *b)
    {
        if((N != K) || (M != L) || (typeid(m_data[0]) != typeid(b.m_data[0])))
        {
            return false;
        }
        for(size_t i = 0; i < N; ++i)
        {
            for(size_t j = 0; j < M; ++j)
            {
                if(m_data[i][j] != b.m_data[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    template<size_t K, size_t L, typename Number>
    bool operator!=(CMatrix<K, L, Number> *b)
    {
        return !this->operator==(b);
    }


    ////////////////////////////////////////////////


    template<size_t L, size_t S, typename str, size_t K, typename Number>
    friend CMatrix<L, K, decltype(str(0) * Number(0))>
    operator*(CMatrix<L, S, str>&a, CMatrix<S, K, Number> &b);

    template<size_t L, size_t S, typename Num, typename Number>
    friend CVector<L, decltype(Num(0) * Number(0))>
    operator*(CMatrix<L, S, Num> &a, CVector<S, Number> &b);

    template<size_t L, size_t S, typename Num, typename Number>
    friend CVector<S, decltype(Num(0) * Number(0))>
    operator*(CVector<L, Number> &a, CMatrix<L, S, Num> &b);


    ///////////////////////////////////////////////////

    template<typename Number>
    friend auto operator*(Number k, CMatrix<N, M, type> &a)->   \
        CMatrix<N, M, decltype(type(0) * Number(0))>;

    template<typename Number>
    friend auto operator/(Number k, CMatrix<N, M, type> &a)->   \
        CMatrix<N, M, decltype(Number(0) / type(0))>;

    template<typename Number>
    friend auto operator+(Number k, CMatrix<N, M, type> &a)->   \
        CMatrix<N, M, decltype(type(0) + Number(0))>;

    template<typename Number>
    friend auto operator-(Number k, CMatrix<N, M, type> &a)->   \
        CMatrix<N, M, decltype(Number(0) - type(0))>;

    ////////////////////////////////////////////
    template<typename Number>
    friend auto operator*(CMatrix<N, M, type> &a, Number k)->   \
        CMatrix<N, M, decltype(type(0) * Number(0))>;


    template<typename Number>
    friend auto operator+(CMatrix<N, M, type> &a, Number k)->   \
    CMatrix<N, M, decltype(type(0) + Number(0))>;

    template<typename Number>
    friend auto operator/(CMatrix<N, M, type> &a, Number k)->   \
        CMatrix<N, M, decltype(type(0) / Number(0))>;

    template<typename Number>
    friend auto operator-(CMatrix<N, M, type> &a, Number k)->   \
        CMatrix<N, M, decltype(type(0) - Number(0))>;

    template<size_t K, size_t L, typename Num>
    friend std::ostream &operator<<(std::ostream &out, const CMatrix<K, L, Num> &a);

private:
    type m_data[N][M];

};

//template<size_t N, size_t M, typename type, size_t K, size_t L, typename Number>
//bool operator==(CMatrix<N, M, type> &a, CMatrix<K, L, Number> *b)
//{
//    if((N != K) || (M != L) || (typeid(a.m_data[0]) != typeid(b.m_data[0])))
//    {
//        return false;
//    }
//    for(size_t i = 0; i < N; ++i)
//    {
//        for(size_t j = 0; j < M; ++j)
//        {
//            if(a.m_data[i][j] != b.m_data[i][j])
//            {
//                return false;
//            }
//        }
//    }
//    return true;
//}
//
//template<size_t N, size_t M, typename type, size_t K, size_t L, typename Number>
//bool operator!=(CMatrix<N, M, type> &a, CMatrix<K, L, Number> *b)
//{
//    return !operator==(a, b);
//}



template<size_t L, size_t S, typename str, size_t K, typename Number>
CMatrix<L, K, decltype(str(0) * Number(0))>
operator*(CMatrix<L, S, str>&a, CMatrix<S, K, Number> &b)
{
    using reType = decltype(str(0) * Number(0));
    CMatrix<L, K, reType> result;
    for(size_t i = 0; i < L; ++i)
    {
        for(size_t j = 0; j < K; ++j)
        {
            result[i][j] = 0;
            for(size_t t = 0; t < S; ++t)
            {
                result[i][j] += a[i][t] * b[t][j];
            }
        }
    }
    return result;
}


template<size_t L, size_t S, typename Num, typename Number>
CVector<L, decltype(Num(0) * Number(0))>
operator*(CMatrix<L, S, Num> &a, CVector<S, Number> &b)
{
    using reType = decltype(Num(0) * Number(0));
    CVector<L, reType> result;
    for(size_t i = 0; i < L; ++i)
    {
        result[i] = 0;
        for(size_t j = 0; j < S; ++j)
        {
            result[i] += a.m_data[i][j] * b[j];
        }
    }
    return result;
}


template<size_t L, size_t S, typename Num, typename Number>
CVector<S, decltype(Num(0) * Number(0))>
operator*(CVector<L, Number> &a, CMatrix<L, S, Num> &b)
{
    using reType = decltype(Num(0) * Number(0));
    CVector<S, reType> result;
    for(size_t i = 0; i < S; ++i)
    {
        result[i] = 0;
        for(size_t j = 0; j < L; ++j)
        {
            result[i] += a[j] * b.m_data[j][i];
        }
    }
    return result;
}



///////////////////////////////////////////////////

template<size_t N, size_t M,typename type, typename Number>
auto operator*(Number k, CMatrix<N, M, type> &a)->   \
CMatrix<N, M, decltype(type(0) * Number(0))>
{
    return operator*(a, k);
}

template<size_t N, size_t M,typename type, typename Number>
auto operator/(Number k, CMatrix<N, M, type> &a)->   \
CMatrix<N, M, decltype(Number(0) / type(0))>
{
    using reType = decltype(Number(0) / type(0));
    CMatrix<N, M, reType> result;
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = 0; j < M; ++j)
        {
            result[i][j] = k / a.m_data[i][j];
        }
    }
    return result;
}


template<size_t N, size_t M, typename type,typename Number>
auto operator+(Number k, CMatrix<N, M, type> &a)->   \
CMatrix<N, M, decltype(type(0) + Number(0))>
{
    return operator+(a, k);
}

template<size_t N, size_t M, typename type,typename Number>
auto operator-(Number k, CMatrix<N, M, type> &a)->   \
CMatrix<N, M, decltype(Number(0) - type(0))>
{
    using reType = decltype(Number(0) - type(0));
    CMatrix<N, M, reType> result;
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = 0; j < M; ++j)
        {
            result[i][j] = k - a.m_data[i][j];
        }
    }
    return result;
}


////////////////////////////////////////////
template<size_t N, size_t M, typename type,typename Number>
auto operator*(CMatrix<N, M, type> &a, Number k)->   \
CMatrix<N, M, decltype(type(0) * Number(0))>
{
    using reType = decltype(type(0) * Number(0));
    CMatrix<N, M, reType> result;
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = 0; j < M; ++j)
        {
            result[i][j] = a.m_data[i][j] * k;
        }
    }
    return result;
}

template<size_t N, size_t M, typename type,typename Number>
auto operator+(CMatrix<N, M, type> &a, Number k)->   \
CMatrix<N, M, decltype(type(0) + Number(0))>
{
    using reType = decltype(type(0) + Number(0));
    CMatrix<N, M, reType> result;
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = 0; j < M; ++j)
        {
            result[i][j] = a.m_data[i][j] + k;
        }
    }
    return result;
}




template<size_t N, size_t M, typename type,typename Number>
auto operator/(CMatrix<N, M, type> &a, Number k)->   \
CMatrix<N, M, decltype(type(0) / Number(0))>
{
    using reType = decltype(type(0) / Number(0));
    CMatrix<N, M, reType> result;
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = 0; j < M; ++j)
        {
            result[i][j] = a.m_data[i][j] / k;
        }
    }
    return result;
}

template<size_t N, size_t M, typename type,typename Number>
auto operator-(CMatrix<N, M, type> &a, Number k)->   \
CMatrix<N, M, decltype(type(0) - Number(0))>
{
    using reType = decltype(type(0) - Number(0));
    CMatrix<N, M, reType> result;
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = 0; j < M; ++j)
        {
            result[i][j] = a.m_data[i][j] - k;
        }
    }
    return result;
}



template<size_t K, size_t L, typename Num>
std::ostream &operator<<(std::ostream &out, const CMatrix<K, L, Num> &a)
{
    for(size_t i = 0; i < K; ++i)
    {
        for(size_t j = 0; j < L; ++j)
        {
            out << a.m_data[i][j] << "\t";
        }
        out << "\n";
    }
	out << "\n";
    return out;
}


#endif // CMATRIX_H_INCLUDED
