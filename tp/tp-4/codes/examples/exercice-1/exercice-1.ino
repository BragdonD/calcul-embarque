#include<Arduino.h>

#define N 1000
double alpha(0.01); // Taux d'apprentissage
double precision(0.01); // Précision de l'algorithme

double theta0(0.); // Paramètre initial θ0
double theta1(0.); // Paramètre initial θ1

double sum_error(0.); // Somme des erreurs pour chaque itération
double prev_error(0.); // Erreur de la précédente itération

double x[] = {1., 4., 6.}; // Les valeurs d'entrée x
double y[] = {3,4.5,7.}; // Les valeurs de sortie y
int m = sizeof(x) / sizeof(x[0]); // Le nombre de données d'entrée

double  hypoLR(double t0, double t1,doub/le x)
{
  return(t0+t1*x);
}

void learnerRL()
{
 for (int i = 0; i < N; i++) { // Effectuer 1000 itérations
    int index = i%m;
    double prediction =hypoLR(theta0,theta1,x[index]); // Prédire la sortie
    double error = prediction - y[index]; // Calculer l'erreur
    theta0 = theta0 - alpha * error/m; // Mettre à jour θ0
    theta1 = theta1 - alpha * error * x[index]/m; // Mettre à jour θ1
    sum_error += error * error/m; // Ajouter l'erreur à la somme des erreurs

    // Vérifier si l'erreur a convergé
    if (i > 0 && abs(sum_error - prev_error) < precision) {
      break;
    }
    prev_error = sum_error; // Mettre à jour l'erreur précédente

    // Afficher les paramètres et l'erreur actuels
    Serial.print("theta0: ");
    Serial.print(theta0);
    Serial.print(", theta1: ");
    Serial.print(theta1);
    Serial.print(", error: ");
    Serial.println(sum_error);
    Serial.print(", index i: ");
    Serial.println(i);
  }

}
//
void prediction(double t0, double t1,double X) {
    double predict = t0 + t1* X;
	Serial.print("Y Prediction = \t");
  Serial.print(t0);
  Serial.print("+\t");
  Serial.print(t1); 
  Serial.println("*x");
   Serial.print("+\t");
   Serial.print(predict);
}
//
void setup() {
  Serial.begin(115200); // Initialiser la communication série
  Serial.print("Apprentissage des paramètres");
  learnerRL();  // Entraîner le modèle avec la descente de gradient 
   Serial.print("Fin Apprentissage des paramètres");
 
}

void loop() {
  
  // 
  double sensor(12.5);
  prediction(theta0,theta1,sensor);
 
}
