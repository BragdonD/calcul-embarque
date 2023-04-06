# Neural Network with CNN

The following code is an implementation of a neural network with two convolutional layers using Arduino and the library ``NeuralNetwork.h``.
## Libraries

The code uses the following libraries:

- ``Arduino.h``
- ``NeuralNetwork.h``
- ``math.h``

## Constants

The code defines the following constants:

- ``INPUT_SIZE``: the size of the input image.
- ``KERNEL_SIZE``: the size of the kernel.
- ``PADDING_SIZE``: the size of the padding.
- ``STRIDE_SIZE``: the size of the stride.
- ``POOL_SIZE``: the size of the max-pooling.
- ``OUTPUT_SIZE``: the size of the output.
- ``NumberOf(arg):`` a macro function that returns the number of elements in an array.
- ``_1_OPTIMIZE``: a constant that optimizes the neural network.
- ``ACTIVATION__PER_LAYER``: a constant that specifies the activation function for each layer.
- ``SIGMOID``: a constant that represents the sigmoid activation function.
- ``TANH``: a constant that represents the hyperbolic tangent activation function.

## Variables

The code initializes the following variables:

- ``eoutput[OUTPUT_SIZE][OUTPUT_SIZE]``: an array to store the output of the first convolutional layer.
- ``epool[OUTPUT_SIZE / POOL_SIZE][OUTPUT_SIZE / POOL_SIZE]``: an array to store the output of the max-pooling layer.
- ``eflattened[(OUTPUT_SIZE / POOL_SIZE) * (OUTPUT_SIZE / POOL_SIZE)][1]``: a flattened array to store the output of the first convolutional layer after max-pooling.
- ``ebias``: a float to store the bias of the first convolutional layer.
- ``einput[INPUT_SIZE][INPUT_SIZE]``: a two-dimensional array to store the input image.
- ``ekernel[KERNEL_SIZE][KERNEL_SIZE]``: a two-dimensional array to store the kernel of the first convolutional layer.
- ``expectedOutput[NumberOf(eflattened)][1]``: an array to store the expected output of the neural network.
- ``ekernel2[KERNEL_SIZE][KERNEL_SIZE]``: a two-dimensional array to store the kernel of the second convolutional layer.
- ``ebias2``: a float to store the bias of the second convolutional layer.
- ``eoutput2[OUTPUT_SIZE][OUTPUT_SIZE]``: an array to store the output of the second convolutional layer.
- ``epool2[OUTPUT_SIZE / POOL_SIZE][OUTPUT_SIZE / POOL_SIZE]``: an array to store the output of the max-pooling layer in the second convolutional layer.
- ``eflattened2[(OUTPUT_SIZE / POOL_SIZE) * (OUTPUT_SIZE / POOL_SIZE)][1]``: a flattened array to store the output of the second convolutional layer after max-pooling.
- ``layers[]``: an array to specify the number of neurons in each layer.
- ``Actv_Functions[]:`` an array to specify the activation function for each layer.
- ``NN``: a NeuralNetwork object with the specified number of layers and activation functions.
- ``outputs``: an array to store the output of the neural network.

## Functions

The code defines the following functions:

- ``printflatten2vector()``: a function to print the elements of a flattened 2D array.
- ``convolution2D()``: a function to perform 2D convolution on an input image using a kernel and a bias.
- ``maxPooling()``: a function to perform max-pooling on an input array.
- ``flatten2vector()``: a function to flatten a 2D array