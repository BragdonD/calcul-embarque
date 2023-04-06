#include<Arduino.h>
#include <NeuralNetwork.h>
#define NumberOf(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0]))) 
#define _1_OPTIMIZE B00010000 
#define SELU                  
const unsigned int layers[] = {2,4,1}; 
float *output_data;

const float input_data[4][2] = {
  {0, 0}, //0
  {0, 1}, //1
  {1, 0}, //1
  {1, 1}  //0
};

const float expectedOutput[4][1] = {{0},{1},{1},{0}}; 
NeuralNetwork NN(layers,NumberOf(layers));
void setup()
{
  Serial.begin(115200);
  
  do{ 
    for (int j = 0; j < NumberOf(input_data); j++) 
    {
       NN.FeedForward(input_data[j]);      
       NN.BackProp(expectedOutput[j]); 
    }
    
    Serial.print("MSE: "); 
    Serial.println(NN.MeanSqrdError,3);
      }while(NN.GetMeanSqrdError(NumberOf(input_data)) > 0.001);
    NN.print(); 
Serial.println("\n =-[OUTPUTS]-=");
}
void loop() {
  
  for (int i = 0; i < NumberOf(input_data); i++)
  {
    output_data = NN.FeedForward(input_data[i]); 
    //Serial.print(round(output_data[0]));    
    //Serial.print(" ≅ "); 
     Serial.println(output_data[0], 3);     
  }
}
