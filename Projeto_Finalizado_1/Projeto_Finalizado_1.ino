// ========================================================================================================
                                 // Bibliotecas Auxiliares

           //Projeto Coqueteleira Automatizada Arduino   Autor: Daniel Pinhero    2018


#include <Wire.h>                        //Biblioteca para o módulo I2C
#include <LiquidCrystal_I2C.h>                                 //Biblioteca do dispray
#include <AFMotor.h>                                           //Biblioteca do motor de passo    

                                 // Mapeamento de Hardware
                                 
#define     menu     A3                                        //Botão para mover no menu
#define     enter    A2                                        //Botão para selecionar

                                 //Lógica para fim de curso

const int fim_de_curso = 2;                                    //Ponto de inicio de trabalho
int contador_botao = 0;                                        //Vezes que o botão é pressionado
int estado_botao   = 0;                                        //Estado do botão atual
int ultimo_estado_botao = 0;                                   //Ultimo estado do botão 
int encontrado = false;                                        //Fim de curso encontrado para inicio de trabalho
int buzzer = A0;

//Pinos para led RGB
int led_verde = 10;                     
int led_azul  = A1;
int led_vermelho = 9;
int led2_verde = 13;
int i;
                              
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);



                                   // Declaração de configuração para lcd e motor de passo
AF_Stepper motor1 (48, 1);                                      //Configuração motor de passo 1
AF_Stepper motor2 (48, 2);                                      //Configuração motor de passo 2
  

// ========================================================================================================
                                        // Protótipo das Funções para as lista de menu e sub menu.
void readButts();
void list_menu();
void menu_select();



// ========================================================================================================
                                        // Variáveis Globais  para controle dos menus
                
int      line[5] = {0,1,2,3,4},  
         line_bk[5],             
         index,                   
         menu_number = 1;

boolean  menu_flag  = 0,  
         enter_flag = 0,  
         sub_menu = 0;


// ========================================================================================================
                                          //Configurações Iniciais, declarando as entradas e saídas
                                          
void setup()
{
   
  pinMode(fim_de_curso, INPUT);                                //inicializa o pino para entrada
  pinMode(menu,  INPUT_PULLUP);                                //botao que navega os menu
  pinMode(enter, INPUT_PULLUP);                                //botao que seleciona o menu

  pinMode(led_verde, OUTPUT);
  pinMode(led_azul, OUTPUT);                                   //Leds definidos como saida
  pinMode(led_vermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led2_verde, OUTPUT);
//=========================================================================================================

  motor1.setSpeed(600);                                        // Velocidade para os motores de passo
  motor2.setSpeed(600);
  
  lcd.begin (16,4);                                            // Tipo de Lcd
  lcd.setBacklight(HIGH);                                      //Luz de funo defindo como ligado
  lcd.setCursor(2,0);                                          //inicio da escrita
  lcd.print("COQUETELEIRA");
  delay(1500);
  lcd.setCursor(2,1);
  lcd.print("AUTOMATIZADA");
  delay(5500);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("BEBA COM");
  delay(1500);
  lcd.setCursor(3,1);
  lcd.print("MODERACAO");
  delay(5500);
  lcd.clear(); 
  
 
   
 
} //end setup


// ========================================================================================================
                                   // --- Loop Infinito ---
void loop()
{
   lcd.setCursor(0,0);                                          //inicio da escrita
   lcd.print(">");
   delay(50);

   
  
 // if(!sub_menu)                                                 //Menu principal
 // {
    lcd.setCursor(1,line[0]);
    lcd.print("1) COCA COLA        ");
    lcd.setCursor(1,line[1]);
    lcd.print("2) DRINK ELITE      ");
    lcd.setCursor(1,line[2]);
    lcd.print("3) BIU  BIU            ");
    lcd.setCursor(1,line[3]);
    lcd.print("4) COQUETEL        ");
    lcd.setCursor(1,line[4]);
    lcd.print("5) CRIADORES       ");
   
   

  readButts();

} 

void setColor(int verde, int azul, int vermelho)                 //função para os leds por PWM
{
    #ifdef COMUM_ANODO
    verde = 255 - verde;
    azul = 255 - azul;
    vermelho = 255 - vermelho;
    #endif
    analogWrite(led_verde, verde);
    analogWrite(led_azul, azul);
    analogWrite(led_vermelho, vermelho);  
}


void readButts()                                                  //Função para sub menu
{
  
   if(!digitalRead(menu))  menu_flag  = 0x01;
   if(!digitalRead(enter)) enter_flag = 0x01;

   if(digitalRead(menu) && menu_flag)
   {
       menu_flag = 0x00;
       list_menu();
       menu_number+=1;
       if(menu_number > 5) menu_number = 1;
    
   } //fim do if menu

   if(digitalRead(enter) && enter_flag)
   {
      enter_flag = 0x00; 
      sub_menu = !sub_menu;
      menu_select();
    
   }                                                              //fim do if menu
  
  
}                                                                 //fim da funçao readButts


void list_menu()
{
  for(int i=4; i>-1; i--) 
  {
     index = i-1;
     line_bk[i] = line[i];
     
     if(index < 0) line[i] = line_bk[i+4];
     
     else line[i] = line[i-1];

  }

}                                                                 //fim da list_menu


void menu_select()                                                //Função para selecao dos menu
{

    switch(menu_number)
    {
       case 1: 
               lcd.setCursor(1,0);                                //Cada case será inserido um programa
               lcd.print("COMPUTANDO... ");                            // diferente de acordo com a receita
               lcd.setCursor(0,1);
               lcd.print("                ");
               delay(1000);
               lcd.clear();
               lcd.setCursor(1,0);
               lcd.print("PREPARANDO...  ");
               delay(1000);

                        
                        // Encontrando a localização do fim de curso
                        
  
       estado_botao = digitalRead(fim_de_curso);            //Ler o pino de entrada do botao(fim de curso)       
       while(estado_botao == LOW && encontrado == false){          //Mover o motor ate o fim de curso ser encontrado
       setColor(255, 0, 0);  // vermelho                           //led fica vermelho ate encontrar o fim de curso
       motor1.step(10, BACKWARD, SINGLE);                           //Ação do motor de passo
       estado_botao = digitalRead(fim_de_curso);               
  }                                                               

      encontrado = true;                                            //Condicão para iniciar a receita 
      setColor(0, 0, 255);                                          //Ascende led quando fim de curso for encontrado
      delay(300);


                       //Momento em que a mesa se movimenta e aciona os bicos para saida de bebida 
  
  
      motor1.step(400, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(250, BACKWARD, SINGLE);
      motor1.release();
      delay(500);
      lcd.setCursor(1,0);
      lcd.print("RETIRE A BEBIDA" );
      digitalWrite(led2_verde, HIGH);
      delay(2000);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      delay(3000);
      digitalWrite(led2_verde, LOW);
      encontrado = false;
               break;
       case 2: 
                
              lcd.setCursor(1,0);                                //Cada case será inserido um programa
               lcd.print("COMPUTANDO... ");                            // diferente de acordo com a receita
               lcd.setCursor(0,1);
               lcd.print("                ");
               delay(1000);
               lcd.clear();
               lcd.setCursor(1,0);
               lcd.print("PREPARANDO...  ");
               delay(1000);      

                        
                        // Encontrando a localização do fim de curso
                        
  
       estado_botao = digitalRead(fim_de_curso);            //Ler o pino de entrada do botao(fim de curso)       
       while(estado_botao == LOW && encontrado == false){          //Mover o motor ate o fim de curso ser encontrado
       setColor(255, 0, 0);  // vermelho                           //led fica vermelho ate encontrar o fim de curso
       motor1.step(10, BACKWARD, SINGLE);                           //Ação do motor de passo
       estado_botao = digitalRead(fim_de_curso);               
  }                                                               

      encontrado = true;                                            //Condicão para iniciar a receita 
      setColor(0, 0, 255);                                          //Ascende led quando fim de curso for encontrado
      delay(300);


                       //Momento em que a mesa se movimenta e aciona os bicos para saida de bebida 
  
  
      motor1.step(400, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(400, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD,DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(650, BACKWARD,SINGLE);
      delay(500);
      lcd.setCursor(1,0);
      lcd.print("RETIRE A BEBIDA" );
       digitalWrite(led2_verde, HIGH);
       delay(2000);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      delay(3000);
      digitalWrite(led2_verde, LOW);
      encontrado = false;
               break;
       case 3: 
                lcd.setCursor(1,0);                                //Cada case será inserido um programa
               lcd.print("COMPUTANDO... ");                            // diferente de acordo com a receita
               lcd.setCursor(0,1);
               lcd.print("                ");
               delay(1000);
               lcd.clear();
               lcd.setCursor(1,0);
               lcd.print("PREPARANDO...  ");
               delay(1000);      

                        
                        // Encontrando a localização do fim de curso
                        
  
       estado_botao = digitalRead(fim_de_curso);            //Ler o pino de entrada do botao(fim de curso)       
       while(estado_botao == LOW && encontrado == false){          //Mover o motor ate o fim de curso ser encontrado
       setColor(255, 0, 0);  // vermelho                           //led fica vermelho ate encontrar o fim de curso
       motor1.step(10, BACKWARD, SINGLE);                           //Ação do motor de passo
       estado_botao = digitalRead(fim_de_curso);               
  }                                                               

      encontrado = true;                                            //Condicão para iniciar a receita 
      setColor(0, 0, 255);                                          //Ascende led quando fim de curso for encontrado
      delay(300);


                       //Momento em que a mesa se movimenta e aciona os bicos para saida de bebida 
  
  
      motor1.step(400, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(400, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(400, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(1050, BACKWARD, SINGLE);
      motor1.release();
      lcd.setCursor(1,0);
      lcd.print("RETIRE A BEBIDA" );
      delay(2000);
       digitalWrite(led2_verde, HIGH);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      delay(3000);
      digitalWrite(led2_verde, LOW);
      encontrado = false;
               break;
       case 4: 
                lcd.setCursor(1,0);                                //Cada case será inserido um programa
               lcd.print("COMPUTANDO... ");                            // diferente de acordo com a receita
               lcd.setCursor(0,1);
               lcd.print("                ");
               delay(1000);
               lcd.clear();
               lcd.setCursor(1,0);
               lcd.print("PREPARANDO...  ");
               delay(1000);      

                        
                        // Encontrando a localização do fim de curso
                        
  
       estado_botao = digitalRead(fim_de_curso);            //Ler o pino de entrada do botao(fim de curso)       
       while(estado_botao == LOW && encontrado == false){          //Mover o motor ate o fim de curso ser encontrado
       setColor(255, 0, 0);  // vermelho                           //led fica vermelho ate encontrar o fim de curso
       motor1.step(10, BACKWARD, SINGLE);                           //Ação do motor de passo
       estado_botao = digitalRead(fim_de_curso);               
  }                                                               

      encontrado = true;                                            //Condicão para iniciar a receita 
      setColor(0, 0, 255);                                          //Ascende led quando fim de curso for encontrado
      delay(300);


                       //Momento em que a mesa se movimenta e aciona os bicos para saida de bebida 
  
      motor1.step(600, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(600, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(600, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(600, FORWARD, SINGLE);
      motor1.release();
      delay(500);
      motor2.step(1000, BACKWARD, DOUBLE);
      delay(500);
      motor2.step(1000, FORWARD, DOUBLE);
      motor2.release();
      delay(500);
      motor1.step(2250, BACKWARD, SINGLE);
      motor1.release();
      delay(500);
      lcd.setCursor(1,0);
      lcd.print("RETIRE A BEBIDA" );
      delay(2000);
       digitalWrite(led2_verde, HIGH);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      tone(buzzer, 4000);
      delay(1000);
      noTone(buzzer);
      delay(500);
      delay(3000);
      digitalWrite(led2_verde, LOW);
      encontrado = false;
               break;
       case 5: 
               lcd.setCursor(0,0);
               lcd.print("DANIEL PINHEIRO RAMOS   ");
               lcd.setCursor(0,1);
               lcd.print("                           ");
               delay(2000); 
               for(i=0; i<5; i++){    
               lcd.scrollDisplayLeft(); 
               delay(1000); //Aguarda 1 segundo
               }
              
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("VENANCIO DE SOUZA REIS"); 
               lcd.setCursor(0,1);
               lcd.print("                           ");
               delay(2000); 
               for(i=0; i<6; i++){    
               lcd.scrollDisplayLeft(); 
               delay(1000);
               }
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("PAULO HENRIQUE FIGUEIREDO");
               lcd.setCursor(0,1);
               lcd.print("                           ");
               delay(2000); 
               for(i=0; i<9; i++){    
               lcd.scrollDisplayLeft(); 
               delay(1000); //Aguarda 1 segundo
               }
              
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("DANILO PASSOS ");
               lcd.setCursor(0,1);
               lcd.print("                           ");
               delay(3000); 
               
              
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("FREDSON OLIVEIRA SANTOS  ");
               lcd.setCursor(0,1);
               lcd.print("                           ");
               delay(2000); 
               for(i=0; i<5; i++){    
               lcd.scrollDisplayLeft(); 
               delay(1000); //Aguarda 1 segundo
               }

               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("DANILO PASSOS ");
               lcd.setCursor(0,1);
               lcd.print("                           ");
               delay(3000); 
              
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("JUNIAS MANOEL ");
               lcd.setCursor(0,1);
               lcd.print("                           ");
               delay(3000); 
               lcd.clear();
               
                  
               break;
              
      
                                                                      //fim do switch
    }
    
 
}                                                                      //fim do menu_select


