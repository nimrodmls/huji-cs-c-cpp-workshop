//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

/**
 * @class MlpNetwork
 * @brief Represents a neural network with multiple layers
 */
class MlpNetwork
{
public:
	/**
	* Constructs a neural network of 4 layers.
	* @param weights - The weights matrices for each layer
	* @param biases - The biases matrices for each layer.
	*/
	MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]);

	// Explicitly defining behavior to prevent implicit behavior
	MlpNetwork() = delete;
	MlpNetwork(const MlpNetwork&) = delete;
	MlpNetwork& operator=(MlpNetwork&) = delete;
	~MlpNetwork() = default;

	/**
	* Activates the neural network on a given image.
	* @param image - The image to analyze.
	* @return The neural network results.
	*/
	digit operator()(Matrix image) const;

private:
	// All layers of the network
	const Dense _layer1;
	const Dense _layer2;
	const Dense _layer3;
	const Dense _layer4;
};

#endif // MLPNETWORK_H