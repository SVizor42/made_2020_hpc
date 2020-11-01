# Домашнее задание №4. Pagerank, матрицы и графы
### Возведение матрицы в степень
Реализован алгоритм [возмедения матрицы в степень](https://github.com/SVizor42/made_2020_hpc/tree/main/assignment4/power) за O(N<sup>3</sup> * log<sub>2</sub>(m)). Для сборки можно использовать имеющийся Makefile. \
При запуске в качестве аргументов можно передать размер матрицы `N` и степень `power`:
```sh
$ ./<executable> N power
```

### Алгоритм Pagerank
Реализован итеративный подход для расчета процедуры [Pagerank](https://github.com/SVizor42/made_2020_hpc/tree/main/assignment4/pagerank). На вход программа принимает граф, который может быть получен из внешнего файла или сгенерирован случайным образом.

Компиляция:
```sh
$ g++ -fopenmp -O3 pagerank.c -o <executable>
```
При запуске в качестве аргументов можно передать размер генерируемой матрицы `N`, точность `EPS` и путь к внешнему файлу `File`:
```sh
$ ./<executable> N EPS File
```

В качестве примера при проведении расчетов использовался файл с графом связности станций московского метро.

### Оптимизации
В приведенных программах были использованы различные оптимизации: 
  - грамотное расположение циклов;
  - сборка c использованием флага `-O3`;
  - распараллеливание при помощи библиотеки OpenMP.