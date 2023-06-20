#include "MlpNetwork.h"

typedef enum layer_index
{
	LAYER_INDEX_1 = 0,
	LAYER_INDEX_2,
	LAYER_INDEX_3,
	LAYER_INDEX_4,

	MAX_LAYERS
} layer_index;

// See documentation at header file
MlpNetwork::MlpNetwork(
	Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]) :
	_layer1(
		weights[LAYER_INDEX_1], 
		biases[LAYER_INDEX_1], 
		activation::relu),
	_layer2(
		weights[LAYER_INDEX_2],
		biases[LAYER_INDEX_2],
		activation::relu),
	_layer3(
		weights[LAYER_INDEX_3],
		biases[LAYER_INDEX_3],
		activation::relu),
	_layer4(
		weights[LAYER_INDEX_4],
		biases[LAYER_INDEX_4],
		activation::softmax)
{}

// See documentation at header file
digit MlpNetwork::operator()(Matrix image) const
{
	auto output = _layer4(_layer3(_layer2(
		_layer1(image.vectorize()))));
	auto result_index = output.argmax();

	return { 
		static_cast<unsigned int>(result_index), 
		output[result_index]
	};
}