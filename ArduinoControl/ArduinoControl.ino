


//--------------------------------------------------感測器及開關輸入----
int sensor01 = 2;//下方
int sensor02 = 15;
int sw = 5;
//--------------------------------------------------步進馬達輸出及控制----
//----m0----
int pin_step = 4;
int pin_dir = 3;
//----m1----
int pinA = 16;
int pinB = 17;
int pinC = 18;
int pinD = 19;

int TAB1[8] = {1,1,0,0,0,0,0,1};
int TAB2[8] = {0,1,1,1,0,0,0,0};
int TAB3[8] = {0,0,0,1,1,1,0,0};
int TAB4[8] = {0,0,0,0,0,1,1,1};


uint16_t delayset=0;//延遲設定

int STEPS_m0 = 3200; //每圈步數
int STEPS_m1 = 1024; //每圈步數
float STEPS2_m0;     //簡化參數
float STEPS2_m1;     //簡化參數
float STEPS2;
int SETSpeed=1;   //設定速度(RPM)
int SETsetp=1;    //設定步數

uint32_t step_count=0; //步數計數
int speed_count=0;//速度計數
float acceleration_set=0.5; //加速度設定
int stop_flag=0;  //啟停旗標
int state_set=0;  //現態設定
uint32_t total_step;   //總步數
int motorDir;//1為正轉，0為反轉，用於m1
int j=0,j2=0,a;  //暫存
int motor_flag;

uint32_t motor0_location = 0;
uint32_t motor1_location = 0;

void Motor_0_control(uint32_t,int,int,float);
void Motor_1_control(uint32_t,int,int,float);

//--------------------------------------------------LED輸出----
#include <WS2812FX.h>

#define LED_COUNT 20
#define LED_PIN 14

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int LED = 13;

//--------------------------------------------------字串讀取----
String c = "";//讀近來的字串
char readData;
int available_count;

void reStart(void);

void setup() {
  // put your setup code here, to run once:
  pinMode(sensor01,INPUT);
  pinMode(sensor02,INPUT);
  pinMode(sw,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,0);

  pinMode(pin_step,OUTPUT);
  pinMode(pin_dir,OUTPUT);
  digitalWrite(pin_step,0);
  digitalWrite(pin_dir,0);//1往原點，0往前

  pinMode(pinA,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinC,OUTPUT);
  pinMode(pinD,OUTPUT);
  
  Serial.begin(115200);

  //----步數預算----
  STEPS2_m0 = 1000000/(float)(STEPS_m0/60.0);
  STEPS2_m1 = 1000000/(float)(STEPS_m1/60.0);
  //-----------------------------------------------中斷設定----
  cli();////關閉全局中斷

  //設置定時器1爲1kHz
  TCCR1A = 0;//將整個TCCR1A寄存器設置爲0
  TCCR1B = 0;//將整個TCCR1B寄存器設置爲0
  TCNT1  = 0;//將計數器值初始化爲0
  //設置計數器爲1kHZ，即1ms
  OCR1A = 1999;// = (16*10^6)/(1000*8) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12);//打開CTC模式
  TCCR1B |= (1 << CS11);//設置CS11位爲1(8倍預分頻)
  TIMSK1 |= (1 << OCIE1A);

  sei();//打開全局中斷

  //-----------------------------------------------LED初始化----
  ws2812fx.init();
  ws2812fx.setBrightness(180);
  ws2812fx.setSpeed(1000);
  ws2812fx.setColor(0x000000);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();


  reStart();
}

void loop() {
  int32_t loc;


  //-------------讀取字串-----------------
  if(Serial.available()>0){
    delay(50);
    c = "";
    while(Serial.available()>0){
      readData = Serial.read();
      c += readData;
    }
  }

  if(c=="RESTART"){//----------------重置---------------
    reStart();
    Serial.print("ok");
  }
  else if(c=="move0"){//------------移動到出料位(前)-----------
    loc = motor0_location;
    motor0_location = 0;
    Motor_0_control(loc,1,60,0.5);
    while(stop_flag==1)delay(1);
    Serial.print("ok");
  }
  else if(c=="move1"){//------------移動到進料位------------
    loc = motor0_location-5600;
    motor0_location = 5600;
    if(loc>0){
      Motor_0_control(loc,1,60,0.5);
    }
    else if(loc<0){
      loc = (~loc)+1;
      Motor_0_control(loc,0,60,0.5);
    }
    while(stop_flag==1)delay(1);
    Serial.print("ok");
  }
  else if(c=="move2"){//------------移動到下方拍照位-----------
    loc = motor0_location-17700;
    motor0_location = 17700;
    if(loc>0){
      Motor_0_control(loc,1,60,0.5);
    }
    else if(loc<0){
      loc = (~loc)+1;
      Motor_0_control(loc,0,60,0.5);
    }
    while(stop_flag==1)delay(1);
    Serial.print("ok");
  }
  else if(c=="move3"){//-------------移動到上方錄影位------------
    loc = motor0_location-23000;
    motor0_location = 23000;
    if(loc>0){
      Motor_0_control(loc,1,60,0.5);
    }
    else if(loc<0){
      loc = (~loc)+1;
      Motor_0_control(loc,0,60,0.5);
    }
    while(stop_flag==1)delay(1);
    Serial.print("ok");
  }
  else if(c=="move4"){//------------移動到出料位(後)-----------
    loc = 37600-motor0_location;
    motor0_location = 37600;
    Motor_0_control(loc,0,60,0.5);
    while(stop_flag==1)delay(1);
    Serial.print("ok");
  }
  else if(c=="photo"){//------------拍照控制--------------
    ws2812fx.setBrightness(50);
    for(int g=0;g<20;g++){
      if(g<8){
        ws2812fx.setPixelColor(g, 0xFFFFFF);
      }
      else {
        ws2812fx.setPixelColor(g, 0x000000);
      }
    }
    ws2812fx.show();
    delay(3000);
    for(int g=0;g<20;g++){
      ws2812fx.setPixelColor(g, 0x000000);
    }
    ws2812fx.show();
    Serial.print("ok");
  }
  else if(c=="video"){//------------錄影控制--------------
    ws2812fx.setBrightness(120);
    for(int g=0;g<20;g++){
      if(g<8){
        ws2812fx.setPixelColor(g, 0x000000);
      }
      else {
        ws2812fx.setPixelColor(g, 0xFFFFFF);
      }
    }
    ws2812fx.show();
    delay(500);
    if(motor1_location==0){
      Motor_1_control(4150,1,3,0.2);
      motor1_location = 4150;
    }
    else {
      Motor_1_control(4150,0,3,0.2);
      motor1_location = 0;
    }
    while(stop_flag==1)delay(1);
    delay(500);
    for(int g=0;g<20;g++){
      ws2812fx.setPixelColor(g, 0x000000);
    }
    ws2812fx.show();
    Serial.print("ok");
  }
  else if(c=="perfect"){//----------無缺陷出料------------
    loc = 37600-motor0_location;
    Motor_0_control(loc,0,60,0.5);
    while(stop_flag==1)delay(1);
    Motor_0_control(32000,1,60,0.5);
    while(stop_flag==1)delay(1);
    motor0_location = 5600;
    Serial.print("ok");
  }
  else if(c=="defect"){//----------有缺陷出料-----------
    loc = motor0_location;
    Motor_0_control(loc,1,60,0.5);
    while(stop_flag==1)delay(1);
    Motor_0_control(5600,0,60,0.5);
    while(stop_flag==1)delay(1);
    motor0_location = 5600;
    Serial.print("ok");
  }
  else if(c=="feed_detection"){//---------進料確認-------------
    while(digitalRead(sw)==1)delay(1);
    Serial.print("ok");
  }

  if(c!=""){
    c = "";
    //Serial.println(" ");
  }

}
ISR(TIMER1_COMPA_vect){
  if(stop_flag==1){//啟停旗標控制
    if(step_count>0){     //步數計數大於零判斷條件

      if(motor_flag==0){//判斷哪顆馬達
        j++;//暫存加一，用於步進解碼
        digitalWrite(pin_step,(j&0x0001));//設定步進控制輸出
      }
      else if(motor_flag==1){
        if(motorDir==0)j2++;//暫存加一，用於步進解碼
        else j2--;
        //設定步進控制輸出
        digitalWrite(pinA,TAB1[(j2&0x000e)>>1]);
        digitalWrite(pinB,TAB2[(j2&0x000e)>>1]);
        digitalWrite(pinC,TAB3[(j2&0x000e)>>1]);
        digitalWrite(pinD,TAB4[(j2&0x000e)>>1]);
      }
      
      step_count--;//步進次數減1
    }
    if(step_count==0){    //步數計數等於零判斷條件
      switch(state_set) {//switch狀態選擇
          case 0:
            speed_count=1;//設定初始速度
            state_set=1;//設定次態
            a=0;//初始化暫存
            //b=0;//--------------------------------------------------------------------------------------檢測步數
            //c=0;
            break;
          case 1:
            if(speed_count<=SETSpeed){   //當運行速度小於目標速度進入
              delayset = STEPS2/speed_count;//算出延遲
              OCR1A = delayset-1;//設定延遲到中斷計數器中
              step_count = speed_count/acceleration_set;//設定行走步數
              a=a+step_count;//累計步數
              speed_count++;//速度加1
            }
            else {
              speed_count--;//最高速度回減1
              state_set=2;//設定次態
              step_count = total_step-(2*a);//設定剩餘步數
              //c=step_count;
              //b=step_count+a;//-----------------------------------------------------------------------------檢測步數
            }
            break;
          case 2:
            if(speed_count>0){    //當運行速度大於0
              delayset = STEPS2/speed_count;//算出延遲
              OCR1A = delayset-1;//設定延遲到中斷計數器中
              step_count = speed_count/acceleration_set;//設定行走步數
              //b=b+step_count;//-----------------------------------------------------------------------------檢測步數
              speed_count--;//速度減1
            }
            else {
              OCR1A = 1999;//中斷速度重設為1kHz
              stop_flag = 0;//暫停迴圈
              state_set=0;//設定次態
            }
            break;
          default:
            stop_flag = 0;
            state_set=0;
            break;
      }
  
    }
    
  }
}
void Motor_0_control(uint32_t step_set,int Dir,int MAX_speed,float acceleration){
  digitalWrite(pin_dir,Dir);//設定方向
  acceleration_set = acceleration;  //設定加速度
  STEPS2 = STEPS2_m0;
  motor_flag = 0;
  uint32_t k = step_set; //設定步數(2倍化)
  int m,n;
  for(m=1,n=0;(n<k && m<MAX_speed);m++){ //預先計數，計算範圍內的最高速度
    n = n + (m/acceleration_set);
  }
  if(m==MAX_speed);
  else if(m>=3) m=m-2;
  else m=1;
  SETSpeed = m;//載入最高速度
  total_step = step_set*2;//設定總步數(2倍化)

  state_set=0;//switch狀態設為0
  step_count=0;//初始步數設定為0
  stop_flag=1;//啟動函數旗標
}
void Motor_1_control(uint32_t step_set,int Dir,int MAX_speed,float acceleration){
  motorDir = Dir;//設定方向
  acceleration_set = acceleration;  //設定加速度
  STEPS2 = STEPS2_m1;
  motor_flag = 1;
  uint32_t k = step_set; //設定步數(2倍化)
  int m,n;
  for(m=1,n=0;(n<k && m<(MAX_speed<<2));m++){ //預先計數，計算範圍內的最高速度
    n = n + (m/acceleration_set);
  }
  if(m==MAX_speed<<2);
  else if(m>=3) m=m-2;
  else m=1;
  SETSpeed = m;//載入最高數度
  total_step = step_set*2;//設定總步數(2倍化)

  state_set=0;//switch狀態設為0
  step_count=0;//初始步數設定為0
  stop_flag=1;//啟動函數旗標
}
void reStart(){

  
  Motor_0_control(41600,1,25,1);
  while(stop_flag==1){
    if(digitalRead(sensor01)==0){
      stop_flag = 0;
      motor0_location = 5600;
    }
    delay(1);
  }
  Motor_0_control(5600,0,50,0.5);
  while(stop_flag==1)delay(1);
  
  Motor_1_control(5000,0,3,0.2);
  while(stop_flag==1){
    if(digitalRead(sensor02)==0){
      stop_flag = 0;
      motor1_location = 0;
    }
    delay(1);
  }

  for(int fff=0;fff<20;fff++){
    ws2812fx.setPixelColor(fff, 0x000000);
  }
  ws2812fx.show();
  Serial.print("start");
}
