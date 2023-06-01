#pragma once

#include <iterator>
#include <cassert>
#include <iostream>

const unsigned char kfirstBitMask = 128;	// 0b1000'0000
const unsigned char kSecondBitMask = 64;	// 0b0100'0000
const unsigned char kThirdBitMask  = 32;	// 0b0010'0000
const unsigned char kfourthBitMask = 16;	// 0b0001'0000
const unsigned char kfifthBitMask   = 8;	// 0b0000'1000

const unsigned char ktwoLeftBits    = 0xc0;	// 0b1100'0000
const unsigned char kthreeLeftBits  = 0xe0;	// 0b1110'0000
const unsigned char kfourLeftBits   = 0xf0;	// 0b1111'0000
const unsigned char kfiveLeftBits   = 0xf8;	// 0b1111'1000

const unsigned char kthreeRightBits = 0x07;	// 0b0000'0111
const unsigned char kfourRightBits  = 0x0f;	// 0b0000'1111
const unsigned char kfiveRightBits  = 0x1f;	// 0b0001'1111
const unsigned char ksixRightBits   = 0x3f;	// 0b0011'1111

const char32_t koneByteMaxValue    = 0x7f;
const char32_t ktwoBytesMaxValue   = 0x7ff;
const char32_t kthreeBytesMaxValue = 0xffff;
const char32_t kfourBytesMaxValue  = 0x1fffff;

class UString
{
public:
	UString() = default;
	UString(const std::string &str)
	{
		// "Разная логика у конструктора от string и оператора присваивания"
		//
		// Меняем конструктор на более сложный вариант.
		// Теперь невозможно выполнить инициализацию некорректными UTF8 символами, т.к.
		// метод push_back выполняет входной контроль данных.
		// Тест "[is_well]" пришлось переделать.
		size_t len_ = str.length();
		data_.clear();
#ifdef DEBUG
		if( len_ > 0 )
		{
#endif
			for(size_t i = 0; i < len_; ++i )
			{
				push_back(str.substr(i));
			}
#ifdef DEBUG
		}
		else
			debug_ = "Skip assigment of the same text in constructor.";
#endif
	}

	UString(const std::u32string &str)
	{
		// В данном методе невозможно сравнить что-то со строкой u32string, 
		// т.к. класс не хранит строку в таком виде, а только в преобразованном,
		// который выяснить заранее проблематично.
		// Возможна ситуация, когда присваивание не поменяет результат, но
		// код всё равно выполнится.
		for(auto c: str)
			push_back(c);
	}

	// Операторы
	UString &operator +=(const UString &str) noexcept
	{
		data_ += str.data_; //правильный порядок
		return *this;
	}

	UString &operator =(const std::string &str)
	{
		if( data_ != str )
		{
			size_t len_ = str.length();
			data_.clear();
			if( len_ > 0 )
			{
				size_t s_len;
				for(size_t i = 0; i < len_; )
				{
					s_len = size();	// Выясняем текущий размер строки UTF в байтах
					push_back(str.substr(i));
					if (s_len == size()) // Размер не изменился
						++i;
					else
						i += size() - s_len;
				}
			}
		}
#ifdef		DEBUG
		else
			debug_ = "Skip assigment of the same text.";
#endif
		return *this;
	}

	UString &operator =(const std::u32string &str)
	{
		// то есть программа должна падать если мы копируем пустую строку?
//		assert(!str.empty());
		data_.clear();
		if(!str.empty())
		{
			for(auto c: str)
			{
				push_back(c);
			}
		}
		return *this;
	}

	friend UString operator +(UString lhs, const UString &rhs) noexcept
	{
		lhs += rhs;
		return lhs;
	}

	friend bool operator ==(const UString &lhs, const UString &rhs) noexcept
	{
		return lhs.data_ == rhs.data_;
	}

	friend bool operator !=(const UString &lhs, const UString &rhs) noexcept
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
	size_t charlen(size_t& it) const
	{
		size_t ret;

		assert( it < data_.length() );
		if ((data_[it] & kfirstBitMask) == 0)
			ret = 1;
		else if ((data_[it] & kthreeLeftBits) == ktwoLeftBits)
			ret = 2;
		else if ((data_[it] & kfourLeftBits) == kthreeLeftBits)
			ret = 3;
		else if ((data_[it] & kfiveLeftBits) == kfourLeftBits)
			ret = 4;
		else
		{
			throw std::runtime_error("Обнаружен некорректный utf-8 символ");
			ret = 0; // Неверное значение
		}
		return ret;
	}
public:
	size_t length() const noexcept
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

	size_t size() const noexcept
	{
		// Размер в байтах
		return data_.length();
	}

private:
	bool multibyte(unsigned char c) const noexcept // Продолжение символа utf8
	{
		return c >= kfirstBitMask && c < ktwoLeftBits;
	}

public:
	void push_back(const std::string &b)
	{
		// Выносим все вызовы push_back в цикл и
		// Переносим вызовы multibyte в отдельный цикл
		size_t len = b.length();
		size_t i = 0;
		size_t usize = 0; // Признак некорректного символа
		if (len > 0 && ((b[i] & kfirstBitMask) == 0))
		{
			usize = 1;
		}
		else if (len > 1 && ((b[i] & kthreeLeftBits) == ktwoLeftBits))
		{
			usize = 2;
		}
		else if (len > 2 && ((b[i] & kfourLeftBits) == kthreeLeftBits))
		{
			usize = 3;
		}
		else if (len > 3 && ((b[i]& kfiveLeftBits) == kfourLeftBits))
		{
			usize = 4;
		}
		if( usize >= 2 ) // Заявленный размер известен, проверяем байты 2..4
			for( size_t j = 1; j < usize; ++j )
				if( !multibyte(b[i + j]) )
				{
					usize = 0;
					break;
				}
		if( usize == 0 )
			throw std::runtime_error("Обнаружен некорректный utf-8 символ: " + b);
		for( size_t j = 0; j < usize; ++j )
			data_.push_back(b[i + j]);
	}
	// Метод разбирает codePoint и добавляет 1-4 символа в строку data_[]
	void push_back(const char32_t utf32)
	{
		// Ликвидация повторов, указанных ниже, удлинняет текст программы и ухудшает читаемость
/*
		if(utf32 < 0x7f)
		{ // 1 байт
			data_.push_back((unsigned char) utf32);
		}
		else if(utf32 < 0x7ff)
		{ // 2 байта
			data_.push_back((unsigned char) (0xc0 + (utf32 >> 6)));			// Уникальная строка
			data_.push_back((unsigned char) (0x80 + (utf32 & 0x3f)));		// 3 повтора
		}
		else if(utf32 < 0x10000)
		{ // 3 байта
			data_.push_back((unsigned char) (0xe0 + (utf32 >> 12)));		// Уникальная строка
			data_.push_back((unsigned char) (0x80 + ((utf32 >> 6) & 0x3f)));	// 2 повтора
			data_.push_back((unsigned char) (0x80 + (utf32 & 0x3f)));		// 3 повтора см.выше
		}
		else if(utf32 < 0x110000)
		{ // 4 байта
			data_.push_back((unsigned char) (0xf0 + (utf32 >> 18))); 		// Уникальная строка
			data_.push_back((unsigned char) (0x80 + ((utf32 >> 12) & 0x3f))); 	// Уникальная строка
			data_.push_back((unsigned char) (0x80 + ((utf32 >> 6) & 0x3f)));	// 2 повтора см.выше
			data_.push_back((unsigned char) (0x80 + (utf32 & 0x3f)));		// 3 повтора см.выше
		}
		else 
			throw std::runtime_error("Обнаружен некорректный utf-8 символ.");
*/
		size_t usize = 0;

		if(utf32 <= koneByteMaxValue)
		{ // 1 байт
			data_.push_back((unsigned char) utf32);
		}
		else if(utf32 <= ktwoBytesMaxValue)
		{ // 2 байта
			data_.push_back((unsigned char) (ktwoLeftBits + (utf32 >> 6)));
			usize = 2;
		}
		else if(utf32 <= kthreeBytesMaxValue)
		{ // 3 байта
			data_.push_back((unsigned char) (kthreeLeftBits + (utf32 >> 12)));
			usize = 3;
		}
		else if(utf32 <= kfourBytesMaxValue)
		{ // 4 байта
			data_.push_back((unsigned char) (kfourLeftBits + (utf32 >> 18)));
			data_.push_back((unsigned char) (kfirstBitMask + ((utf32 >> 12) & ksixRightBits)));
			usize = 4;
		}
		else 
			throw std::runtime_error("Обнаружен некорректный utf-8 символ.");
		if( usize >=3 )
			data_.push_back((unsigned char) (kfirstBitMask + ((utf32 >> 6) & ksixRightBits)));
		if( usize >= 2 )
			data_.push_back((unsigned char) (kfirstBitMask + (utf32 & ksixRightBits)));
	}

	bool is_well() const noexcept
	{
		// Оптимизируем код, убирая повторы
		size_t bytes;
		std::string::const_iterator it = data_.begin();
		while (it != data_.end())
		{
			bytes = 0;
			if ((*it &kfirstBitMask) == 0)
			{
				bytes = 1;
			}
			else if ((*it &kthreeLeftBits) == ktwoLeftBits)
			{
				bytes = 2;
			}
			else if ((*it &kfourLeftBits) == kthreeLeftBits)
			{
				bytes = 3;
			}
			else if ((*it &kfiveLeftBits) == kfourLeftBits)
			{
				bytes = 4;
			}
			else
			{ // Некорректный символ
				return false;
			}
			if( bytes != 0 )
			{
				if(std::distance(it, data_.end()) < (int) bytes)
				{ // Ожидались символы, но строка закончилась
					return false;
				}
				for( size_t i = 1; i < bytes; ++i )
					if(!multibyte(*(it+i)))
						return false;
				it += bytes;
			}
		}
		return true;
	}

	void clear() noexcept
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
		std::string::iterator tmp = data_.begin();
		while ((*last &ktwoLeftBits) == kfirstBitMask)
		{
			assert(last != tmp);
			--last;
		}

		data_.erase(last, data_.end());
	}

	friend std::ostream &operator<<(std::ostream &os, const UString &s) noexcept
	{
		os << s.data_;
		return os;
	}
public:
	class Iterator
	{
public:
		Iterator() noexcept: str_(), curr_pos_(0) {}
		~Iterator() noexcept {}
		Iterator(UString *ptr) noexcept: str_(ptr) {}
		Iterator(UString *ptr, size_t pos) noexcept: str_(ptr), curr_pos_(pos) {}

		/*	
			0xxxxxxx = 1 байт  
			110xxxxx = 2 байта 10xxxxxx
			1110xxxx = 3 байта 10xxxxxx 10xxxxxx
			11110xxx = 4 байта 10xxxxxx 10xxxxxx 10xxxxxx
		*/
		char32_t operator* () const noexcept
		{
			char32_t codePoint = 0;
			// Общие строки перемещены и таким образом код стал оптимальнее
			char firstByte = str_->data_[curr_pos_];
		
			if (firstByte & kfirstBitMask)
			{
				char secondByte = str_->data_[curr_pos_+1];
				if (firstByte & kThirdBitMask)
				{
					char thirdByte = str_->data_[curr_pos_+2];
					if (firstByte & kfourthBitMask)
					{ // 4 БАЙТА
						codePoint = (firstByte & kthreeRightBits) << 18; // 3 младших бита сдвигаем влево на 18 позиций
						codePoint += (secondByte & ksixRightBits) << 12; // 6 младших битов второго байта сдвигаем влево на 12
						codePoint += (thirdByte & ksixRightBits) << 6;;	 // 6 младших битов третьего байта сдвигаем влево на 6
						char fourthByte = str_->data_[curr_pos_+3];
						codePoint += (fourthByte & ksixRightBits);	 // 6 младших битов четвёртого байта добавляем к итогу
					}
					else
					{ // 3 БАЙТА
						codePoint = (firstByte & kfourRightBits) << 12;	 // 3 младших бита сдвигаем влево на 12 позиций
						codePoint += (secondByte & ksixRightBits) << 6;	 // 6 младших битов второго байта сдвигаем влево на 6
						codePoint += (thirdByte & ksixRightBits);	 // 6 младших битов третьего байта добавляем к итогу
					}
				}
				else
				{ // 2 БАЙТА
					codePoint = (firstByte & kfiveRightBits) << 6;		 // 5 младших битов сдвигаем влево на 6
					codePoint += (secondByte & ksixRightBits);		 // 6 младших битов второго байта добавляем к итогу
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
		Iterator& operator++() noexcept
		{ // Преинкремент
//			assert(this);
			assert(str_); // Проверяем существование родителя
			if( curr_pos_ < str_->data_.length() )
			{
				curr_pos_ += str_->charlen(curr_pos_); // Смещаемся вперёд
			}
#ifdef DEBUG
			else
				str_->debug_ = "Ignore an attempt to go beyond end().";
#endif
			return *this;
		}

		Iterator operator++(int) noexcept
		{ // Постинкремент
//			assert(this);
			assert(str_); // Проверяем существование родителя
			Iterator save_pos_ = *this; // Запоминаем текущее значение
			++*this; // Используем преинкремент
			return save_pos_; // Возвращаем запомненное значение
		}

		Iterator& operator--() noexcept
		{ // Предекремент
//			assert(this);
			assert(str_); // Проверяем существование родителя
			if(curr_pos_ > 0)
			{
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
			}
#ifdef	DEBUG
			else
				str_->debug_ = "Ignore an attempt to go beyond begin().";
#endif
			return *this;
		}

		Iterator operator--(int) noexcept
		{ // Постдекремент
//			assert(this);
			assert(str_); // Проверяем существование родителя
			Iterator save_pos_ = *this; // Запоминаем текущее значение
			--*this; // Используем предекремент
			return save_pos_; // Возвращаем запомненное значение
		}

		bool operator==(const Iterator& rhs) const noexcept { return str_ ==  rhs.str_ && curr_pos_ == rhs.curr_pos_; }
		bool operator!=(const Iterator& rhs) const noexcept { return str_ !=  rhs.str_ || curr_pos_ != rhs.curr_pos_; }
	private: // модификатор относится только к двум переменным ниже
		UString * str_;
		size_t curr_pos_ = 0; // Текущая позиция итератора в utf8 строке
	};
	// продолжение публичной секции
	Iterator begin() noexcept
	{
//		assert(this);
		return Iterator(this);
	}

	Iterator end() noexcept
	{
//		assert(this);
		return Iterator(this, data_.length()); // Вызываем конструктор итератора за последний символ
	}
	// ~UString() {} // Деструктор по-умолчанию подходит.

private:
	std::string data_;
#ifdef	DEBUG
public:
	std::string debug_;
#endif
};
