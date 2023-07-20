#include <DHT.h>
#define         MQ2                       (15)     //define la entrada analogica para el sensor
#define         MQ7                   (2)
#define         MQ8                   (4)
#define         RL_VALOR             (3.3)     //define el valor de la resistencia mde carga en kilo ohms
#define         RAL       (9.83)  // resistencia del sensor en el aire limpio / RO, que se deriva de la                                             tabla de la hoja de datos
#define         GAS                      (0)
#define          DHTPIN (23)
#define          DHTTYPE DHT11
String inputstring = "";                                                        //Cadena recibida desde el PC
float           LPCurve[3]  =  {2.3,0.21,-0.47}; 
float           COCurve[3]  =  {1.8, 0.09, -0.00043};
float           H2Curve[3]  =  {9.0, 0.04, -0.00091};
float           RoMQ2           =  0.46;
float           RoMQ7           =  1.12;
float           RoMQ8           =  0.11;

DHT dht(DHTPIN, DHTTYPE);

void setup(){
Serial.begin(9600);                                                                  //Inicializa Serial a 9600 baudios
 Serial.println("Iniciando ...");
   //configuracion del sensor
  Serial.print("Calibrando...\n");
  RoMQ2 = Calibracion(MQ2);                        //Calibrando el sensor. Por favor de asegurarse que el sensor se encuentre en una zona de aire limpio mientras se calibra
  RoMQ7 = Calibracion(MQ7);
  RoMQ8 = Calibracion(MQ8);
  Serial.print("Calibracion finalizada...\n");
  Serial.print("Ro=");
  Serial.print(RoMQ2);
  Serial.print("kohm");
  Serial.print("\n");
  Serial.print("Ro=");
  Serial.print(RoMQ7);
  Serial.print("kohm");
  Serial.print("\n");
  Serial.print("Ro=");
  Serial.print(RoMQ8);
  Serial.print("kohm");
  Serial.print("\n");
  dht.begin();
}
 
void loop(){
   Serial.print("LP:");
   Serial.print(porcentaje_gasLP(lecturaMQ(MQ2)/RoMQ2,GAS) );
   Serial.print( "ppm" );
   Serial.print("    ");
   Serial.print("\n");
   Serial.print("CO:");
   Serial.print(porcentaje_gasCO(lecturaMQ(MQ7)/RoMQ7,GAS) );
   Serial.print( "ppm" );
   Serial.print("    ");
   Serial.print("\n");
   Serial.print("H2:");
   Serial.print(porcentaje_gasH2(lecturaMQ(MQ8)/RoMQ8,GAS) );
   Serial.print( "ppm" );
   Serial.print("    ");
   Serial.print("\n");

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.print("%\tTemperatura: ");
  Serial.print(temperature);
  Serial.println("°C");
   delay(10000);
}
 
float calc_res(int raw_adc)
{
  return ( ((float)RL_VALOR*(4095-raw_adc)/raw_adc)); //4095 3.3
}
 
float Calibracion(float mq_pin){
  int i;
  float val=0;
    for (i=0;i<50;i++) {                                                                               //tomar múltiples muestras
    val += calc_res(analogRead(mq_pin));
    delay(500);
  }
  val = val/50;                                                                                         //calcular el valor medio
  val = val/RAL;
  return val;
}
 
float lecturaMQ(int mq_pin){
  int i;
  float rs=0;
  for (i=0;i<5;i++) {
    rs += calc_res(analogRead(mq_pin));
    delay(50);
  }
rs = rs/5;
return rs;
}
 
float porcentaje_gasLP(float rs_ro_ratio, int gas_id){
   if ( gas_id == GAS ) {
     return porcentaje_gas(rs_ro_ratio,LPCurve);
   }
  return 0;
}

float porcentaje_gasCO(float rs_ro_ratio, int gas_id){
   if ( gas_id == GAS ) {
     return porcentaje_gas(rs_ro_ratio,COCurve);
   }
  return 0;
}

float porcentaje_gasH2(float rs_ro_ratio, int gas_id){
   if ( gas_id == GAS ) {
     return porcentaje_gas(rs_ro_ratio,H2Curve);
   }
  return 0;
}
 
float porcentaje_gas(float rs_ro_ratio, float *pcurve){
  return (pow(10, (((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
/*void setup() {
  Serial.begin(9600);
}

void loop() {
  float sensor_volt;
  float RS_gas; // Obtener valor de RS en un GAS
  float ratio; // Obtener ratio RS_GAS/RS_air
  int sensorValue = analogRead(15);
  sensor_volt = (float)sensorValue / 4095 * 3.3;
  RS_gas = (3.3 - sensor_volt) / sensor_volt; // omitir *RL
  /- Reemplazar el nombre "R0" con el valor de R0 en la demostración del primer test -/
  /*float R0 = 0.27; // Asigna el valor adecuado a R0 0.00,0.01,0.02
  ratio = RS_gas / R0; // ratio = RS/R0
  /-----------------------------------------------------------------------/
  //Serial.println(sensorValue)
  /*Serial.print("sensor_volt = ");
  Serial.println(sensor_volt);
  Serial.print("RS_ratio = ");
  Serial.println(RS_gas);
  Serial.print("Rs/R0 = ");
  Serial.println(ratio);
  Serial.print("\n\n");
  delay(10000);
}*/
