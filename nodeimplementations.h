//
// Created by niko on 4/21/16.
//

#ifndef TINYGRAD_NODEIMPLEMENTATIONS_H
#define TINYGRAD_NODEIMPLEMENTATIONS_H

#include <math.h>
#include <iostream>
#include "node.h"



class NodeElementWiseAddConstant : public Node{
private:
    double addition;
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
    NodeElementWiseAddConstant(double);
};

class NodeSumAllTensorElements : public Node{
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
};

class NodeTransposeMatrix : public Node{
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
};

class NodeMultiplyRightWithMatrix : public Node{
public:
    Tensor *mulmat;
    void calculate_value(void);
    void calculate_gradient(bool last);
    void update_matrix(Tensor *);
    NodeMultiplyRightWithMatrix(Tensor*);
};

class NodeElementWisePower : public Node{
private:
    double power;
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
    NodeElementWisePower(double);
};

class NodeElementWiseSigmoidFunction : public Node{
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
};

class NodeElementWiseLog : public Node{
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
};

class NodeIdentity : public Node{
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
};

class NodeElementWiseConstantMultiply : public Node{
private:
    double multiple;
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
    NodeElementWiseConstantMultiply(double);
};

class NodeSingleSquaredError : public Node{
private:
    double target;
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
    void update_target(double);
    NodeSingleSquaredError(double);
};

class NodeSingleBinaryCrossEntropy : public Node{
private:
    double target;
public:
    void calculate_value(void);
    void calculate_gradient(bool last);
    void update_target(double);
    NodeSingleBinaryCrossEntropy(double);
};

#endif //TINYGRAD_NODEIMPLEMENTATIONS_H