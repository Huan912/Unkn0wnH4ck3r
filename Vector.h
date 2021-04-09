class QAngle;
class Vector;

class QVector
{
public:

	QVector::QVector(void) : x(0.0f), y(0.0f), z(0.0f)
	{

	}

	QVector::QVector(float X, float Y, float Z)
		: x(X), y(Y), z(Z)
	{

	}

	QVector::QVector(float* p)
	{
		*this = p;
	};

	QVector::QVector(const QVector& in)
	{
		x = in.x;
		y = in.y;
		z = in.z;
	};

	QVector::~QVector()
	{

	}

	inline QVector& operator= (const QVector& in);
	inline QVector& operator= (float* p);
	inline QVector& operator= (float f);

	inline float& operator[] (int i) const;

	inline bool operator! (void) const;

	inline bool operator== (const QVector& other) const;

	inline bool operator!= (const QVector& other) const;

	inline QVector& operator+= (const QVector& other);
	inline QVector& operator+= (float* p);
	inline QVector& operator+= (float f);

	inline QVector& operator-= (const QVector& other);
	inline QVector& operator-= (float* p);
	inline QVector& operator-= (float f);

	inline QVector& operator*= (const QVector& other);
	inline QVector& operator*= (float* p);
	inline QVector& operator*= (float f);

	inline QVector& operator/= (const QVector& other);
	inline QVector& operator/= (float* p);
	inline QVector& operator/= (float f);

	inline QVector operator+ (const QVector& other) const;
	inline QVector operator+ (float* p) const;
	inline QVector operator+ (float f) const;

	inline QVector operator- (const QVector& other) const;
	inline QVector operator- (float* p) const;
	inline QVector operator- (float f) const;
	inline QVector operator- (void) const;

	inline QVector operator* (const QVector& other) const;
	inline QVector operator* (float* p) const;
	inline QVector operator* (float f) const;

	inline QVector operator/ (const QVector& other) const;
	inline QVector operator/ (float* p) const;
	inline QVector operator/ (float f) const;

	operator float* () { return &x; }
	operator const float* () const { return &x; }

	inline bool QVector::IsZero(void) const
	{
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	inline bool QVector::IsZero2D(void)
	{
		return x == 0.0f && y == 0.0f;
	}

	inline QVector& Clear(void)
	{
		x = y = z = 0;

		return *this;
	}

	inline QVector& Init(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;

		return *this;
	}

	inline QVector& Init(float* p)
	{
		*this = p;

		return *this;
	}

	inline QVector& Negate(void)
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}

	inline float Dot(const QVector& other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	inline float Dot2D(const QVector& other)
	{
		return x * other.x + y * other.y;
	}

	inline float Length(void)
	{
		float Length = 0;

		Length = sqrt(LengthSqr());

		return Length;
	}

	inline float Length2D(void)
	{
		float Length = 0;

		Length = sqrt(LengthSqr2D());

		return Length;
	}

	inline float LengthSqr(void)
	{
		return x * x + y * y + z * z;
	}

	inline float LengthSqr2D(void)
	{
		return x * x + y * y;
	}

	inline float Distance(QVector& ToVector)
	{
		return (ToVector - *this).Length();
	}

	inline float Distance2D(QVector& ToVector)
	{
		return (ToVector - *this).Length2D();
	}

	inline float Normalize(void)
	{
		float RecipLength, VectorLength;

		VectorLength = Length();

		if (VectorLength != 0)
		{
			RecipLength = 1 / VectorLength;

			x *= RecipLength;
			y *= RecipLength;
			z *= RecipLength;
		}

		return VectorLength;
	}

	inline float Normalize2D(void)
	{
		float Length = LengthSqr2D();
		float RecipLength;

		if (Length != 0)
		{
			RecipLength = 1 / Length;

			x *= RecipLength;
			y *= RecipLength;
		}

		return Length;
	}

	inline float AngleBetween(QVector& other)
	{
		other.Normalize();
		Normalize();

		return acos(Dot(other)) * 180 / M_PI;
	}

	QVector& CrossProduct(const QVector& a, const QVector& b)
	{
		x = (a.y * b.z) - (a.z * b.y);
		y = (a.z * b.x) - (a.x * b.z);
		z = (a.x * b.y) - (a.y * b.x);

		return *this;
	}
	QAngle ToEulerAngles();
	QAngle ToEulerAngles(QVector* PseudoUp);
	void AngleMatrix(QAngle& Rotation, float(*matrix)[3]);
	void VectorRotate(QVector& In, QAngle& Rotation);

	void VectorVectors(QVector& Right, QVector& Up)
	{
		QVector tmp;

		if (x == 0 && y == 0)
		{
			Right[0] = 1;
			Right[1] = 0;
			Right[2] = 0;
			Up[0] = -z;
			Up[1] = 0;
			Up[2] = 0;
			return;
		}

		tmp[0] = 0; tmp[1] = 0; tmp[2] = 1.0;
		CrossProduct(tmp, Right);
		Right.Normalize();
		Right.CrossProduct(*this, Up);
		Up.Normalize();
	}

	inline bool IsValid()
	{
		return isfinite(x) && isfinite(y) && isfinite(z);
	}

public:

	float x, y, z;
};

inline QVector& QVector::operator= (const QVector& in)
{
	x = in.x;
	y = in.y;
	z = in.z;

	return *this;
}

inline QVector& QVector::operator= (float* p)
{
	if (p)
	{
		x = p[0]; y = p[1]; z = p[2];
	}
	else
	{
		x = y = z = 0;
	}

	return *this;
}

inline QVector& QVector::operator= (float f)
{
	x = y = z = f;

	return *this;
}

inline float& QVector::operator[] (int i) const
{
	if (i >= 0 && i < 3)
	{
		return ((float*)this)[i];
	}

	return ((float*)this)[0];
}

inline bool QVector::operator! (void) const
{
	return IsZero();
}

inline bool QVector::operator== (const QVector& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

inline bool QVector::operator!= (const QVector& other) const
{
	return x != other.x || y != other.y || z != other.z;
}

inline QVector& QVector::operator+= (const QVector& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

inline QVector& QVector::operator+= (float* p)
{
	x += p[0];
	y += p[1];
	z += p[2];

	return *this;
}

inline QVector& QVector::operator+= (float f)
{
	x += f;
	y += f;
	z += f;

	return *this;
}

inline QVector& QVector::operator-= (const QVector& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

inline QVector& QVector::operator-= (float* p)
{
	x -= p[0];
	y -= p[1];
	z -= p[2];

	return *this;
}
inline QVector& QVector::operator-= (float f)
{
	x -= f;
	y -= f;
	z -= f;

	return *this;
}

inline QVector& QVector::operator*= (const QVector& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

inline QVector& QVector::operator*= (float* p)
{
	x *= p[0];
	y *= p[1];
	z *= p[2];

	return *this;
}

inline QVector& QVector::operator*= (float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

inline QVector& QVector::operator/= (const QVector& other)
{
	if (other.x != 0 && other.y != 0 && other.z != 0)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
	}

	return *this;
}

inline QVector& QVector::operator/= (float* p)
{
	if (p[0] != 0 && p[1] != 0 && p[2] != 0)
	{
		x /= p[0];
		y /= p[1];
		z /= p[2];
	}

	return *this;
}

inline QVector& QVector::operator/= (float f)
{
	if (f != 0)
	{
		x /= f;
		y /= f;
		z /= f;
	}

	return *this;
}

inline QVector QVector::operator+ (const QVector& other) const
{
	return QVector(x + other.x, y + other.y, z + other.z);
}

inline QVector QVector::operator+ (float* p) const
{
	return QVector(x + p[0], y + p[1], z + p[2]);
}

inline QVector QVector::operator+ (float f) const
{
	return QVector(x + f, y + f, z + f);
}

inline QVector  QVector::operator- (const QVector& other) const
{
	return QVector(x - other.x, y - other.y, z - other.z);
}

inline QVector  QVector::operator- (float* p) const
{
	return QVector(x - p[0], y - p[1], z - p[2]);
}

inline QVector  QVector::operator- (float f) const
{
	return QVector(x - f, y - f, z - f);
}

inline QVector QVector::operator- (void) const
{
	return QVector(-x, -y, -z);
}

inline QVector QVector::operator* (const QVector& other) const
{
	return QVector(x * other.x, y * other.y, z * other.z);
}

inline QVector QVector::operator* (float* p) const
{
	return QVector(x * p[0], y * p[1], z * p[2]);
}

inline QVector QVector::operator* (float f) const
{
	return QVector(x * f, y * f, z * f);
}

inline QVector QVector::operator/ (const QVector& other) const
{
	if (other.x != 0 && other.y != 0 && other.z != 0)
	{
		return QVector(x / other.x, y / other.y, z / other.z);
	}

	return *this;
}

inline QVector QVector::operator/ (float* p) const
{
	if (p[0] != 0 && p[1] != 0 && p[2] != 0)
	{
		return QVector(x / p[0], y / p[1], z / p[2]);
	}

	return *this;
}

inline QVector QVector::operator/ (float f) const
{
	if (f != 0)
	{
		return QVector(x / f, y / f, z / f);
	}

	return *this;
}

inline QVector operator*(float fl, const QVector& v) { return v * fl; };