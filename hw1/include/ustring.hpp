#pragma once
#include <iterator>
#include <cassert>
#include <iostream>

const unsigned char kfirstBitMask = 128;	// 1000000
const unsigned char kSecondBitMask = 64;	// 0100000
const unsigned char kThirdBitMask  = 32;	// 0010000
const unsigned char kfourthBitMask = 16;	// 0001000
const unsigned char kfifthBitMask   = 8;	// 0000100

class UString
{
public:
	UString() = default;
	UString(const std::string &str): data_(str) {}

	UString(const std::u32string &str)
	{
		for(auto c: str)
			push_back(c);
	}

	// Операторы
	UString &operator +=(const UString &str)
	{
		assert(this);
		data_ += str.data_; //правильный порядок
		return *this;
	}

	UString operator =(const std::string &mystr)
	{
		assert(this);
		if( data_ != mystr )
		{
			size_t i;
			size_t s_len;
			size_t len_ = mystr.length();

			data_.clear();
			for (i = 0; i < len_; )
			{
				s_len = size();	// Выясняем текущий размер строки UTF в байтах
				push_back(mystr.substr(i));
				if (s_len == size()) // Размер не изменился
					++i;
				else
					i += size() - s_len;
			}
		}
		return *this;
	}

	UString operator =(const std::u32string &str)
	{
		data_.clear();
		assert(!str.empty());
		for(auto c: str)
		{
			push_back(c);
		}
		return *this;
	}

	friend UString operator +(UString lhs, const UString &rhs)
	{
		lhs += rhs;
		return lhs;
	}

	friend bool operator ==(const UString &lhs, const UString &rhs)
	{
		return lhs.data_ == rhs.data_;
	}

	friend bool operator !=(const UString &lhs, const UString &rhs)
	{
		return !(lhs == rhs);
	}

private:
	// Методы
	/*  0xxxxxxx = 1 байт  
	    110xxxxx = 2 байта 10xxxxxx
	    1110xxxx = 3 байта 10xxxxxx 10xxxxxx
	    11110xxx = 4 байта 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	size_t charlen(size_t it) const // эффективнее передача по значению
	{
		size_t ret;

		if ((data_[it] & 0x80) == 0)
			ret = 1;
		else if ((data_[it] & 0xe0) == 0xc0)
			ret = 2;
		else if ((data_[it] & 0xf0) == 0xe0)
			ret = 3;
		else if ((data_[it] & 0xf8) == 0xf0)
			ret = 4;
		else
		{
			throw std::runtime_error("Обнаружен некорректный utf-8 символ");
			ret = 0; // Неверное значение
		}
		return ret;
	}
public:
	size_t length() const
	{
		size_t ret = 0; // Возвращаемое значение
		size_t it = 0;
		size_t len_ = data_.length();

		while(it < len_)
		{
			it += charlen(it);
			++ret;
		}
		return ret;
	}

	// Подсчёт символов UTF8, которые занимают k байт
	size_t count_by_size(size_t k) const
	{
		assert(k >= 1 && k <= 4);
		size_t ret = 0;
		size_t tmp;
		size_t len_ = data_.length();

		for(size_t i = 0; i < len_;)
		{
			tmp = charlen(i);
			if(tmp == k)
				++ret;
			i += tmp;
		}
		return ret;
	}

	size_t size() const
	{
		// Размер в байтах
		return data_.length();
	}

private:
	bool multibyte(unsigned char c) const
	{
		return c >= 0x80 && c < 0xc0; // return (c & 0xc0) == 0x80;
	}

public:
	void push_back(const std::string &b)
	{
		size_t len = b.length();
		size_t i = 0;
		if (len > 0 && ((b[i] & 0x80) == 0))
		{
			data_.push_back(b[i]);
		}
		else if (len > 1 && ((b[i] & 0xe0) == 0xc0) && multibyte(b[i + 1]))
		{
			data_.push_back(b[i]);
			data_.push_back(b[i + 1]);
		}
		else if (len > 2 && ((b[i] & 0xf0) == 0xe0) && multibyte(b[i + 1]) && multibyte(b[i + 2]))
		{
			data_.push_back(b[i]);
			data_.push_back(b[i + 1]);
			data_.push_back(b[i + 2]);
		}
		else if (len > 3 && ((b[i]& 0xf8) == 0xf0) &&  multibyte(b[i + 1]) && multibyte(b[i + 2]) && multibyte(b[i + 3]))
		{
			data_.push_back(b[i]);
			data_.push_back(b[i + 1]);
			data_.push_back(b[i + 2]);
			data_.push_back(b[i + 3]);
		}
		else
			throw std::runtime_error("Обнаружен некорректный utf-8 символ: " + b);
	}
	// Метод разбирает codePoint и добавляет 1-4 символа в строку data_[]
	void push_back(const char32_t utf32)
	{
		if(utf32 < 0x7f)
		// 1 байт
			data_.push_back((unsigned char) utf32);
		else if(utf32 < 0x7ff)
		{ // 2 байта
			data_.push_back((unsigned char) 0b1100'0000 + (utf32 >> 6));
			data_.push_back((unsigned char) 0b1000'0000 + (utf32 & 0b0011'1111));
		}
		else if(utf32 < 0x10000)
		{ // 3 байта
			data_.push_back((unsigned char) (0b1110'0000 + (utf32 >> 12)));
			data_.push_back((unsigned char) (0b1000'0000 + ((utf32 >> 6) & 0b0011'1111)));
			data_.push_back((unsigned char) (0b1000'0000 + (utf32 & 0b0011'1111)));
		}
		else if(utf32 < 0x110000)
		{ // 4 байта
			data_.push_back((unsigned char) (0b1111'0000 + (utf32 >> 18)));
			data_.push_back((unsigned char) (0b1000'0000 + ((utf32 >> 12) & 0b0011'1111)));
			data_.push_back((unsigned char) (0b1000'0000 + ((utf32 >> 6) & 0b0011'1111)));
			data_.push_back((unsigned char) (0b1000'0000 + (utf32 & 0b0011'1111)));
		}
		else
			throw std::runtime_error("Обнаружен некорректный utf-8 символ.");

	}

	bool is_well() const
	{
		std::string::const_iterator it = data_.begin();
		while (it != data_.end())
		{
			if ((*it &0x80) == 0)
			{
				++it;
			}
			else if ((*it &0xe0) == 0xc0)
			{
				if (std::distance(it, data_.end()) < 2)
				{
					return false;
				}

				if ((*(it+1) &0xc0) != 0x80)
				{
					return false;
				}

				it += 2;
			}
			else if ((*it &0xf0) == 0xe0)
			{
				if (std::distance(it, data_.end()) < 3)
				{
					return false;
				}
				if ((*(it+1) &0xc0) != 0x80)
				{
					return false;
				}

				if ((*(it+2) &0xc0) != 0x80)
				{
					return false;
				}

				it += 3;
			}
			else if ((*it &0xf8) == 0xf0)
			{
				if (std::distance(it, data_.end()) < 4)
				{
					return false;
				}

				if ((*(it+1) &0xc0) != 0x80)
				{
					return false;
				}

				if ((*(it+2) &0xc0) != 0x80)
				{
					return false;
				}

				if ((*(it+3) &0xc0) != 0x80)
				{
					return false;
				}

				it += 4;
			}
			else
			{
				// Некорректный символ
				return false;
			}
		}
		return true;
	}

	void clear() // data_.erase(0, data_.end());
	{
		data_.erase(data_.begin(), data_.end());
	}

	void pop_back()
	{
		if (data_.empty())
		{
			return;
		}

		std::string::iterator last = data_.end() - 1;
		while ((*last &0xc0) == 0x80)
		{
			assert(last != data_.begin());
			--last;
		}

		data_.erase(last, data_.end());
	}

	friend std::ostream &operator<<(std::ostream &os, const UString &s)
	{
		os << s.data_;
		return os;
	}
public:
	class Iterator
	{
public:
		Iterator() noexcept: str_(), curr_pos_(0) {}
		~Iterator() {}
		Iterator(UString *ptr) noexcept: str_(ptr) {}
		Iterator(UString *ptr, size_t pos) noexcept: str_(ptr), curr_pos_(pos) {}

		/*	
			0xxxxxxx = 1 байт  
			110xxxxx = 2 байта 10xxxxxx
			1110xxxx = 3 байта 10xxxxxx 10xxxxxx
			11110xxx = 4 байта 10xxxxxx 10xxxxxx 10xxxxxx
		*/
		char32_t operator* () const
		{
			char32_t codePoint = 0;
			char firstByte = str_->data_[curr_pos_];
		
			if (firstByte & kfirstBitMask)
			{
				if (firstByte & kThirdBitMask)
				{
					if (firstByte & kfourthBitMask)
					{ // 4 БАЙТА
						codePoint = (firstByte & 0x07) << 18;		// 3 младших бита сдвигаем влево на 18 позиций
						char secondByte = str_->data_[curr_pos_+1];
						codePoint += (secondByte & 0x3f) << 12;		// 6 младших битов второго байта сдвигаем влево на 12
						char thirdByte = str_->data_[curr_pos_+2];
						codePoint += (thirdByte & 0x3f) << 6;;		// 6 младших битов третьего байта сдвигаем влево на 6
						char fourthByte = str_->data_[curr_pos_+3];
						codePoint += (fourthByte & 0x3f);		// 6 младших битов четвёртого байта добавляем к итогу
					}
					
					else
					{ // 3 БАЙТА
						codePoint = (firstByte & 0x0f) << 12;		// 3 младших бита сдвигаем влево на 12 позиций
						char secondByte = str_->data_[curr_pos_+1];
						codePoint += (secondByte & 0x3f) << 6;		// 6 младших битов второго байта сдвигаем влево на 6
						char thirdByte = str_->data_[curr_pos_+2];
						codePoint += (thirdByte & 0x3f);		// 6 младших битов третьего байта добавляем к итогу
					}
				}
				else
				{ // 2 БАЙТА
												// Первые 2 бита всегда 1, поэтому
					codePoint = (firstByte & 0x1f) << 6;			// 5 младших битов сдвигаем влево на 6
					char secondByte = str_->data_[curr_pos_+1];
					codePoint += (secondByte & 0x3f);			// 6 младших битов второго байта добавляем к итогу
				}
			}
			
			else
			{ // 1 БАЙТ
				// Первый бит всегда 0 - но здесь 7 битов младшие, поэтому ни маска, ни битовый сдвиг не нужны
				codePoint = firstByte;
			}
			return codePoint;
		}
public:
		Iterator& operator++()
		{ // Преинкремент
			assert(this);
			assert(str_); // Проверяем существование родителя
			curr_pos_ += str_->charlen(curr_pos_); // Смещаемся вперёд
			return *this;
		}

		Iterator operator++(int)
		{ // Постинкремент
			assert(this);
			assert(str_); // Проверяем существование родителя
			Iterator save_pos_ = *this; // Запоминаем текущее значение
			++*this; // Используем преинкремент
			return save_pos_; // Возвращаем запомненное значение
		}

		Iterator& operator--()
		{ // Предекремент
			assert(this);
			assert(str_); // Проверяем существование родителя
			if(curr_pos_ > 0)
				--curr_pos_;

			// В "худшем" случае придётся пропустить 3 байта и лишь 4-й будет началом символа.
			if(str_->multibyte(str_->data_[curr_pos_]))
			{ // Это означает что предыдущий байт не ASCII символ.
				assert(curr_pos_ > 0);
				--curr_pos_;
				// Добавить проверку на <0
				if(str_->multibyte(str_->data_[curr_pos_]))
				{
					assert(curr_pos_ > 0);
					--curr_pos_;
					if(str_->multibyte(str_->data_[curr_pos_]))
					{
						assert(curr_pos_ > 0);
						--curr_pos_;
					}
				}
			}
			return *this;
		}

		Iterator operator--(int)
		{ // Постдекремент
			assert(this);
			assert(str_); // Проверяем существование родителя
			Iterator save_pos_ = *this; // Запоминаем текущее значение
			--*this; // Используем предекремент
			return save_pos_; // Возвращаем запомненное значение
		}

		bool operator==(const Iterator& rhs) const { return str_ ==  rhs.str_ && curr_pos_ == rhs.curr_pos_; }
		bool operator!=(const Iterator& rhs) const { return str_ !=  rhs.str_ || curr_pos_ != rhs.curr_pos_; }
private:
		UString * str_;
		size_t curr_pos_ = 0; // Текущая позиция итератора в utf8 строке
	};

	Iterator begin()
	{
		assert(this);
		return Iterator(this);
	}

	Iterator end()
	{
		assert(this);
		return Iterator(this, data_.length()); // Вызываем конструктор итератора за последний символ
	}
	// ~UString() {} // Деструктор по-умолчанию подходит.

private:
	std::string data_;
};
