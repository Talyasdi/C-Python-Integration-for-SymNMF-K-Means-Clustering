
# C-Python-Integration-for-SymNMF-K-Means-Clustering READNE

This project implements Symmetric Non-negative Matrix Factorization (SymNMF) and K-Means clustering algorithms. The implementation is divided into Python modules and corresponding C extension modules.

## Files

### SymNMF
1. **symnmf.py**
   - Main Python module for SymNMF.
   - Provides functions for reading data, building matrices, and performing various operations like calculating similarity matrices, diagonal degree matrices, normalized similarity matrices, and performing full SymNMF.

2. **symnmfmodule.c**
   - C extension module containing core functionality for SymNMF.
   - Implements functions that act as wrappers for corresponding C functions (`symnmf_c`, `sym_c`, `ddg_c`, `norm_c`) performing matrix computations related to Symmetric Non-negative Matrix Factorization.

3. **symnmf.c**
   - C source file containing the actual implementation of SymNMF functions.

4. **symnmf.h**
   - Header file for the C extension module, defining function prototypes and necessary includes.

### K-Means
5. **kmeans.py**
   - Python module for K-Means clustering.
   - Implements K-Means algorithm for clustering data points into 'k' clusters.

### Analysis
6. **analysis.py**
   - Python module for calculating silhouette scores using SymNMF and K-Means.
   - Utilizes functions from `symnmf.py` and `kmeans.py` to calculate and compare silhouette scores.

### Configuration
7. **setup.py**
   - Configuration file for setuptools to build the C extension module (`mysymnmf`).

8. **Makefile**
   - Makefile for compiling the C extension module.

## Usage

### Running SymNMF Algorithm

#### Compiling the C Module:

```bash
make
```

#### Running SymNMF with Example Arguments:

```bash
python symnmf.py 3 sym data.txt
```

- `3`: Number of clusters.
- `sym`: Goal (Symmetric Non-negative Matrix Factorization).
- `data.txt`: Input file containing data points.

### Running KMeans Algorithm

#### Running KMeans with Example Arguments:

```bash
python kmeans.py 3 data.txt
```

- `3`: Number of clusters.
- `data.txt`: Input file containing data points.

### Running Analysis Script

#### Running Analysis Script with Example Arguments:

```bash
python analysis.py 3 data.txt
```

- `3`: Number of clusters.
- `data.txt`: Input file containing data points.

Make sure to replace `3` and `data.txt` with the desired number of clusters and the actual data file path. Adjust the commands according to your specific use case and file locations.

## Dependencies

- Python 3
- NumPy
- scikit-learn
