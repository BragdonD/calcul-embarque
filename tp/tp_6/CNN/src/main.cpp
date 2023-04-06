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

// 1 = Tanh and 0 = Sigmoid
//  Create the new Neural Network
float *outputs;
//

//
void convolution2D(float input[INPUT_SIZE][INPUT_SIZE], float kernel[KERNEL_SIZE][KERNEL_SIZE], float output[OUTPUT_SIZE][OUTPUT_SIZE], float bias)
{
  Serial.println("Convolution2D");
  for (int i = 0; i < OUTPUT_SIZE; i++)
  {
    for (int j = 0; j < OUTPUT_SIZE; j++)
    {
      output[i][j] = 0;
      for (int k = 0; k < KERNEL_SIZE; k++)
      {
        for (int l = 0; l < KERNEL_SIZE; l++)
        {
          int input_row = i * STRIDE_SIZE + k - PADDING_SIZE;
          int input_col = j * STRIDE_SIZE + l - PADDING_SIZE;
          if (input_row >= 0 && input_row < INPUT_SIZE && input_col >= 0 && input_col < INPUT_SIZE)
          {
            Serial.print("input - ");
            Serial.print(input_row);
            Serial.print(", ");
            Serial.print(input_col);
            Serial.print(": ");
            Serial.println(input[input_row][input_col]);
            output[i][j] += input[input_row][input_col] * kernel[k][l];
          }
        }
      }
      output[i][j] += bias;
      Serial.print(i);
      Serial.print(", ");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(output[i][j]);
    }
  }
}
//
void maxPooling(float poolinput[OUTPUT_SIZE][OUTPUT_SIZE], float pool[OUTPUT_SIZE / POOL_SIZE][OUTPUT_SIZE / POOL_SIZE])
{
  Serial.println("MaxPooling");
  for (int i = 0; i < OUTPUT_SIZE / POOL_SIZE; i++)
  {
    for (int j = 0; j < OUTPUT_SIZE / POOL_SIZE; j++)
    {
      float maxVal = -INFINITY;
      for (int k = 0; k < POOL_SIZE; k++)
      {
        for (int l = 0; l < POOL_SIZE; l++)
        {
          maxVal = max(maxVal, poolinput[i * POOL_SIZE + k][j * POOL_SIZE + l]);
        }
      }
      pool[i][j] = maxVal;
      Serial.print(i);
      Serial.print(", ");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(maxVal);
    }
  }
}

void flatten2vector(float flattened[(OUTPUT_SIZE / POOL_SIZE) * (OUTPUT_SIZE / POOL_SIZE)][1], float pool[OUTPUT_SIZE / POOL_SIZE][OUTPUT_SIZE / POOL_SIZE])
{
  int idx = 0;
  for (int i = 0; i < OUTPUT_SIZE / POOL_SIZE; i++)
  {
    for (int j = 0; j < OUTPUT_SIZE / POOL_SIZE; j++)
    {
      flattened[idx++][0] = pool[i][j];
    }
  }
}

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
      {0},
      {0},
      {1},
      {0},
      {0},
      {0},
      {1},
      {0},
      {0},
      {1},
      {1},
      {0},
      {0},
      {0},
      {0},
      {0}};

unsigned int layers[] = {NumberOf(eflattened), 6, 3, 1};
byte Actv_Functions[] = {TANH, TANH, SIGMOID};

NeuralNetwork NN(layers, NumberOf(layers), Actv_Functions);

float *output;

void printflatten2vector();

void setup()
{
  Serial.begin(115200);

  convolution2D(einput, ekernel, eoutput, ebias);
  maxPooling(eoutput, epool);
  flatten2vector(eflattened, epool);
  printflatten2vector();

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
    output = NN.FeedForward(eflattened[i]);
    Serial.print(round(output[0]));
    Serial.print("≅");
    Serial.println(output[0], 7);
  }
  NN.print();
}

void loop()
{
}

/**
 * @brief Prints the flattened vector
 */
void printflatten2vector()
{
  Serial.print("Flattened Vector Size: ");
  Serial.println(NumberOf(eflattened));
  for (unsigned int i = 0; i < NumberOf(eflattened); i++)
  {
    Serial.print(eflattened[i][0]);
    Serial.print(" ");
  }
  Serial.println();
}