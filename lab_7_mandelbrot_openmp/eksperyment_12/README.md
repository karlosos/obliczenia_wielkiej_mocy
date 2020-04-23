Kompilacja jak run.sh

Aby przetestować różne typy schedule należy przed wywołaniem ustawić zmienną:

```
export OMP_SCHEDULE="dynamic" 
```

jak na wikipedii

Do testowania

```
x 1 export OMP_SCHEDULE="static"
x 2 export OMP_SCHEDULE="static, 100"
x 3 export OMP_SCHEDULE="static, 400"
x 4 export OMP_SCHEDULE="static, 1000"
x 5 export OMP_SCHEDULE="dynamic"
x 6 export OMP_SCHEDULE="dynamic, 100"
x 7 export OMP_SCHEDULE="dynamic, 400"
x 8 export OMP_SCHEDULE="dynamic, 1000"
9 export OMP_SCHEDULE="guided"
10 export OMP_SCHEDULE="guided, 100"
11 export OMP_SCHEDULE="guided, 400"
12 export OMP_SCHEDULE="guided, 1000"
```
