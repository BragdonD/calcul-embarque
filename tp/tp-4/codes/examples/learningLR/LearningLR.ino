// author : Foued DERRAZ : ECE
#include <Arduino.h>
#define NNmax 5 // size of the vector to be learned
#define Nmax 200 // max iteration of your algorithm
double  error[Nmax];
double x[NNmax] = {20.,50.,42.,25.,70.};
double y[NNmax] = {80.,60.,50.,30.,90.};

double theta0(0.); // Slope
double theta1(0.); // Intercept
double alpha(0.0002); // Learning Rate
double Y_predict(0.);


// Use linear regression formula y = theta1.x + theta0
// Use error formula error = predicted(i) - y(i)
// Use update the weights formula = theta0(t+1) = theta0(t) - alpha * error
// Use update the slope formula = theta1(t+1) = theta1(t) - alpha * error * x



double  hypoLR(double t0, double t1,double x)
{
  return(t0+t1*x);
}


//gradient_descent()
void learnerMLRLinear() {
  int i(0) ;
  int index(0.);
  double err0,err1;
  double tmperr0(0.), tmperr1(0.);
    do 
    { 
        //int index = i % NNmax;
        err0=0.;
        err1=0.;
        for(int j=0;j< NNmax;j++){
        index =j % NNmax;
        Y_predict = hypoLR(theta0, theta1,x[index]);
        tmperr0 = Y_predict - y[index];
        tmperr1 = (Y_predict - y[index])*x[index];
        err0+=tmperr0;
        err1+=tmperr1;
        }
        theta0 -= alpha * err0/NNmax;
        theta1 -= alpha * err1/NNmax;
		//theta0 = theta0 - alpha * err;
        //theta1 = theta1 - alpha * err * x[index];
    error[i]=err0*err0/NNmax;
		Serial.print("theta1:=\t");
		Serial.print(theta1);
		Serial.print("\t");
		Serial.print("theta0");
		Serial.print("\t");
		Serial.println(theta0);
		Serial.print("error Prediction\t");
		//Serial.println(err);
		//Serial.print("Cretirion\t");
		Serial.println(error[i]);
    i++;
    }while(i++<Nmax ); 
}

void prediction(double X) {
    double predict = theta0 + theta1* X;
	Serial.print("Y Prediction = \t");
  Serial.print(theta0);
  Serial.print("+\t");
  Serial.print(theta1); 
  Serial.println("*x");


	}
void setup() {
    Serial.begin(115200);
    learnerMLRLinear();

}

void loop() {
     int sensor=1.2;
    // put your main code here, to run repeatedly:
	   prediction(sensor);
}