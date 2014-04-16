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
	//BigInt(BigInt& other)
	//{

	//}
	//BigInt(unsigned int num) { initialize(num, true); }
	//BigInt(long num)

	//~BigInt() { if (chunks) delete [] chunks; chunks = NULL; }
	
private:
	void initialize(unsigned long num, bool pos)
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
		int comp = other > 0 ? other : -other;

		// If the two numbers have different signs then the can't be equal
		if ((sign && comp != other) || (!sign && comp == other)) return false;

		unsigned int memsize = chunks.size() * _CHUNK_BYTE_SIZE;
		if (memsize < sizeof(int))
		{
			if (memcmp(&chunks.front(), &comp, memsize) != 0) return false;

			// If an int is larger than our memsize, then compare the remaining bytes
			// of the int with zero. This can happen if our value is 0 or if we're on
			// a 64 bit machine and we only have one chunk of memory.
			chunk zero = 0;
			return memcmp(&zero, &comp + memsize, sizeof(chunk) - memsize) == 0;
		}
		else
		{
			if (memcmp(&chunks[0], &comp, sizeof(int)) != 0) return false;

			// Can fail if sizeof(int) isn't a multiple of 4, but that should
			// never happen, right?
			return onlyEmptyChunksRemain(sizeof(int) / _CHUNK_BYTE_SIZE + 1);
		}

		return false;
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
		if (other < 0)
		{
			return subtractVal(static_cast<unsigned long long>(-other));
		}
		else
		{
			return addVal(static_cast<unsigned long long>(other));
		}
	}

	BigInt operator+(const long& other)
	{
		
		if (other < 0)
		{
			return subtractVal(static_cast<unsigned long long>(-other));
		}
		else
		{
			return addVal(static_cast<unsigned long long>(other));
		}
	}

	BigInt operator+(const long long& other)
	{
		if (other < 0)
		{
			return subtractVal(static_cast<unsigned long long>(-other));
		}
		else
		{
			return addVal(static_cast<unsigned long long>(other));
		}
	}

	BigInt operator+(const unsigned int& other)
	{
		unsigned long long absval = static_cast<unsigned long long>(other);
		return addVal(absval);
	}

	BigInt operator+(const unsigned long& other)
	{
		unsigned long long absval = static_cast<unsigned long long>(other);
		return addVal(absval);
	}

	BigInt operator+(const unsigned long long& other)
	{
		return addVal(other);
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

	// Adds our number to a normal integer value and returns the result
	BigInt addVal(const unsigned long long& other)
	{
		BigInt result;

		chunk val, swap;
		bool overflow = false;

		for (unsigned int offset = 0; offset * _CHUNK_BYTE_SIZE < sizeof(other); ++offset)
		{
			memcpy(&swap, &other, _CHUNK_BYTE_SIZE);
			val = chunks[offset] + swap;

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

		return result;
	}

	BigInt addBigInt(const BigInt& other)
	{
		// TODO: implement
		return BigInt(0);
	}

	BigInt subtractVal(const unsigned long long &other)
	{
		BigInt result;

		// TODO: add this code

		return result;
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