import math
import sys
import numpy as np
import mysymnmf

SYMNMF = 'symnmf'
DIAGONAL = 'ddg'
NORM = 'norm'
SIMILARITY = 'sym'

np.random.seed(0)

def get_vectors_from_file(filename):
    """
    Reads the txt file and creates a nested list  - a list of datapoint vectors
    :param filename: the name of the file of data points
    :return: nested list: list of data points
    """

    vectors = []
    with open(filename) as f:
        for line in f:
            vector = line.split(",")
            # Remove \n from last coordinate
            vector[-1] = vector[-1][:-1]
            curr_vector = []
            for coordinate in vector:
                curr_vector.append(float(coordinate))

            vectors.append(curr_vector)
    return vectors


def build_h(n, k, m):
    """
       Creates a random H matrix for initialization.
       :param n: number of rows (number of datapoints)
       :param k: number of columns (clusters)
       :param m: average of entries of W
       :return: nested list: the matrix H
       """

    lower_limit = 0.0
    upper_limit = 2 * math.sqrt(m/k)
    H = []
    for i in range(n):
        row = []
        for j in range(k):
            row.append(np.random.uniform(lower_limit, upper_limit))
        H.append(row)

    return H


def matrix_average(matrix):
    """
    Calculates the average of all entries of a given matrix.
    :param matrix: the matrix
    :return: float: The average of the entries
    """

    sum = 0
    counter = 0
    for row in matrix:
        for num in row:
            sum += num
            counter += 1

    return sum / counter

def print_result_final_matrix(matrix):
    """
    Prints the final matrix.
    :param matrix: the list of final vectors
    :return: void
    """

    for vector in matrix:
        output_str = ",".join([("%.4f" % item) for item in vector])
        print(output_str)


def main(k, goal, file_name):
    """
    Main function - Calculates result matrices according to the goal
    :param k: number of clusters
    :param goal: the matrix to calculate
    :param file_name: the file containing the data points
    :return: int: 0 if running was successful, else 1
    """

    X = get_vectors_from_file(file_name)
    result = []

    if goal == SIMILARITY:
        try:
            result = mysymnmf.sym(X)
        except Exception as e:
            print("An Error Has Occurred")
            return 1

    elif goal == DIAGONAL:
        try:
            result = mysymnmf.ddg(X)
        except Exception as e:
            print("An Error Has Occurred")
            return 1

    elif goal == NORM:
        try:
            result = mysymnmf.norm(X)
        except Exception as e:
            print("An Error Has Occurred")
            return 1

    else:
        try:
            W = mysymnmf.norm(X)
        except Exception as e:
            print("An Error Has Occurred")
            return 1

        m = float(matrix_average(W))
        H = build_h(len(X), k, m)

        try:
            result = mysymnmf.symnmf(H,W,len(X), k)
        except Exception as e:
            print("An Error Has Occurred")
            return 1

    print_result_final_matrix(result)
    return 0


if __name__ == '__main__':
    main(int(sys.argv[1]), sys.argv[2], sys.argv[3])