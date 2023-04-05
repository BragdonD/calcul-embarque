# RNN and Arduino DUE

This code implements a small recurrent neural network (``RNN``) using an ``Arduino`` microcontroller board. The ``RNN`` is used to predict a binary sequence, and the predicted sequence is displayed on an ``LED``.

The ``RNN`` consists of an ``input`` layer, a ``hidden`` layer, and an ``output`` layer. The input layer has five neurons, the hidden layer has ten neurons, and the output layer has one neuron. The network is trained using **random** weights and biases.

The ``RNN`` is implemented using the following functions:
- ``initializeWeights()``: Initializes the weights and biases of the network using random values.
- ``forwardPass()``: Computes the output of the network given the current input and hidden state.
- ``activation()``: Computes the sigmoid activation function for a given input.
- ``readPoten():`` Reads the value of a potentiometer and adjusts the prediction threshold of the network.
- ``startPrediction()``: Initializes the input sequence with random values and sets the predict flag to true to start the prediction sequence.

The ``setup()`` function initializes the **serial communication** and sets up the LED pin. The ``loop()`` function reads the **potentiometer** value, starts the prediction sequence using the ``startPrediction()`` function, and then computes the **output** of the network using the ``forwardPass()`` function. Finally, the **output** is displayed on the **LED** using the ``digitalWrite()`` function.