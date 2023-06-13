#include "MlpNetwork.h"

MlpNetwork::MlpNetwork(
	Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]) :
	_layer1(weights[0], biases[0], activation::relu),
	_layer2(weights[1], biases[1], activation::relu),
	_layer3(weights[2], biases[2], activation::relu),
	_layer4(weights[3], biases[3], activation::softmax)
{}

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