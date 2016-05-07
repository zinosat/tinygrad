//
// Created by niko on 4/25/16.
//

#include "models.h"
#include "utils.h"

Tensor* ClassifierLogisticRegression::evaluate(Tensor* input){
    this->g.clean();
    return this->g.forward(input, this->n1, this->n2);
}

void ClassifierLogisticRegression::sgd(Tensor *input, Tensor *target, double lr){
    this->evaluate(input);
    this->n3->update_target(target);
    this->g.backward(this->n3, this->n1);
    for(int i=0; i<this->data_dimensionality; i++){
        this->weights.data[i] = this->weights.data[i] - lr * this->n1->grad[0]->data[i];
    }
}

ClassifierLogisticRegression::ClassifierLogisticRegression(unsigned int data_dimensionality) : weights(data_dimensionality, 1, true){
    this->data_dimensionality = data_dimensionality;
    this->weights.guarded = true;
    for(int i=0;i < data_dimensionality; i++){
        this->weights.data[i] = (double)rand() / RAND_MAX;
    }
    this->n1 = new NodeMultiplyRightWithMatrix(&this->weights);
    this->n2 = new NodeElementWiseSigmoidFunction;
    this->n3 = new NodeBinaryCrossEntropy();
    this->g.add_node(this->n1);
    this->g.add_node(this->n2);
    this->g.add_node(this->n3);
    this->g.connect_to(0, 1);
    this->g.connect_to(1, 2);
}

ClassifierLogisticRegression::~ClassifierLogisticRegression(void){
    this->g.clean();
    delete[] this->weights.data;
    delete this->n1;
    delete this->n2;
    delete this->n3;
}

/*
 * A reference implementation of an autoencoder with one hidden layer and sigmoid activations.
 */
Tensor* AutoencoderSigmoidActivationsOneHiddenLayer::evaluate(Tensor* input){
    this->g.clean();
    return this->g.forward(input, this->n1, this->n3);
}

void AutoencoderSigmoidActivationsOneHiddenLayer::sgd(Tensor *input, Tensor *target, double lr){
    this->evaluate(input);
    this->n4->update_target(target);
    this->g.backward(this->n4, this->n1);
    for(int i=0; i<this->w1.size(); i++){
        this->w1.data[i] = this->w1.data[i] - lr * this->n1->grad[0]->data[i];
    }
    this->g.clear_gradients();
    this->g.backward(this->n4, this->n3);
    for(int i=0; i<this->w2.size(); i++){
        this->w2.data[i] = this->w2.data[i] - lr * this->n3->grad[0]->data[i];
    }
}

AutoencoderSigmoidActivationsOneHiddenLayer::AutoencoderSigmoidActivationsOneHiddenLayer(
        unsigned int data_dimensionality, unsigned int n_hidden) : w1(data_dimensionality, n_hidden, true), w2(n_hidden, data_dimensionality, true) {
    this->data_dimensionality = data_dimensionality;
    this->n_hidden = n_hidden;
    this->w1.guarded = true;
    this->w2.guarded = true;
    for(int i=0;i < data_dimensionality*n_hidden; i++){
        this->w1.data[i] = (double)rand() / RAND_MAX;
        this->w2.data[i] = (double)rand() / RAND_MAX;
    }
    this->n1 = new NodeMultiplyRightWithMatrix(&this->w1);
    this->n2 = new NodeElementWiseSigmoidFunction;
    this->n3 = new NodeMultiplyRightWithMatrix(&this->w2);
    this->n4 = new NodeSquaredError();
    this->g.add_node(this->n1);
    this->g.add_node(this->n2);
    this->g.add_node(this->n3);
    this->g.add_node(this->n4);
    this->g.connect_to(0, 1);
    this->g.connect_to(1, 2);
    this->g.connect_to(2, 3);
}

AutoencoderSigmoidActivationsOneHiddenLayer::~AutoencoderSigmoidActivationsOneHiddenLayer(void){
    this->g.clean();
    delete[] this->w1.data;
    delete[] this->w2.data;
    delete this->n1;
    delete this->n2;
    delete this->n3;
    delete this->n4;
}


/*
 * A reference implementation of a neural network with one hidden layer, sigmoid
 * activations and one sigmoid output (single activation).
 */
Tensor* ClassifierNeuralNetworkSigmoidActivationsOneHiddenLayer::evaluate(Tensor* input){
    this->g.clean();
    return this->g.forward(input, this->n1, this->n4);
}

void ClassifierNeuralNetworkSigmoidActivationsOneHiddenLayer::sgd(Tensor *input, Tensor *target, double lr){
    this->evaluate(input);
    this->n5->update_target(target);
    this->g.backward(this->n5, this->n1);
    for(int i=0; i<this->w1.size(); i++){
        this->w1.data[i] = this->w1.data[i] - lr * this->n1->grad[0]->data[i];
    }
    this->g.clear_gradients();
    this->g.backward(this->n5, this->n3);
    for(int i=0; i<this->w2.size(); i++){
        this->w2.data[i] = this->w2.data[i] - lr * this->n3->grad[0]->data[i];
    }
}

ClassifierNeuralNetworkSigmoidActivationsOneHiddenLayer::ClassifierNeuralNetworkSigmoidActivationsOneHiddenLayer(
        unsigned int data_dimensionality, unsigned int n_hidden) : w1(data_dimensionality, n_hidden, true), w2(n_hidden, 1, true) {
    this->data_dimensionality = data_dimensionality;
    this->n_hidden = n_hidden;
    this->w1.guarded = true;
    this->w2.guarded = true;
    for(int i=0;i < data_dimensionality*n_hidden; i++){
        this->w1.data[i] = (double)rand() / RAND_MAX;
    }
    for(int i=0;i < n_hidden; i++){
        this->w2.data[i] = (double)rand() / RAND_MAX;
    }
    this->n1 = new NodeMultiplyRightWithMatrix(&this->w1);
    this->n2 = new NodeElementWiseSigmoidFunction;
    this->n3 = new NodeMultiplyRightWithMatrix(&this->w2);
    this->n4 = new NodeElementWiseSigmoidFunction;
    this->n5 = new NodeBinaryCrossEntropy();
    this->g.add_node(this->n1);
    this->g.add_node(this->n2);
    this->g.add_node(this->n3);
    this->g.add_node(this->n4);
    this->g.add_node(this->n5);
    this->g.connect_to(0, 1);
    this->g.connect_to(1, 2);
    this->g.connect_to(2, 3);
    this->g.connect_to(3, 4);
}

ClassifierNeuralNetworkSigmoidActivationsOneHiddenLayer::~ClassifierNeuralNetworkSigmoidActivationsOneHiddenLayer(void){
    this->g.clean();
    delete[] this->w1.data;
    delete[] this->w2.data;
    delete this->n1;
    delete this->n2;
    delete this->n3;
    delete this->n4;
    delete this->n5;
}

/*
 * A reference implementation of a neural network with two hidden layers, sigmoid
 * activations and one sigmoid output (single activation).
 */
Tensor* ClassifierNeuralNetworkSigmoidActivationsTwoHiddenLayers::evaluate(Tensor* input){
    this->g.clean();
    return this->g.forward(input, this->n1, this->n6);
}

void ClassifierNeuralNetworkSigmoidActivationsTwoHiddenLayers::sgd(Tensor *input, Tensor *target, double lr){
    this->evaluate(input);
    this->n7->update_target(target);
    this->g.backward(this->n7, this->n1);
    for(int i=0; i<this->w1.size(); i++){
        this->w1.data[i] = this->w1.data[i] - lr * this->n1->grad[0]->data[i];
    }
    this->g.clear_gradients();
    this->n7->update_target(target);
    this->g.backward(this->n7, this->n3);
    for(int i=0; i<this->w2.size(); i++){
        this->w2.data[i] = this->w2.data[i] - lr * this->n3->grad[0]->data[i];
    }
    this->g.clear_gradients();
    this->n7->update_target(target);
    this->g.backward(this->n7, this->n5);
    for(int i=0; i<this->w3.size(); i++){
        this->w3.data[i] = this->w3.data[i] - lr * this->n5->grad[0]->data[i];
    }
}

ClassifierNeuralNetworkSigmoidActivationsTwoHiddenLayers::ClassifierNeuralNetworkSigmoidActivationsTwoHiddenLayers(
        unsigned int data_dimensionality, unsigned int n_hidden1, unsigned int n_hidden2) : w1(data_dimensionality, n_hidden1, true),
                                                                                            w2(n_hidden1, n_hidden2, true),
                                                                                            w3(n_hidden2, 1, true) {
    this->data_dimensionality = data_dimensionality;
    this->n_hidden1 = n_hidden1;
    this->n_hidden2 = n_hidden2;
    this->w1.guarded = true;
    this->w2.guarded = true;
    this->w3.guarded = true;
    for(int i=0;i < data_dimensionality*n_hidden1; i++){
        this->w1.data[i] = fRand_(-2.0, 2.0);
    }
    for(int i=0;i < n_hidden1*n_hidden2; i++){
        this->w2.data[i] = fRand_(-2.0, 2.0);
    }
    for(int i=0;i < n_hidden2; i++){
        this->w3.data[i] = fRand_(-2.0, 2.0);
    }
    this->n1 = new NodeMultiplyRightWithMatrix(&this->w1);
    this->n2 = new NodeElementWiseSigmoidFunction;
    this->n3 = new NodeMultiplyRightWithMatrix(&this->w2);
    this->n4 = new NodeElementWiseSigmoidFunction;
    this->n5 = new NodeMultiplyRightWithMatrix(&this->w3);
    this->n6 = new NodeElementWiseSigmoidFunction();
    this->n7 = new NodeBinaryCrossEntropy();
    this->g.add_node(this->n1);
    this->g.add_node(this->n2);
    this->g.add_node(this->n3);
    this->g.add_node(this->n4);
    this->g.add_node(this->n5);
    this->g.add_node(this->n6);
    this->g.add_node(this->n7);
    this->g.connect_to(0, 1);
    this->g.connect_to(1, 2);
    this->g.connect_to(2, 3);
    this->g.connect_to(3, 4);
    this->g.connect_to(4, 5);
    this->g.connect_to(5, 6);
}

ClassifierNeuralNetworkSigmoidActivationsTwoHiddenLayers::~ClassifierNeuralNetworkSigmoidActivationsTwoHiddenLayers(void){
    this->g.clean();
    delete[] this->w1.data;
    delete[] this->w2.data;
    delete[] this->w3.data;
    delete this->n1;
    delete this->n2;
    delete this->n3;
    delete this->n4;
    delete this->n5;
    delete this->n6;
    delete this->n7;
}
