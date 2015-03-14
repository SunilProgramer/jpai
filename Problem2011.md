# Proximity #

---

## Условие ##

| **Входной файл:** | input.txt |
|:-----------------------------|:----------|
| **Выходной файл:** | output.txt |
| **Временный файл:** | temp.txt |
| **Время:** | 1 сек. |

В игре участвует 2 или более игроков.

Игровое поле состоит из H рядов по W шестиугольных ячеек в каждом. Каждая ячейка либо пуста, либо занята одним из игроков. Каждая занятая ячейка содержит целое число от 1 до 20.

Игроки ходят по очереди. Перед начало хода игроку выдаётся число целое число от 1 до 20. Ход состоит в том, что игрок выбирает одну из свободных ячеек поля. Выбранная ячейка становится занятой текущим игроком и содержащей выданное число. После этого:
Числа во всех соседних ячейках, занятые текущим игроком, и меньшие 20, увеличиваются на единицу.
Все соседние ячейки, занятые другими игроками, и содержащие меньшие числа, переходят к текущему игроку.

Игра продолжается до тех пор, пока все клетки поля не станут заняты.

Побеждает игрок, занявший наибольшее количество клеток.

Числа, выдаваемые игрокам, определяются до начала игры. Игрокам перед каждым ходом сообщаются числа, которые будут выданы в течении нескольких следующих ходов.

## Формат входного файла ##

Входной файл состоит из чисел W (ширина поля) H (высота поля) N T (число игроков), за которыми следуют N чисел от 1 до 20, которые будут выданы игрокам в течении N ходов, начиная с текущего. Далее следует W × H описаний ячеек, перечисленных по рядам слева направо. Ячейка описывается парой чисел p v, где p -- номер игрока, которому принадлежит ячейка, v -- число в ячейке. Для пустых ячеек оба числа равны 0.

**Формат входного файла предполагает, что текущий игрок -- первый.**

## Формат выходного файла ##

Выходной файл должен содержать два числа -- номер ячейки в ряду и номер ряда. Номера отсчитываются начиная с 1.

**Некорректные данные в выходном файле приводят к пропуску хода.**