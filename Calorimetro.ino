
#include <HX711.h>
#include <TimerThree.h>
#include <Adafruit_MAX31856.h>

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(18, 15, 16, 17);
long media_movel_temperatura(int content, const int n);

// DEFINIÇÕES DE PINOS
#define pinDT  2
#define pinSCK  3
/*=============Variáveis globais ===============*/
volatile float  temperatura       = 0;
unsigned volatile long instante_atual    = 0;
unsigned volatile long instante_anterior = 0;
unsigned volatile long tempo = 0;
unsigned volatile long milissegundo = 0;

/*=============Variáveis das entradas do usuário===============*/
volatile bool transmitir = false;
volatile bool exp_ini = 0;
volatile bool x = false;
volatile char user_in  = 0;


/*=====================Incialização das Funções=========================*/
void capacidade_termica();
void serialFlush();
void calor_latente();
void calor_solido_mistura();
void calor_solido_joule();





HX711 scale;
float medida_massa = 0;

void serialFlush() { // limpa o buffer de saída da porta serial
  unsigned char temp = Serial.available();

  for (int n = temp; n >= 0; n--) {
    Serial.read();
  }
}

float media_movel_temperatura(float content, const int n)
{

   float acumulador = 0;                                        //acumulador para somar os pontos da média móvel
   static float number_of_points[3];                             //vetor com os valores para média móvel

   for(int i= n-1; i>0; i--) number_of_points[i] = number_of_points[i-1]; //desloca os elementos do vetor de média móvel

   number_of_points[0] = content;                                //posição inicial do vetor recebe a leitura original

   for(int i=0; i<n; i++) acumulador += number_of_points[i];            //faz a somatória do número de pontos

   return acumulador/n;                                        //retorna a média móvel

} //end moving_average

float get_temperature(){
temperatura = media_movel_temperatura(maxthermo.readThermocoupleTemperature(), 3);
return temperatura;
}

void cronometro(){
  ++milissegundo;
  }

void calor_latente(){
  
}


void setup() {
Serial.begin(57600);



  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(-2300000); // LIMPANDO O VALOR DA ESCALA

  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
   maxthermo.begin();
   maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);
   maxthermo.setConversionMode(MAX31856_CONTINUOUS);

    
  Timer3.initialize(1000); //timer para medição de tempo ---> 1 interrupção a cada 1000us ou 1ms
  Timer3.attachInterrupt(cronometro);
  Timer3.start();

}

void calor_solido_joule(){
//Tarar a balança com o calorímetro + tampa + resistência + o termopar
//Colocar o bloco e medir a massa 
//Mede a massa do bloco
//Acompanha a temperatura em função do tempo
//****medir também a potência
Serial.println(" Coloque o calorímetro na balança");
Serial.println(" Pressione z para Zerar");
    serialFlush();
    Serial.flush();
    //boolean u = Serial.available(); 
    user_in = 0;  
    user_in = Serial.read();
    
    while(user_in != 'z'){
        user_in = Serial.read();
    }
    
    scale.tare();

Serial.println(" m --> medir massa");

    user_in = Serial.read();
    
    while(user_in != 'm'){
        user_in = Serial.read();
    }

  medida_massa = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
  Serial.print("Massa = ");
  Serial.println(medida_massa, 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAIS



  Serial.println("Pressione i para iniciar experimento");
    
    user_in = Serial.read();
    
    while(user_in != 'i'){
        user_in = Serial.read();
    }
    
    Serial.println("******************************** ");
    Serial.println("       Experimento Iniciado      ");
    Serial.println("    pressione x para encerrar    ");
    Serial.println("*********************************");

    Serial.println("Temper. Tempo");

milissegundo = 0;
unsigned long intervalo = 1000 ;
unsigned long t_atual = milissegundo;
unsigned long t_anterior = 0;

while(user_in != 'x'){
//  t_atual = milissegundo;
//  
//  if(t_atual - t_anterior >= intervalo ){
//    t_anterior = t_atual;
    Serial.print(maxthermo.readThermocoupleTemperature());  
    Serial.print("  ;  ");
    Serial.println(milissegundo/1000);
    user_in = Serial.read();
    delay(1000);
//  }
}
Serial.println("       Experimento Finalizado      ");
}

/*
void capacidade_termica(){
//Tarar a balança com o recipiente
//Adiciona a massa no isopor e realiza a  medição desta massa
//Acompanha a temperatura até esabilizar com sistema 
//zera novamente a balança com o calorímetro
//tranfere a água pro calorímetro e mede novamente a massa acompanhando a temperatura em função do tempo 
Serial.println(" Coloque o recipiente na balança");
Serial.println(" Pressione z para Zerar");
    serialFlush();
    Serial.flush();
    //boolean u = Serial.available(); 
    user_in = 0;  
    user_in = Serial.read();
    
    while(user_in != 'z'){
        user_in = Serial.read();
    }
    
    scale.tare();

Serial.println(" m --> medir massa");

    user_in = Serial.read();
    
    while(user_in != 'm'){
        user_in = Serial.read();
    }

  medida_massa = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
  Serial.println(medida_massa, 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAI

Serial.println(" Acompanhar temperatura incial --> pressione t para ENCERRAR");

    user_in = Serial.read();
    
    while(user_in != 't'){
        user_in = Serial.read();
        Serial.println(maxthermo.readThermocoupleTemperature());   
        delay(1000);
        }


Serial.println(" Coloque o calorimetro na balança e pressione z para zerar");
    user_in = Serial.read();
    
    while(user_in != 'z'){
        user_in = Serial.read();
    }
    
    scale.tare();


//***** MEDIR A MASSA NOVAMENTE APÓS A TRANSF???
    

// inciar a MEDIDA    

    user_in = Serial.read();
    
    while(user_in != 't'){
        user_in = Serial.read();
        Serial.println(maxthermo.readThermocoupleTemperature());   
        delay(1000);
        }


}*/

//*******************Versão 2 da rotina para medir o equivalente em água****************
void capacidade_termica(){
//Tarar a balança com o recipiente
//Adiciona a massa no isopor e realiza a  medição desta massa
//Acompanha a temperatura até esabilizar com sistema 
//zera novamente a balança com o calorímetro
//tranfere a água pro calorímetro e mede novamente a massa acompanhando a temperatura em função do tempo 
Serial.println(" Coloque o calorímetro na balança");
Serial.println(" Pressione z para Zerar");
    serialFlush();
    Serial.flush();
    //boolean u = Serial.available(); 
    user_in = 0;  
    user_in = Serial.read();
    
    while(user_in != 'z'){
        user_in = Serial.read();
    }
    
    scale.tare();

Serial.println("Tansfira a massa de água e pressione 'm' para medir");
//Serial.println(" m --> medir massa");

    user_in = Serial.read();
    
    while(user_in != 'm'){
        user_in = Serial.read();
    }

  medida_massa = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
  Serial.print("Massa = ");
  Serial.print(medida_massa, 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAI
  Serial.print("kg");
  Serial.println(" ");

  //preenche a matriz com os valores para o primeiro ponto da média
  for(int i=0; i <= 10; ++i) get_temperature();
  
    Serial.println("******************************** ");
    Serial.println("       Capacidade térmica        ");
    Serial.println("    pressione x para encerrar    ");
    Serial.println("*********************************");

    Serial.println("Temper. Tempo");

milissegundo = 0;
unsigned long intervalo = 1000 ;
unsigned long t_atual = milissegundo;
unsigned long t_anterior = 0;

while(user_in != 'x'){
//  t_atual = milissegundo;
//  
//  if(t_atual - t_anterior >= intervalo ){
//    t_anterior = t_atual;
get_temperature();
    Serial.print(temperatura);  
    Serial.print("  ;  ");
    Serial.println(milissegundo/1000);
    user_in = Serial.read();
    delay(1000);
//  }
  }
Serial.println("       Experimento Finalizado      ");
  }




void loop() {


  
  /*
  Calibrar Balança
  Selecionar Experimento
      --->Capacidade termica do calorímero (equivalente em água)
      --->Calor específico dos metais
      --->Mistura de Líquidos
*/
  delay(200);
  serialFlush(); // limpa o buffer de entrada
  delay(200);
  serialFlush();

  if (exp_ini == false) {
    serialFlush();
    Serial.println("Escolha uma opção abaixo");
    Serial.println(" ");
    Serial.println("1 --> Capacidade Termica");
    Serial.println("2 --> Experimento: calor específico de sólido a partir de mistura");
    Serial.println("3 --> Calor específico de um sólido por efeito Joule");
    Serial.println("4 --> Calor Latente de Fusão da água");
    Serial.println("5 --> Mistura de líquidos");
    Serial.println(" ");
    Serial.flush();// espera até que todos os caracteres no buffer de trasnmissão sejam enviados

  }




  //  Serial.println(Serial.available());
  x = Serial.available();

  while (x == false) {
    //Serial.println("Aguarda Digitar");
    Serial.flush();
    x = Serial.available();
    user_in = Serial.read();
    //Serial.println(user_in);
    //delay(1000);
  }
  delay(1000);


  switch (user_in) {

    case '1' :
      Serial.println("*************************");
      Serial.println("   Capacidade Térmica    ");
      Serial.println("*************************");
      capacidade_termica(); // seleciona o sentido de rotação
      break;

    case '2' :
      //Set_frequency();
      delay(300);
      //Inicia_experimento();
      break;

    case '3' :
      calor_solido_joule();

     break;

    case '4' :
      Serial.println("*************************");
      Serial.println("     NOP       ");
      Serial.println("*************************");
      
     break;

    default:
      Serial.println("OPÇÃO INVALIDA");
      delay(2000);
      x = 0;
      serialFlush();
      // Se nenhum dos anteriores, faz o caso padrão default
      // default é opcional
  }
}
