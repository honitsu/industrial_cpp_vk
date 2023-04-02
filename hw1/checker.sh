#!/usr/bin/ksh
# Часть строк utf8 взята отсюда: https://www.w3.org/2001/06/utf-8-test/UTF-8-demo.html
# убраны ведущие и финишные пробелы и добавлены 4-байтные символы

if [ ! -f /usr/bin/ksh ]; then
	echo "Работа с интерпретатором, отличным от ksh не гарантируется"
fi
APP_TO_RUN="./showstats"
DATA_FILE="test.dat"
IFS="	" # Tab
linenum=0
cat ${DATA_FILE} | while read len size b1 b2 b3 b4 line; do
	((linenum++))
	if [ ${len} -ne 0 ]; then
		print ${line}"Z" | ${APP_TO_RUN} | read -r rlen rsize rb1 rb2 rb3 rb4 rline
		line="${line}Z" # Добавляем символ к строке
		size=${#line} # Вычисляем длину строки в байтах для сравнения со значением из программы
		# Корректируем счётчики, чтобы учесть добавление символа ASCII к строке
		((len++))
		((b1++))
		for v in len size b1 b2 b3 b4 line; do
			rv=r$v
			# Получаем значение переменной из имени, хранящемся в другой переменной
			nameref var=${v}
			nameref rvar=${rv}
			if [ ${rline} == "ጌጥ ያለቤቱ ቁምጥና ነው።Z" ]; then
				print "Симуляция ошибки для строки на амхарском:"
				rline="Красота — это аскетизм без дома."
			fi
			if [ ${var} != ${rvar} ]; then
				print "Файл ${DATA_FILE}, Строка #${linenum}: ожидалось значение переменной ${v} [${var}], оказалось - [${rvar}]"
			fi
		done
	fi
done
exit 0
