from sklearn import metrics
import kmeans
import mysymnmf
import symnmf
import sys


def main(k, file_name):
    """
       Main function - Calculates the silhouette score of Kmeans and SymNMF
       :param k: number of clusters
       :param file_name: the file containing the data points
       :return: int: 0 if running was successful, else 1
       """

    # Gets the vectors from the file
    X = symnmf.get_vectors_from_file(file_name)
    n = len(X)

    # Calculates the Normalized Similarity Matrix
    try:
        W = mysymnmf.norm(X)
    except Exception as e:
        print("An Error Has Occurred")
        return 1

    # Calculates the average of all entries of W
    m = float(symnmf.matrix_average(W))

    # Builds the initialized H
    H = symnmf.build_h(len(X), k, m)

    # Calculates the SymNMF algorithm
    try:
        result_symnmf = mysymnmf.symnmf(H, W, n, k)
    except Exception as e:
        print("An Error Has Occurred")
        return 1

    # Calculates the clusters for each vector for SymNMF
    symnmf_clusters_per_vector = [-1 for i in range(n)]
    for i in range(len(result_symnmf)):
        max_value_for_vector = max(result_symnmf[i])
        curr_cluster = result_symnmf[i].index(max_value_for_vector)
        symnmf_clusters_per_vector[i] = curr_cluster

    # Calculates the clusters for each vector with kmeans algorithm (HW1)
    kmeans_clusters_per_vector = kmeans.clusters(k, file_name)

    # Calculates the metrics for symnmf
    symnmf_coefficient = metrics.silhouette_score(X, symnmf_clusters_per_vector)

    # Calculates the metrics for kmeans
    kmeans_coefficient = metrics.silhouette_score(X, kmeans_clusters_per_vector)

    # Prints the results
    str_nmf = '%.4f' % symnmf_coefficient
    str_kmeans = '%.4f' % kmeans_coefficient
    print("nmf: " + str_nmf)
    print("kmeans: " + str_kmeans)

    return 0


if __name__ == '__main__':
    main(int(sys.argv[1]), sys.argv[2])