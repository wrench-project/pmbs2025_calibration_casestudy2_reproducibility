from typing import List
import numpy as np


def average_explained_variance_error(x_simulated: List[float], y_real: List[List[float]]) -> str:
    assert len(x_simulated) == len(y_real), "Length of simulated and real data must be the same:\n x_simulated: {}\n y_real: {}\n".format(x_simulated, y_real)
    overall_loss = []

    for i in range(len(y_real)):
        y_real[i] = np.array(y_real[i])
        numerator = np.sqrt(np.sum(list(np.power(x_simulated[i] - y_real[i], 2))))
        denominator = np.sqrt(np.sum(list(np.power(y_real[i] - np.mean(y_real[i]), 2))))

        if denominator == 0:
            denominator = 1

        loss = numerator / denominator

        overall_loss.append(loss)

    overall_loss = np.array(overall_loss)

    return np.mean(overall_loss)

def max_explained_variance_error(x_simulated: List[float], y_real: List[List[float]]) -> str:
    assert len(x_simulated) == len(y_real), "Length of simulated and real data must be the same:\n x_simulated: {}\n y_real: {}\n".format(x_simulated, y_real)
    overall_loss = []

    for i in range(len(y_real)):
        y_real[i] = np.array(y_real[i])
        numerator = np.sqrt(np.sum(list(np.power(x_simulated[i] - y_real[i], 2))))
        denominator = np.sqrt(np.sum(list(np.power(y_real[i] - np.mean(y_real[i]), 2))))

        if denominator == 0:
            denominator = 1

        loss = numerator / denominator

        overall_loss.append(loss)

    overall_loss = np.array(overall_loss)

    return np.max(overall_loss)