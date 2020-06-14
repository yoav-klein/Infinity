#ifndef __BITSET_H__
#define __BITSET_H__

#include <iostream>     // std::cout, std::size_t
#include <vector>       // std::vector
#include <algorithm> // std::for_each

namespace ilrd
{

const std::size_t BITS_IN_BYTE = 8;
const std::size_t BITS_IN_SIZET = sizeof(size_t) * BITS_IN_BYTE;
const std::size_t ALL_ONES = 0xFFFFFFFFFFFFFFFF;

template<typename std::size_t SIZE>
class Bitset
{
private:
	class LeftShiftAux;
	class ToStringAux;
	class OrAux;
	class AndAux;
	class FlipAllAux;
	class SetAllAux;
    class BitProxy;
    class CountBitsAux;

public:
    Bitset();
    
    void SetAll(bool val);
    void Flip(std::size_t index);
    void FlipAll();
    std::size_t CountBitsOn() const;
    std::string ToString() const;
	
    bool operator==(const Bitset<SIZE>& other) const;
    bool operator!=(const Bitset<SIZE>& other) const;
    Bitset& operator|=(const Bitset<SIZE>& other);
    Bitset& operator&=(const Bitset<SIZE>& other);
    Bitset operator<<(std::size_t num);
    
    bool operator[](std::size_t index) const; 
    BitProxy operator[](std::size_t index); // trowing exceptions version of operator []

private:
    void CleanDirtyBits();
    bool Get(std::size_t index);
	void Set(bool val, std::size_t index);
   	void CheckIndex(std::size_t index);  // throwing exception if out of range
   
    std::vector<size_t> m_bitset;
    std::size_t m_mask;
	
	
};

template<std::size_t SIZE>
void Bitset<SIZE>::CheckIndex(std::size_t index)
{
	if(index > SIZE)
	{
		throw std::out_of_range("index is out of bounds");
	}
}

void Zero(std::size_t& current)
{
	current = 0;
}

template<typename std::size_t SIZE>
Bitset<SIZE> Bitset<SIZE>::operator<<(std::size_t num)
{
	Bitset<SIZE> res;
	res.m_bitset = m_bitset;
	std::size_t chunks = num / BITS_IN_SIZET - (num % BITS_IN_SIZET == 0);
	std::size_t remainder = num % BITS_IN_SIZET;
	
	// copy from the last one --> begining
	std::transform(res.m_bitset.rbegin() + chunks, res.m_bitset.rend() - 1, 
		 res.m_bitset.rbegin() + chunks + 1, res.m_bitset.rbegin(), LeftShiftAux(remainder));
	
	// bring the remainder of the last block to the new 'first'
	res.m_bitset[chunks] = res.m_bitset[0] << remainder;
	
	// zero all blocks that should be zeroed
	std::for_each(res.m_bitset.begin(), 
	res.m_bitset.begin() + chunks + (num % BITS_IN_SIZET == 0), Zero);
		
	res.CleanDirtyBits(); 
	return res;
}

template<std::size_t SIZE>
bool Bitset<SIZE>::Get(std::size_t index)
{
	CheckIndex(index);
	
	std::size_t chunk_index = index / BITS_IN_SIZET;
	std::size_t chunk = m_bitset[chunk_index];
	std::size_t index_in_chunk = (index % BITS_IN_SIZET);
	return !!((0x1ul << index_in_chunk) & chunk);
}

template<typename std::size_t SIZE>
void Bitset<SIZE>::Set(bool val, std::size_t index)
{
	CheckIndex(index);
	
	std::size_t value = val;
	std::size_t chunk_index = index / BITS_IN_SIZET;
	std::size_t& chunk = m_bitset[chunk_index];
	std::size_t index_in_chunk = (index % BITS_IN_SIZET);
	chunk = (~(0x1ul << index_in_chunk) & chunk) | value << index_in_chunk;
}

template<typename std::size_t SIZE>
void Bitset<SIZE>::CleanDirtyBits()
{
	size_t& last_chunk = m_bitset[SIZE / BITS_IN_SIZET - (SIZE % BITS_IN_SIZET == 0)];
	last_chunk &= m_mask;
}

template<typename std::size_t SIZE>
Bitset<SIZE>::Bitset()
{
	SetAllAux init(0);
	m_bitset.assign(SIZE / BITS_IN_SIZET + ((SIZE % BITS_IN_SIZET) > 0), 0);
	
	m_mask = ALL_ONES >> (BITS_IN_SIZET - (SIZE % BITS_IN_SIZET)) % BITS_IN_SIZET;
}

template<typename std::size_t SIZE>
void Bitset<SIZE>::SetAll(bool val)
{
	SetAllAux setter(val);
	std::for_each(m_bitset.begin(), m_bitset.end(), setter);
	CleanDirtyBits();
}

template<typename std::size_t SIZE>
void Bitset<SIZE>::Flip(std::size_t index)
{	
	CheckIndex(index);
	
	size_t chunk = index / BITS_IN_SIZET - (SIZE % BITS_IN_SIZET == 0);
	index %= BITS_IN_SIZET;
	size_t mask = 0x1ul << index;
	m_bitset[chunk] ^= mask;
}

template<typename std::size_t SIZE>
void Bitset<SIZE>::FlipAll()
{
	FlipAllAux flipper;
	std::for_each(m_bitset.begin(), m_bitset.end(), flipper);
	CleanDirtyBits();
}

template<typename std::size_t SIZE>
std::string Bitset<SIZE>::ToString() const
{
	ToStringAux res;
	res = std::for_each(m_bitset.begin(), m_bitset.end(), ToStringAux());
	
	return res.GetResult();
}

template<typename std::size_t SIZE>
bool Bitset<SIZE>::operator==(const Bitset<SIZE>& other) const
{
	return m_bitset == other.m_bitset;
}

template<typename std::size_t SIZE>
bool Bitset<SIZE>::operator!=(const Bitset<SIZE>& other) const
{
	return m_bitset != other.m_bitset;
}

template<typename std::size_t SIZE>
Bitset<SIZE>& Bitset<SIZE>::operator|=(const Bitset<SIZE>& other)
{
	OrAux aux(other);
	std::for_each(m_bitset.begin(), m_bitset.end(), aux);
	
	return *this;
}

template<typename std::size_t SIZE>
Bitset<SIZE>& Bitset<SIZE>::operator&=(const Bitset<SIZE>& other)
{
	AndAux aux(other);
	std::for_each(m_bitset.begin(), m_bitset.end(), aux);
	
	return *this;
}

template<typename std::size_t SIZE>
typename Bitset<SIZE>::BitProxy Bitset<SIZE>::operator[](std::size_t index)
{
	CheckIndex(index);
	
	return BitProxy(*this, index);
}

template<typename std::size_t SIZE>
std::size_t Bitset<SIZE>::CountBitsOn() const
{
	CountBitsAux count_aux;
	count_aux = std::for_each(m_bitset.begin(), m_bitset.end(), count_aux);
	
	return count_aux.GetTotal();
}


/********* Functor classes **********/
template<typename std::size_t SIZE>
class Bitset<SIZE>::LeftShiftAux
{
public:
LeftShiftAux(std::size_t num)
	: m_num(num) { }
		
std::size_t operator()(std::size_t a, std::size_t b)
{
	return (a << m_num) | (b >> (BITS_IN_SIZET - m_num));
}
		
private:
	std::size_t m_num;
};

template<typename std::size_t SIZE>
class Bitset<SIZE>::ToStringAux
{
public:
	void operator()(std::size_t current)
	{
		std::size_t mask = 0x1ul;
		for(std::size_t i = 0; i < sizeof(std::size_t); ++i)
		{
			for(std::size_t j = 0; j < BITS_IN_BYTE; ++j)
			{
				result.insert(0, 1, 48 + !!(mask & current));
				mask <<= 1;
			}
			result.insert(0, 1, ',');		
		}
		result.insert(0, 1, '\n');
	}
		
	std::string GetResult()
	{
		return result;
	}
		
private:
	std::string result;
};
	

template<typename std::size_t SIZE>
class Bitset<SIZE>::OrAux
{
public:
	OrAux(Bitset<SIZE> other)
			: m_other(other), m_i(0) { }
	void operator()(std::size_t& current)
	{
		current |= m_other.m_bitset[m_i];
		++m_i;
	}
		
private:
	Bitset<SIZE> m_other;
	std::size_t m_i;
};
	
template<typename std::size_t SIZE>
class Bitset<SIZE>::AndAux
{
public:
	AndAux(Bitset<SIZE> other)
		: m_other(other), m_i(0) { }
	void operator()(std::size_t& current)
	{
		current &= m_other.m_bitset[m_i];
		++m_i;
	}
		
private:
	Bitset<SIZE> m_other;
	std::size_t m_i;
};

template<typename std::size_t SIZE>
class Bitset<SIZE>::FlipAllAux
{
public:
	void operator()(std::size_t& current)
	{
		current = ~current;
	}
};

template<typename std::size_t SIZE>
class Bitset<SIZE>::SetAllAux
{
public:
	SetAllAux(bool value)
		: m_value(value){}
	void operator()(std::size_t& current)
	{
		current = (m_value ? ALL_ONES : 0);
	}
		

private:
	bool m_value;
};
	
template<typename std::size_t SIZE>
class Bitset<SIZE>::CountBitsAux
{
public:
    CountBitsAux()
    	:  m_total(0)
    {
    	InitLut();
    }
    	
    void InitLut()
    {
    	for(std::size_t i = 0; i < 256; ++i)
    	{
    		unsigned char curr = i;
    		curr = ((curr & 0x55) + ((curr >> 1) & 0x55));
    		curr = ((curr & 0x33) + ((curr >> 2) & 0x33));
    		curr = ((curr & 0xf) + ((curr >> 4) & 0xf));
    		m_lut[i] = curr;
    	}
    }
    	
    void operator()(std::size_t current)
    {
    	for(std::size_t i = 0; i < sizeof(std::size_t); ++i)
    	{
    		m_total += m_lut[current & 0xFF];
    		current >>= BITS_IN_BYTE;
    	}
    }
    	
    std::size_t GetTotal()
    {
    	return m_total;
   	}
    
private:
    unsigned char m_lut[256];
    std::size_t m_total;
};

template<typename std::size_t SIZE>
class Bitset<SIZE>::BitProxy
{
public:
	BitProxy(Bitset& bitset ,std::size_t index)
    	: m_proxy_bitset(bitset), m_index(index) { }
        
    BitProxy& operator=(const BitProxy& other)
    {
       	m_proxy_bitset.Set(other.m_proxy_bitset.Get(other.m_index), m_index);
      		
     	return *this;
    }
        
    operator bool() 
    {
        std::size_t& chunk = m_proxy_bitset.m_bitset[m_index / BITS_IN_SIZET];
        std::size_t index = m_index % BITS_IN_SIZET;
        return !!(chunk & (0x1ul << index));
	}

private:
	Bitset<SIZE>& m_proxy_bitset;   
	std::size_t m_index;
};

}//namespace ilrd

#endif  //__BITSET_H__

