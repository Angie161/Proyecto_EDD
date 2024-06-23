import pandas as pd
import matplotlib.pyplot as plt


def cargar_y_filtrar_datos(file_path, tecnica, procesamiento):
    df = pd.read_csv(file_path, delimiter=";")
    return df[(df["tecnica"] == tecnica) & (df["procesamiento_datos"] == procesamiento)]


files_huffman = [
    "results_huffman_10MB.csv",
    "results_huffman_20MB.csv",
    "results_huffman_30MB.csv",
    "results_huffman_40MB.csv",
    "results_huffman_50MB.csv",
]

files_lz = [
    "results_LZ_10MB.csv",
    "results_LZ_20MB.csv",
    "results_LZ_30MB.csv",
    "results_LZ_40MB.csv",
    "results_LZ_50MB.csv",
]

tiempos_decodificacion_huffman = []
tiempos_descompresion_lz = []

for file_huffman in files_huffman:
    df_huffman_decodificacion = cargar_y_filtrar_datos(
        file_huffman, "Huffman", "decodificacion"
    )
    tiempos_decodificacion_huffman.append(
        df_huffman_decodificacion["tiempo_ejecucion"].mean()
    )

for file_lz in files_lz:
    df_lz_descompresion = cargar_y_filtrar_datos(file_lz, "LempelZiv", "descompresion")
    tiempos_descompresion_lz.append(df_lz_descompresion["tiempo_ejecucion"].mean())

tamanos_datasets = [10, 20, 30, 40, 50]

plt.figure(figsize=(12, 8))
plt.plot(
    tamanos_datasets,
    tiempos_decodificacion_huffman,
    marker="o",
    linestyle="-",
    color="b",
    label="Huffman - Decodificación",
)

plt.plot(
    tamanos_datasets,
    tiempos_descompresion_lz,
    marker="o",
    linestyle="-",
    color="r",
    label="LZ - Descompresión",
)

plt.xlabel("Tamaño del Dataset (MB)")
plt.ylabel("Tiempo Promedio de Ejecución (ms)")
plt.title("Comparación de Tiempos de Ejecución (Huffman vs Lempel-Ziv)")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()
