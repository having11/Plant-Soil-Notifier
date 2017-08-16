//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BLUE_PIN A3
#define GREEN_PIN A4
#define SOIL_SENSOR A1

//#define RESOLUTION 1024 //Use for 10 bit resolution ADC (Arduino)
#define RESOLUTION 4096 //Use for 12 bit resolution ADC (Particle Photon)

#define DEBUG_VALS //Comment if you don't want values published to the Particle Cloud

LiquidCrystal_I2C lcd(0x20,16,2);  // set the LCD address to 0x20(Cooperate with 3 short circuit caps) for a 16 chars and 2 line display
int level;

/*byte one_bar[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111
};

byte two_bar[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111
};

byte three_bar[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111
};

byte four_bar[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B11111
};

byte five_bar[8] = {
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
};

byte six_bar[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
};

byte seven_bar[8] = {
    B00000,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
};

byte eight_bar[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
}; */

bool is_spam = false;
    
void debug_values(int raw_value){
    Particle.publish("soil_values_raw", String(raw_value));
}

void setup(){
  
   /*lcd.createChar(0, one_bar);
   lcd.createChar(1, two_bar);
   lcd.createChar(2, three_bar);
   lcd.createChar(3, four_bar);
   lcd.createChar(4, five_bar);
   lcd.createChar(5, six_bar);
   lcd.createChar(6, seven_bar);
   lcd.createChar(7, eight_bar);*/
  
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(SOIL_SENSOR, INPUT);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  lcd_clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Plant hydration");
  lcd.setCursor(0, 1);
  lcd.print("monitor");
  delay(4000);
}
  
void loop(){
  
  int reading2 = take_reading();
  
  float reading = reading2;
  
  lcd.print("Water level:");
  lcd.setCursor(0,1);
  lcd.print(reading);
  delay(1000);
  
  #ifdef DEBUG_VALS
  debug_values(reading);
  #endif
  
  Particle.publish("level", String(reading));
  
  if(reading >= .95*RESOLUTION){
      lcd_clear();
      lcd.print("Did you water");
      lcd.setCursor(0,1);
      lcd.print("too much?");
      level = 16;
  }
  else if(reading >= .85*RESOLUTION){
      lcd_clear();
      lcd.print("That's plenty");
      level = 14;
  }
  else if(reading >= .78*RESOLUTION){
      lcd_clear();
      lcd.print("Thanks for all");
      lcd.setCursor(0,1);
      lcd.print("of that water");
      level = 13;
  }
  else if(reading >= .70*RESOLUTION){
      lcd_clear();
      lcd.print("Woah, that's");
      lcd.setCursor(0,1);
      lcd.print("a good drink");
      level = 12;
  }
  else if(reading >= .65*RESOLUTION){
      lcd_clear();
      lcd.print("That makes me");
      lcd.setCursor(0,1);
      lcd.print("very happy");
      level = 11;
  }
  else if(reading >= .60*RESOLUTION){
      lcd_clear();
      lcd.print("I love you");
      lcd.setCursor(0,1);
      lcd.print("more now");
      level = 10;
  }
  else if(reading >= .55*RESOLUTION){
      lcd_clear();
      lcd.print("Meh, almost");
      lcd.setCursor(0,1);
      lcd.print("time for more");
      level = 9;
  }
  else if(reading >= .50*RESOLUTION){
      lcd_clear();
      lcd.print("Did you water");
      lcd.setCursor(0,1);
      lcd.print("too much?");
      level = 8;
  }
  else if(reading >= .45*RESOLUTION){
      lcd_clear();
      lcd.print("Still pretty");
      lcd.setCursor(0,1);
      lcd.print("content");
      level = 7;
  }
  else if(reading >= .40*RESOLUTION){
      lcd_clear();
      lcd.print("Get ready for");
      lcd.setCursor(0,1);
      lcd.print("some watering!");
      level = 6;
  }
  else if(reading >= .35*RESOLUTION){
      lcd_clear();
      lcd.print("I'm still");
      lcd.setCursor(0,1);
      lcd.print("waiting");
      level = 5;
  }
  else if(reading >= .30*RESOLUTION){
      lcd_clear();
      lcd.print("OK, water me");
      lcd.setCursor(0,1);
      lcd.print("I'm very thirsty");
      level = 4;
  }
  else if(reading >= .22*RESOLUTION){
      lcd_clear();
      lcd.print("Still no water?");
      lcd.setCursor(0,1);
      lcd.print("You're mean");
      level = 3;
  }
  else if(reading >= .15*RESOLUTION){
      lcd_clear();
      lcd.print("Water please");
      lcd.setCursor(0,1);
      lcd.print("it's dry here!");
      level = 2;
  }
  else if(reading >= .08*RESOLUTION){
      lcd_clear();
      lcd.print("I'm about");
      lcd.setCursor(0,1);
      lcd.print("to die");
      level = 1;
  }
  else if(reading >= .04*RESOLUTION){
      lcd_clear();
      lcd.print("Blahhh :(");
      lcd.setCursor(0,1);
      lcd.print("you killed me");
      level = 0;
  }
  delay(3000);
  eval_level();
  delay(4000);
  lcd_clear();
  no_green();
  
}

void eval_level(){
    if(level>8){
        solid_green();
    }
    else if(level<8 && level != 0){
        blink_blue(8, 250);
        if(is_spam==false){
        Particle.publish("dry", "True");
        }
        is_spam = true;
    }
    else if(level==8){
        blink_blue(3, 250);
    }
    else if(level==0){
        blink_blue(10, 250);
    }
}

int take_reading(){
    lcd_clear();
    lcd.print("Taking reading");
    int amount = analogRead(SOIL_SENSOR);
    delay(1000);
    lcd_clear();
    return amount;
}

void blink_blue(int number_of_loops, int duration){
    for(int i=0; i< number_of_loops; i++){
        digitalWrite(BLUE_PIN, 1);
        delay(duration);
        digitalWrite(BLUE_PIN, 0);
        delay(duration);
    }
}

void solid_green(){
    digitalWrite(GREEN_PIN, 1);
}

void no_green(){
    digitalWrite(GREEN_PIN, 0);
}

void lcd_clear(){
    lcd.clear();
    lcd.setCursor(0,0);
}
