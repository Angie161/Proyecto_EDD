#!/bin/bash

# Crear archivos CSV con cabeceras
echo "tecnica;tamaño;tiempo_ejecucion" > results.csv

for ((i = 1 ; i <= 20; i++))
do
    ./test $n >> results.csv
done