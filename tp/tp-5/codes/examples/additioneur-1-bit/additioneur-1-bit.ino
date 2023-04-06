#include<Arduino.h>
#include <NeuralNetwork.h>
#define NumberOf(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0]))) 
#define _1_OPTIMIZE B00010000 
#define ACTIVATION__PER_LAYER 
unsigned int layers[] = {3, 5, 3, 2}; 
byte Actv_Functions[] = {   1, 1, 0}; 
// 1 = Tanh and 0 = Sigmoid 
//  Create the new Neural Network
NeuralNetwork NN(layers, NumberOf(layers), Actv_Functions);
float *outputs[2]; 
//Default Inputs/Training-Data
const float inputs[8][3] = {
  {0, 0, 0}, //0
  {0, 0, 1}, //1
  {0, 1, 0}, //1
  {0, 1, 1}, //0
  {1, 0, 0}, //1
  {1, 0, 1}, //0
  {1, 1, 0}, //0
  {1, 1, 1}  //1
};
const float expectedOutput[8][2] = {
  {0,0}, 
  {1,0}, 
  {1,0},
  {0,1}, 
  {1,0}, 
  {0,1}, 
  {0,1}, 
  {1,1}}; 
  void setup()
{
  Serial.begin(115200);
   do{ 
    for (int j=0; j < NumberOf(inputs); j++) 
    {
      NN.FeedForward(inputs[j]);             
      NN.BackProp(expectedOutput[j]);        
    }
    Serial.print("J cretirion Error: ");// Prints the Error.
    Serial.println(NN.MeanSqrdError,2);
  }while(NN.GetMeanSqrdError(NumberOf(inputs)) > 0.01);
NN.print();
}

void loop() {
  Serial.println("\n =-[OUTPUTS]-=");
  for (int i=0; i < NumberOf(inputs); i++) 
  {
    outputs[i] = NN.FeedForward(inputs[i]);
    Serial.print(round(outputs[i][1]));    
    Serial.print(" ≅ ");                   
    Serial.println(outputs[i][1], 7);
  }
}
