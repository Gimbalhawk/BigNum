#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
#include <string>
#include <cstring>

#include <vector>

// We store the number in 32 bit chunks
#define _CHUNK_BYTE_SIZE 4
#define _CHUNK_BIT_SIZE 32

// A signed-magnitude representation of an arbitrarily large integer.
class BigInt
{
	typedef uint32_t chunk;

private:
	inline size_t memsize() const { return chunks.size() * _CHUNK_BYTE_SIZE; } 

	////////////////////////////
	// Constructors!
	////////////////////////////
public:
	BigInt() { initialize(0, true); }
	BigInt(int num) { initialize(num >= 0 ? num : -num, num >= 0); }
	BigInt(long num) { initialize(num >= 0 ? num : -num, num >= 0); }
	BigInt(long long num) { initialize(num >= 0 ? num : -num, num >= 0); }
	BigInt(unsigned int num) { initialize(num, true); }
	BigInt(unsigned long num) { initialize(num, true); }
	BigInt(unsigned long long num) { initialize(num, true); }

	// TODO: Add float support

	//BigInt(BigInt& other) { initialize(other); }

	//~BigInt() { if (chunks) delete [] chunks; chunks = NULL; }
	
private:
	void initialize(unsigned long long num, bool pos)
	{
		chunks.clear();
		sign = pos;

		if (num == 0)
		{
			return;
		}
		
		// Number of 32-bit chunks that make up a long long (hurray for lack of standardization!)
		int numchunks = sizeof(num) / _CHUNK_BYTE_SIZE;

		// Add the appropriate number of chunks to the vector
		for (int i = 0; i++ < numchunks; chunks.push_back(0));

		// std vectors are guaranteed to be contiguous in memory, so we can just copy memory over
		memcpy(&chunks[0], &num, sizeof(num));

		pack();
	}

	void initialize(const BigInt& other)
	{
		chunks.clear();
		sign = other.sign;

		for (unsigned int i = 0; i < other.chunks.size(); ++i) 
		{
			chunks.push_back(other.chunks[i]);
		}
	}

	// Checks whether there are any non-zero bits at or after the chunk given by the index
	bool onlyEmptyChunksRemain(unsigned int index) const
	{
		while (index < chunks.size()) if (chunks[index++]) return false;
		return true;
	}

	
public:

	////////////////////////////
	// Operators : Equality 
	////////////////////////////

	bool operator==(const int &other) const
	{
		if (other >= 0 && sign)
		{
			// Both are positive. Basic case.
			return equalsVal(static_cast<unsigned long long>(other));
		}
		else if (other < 0 && !sign)
		{
			// Both are negative, so test whether their absolute values are the same.
			return equalsVal(static_cast<unsigned long long>(-other));
		}
		else 
		{
			// Signs don't match
			return false;
		}
	}

	bool operator==(const long &other) const
	{
		if (other >= 0 && sign)
		{
			// Both are positive. Basic case.
			return equalsVal(static_cast<unsigned long long>(other));
		}
		else if (other < 0 && !sign)
		{
			// Both are negative, so test whether their absolute values are the same.
			return equalsVal(static_cast<unsigned long long>(-other));
		}
		else 
		{
			// Signs don't match
			return false;
		}
	}

	bool operator==(const long long &other) const
	{
		if (other >= 0 && sign)
		{
			// Both are positive. Basic case.
			return equalsVal(static_cast<unsigned long long>(other));
		}
		else if (other < 0 && !sign)
		{
			// Both are negative, so test whether their absolute values are the same.
			return equalsVal(static_cast<unsigned long long>(-other));
		}
		else 
		{
			// Signs don't match
			return false;
		}
	}

	bool operator==(const unsigned int& other) const
	{
		if (sign)
		{
			return equalsVal(static_cast<unsigned long long>(other));
		}
		else
		{
			// A negative BigInt can never equal an unsigned number
			return false;
		}
	}

	bool operator==(const unsigned long& other) const
	{
		if (sign)
		{
			return equalsVal(static_cast<unsigned long long>(other));
		}
		else
		{
			// A negative BigInt can never equal an unsigned number
			return false;
		}
	}

	bool operator==(const unsigned long long& other) const
	{
		if (sign)
		{
			return equalsVal(other);
		}
		else
		{
			// A negative BigInt can never equal an unsigned number
			return false;
		}
	}

	bool operator==(const BigInt &other) const
	{
		if (sign != other.sign)
		{
			return false;
		}

		unsigned int index = 0;

		while (index < chunks.size() && index < other.chunks.size())
		{
			if (chunks[index] != other.chunks[index]) return false;

			++index;
		}

		return onlyEmptyChunksRemain(index) && other.onlyEmptyChunksRemain(index);
	}

	////////////////////////////
	// Operators : +
	////////////////////////////
	BigInt operator+(const int& other)
	{
		if (other < 0)
		{
			return performValueAddition(-other, false);
		}
		else
		{
			return performValueAddition(other, true);
		}
	}

	BigInt operator+(const long& other)
	{
		if (other < 0)
		{
			return performValueAddition(-other, false);
		}
		else
		{
			return performValueAddition(other, true);
		}
	}

	BigInt operator+(const long long& other)
	{
		if (other < 0)
		{
			return performValueAddition(-other, false);
		}
		else
		{
			return performValueAddition(other, true);
		}
	}

	BigInt operator+(const unsigned int& other)
	{
		return performValueAddition(other, true);
	}

	BigInt operator+(const unsigned long& other)
	{
		return performValueAddition(other, true);
	}

	BigInt operator+(const unsigned long long& other)
	{
		return performValueAddition(other, true);
	}

	// TODO: Add floating point support
	BigInt operator+(const float& other)
	{
		return BigInt(*this);
	}

	BigInt operator+(const double& other)
	{
		return BigInt(*this);
	}

	BigInt operator+(const BigInt& other)
	{
		if (other.sign)
		{
			return addBigInt(other);
		}
		else
		{
			return subtractBigInt(other);
		}
	}

	////////////////////////////
	// Operators : -
	////////////////////////////
	BigInt operator-(const int& other)
	{
		if (other < 0)
		{
			return performValueSubtraction(-other, false);
		}
		else
		{
			return performValueSubtraction(other, true);
		}
	}

	BigInt operator-(const long& other)
	{
		if (other < 0)
		{
			return performValueSubtraction(-other, false);
		}
		else
		{
			return performValueSubtraction(other, true);
		}
	}

	BigInt operator-(const long long& other)
	{
		if (other < 0)
		{
			return performValueSubtraction(-other, false);
		}
		else
		{
			return performValueSubtraction(other, true);
		}
	}

	BigInt operator-(const unsigned int& other)
	{
		return performValueSubtraction(other, true);
	}

	BigInt operator-(const unsigned long& other)
	{
		return performValueSubtraction(other, true);
	}

	BigInt operator-(const unsigned long long& other)
	{
		return performValueSubtraction(other, true);
	}

	// TODO: Add floating point support
	BigInt operator-(const float& other)
	{
		return BigInt(*this);
	}

	BigInt operator-(const double& other)
	{
		return BigInt(*this);
	}

	BigInt operator-(const BigInt& other)
	{
		// TODO: implement
		return BigInt(*this);
	}
	
	////////////////////////////
	// Operators : +=
	////////////////////////////
	
	// TODO: Rewrite this to actually work!
	const BigInt& operator+=(const int &other)
	{
		// Doesn't work if either one is negative
		//if (other < 0) return *this -= -other;

		chunk swap;
		bool overflow = false;

		for (unsigned int offset = 0; offset * _CHUNK_BYTE_SIZE < sizeof(other); ++offset)
		{
			memcpy(&swap, &other, _CHUNK_BYTE_SIZE);
			
			if (offset >= chunks.size())
			{
				chunks.push_back(0);
			}

			// Whether we'll need to keep track of overflow next time
			//bool nextOverflow = mostSignificantBit(chunks[offset]) && mostSignificantBit(swap);
			bool nextOverflow = false;

			if (nextOverflow)
			{
				swap &= 0x7fffffff;
				chunks[offset] &= 0x7fffffff;
			}

			chunks[offset] = chunks[offset] + swap;
			overflow = nextOverflow;
		}

		if (overflow)
		{
			chunks.push_back(1);
		}

		return *this;
	}

private:
	////////////////////////////
	// Operator helper functions
	////////////////////////////

	bool equalsVal(const unsigned long long& other) const
	{
		unsigned int memsize = chunks.size() * _CHUNK_BYTE_SIZE;
		
		if (memsize == 0)
		{
			// We don't want to memcmp an empty array
			return other == 0;
		}
		if (memsize < sizeof(other))
		{
			if (memcmp(&chunks.front(), &other, memsize) != 0) 
			{
				return false;
			}

			// We bitshift the comparison number by the number of bits
			// currently in our chunks vector. This leaves only the portion
			// of the number we haven't tested. If this portion contains any
			// set bits, then our two numbers aren't equal.
			unsigned testedBits = chunks.size() * _CHUNK_BIT_SIZE;
			return (other >> testedBits) == 0;
		}
		else
		{
			if (memcmp(&chunks[0], &other, sizeof(other)) != 0) return false;

			// Can fail if sizeof(other) isn't a multiple of 4, but that should
			// never happen, right?
			return onlyEmptyChunksRemain(sizeof(other) / _CHUNK_BYTE_SIZE + 1);
		}

		return false;
	}

	BigInt performValueAddition(const unsigned long long& other, const bool othersign)
	{
		BigInt result(0);

		// If the numbers are both positive or both negative,
		// adding them together will result in adding their values.
		// Otherwise their values are subtracted.
		if ((sign && othersign) || (!sign && !othersign))
		{
			addVal(other, result);
		}
		else
		{
			subtractVal(other, result);
		}

		return result;
	}

	BigInt performValueSubtraction(const unsigned long long& other, const bool othersign)
	{
		BigInt result(0);

		// If the numbers are both positive or both negative,
		// subtracting them will result in subtracting their values.
		// Otherwise their values are added.
		if (sign == othersign)
		{
			subtractVal(other, result);
		}
		else
		{
			addVal(other, result);
		}

		return result;
	}

	// Adds our absolute value with that of an integer
	void addVal(const unsigned long long& other, BigInt& result)
	{
		chunk val, swap;
		bool overflow = false;

		for (unsigned int offset = 0; offset * _CHUNK_BYTE_SIZE < sizeof(other); ++offset)
		{
			memcpy(&swap, &other + offset*_CHUNK_BYTE_SIZE, _CHUNK_BYTE_SIZE);
			
			val = offset < chunks.size() ? chunks[offset] : 0;
			val += swap;

			if (overflow)
			{
				// If there was overflow in the last calculation, carry the one
				++val;
			}

			overflow = (val <= swap);
			result.chunks.push_back(val);
		}

		if (overflow)
		{
			result.chunks.push_back(1);
		}
	}

	// Subtracts a value from this big int's absolute value, which may
	// cause our sign to flip
	void subtractVal(const unsigned long long &other, BigInt& result)
	{
		// TODO: implement
	}

	BigInt addBigInt(const BigInt& other)
	{
		// TODO: implement
		return BigInt(0);
	}

	BigInt subtractBigInt(const BigInt& other)
	{
		// TODO: implement
		return BigInt(0);
	}

public:
	// Removes any leading empty chunks
	void pack()
	{
		int emptychunks = emptyChunks();
		while (emptychunks-- > 0)
		{
			chunks.pop_back();
		}
	}

private:
	// Finds the number of empty blocks at the front of the vector
	int emptyChunks()
	{
		int emptychunks = 0;

		for (std::vector<chunk>::reverse_iterator it = chunks.rbegin(); it != chunks.rend() && *it == 0; ++it)
		{
			++emptychunks;
		}

		return emptychunks;
	}


	
	////////////////////////////
	// Member Variables
	////////////////////////////
private:
	std::vector<chunk> chunks;

	// Our current sign - true for positive, false for negative
	bool sign;
};

#endif
