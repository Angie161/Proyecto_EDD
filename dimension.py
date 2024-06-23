import pandas as pd
import matplotlib.pyplot as plt


def calcular_promedio_tamanos(df, tecnica, procesamiento):
    if tecnica == "Huffman" and procesamiento == "decodificacion":
        return df[df["procesamiento_datos"] == "decodificacion"][
            "dimension_archivo"
        ].mean()
    elif tecnica == "LempelZiv" and procesamiento == "descompresion":
        return df[df["procesamiento_datos"] == "descompresion"][
            "dimension_archivo"
        ].mean()


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

promedios_huffman_codificacion = []
promedios_lz_compresion = []

for file_huffman in files_huffman:
    df_huffman = pd.read_csv(file_huffman, delimiter=";")
    promedio_huffman_codificacion = calcular_promedio_tamanos(
        df_huffman, "Huffman", "decodificacion"
    )
    promedios_huffman_codificacion.append(promedio_huffman_codificacion)

for file_lz in files_lz:
    df_lz = pd.read_csv(file_lz, delimiter=";")
    promedio_lz_compresion = calcular_promedio_tamanos(
        df_lz, "LempelZiv", "descompresion"
    )
    promedios_lz_compresion.append(promedio_lz_compresion)

tamanos_datasets = [10, 20, 30, 40, 50]

plt.figure(figsize=(10, 6))

plt.plot(
    tamanos_datasets,
    promedios_huffman_codificacion,
    marker="o",
    linestyle="-",
    color="b",
    label="Huffman - Decodificación",
)

plt.plot(
    tamanos_datasets,
    promedios_lz_compresion,
    marker="o",
    linestyle="-",
    color="r",
    label="LZ - Descompresión",
)

plt.xlabel("Tamaño del Dataset (MB)")
plt.ylabel("Promedio de Tamaño de Archivo (B)")
plt.title("Comparación de Promedios de Tamaños de Archivo (Huffman vs Lempel-Ziv)")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()
