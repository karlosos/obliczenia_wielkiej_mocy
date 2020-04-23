Kompilacja jak run.sh

Aby przetestować różne typy schedule należy przed wywołaniem ustawić zmienną:

```
export OMP_SCHEDULE="dynamic" 
```

jak na wikipedii

Do testowania

```
1 export OMP_SCHEDULE="static"
2 export OMP_SCHEDULE="static, 100"
3 export OMP_SCHEDULE="static, 400"
4 export OMP_SCHEDULE="static, 1000"
5 export OMP_SCHEDULE="dynamic"
6 export OMP_SCHEDULE="dynamic, 100"
7 export OMP_SCHEDULE="dynamic, 400"
8 export OMP_SCHEDULE="dynamic, 1000"
9 export OMP_SCHEDULE="guided"
10 export OMP_SCHEDULE="guided, 100"
11 export OMP_SCHEDULE="guided, 400"
12 export OMP_SCHEDULE="guided, 1000"
```
