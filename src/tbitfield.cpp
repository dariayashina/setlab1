// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
	if (len < 0) throw logic_error("Invalid size");
	BitLen = len;
	MemLen = (len / (sizeof(TELEM) * 8)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.GetLength();
	MemLen = bf.GetMemLen();
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	BitLen = 0;
	MemLen = 0;
	delete[] pMem;

}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return(n / (sizeof(TELEM) * 8));
}

int TBitField::GetMemLen()const
{
	return MemLen;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int index = (n % (sizeof(TELEM) * 8));
	TELEM mask = 0;
	mask = mask + pow(2, index);
	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= (*this).BitLen)) throw logic_error("invalid position");
	TELEM mask = (*this).GetMemMask(n);
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) throw logic_error("invalid position");
	TELEM mask = (*this).GetMemMask(n);
	mask = ~mask;
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || (n >= BitLen)) throw logic_error("Invalid index");
	TELEM mask = GetMemMask(n);
	TELEM bit = pMem[GetMemIndex(n)];
	bit = bit & mask;
	(bit != 0) ? (bit = 1) : (bit);
	return bit;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this == &bf) {
		return (*this);
	}
	BitLen = bf.GetLength();
	MemLen = bf.GetMemLen();
	delete[](*this).pMem;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}


int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if ((*this).GetLength() == bf.GetLength())
	{
		for (int i = 0; i < (*this).GetMemLen(); i++)
		{
			if ((*this).pMem[i] != bf.pMem[i])
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if ((*this).GetLength() == bf.GetLength())
	{
		for (int i = 0; i < (*this).GetMemLen(); i++)
		{
			if ((*this).pMem[i] != bf.pMem[i])
			{
				return 1;
			}
		}
		return 0;
	}
	return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	if ((*this).BitLen != bf.BitLen)
	{
		TBitField s1(max((*this).BitLen, bf.BitLen));
		TBitField	s2(max((*this).BitLen, bf.BitLen));
		for (int i = 0; i < (*this).BitLen; i++)
		{
			if ((*this).GetBit(i))
			{
				s1.SetBit(i);
			}
		}
		for (int i = 0; i < bf.BitLen; i++)
		{
			if (bf.GetBit(i))
			{
				s2.SetBit(i);
			}
		}
		for (int i = 0; i < s1.GetMemLen(); i++)
		{
			s1.pMem[i] = s1.pMem[i] | s2.pMem[i];
		}
		return s1;
	}
	TBitField s1(*this);
	for (int i = 0; i < bf.GetMemLen(); i++)
	{
		s1.pMem[i] = s1.pMem[i] | bf.pMem[i];
	}
	return s1;

}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	if ((*this).BitLen != bf.BitLen)
	{
		TBitField s1(max((*this).BitLen, bf.BitLen));
		TBitField	s2(max((*this).BitLen, bf.BitLen));
		for (int i = 0; i < (*this).BitLen; i++)
		{
			if ((*this).GetBit(i))
			{
				s1.SetBit(i);
			}
		}
		for (int i = 0; i < bf.BitLen; i++)
		{
			if (bf.GetBit(i))
			{
				s2.SetBit(i);
			}
		}
		for (int i = 0; i < s1.GetMemLen(); i++)
		{
			s1.pMem[i] = s1.pMem[i] & s2.pMem[i];
		}
		return s1;
	}
	TBitField s1(*this);
	for (int i = 0; i < bf.GetMemLen(); i++)
	{
		s1.pMem[i] = s1.pMem[i] & bf.pMem[i];
	}
	return s1;
}
TBitField TBitField::operator~(void) // отрицание
{
	TBitField s2(*this);

	TBitField mask((*this).BitLen);
	for (int i = 0; i < mask.BitLen; i++)
	{
		mask.SetBit(i);
	}
	for (int i = 0; i < s2.GetMemLen(); i++)
	{
		s2.pMem[i] = ~s2.pMem[i];
		s2.pMem[i] = s2.pMem[i] & mask.pMem[i];
	}
	return s2;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int n;
	istr >> n;
	TBitField s(n);
	for (int i = 0; i < n; i++)
	{
		istr >> n;
		if (n == 1)
		{
			s.SetBit(i);
		}
	}
	bf = s;
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	ostr << bf.GetLength() << endl;

	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	ostr << endl;
	return ostr;
}