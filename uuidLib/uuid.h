#pragma once
struct uuid
{
	struct value
	{
		unsigned int a;
		unsigned int b;
		unsigned int c;
		unsigned int d;

	} value;

	static void init();

	uuid();

	inline bool operator==(const uuid& b) {
		return (value.a == b.value.a) &&
			(value.b == b.value.b) &&
			(value.c == b.value.c) &&
			(value.d == b.value.d);
	}

	inline bool operator<(const uuid& b) {
		if (value.a < b.value.a) return true;
		else if (value.a == value.b) 
		{
			if (value.b < b.value.b) return true;
			else if (value.b == b.value.b)
			{
				if (value.c < b.value.b)	return true;
				else if (value.c == b.value.c)
				{
					return value.d < b.value.c;
				}
			}
		}
		return false;
	}
};

