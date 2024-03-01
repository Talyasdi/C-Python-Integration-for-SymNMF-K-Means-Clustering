import sys

EPSILON = 0.0001
ERROR_GENERAL = "An Error Has Occurred"


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
            int_vector = []
            for coordinate in vector:
                int_vector.append(float(coordinate))

            vectors.append(int_vector)
    return vectors


def euclidean_distance(vector1, vector2):
    """
    Calculates the euclidian distance between two vectors given as lists
    :param vector1: first vector
    :param vector2: second vector
    :return: float: the distance between the vectors
    """

    result_not_sqrt = 0.0
    # Vectors are not in the same length
    if len(vector1) != len(vector2):
        return -1

    for i in range(len(vector1)):
        result_not_sqrt += pow(vector1[i] - vector2[i], 2)
    result = pow(result_not_sqrt, 0.5)

    return result


def update_centroid(vectors, centroid_index, vectors_cluster, d, n):
    """
    Calculates the new centroid vector for the cluster index (centroid_index)
    :param vectors: all data points
    :param centroid_index: the index of cluster of the centroid
    :param vectors_cluster: list of index of cluster per data point
    :param d: number of coordinates
    :param n: number of data points
    :return: list: the new calculated centroid
    """

    new_centroid = [0 for i in range(d)]
    number_of_vectors = 0.0

    for i in range(n):
        # Update the list of cluster numbers for each datapoint
        if vectors_cluster[i] == centroid_index:
            number_of_vectors += 1.0
            for j in range(d):
                new_centroid[j] += vectors[i][j]

    for i in range(d):
        new_centroid[i] /= number_of_vectors

    return new_centroid


def update_vector_cluster(vector, centroids):
    """
    Finds the new cluster of the datapoint vector and returns its index
    :param vector: the data point
    :param centroids: the list of centroids
    :return: int: index of new cluster for the vector
    """

    cluster_num = 0
    min_dist = sys.maxsize

    for i in range(0, len(centroids)):
        curr_dist = euclidean_distance(vector, centroids[i])
        # In case vectors are not in the same length
        if curr_dist == -1:
            print(ERROR_GENERAL)
            exit(1)
        if curr_dist < min_dist:
            min_dist = curr_dist
            cluster_num = i

    return cluster_num


def print_result(centroids):
    """
    Prints the results
    :param centroids: the list of final centroids
    :return: void
    """

    for vector in centroids:
        output_str = ",".join([("%.4f" % item) for item in vector])
        print(output_str)


def clusters(k, filename):
    """
    Updated - used to be the main function of HW1 - now returns the cluster of each datapoint
    :param k: number of clusters
    :param filename: file of datapoints
    :return: cluster of each datapoint
    """

    max_iter = 300
    vectors = get_vectors_from_file(filename)
    n = len(vectors)
    k = int(k)

    # In order to determine if all delta centroids are less than EPSILON
    check_under_epsilon = 1

    curr_iter = 0
    centroids = [[] for i in range(k)]
    cluster_per_vector = [-1 for i in range(n)]  # Maintaining each datapoint's cluster index

    # Checking if all datapoints are of the same dimension
    d = len(vectors[0])
    for i in range(n):
        if len(vectors[i]) != d:
            print(ERROR_GENERAL)
            return 1

    # Initialising centroids to the first k datapoints
    for i in range(k):
        centroids[i] = vectors[i][:]
        cluster_per_vector[i] = i

    while (check_under_epsilon == 1) and (curr_iter < max_iter):
        curr_iter += 1
        # Assign each data point to the closest cluster
        for i in range(n):
            new_cluster = update_vector_cluster(vectors[i], centroids)
            cluster_per_vector[i] = new_cluster

        # Update the centroids and check if the difference is under epsilon
        check_under_epsilon = 0
        for i in range(k):
            new_centroid_vector = update_centroid(vectors, i, cluster_per_vector, d, n)
            difference_new_and_old_centroids = euclidean_distance(centroids[i], new_centroid_vector)
            # In case vectors are not in the same length
            if difference_new_and_old_centroids == -1:
                print(ERROR_GENERAL)
                exit(1)
            centroids[i] = new_centroid_vector
            if difference_new_and_old_centroids >= EPSILON:
                check_under_epsilon = 1

    return cluster_per_vector

