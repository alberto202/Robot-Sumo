

void empujar(){
   if(IN3){  //jumper delantero
      direccion = 0;
      pwm1 = 255;
   }
   else if(IN4){  //jumper trasero
      direccion = 1;
      pwm1 = 255;
   }
}

void atacar()
{
   pwm1 = 128;
}

void buscar()
{/*
   pwm1 = 60;
   direccion = 3;
   delay_ms(200);
   direccion = 0;
   delay_ms(1000);
   direccion = 2;
   delay_ms(200);
*/3}
      parar();
}

#INT_RDA
void rda_isr(void){
   disable_interrupts(GLOBAL);
   char cadena[2];
   char *search = " ";
   char *token;
   int i=0;
   int exp = 1;
   long  duracion2;
   char *enviar;
   
   while(kbhit() && i < 2){
      cadena[i] = getchar();
      i++;
   }
   
   puts(cadena);
   //D
   //token = strtok(cadena, search);
   //Direccion
   //token = strtok(cadena, search);
   switch(cadena[0]){
      case "1":
         direccion = 1;
         break;
      case "2":
         direccion = 2;
         break;
      case "3":
         direccion = 3;
         break;
      case "0":
         direccion = 0;
         break;
      default:
         direccion = 0;
   }
   duracion = 500;
   putchar(direccion);
   
   encontrado=1;
   
   /*
   direccion = token-48;
   //T
   token = strtok(cadena, search);
   //Tiempo
   token = strtok(cadena, search);
   duracion2 = 0;
   
   for(int j=strlen(token) -1 ; j>=0; j--)
   {
      duracion2 += (token[j]-48) * exp;
      exp *= 10;
   } 
   
   duracion = duracion2;
   sprintf(enviar, "%ld", duracion);

   putc(direccion+48);
   puts(enviar);
    
   //int num;
   //num = (tiempo[0]-48)*100 + (tiempo[1]-48)*10 + (tiempo[2]-48); 
   
   /*movimiento(4, duracion);
   
   if(getchar() == 'b' )
      direccion = 1;
   else duracion = 0;*/
   
   enable_interrupts(GLOBAL);
}

void main() {
   
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2|RTCC_8_BIT);
   enable_interrupts(INT_TIMER0);   
   enable_interrupts(INT_RDA);
   
   delay_ms(500);
   putc('1');        // Comienza a contar arduino
   delay_ms(2500);
   
   enable_interrupts(GLOBAL);
   
   while (1){
      linea();
      empujar();      
      if(encontrado) atacar();
      else buscar();
   }
}

void linea(){
    
   //if(IN1 || IN2 || IN5)
   if(IN1 || IN2)
      led_on();
   else
      led_off();
      
   if(IN1 && IN2)
      direccion = 0;
   if(IN1 && !IN2)
      direccion = 3;
   if(!IN1 && IN2)
      direccion = 2;
   if(!IN1 && !IN2){
      direccion = 1;
      delay_ms(1000);
      direccion = 3;
      delay_ms(1000);
   }
   
   encontrado = 0;
}

void empujar(){
   if(IN3){  //jumper delantero
      direccion = 0;
      pwm1 = 255;
   }
   else if(IN4){  //jumper trasero
      direccion = 1;
      pwm1 = 255;
   }
}

void atacar()
{
   pwm1 = 128;
}

void buscar()
{/*
   pwm1 = 60;
   direccion = 3;
   delay_ms(200);
   direccion = 0;
   delay_ms(1000);
   direccion = 2;
   delay_ms(200);
*/}
