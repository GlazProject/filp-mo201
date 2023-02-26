#pragma once

class CStringComparator
{
public:

	/*
		A < B --> true
	*/
	bool operator()(const char* A, const char* B) const
	{
		while (true)
		{
			if (A[0] == B[0])
			{
				//A = B
				if (!A[0])
					return false;

				A++;
				B++;
			}
			else
			{
				return A[0] < B[0];
			}
		}
	}
};