# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Компиляция программы
echo -e "${BLUE}Компиляция программы...${NC}"
gcc -Wall -Werror -Wextra -std=c99 *.c -o app.exe
if [ $? -ne 0 ]; then
    echo -e "${RED}Ошибка компиляции!${NC}"
    exit 1
fi

cat > test_cases.txt << 'EOF'
# ТЕСТЫ ДЛЯ УМНОЖЕНИЯ ДЛИННЫХ ЧИСЕЛ
# Формат: 
# input1 (целое число)
# input2 (дробное/вещественное число)
# expected_output



# Базовые тесты
123
456.0
+0.56088 E +5

0
123456.789
+0.0 E +0

1
1.0
+0.1 E +1

100
0.05
+0.5 E +1

-123
456.0
-0.56088 E +5

123
-456.0
-0.56088 E +5

-123
-456.0
+0.56088 E +5

123
1.234E+2
+0.151782 E +5

1
5.678E+1
+0.5678 E +2

100
1.0E-5
+0.1 E -2

99999
9.9999E+4
+0.9999800001 E +10

100000
1.0E+5
+0.1 E +11

1
0.0001e-99999
ERROR

100
0.09e-99999
+0.9 E -99998

1
1.0E-99999
+0.1 E -99998

12345
8.0E+4
+0.9876 E +9

123456
8.0E+4
+0.987648 E +10

0
0.0
+0.0 E +0

0
1.234E-10
+0.0 E +0

0
1.234E+10
+0.0 E +0

9999999999
1.0
+0.9999999999 E +10

9999999999
0.0000000001
+0.9999999999 E +0

1
0.0000000001
+0.1 E -9

1
1.0E-10
+0.1 E -9

-123
-456.0
+0.56088 E +5

-123
456.0
-0.56088 E +5

123
-456.0
-0.56088 E +5

1
0.123456789
+0.123456789 E +0

1
1.2345678912345678912345678912345678912
+0.12345678912345678912345678912345678912 E +1

999999999999999999999999999999999999999
99999999999999999999999999999.0
+0.999999999999999999999999999989999999999 E +68

EOF

echo -e "${BLUE}Запуск тестов...${NC}"
echo "=========================================="

total_tests=0
passed_tests=0

# Чтение тестовых случаев
while IFS= read -r input1 && IFS= read -r input2 && IFS= read -r expected; do
    # Пропускаем комментарии и пустые строки
    if [[ "$input1" =~ ^# ]] || [[ -z "$input1" ]]; then
        continue
    fi
    
    total_tests=$((total_tests + 1))
    
    # Запуск программы с автоматическим вводом двух чисел
    actual=$(printf "%s\n%s\n" "$input1" "$input2" | ./app.exe 2>/dev/null)  

    # Убираем лишние пробелы и переводы строк для сравнения
    actual_clean=$(echo "$actual" | tr -d ' ' | tr -d '\n')
    expected_clean=$(echo "$expected" | tr -d ' ' | tr -d '\n')
    
    # Сравнение результатов
    if [ "$actual_clean" = "$expected_clean" ]; then
        echo -e "TEST $total_tests: ${GREEN}PASSED${NC}"
        echo "  Input 1:  $input1"
        echo "  Input 2:  $input2"
        echo "  Expected: $expected"
        echo "  Actual:   $actual"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "TEST $total_tests: ${RED}FAILED${NC}"
        echo "  Input 1:  $input1"
        echo "  Input 2:  $input2"
        echo "  Expected: $expected"
        echo "  Actual:   $actual"
    fi
    echo "------------------------------------------"
    
    # Пропускаем пустую строку между тестами
    IFS= read -r empty_line
    
done < test_cases.txt

# Удаляем временные файлы
rm -f test_cases.txt

echo "=========================================="
echo -e "${BLUE}РЕЗУЛЬТАТЫ:${NC}"
echo -e "Пройдено: ${GREEN}$passed_tests${NC} / ${BLUE}$total_tests${NC}"

if [ $passed_tests -eq $total_tests ]; then
    echo -e "${GREEN}Все тесты пройдены успешно!${NC}"
    exit 0
else
    echo -e "${RED}Есть непройденные тесты!${NC}"
    exit 1
fi