Kompilacja jak run.sh

Aby przetestować różne typy schedule należy przed wywołaniem ustawić zmienną:

```
export OMP_SCHEDULE="dynamic" 
```

jak na wikipedii

Do testowania

```
export OMP_SCHEDULE="static, 100"
export OMP_SCHEDULE="static, 400"
export OMP_SCHEDULE="static, 1000"
export OMP_SCHEDULE="dynamic"
export OMP_SCHEDULE="dynamic, 100"
export OMP_SCHEDULE="dynamic, 400"
export OMP_SCHEDULE="dynamic, 1000"
export OMP_SCHEDULE="guided"
export OMP_SCHEDULE="guided, 100"
export OMP_SCHEDULE="guided, 400"
export OMP_SCHEDULE="guided, 1000"

```
