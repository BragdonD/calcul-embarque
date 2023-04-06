#include <Arduino.h>
#include <NeuralNetwork.h>
#include <math.h>

#define INPUT_SIZE 9   // input image size
#define KERNEL_SIZE 3  // kernel size
#define PADDING_SIZE 1 // padding size
#define STRIDE_SIZE 1  // stride size
#define POOL_SIZE 2    // max-pooling size
#define OUTPUT_SIZE ((INPUT_SIZE - KERNEL_SIZE + (2 * PADDING_SIZE)) / STRIDE_SIZE + 1)
#define NumberOf(arg) ((unsigned int)(sizeof(arg) / sizeof(arg[0])))
#define _1_OPTIMIZE B00010000
#define ACTIVATION__PER_LAYER
#define SIGMOID 0
#define TANH 1

float eoutput[OUTPUT_SIZE][OUTPUT_SIZE];
float epool[OUTPUT_SIZE / POOL_SIZE][OUTPUT_SIZE / POOL_SIZE];
float eflattened[(OUTPUT_SIZE / POOL_SIZE) * (OUTPUT_SIZE / POOL_SIZE)][1];
float ebias(0.);

// initialize input, kernel, and bias
float einput[INPUT_SIZE][INPUT_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}};

float ekernel[KERNEL_SIZE][KERNEL_SIZE] =
    {{0, 1, 0},
     {1, 1, 1},
     {0, 1, 1}};

float expectedOutput[NumberOf(eflattened)][1] = {
    {0.5},
    {0.5},
    {0.0},
    {0.5},
    {0.5},
    {0.5},
    {1.0},
    {0.5},
    {0.0},
    {1.0},
    {1.0},
    {0.5},
    {0.5},
    {0.5},
    {0.5},
    {0.5}};

float ekernel2[KERNEL_SIZE][KERNEL_SIZE] = 
    {{0, 1, 0},
     {1, 1, 1},
     {0, 1, 1}};
float ebias2(0.);

// initialize output, pool, and flattened arrays for second layer
float eoutput2[OUTPUT_SIZE][OUTPUT_SIZE];
float epool2[OUTPUT_SIZE / POOL_SIZE][OUTPUT_SIZE / POOL_SIZE];
float eflattened2[(OUTPUT_SIZE / POOL_SIZE) * (OUTPUT_SIZE / POOL_SIZE)][1];

// Neural Network Parameters
unsigned int layers[] = {NumberOf(eflattened), 6, 3, 1};
byte Actv_Functions[] = {TANH, TANH, SIGMOID};
NeuralNetwork NN(layers, NumberOf(layers), Actv_Functions);
float *outputs; // output of the neural network

template <size_t flattenedSize1D> 
void printflatten2vector(float (&eflattened)[flattenedSize1D][1]);

template <int InputSize1D, int InputSize2D, int KernelSize1D, int KernelSize2D, int OutputSize1D, int OutputSize2D>
void convolution2D(float (&input)[InputSize1D][InputSize2D], float (&kernel)[KernelSize1D][KernelSize2D], float (&output)[OutputSize1D][OutputSize2D], float bias);

template <size_t rows, size_t cols>
void maxPooling(float (&poolinput)[rows][cols], float (&pool)[rows / 2][cols / 2]);

template <size_t flattenedSize1D, size_t poolSize1D, size_t poolSize2D>
void flatten2vector(float (&flattened)[flattenedSize1D][1], float (&pool)[poolSize1D][poolSize2D]);

void setup()
{
  Serial.begin(115200);
  /// First Convolutional Layer
  convolution2D<INPUT_SIZE, INPUT_SIZE, KERNEL_SIZE, KERNEL_SIZE, OUTPUT_SIZE, OUTPUT_SIZE>(einput, ekernel, eoutput, ebias);
  maxPooling<OUTPUT_SIZE, OUTPUT_SIZE>(eoutput, epool);
  flatten2vector<(OUTPUT_SIZE/POOL_SIZE)*(OUTPUT_SIZE/POOL_SIZE), OUTPUT_SIZE/POOL_SIZE, OUTPUT_SIZE/POOL_SIZE>(eflattened, epool);
  printflatten2vector<(OUTPUT_SIZE/POOL_SIZE)*(OUTPUT_SIZE/POOL_SIZE)>(eflattened);
  
  /// Second Convolutional Layer
  convolution2D(eoutput, ekernel2, eoutput2, ebias2);
  maxPooling(eoutput2, epool2);
  flatten2vector(eflattened2, epool2);
  printflatten2vector<(OUTPUT_SIZE/POOL_SIZE)*(OUTPUT_SIZE/POOL_SIZE)>(eflattened2);

  /// Feed Forward and Back Propagation
  do
  {
    for (unsigned int j = 0; j < NumberOf(eflattened); j++)
    {
      NN.FeedForward(eflattened[j]);
      NN.BackProp(expectedOutput[j]);
    }
    Serial.print("J cretirion Error: "); // Prints the Error.
    Serial.println(NN.MeanSqrdError, 4);
  } while (NN.getMeanSqrdError(NumberOf(eflattened)) > 0.003);
  Serial.print("==OUTPUT==");
  for(unsigned int i = 0; i < NumberOf(eflattened); i++)
  {
    outputs = NN.FeedForward(eflattened[i]);
    Serial.print(roundf(outputs[0]));
    Serial.print("≅");
    Serial.println(outputs[0], 7);
  }
  NN.print();
}

void loop()
{
}

/**
 * @brief Convolution 2D function for a 2D array of floats with a 2D kernel of floats and a bias of float type.
 * @param input 2D array of floats
 * @param kernel 2D array of floats
 * @param output 2D array of floats
 * @param bias float type
 * @tparam InputSize1D size of the first dimension of the input array
 * @tparam InputSize2D size of the second dimension of the input array
 * @tparam KernelSize1D size of the first dimension of the kernel array
 * @tparam KernelSize2D size of the second dimension of the kernel array
 * @tparam OutputSize1D size of the first dimension of the output array
 * @tparam OutputSize2D size of the second dimension of the output array
*/
template <int InputSize1D, int InputSize2D, int KernelSize1D, int KernelSize2D, int OutputSize1D, int OutputSize2D>
void convolution2D(float (&input)[InputSize1D][InputSize2D], float (&kernel)[KernelSize1D][KernelSize2D], float (&output)[OutputSize1D][OutputSize2D], float bias)
{
  for (int i = 0; i < OutputSize1D; i++)
  {
    for (int j = 0; j < OutputSize2D; j++)
    {
      output[i][j] = 0;
      for (int k = 0; k < KernelSize1D; k++)
      {
        for (int l = 0; l < KernelSize2D; l++)
        {
          int input_row = i * STRIDE_SIZE + k - PADDING_SIZE;
          int input_col = j * STRIDE_SIZE + l - PADDING_SIZE;
          if (input_row >= 0 && input_row < InputSize1D && input_col >= 0 && input_col < InputSize2D)
          {
            output[i][j] += input[input_row][input_col] * kernel[k][l];
          }
        }
      }
      output[i][j] += bias;
    }
  }
}

/**
 * @brief Max pooling function for 2D vectors which create a new 2D vector with half the size of the input
 * @param poolinput The input 2D vector
 * @param pool The output 2D vector
 * @tparam rows The number of rows in the input vector
 * @tparam cols The number of columns in the input vector
*/
template <size_t rows, size_t cols>
void maxPooling(float (&poolinput)[rows][cols], float (&pool)[rows / 2][cols / 2])
{
  for (int i = 0; i < rows / 2; i++)
  {
    for (int j = 0; j < cols / 2; j++)
    {
      float maxVal = -INFINITY;
      for (int k = 0; k < 2; k++)
      {
        for (int l = 0; l < 2; l++)
        {
          maxVal = max(maxVal, poolinput[i * 2 + k][j * 2 + l]);
        }
      }
      pool[i][j] = maxVal;
    }
  }
}

/**
 * @brief Flattens a 2D vector into a 1D vector
 * @param flattened The flattened 1D vector
 * @param pool The 2D vector to be flattened
 * @tparam flattenedSize1D The size of the flattened vector
 * @tparam poolSize1D The size of the 2D vector in the first dimension
 * @tparam poolSize2D The size of the 2D vector in the second dimension
*/
template <size_t flattenedSize1D, size_t poolSize1D, size_t poolSize2D>
void flatten2vector(float (&flattened)[flattenedSize1D][1], float (&pool)[poolSize1D][poolSize2D]) {
  int idx = 0;
  for (int i = 0; i < poolSize1D; i++) {
    for (int j = 0; j < poolSize2D; j++) {
      flattened[idx++][0] = pool[i][j];
    }
  }
}

/**
 * @brief Prints the flattened 1D vector
 * @param eflattened The flattened 1D vector
 * @tparam flattenedSize1D The size of the flattened vector 
*/
template <size_t flattenedSize1D>
void printflatten2vector(float (&eflattened)[flattenedSize1D][1])
{
  Serial.print("Flattened Vector Size: ");
  Serial.println(flattenedSize1D);
  for (unsigned int i = 0; i < flattenedSize1D; i++)
  {
    Serial.print(eflattened[i][0]);
    Serial.print(" ");
  }
  Serial.println();
}