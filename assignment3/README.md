# Домашнее задание №3. OpenMP
### Демонстрационные файлы
Папка [demos](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/demos/) содержит демонстрационные файлы – работающие примеры кодов для понимания OpenMP.
| Файл | Описание |
| ------ | ------ |
| [omp_hello.c](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/demos/omp_hello.c) | Введение в конструкцию `parallel`|
| [omp_outmes.cpp](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/demos/omp_outmes.c) | Пример конструкции `critical` |
| [omp_privateshared.c](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/demos/omp_privateshared.c) | Пример общих и приватных переменных, конструкции `for` |
| [omp_parsec.c](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/demos/omp_parsec.c) | Пример использования конструкции `section` |
| [omp_sumarray.c](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/demos/omp_sumarray.c) | Пример использования редукции |

Компиляция:
```sh
$ <cc> <codefile.c> -o <executable> -fopenmp
```
Запуск:
```sh
$ ./<executable>
```
### Исправленные программы
В папке [fixed](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/fixed/) находятся исправленные файлы программ.
### Оценки числа pi
Реализована параллельная программа, использующая метод Монте-Карло для оценки числа [pi](https://github.com/SVizor42/made_2020_hpc/blob/main/assignment3/pi/omp_pi_calc.c). Для сборки можно использовать имеющийся Makefile.
