#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
#include <string>
#include <cstring>

#include <vector>

// We store the number in 32 bit chunks

#define _CHUNK_BYTE_SIZE 4
#define _CHUNK_BIT_SIZE 32
#define _CHUNK_BIT_MASK 0x00000000

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
	//BigInt(BigInt& other)
	//{

	//}
	//BigInt(unsigned int num) { initialize(num, true); }
	//BigInt(long num)

	//~BigInt() { if (chunks) delete [] chunks; chunks = NULL; }
	
private:
	void initialize(unsigned long long num, bool pos)
	{
		chunks.clear();
		sign = pos;

		if (num == 0) return;
		
		// number of 32-bit chunks that make up a long (hurray for lack of standardization!)
		int numchunks = sizeof(num) / _CHUNK_BYTE_SIZE; 
		// Add the appropriate number of chunks to the vector
		for (int i = 0; i++ < numchunks; chunks.push_back(0));

		// std vectors are guaranteed to be contiguous in memory, so we can just copy memory over
		memcpy(&chunks[0], &num, sizeof(num));

		pack();
	}

	void initialize(BigInt& other)
	{
		chunks.clear();
		sign = other.sign;

		for (unsigned int i = 0; i < other.chunks.size(); ++i) chunks.push_back(0);

		memcpy(&chunks[0], &other.chunks[0], memsize());
	}

	// Checks whether there are any non-zero bits at or after the chunk given by the index
	bool onlyEmptyChunksRemain(unsigned int index) const
	{
		while (index < chunks.size()) if (chunks[index++]) return false;
		return true;
	}

	inline bool mostSignificantBit(int i)
	{
		return (i & 0x80000000) != 0;
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
			equalsVal(static_cast<unsigned long long>(other));
		}
		else if (other < 0 && !sign)
		{
			// Both are negative, so test whether their absolute values are the same.
			equalsVal(static_cast<unsigned long long>(-other));
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
			equalsVal(static_cast<unsigned long long>(other));
		}
		else if (other < 0 && !sign)
		{
			// Both are negative, so test whether their absolute values are the same.
			equalsVal(static_cast<unsigned long long>(-other));
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
			equalsVal(static_cast<unsigned long long>(other));
		}
		else if (other < 0 && !sign)
		{
			// Both are negative, so test whether their absolute values are the same.
			equalsVal(static_cast<unsigned long long>(-other));
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
			equalsVal(static_cast<unsigned long long>(other));
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
			equalsVal(static_cast<unsigned long long>(other));
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
			equalsVal(other);
		}
		else
		{
			// A negative BigInt can never equal an unsigned number
			return false;
		}
	}

	bool operator==(const BigInt &other) const
	{
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
		BigInt result(0);

		if (other < 0)
		{
			subtractVal(-other, result);
		}
		else
		{
			addVal(other, result);
		}

		return result;
	}

	BigInt operator+(const long& other)
	{
		BigInt result(0);

		if (other < 0)
		{
			subtractVal(-other, result);
		}
		else
		{
			addVal(other, result);
		}

		return result;
	}

	BigInt operator+(const long long& other)
	{
		BigInt result(0);

		if (other < 0)
		{
			subtractVal(-other, result);
		}
		else
		{
			addVal(other, result);
		}

		return result;
	}

	BigInt operator+(const unsigned int& other)
	{
		BigInt result(0);
		addVal(other, result);
		return result;
	}

	BigInt operator+(const unsigned long& other)
	{
		BigInt result(0);
		addVal(other, result);
		return result;
	}

	BigInt operator+(const unsigned long long& other)
	{
		BigInt result(0);
		addVal(other, result);
		return result;
	}

	// TODO: Add floating point support

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
			bool nextOverflow = mostSignificantBit(chunks[offset]) && mostSignificantBit(swap);

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
	// Operator helper function
	////////////////////////////

	bool equalsVal(const unsigned long long& other) const
	{
		unsigned int memsize = chunks.size() * _CHUNK_BYTE_SIZE;
		if (memsize < sizeof(other))
		{
			if (memcmp(&chunks.front(), &other, memsize) != 0) return false;

			// If an int is larger than our memsize, then compare the remaining bytes
			// of the int with zero.
			chunk zero = 0;
			return memcmp(&zero, &other + memsize, sizeof(chunk) - memsize) == 0;
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

	// Adds our number to a normal integer value
	void addVal(const unsigned long long& other, BigInt& result)
	{
		// TODO: deal with negative BigInts
		//BigInt result;

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

		//return result;
	}

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