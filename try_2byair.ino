/*
 　_____________|Airball1,2  | Airrale |
 初期位置　　　　|low　　  | low     |
 tryゾーンついた |low      |  high   |
 ボールおいたよ  | high    |  high   |
 
 */
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>

//PWM
const int Airball1 = 40;
const int Airball2 = 41;
const int Airrale =  42;
//const int flag = 20;

//tryしたことの判定
//bool leave = false;

//ノードハンドルの宣言 
ros::NodeHandle nh;

std_msgs::String chat;
std_msgs::Bool tf_msg;

ros::Publisher ACtry("bool", &tf_msg);//ボールをおいたかどうかの判断(true:put ball, false:Don't put ball)
ros::Publisher CHpub("ACmode", &chat);//Actuatorでのmode(start,ballcatch,try,ballleave)を表示(削除可)

void  Actuator(const std_msgs::Int32& try_msg){
    if( try_msg.data == 0){//初期位置
       digitalWrite(Airball1, LOW);
       digitalWrite(Airball2, LOW);
       digitalWrite(Airrale,LOW);
       //digitalWrite(flag,LOW);
        chat.data = "start";
    }else if( try_msg.data == 1){//tryゾーン到着!
       digitalWrite(Airball1, LOW);
       digitalWrite(Airball2, LOW);
       digitalWrite(Airrale,HIGH);
       //digitalWrite(flag,LOW);
        chat.data = "balecatch";
    }else if( try_msg.data == 2){//ボールを置く
       digitalWrite(Airball1, HIGH);
       digitalWrite(Airball2, HIGH);
       digitalWrite(Airrale,HIGH);
      // digitalWrite(flag,HIGH);
        chat.data = "try";
    }
    CHpub.publish(&chat);
}

// トピック名をtryとしてSubscriberをインスタンス化し、try呼び出す
ros::Subscriber<std_msgs::Int32> sub("try", &Actuator );

void setup()
{ 
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(CHpub);
  nh.advertise(ACtry);//うまく機能せず
 
  //pinMode(flag, INPUT);
  pinMode(Airball1,OUTPUT);
  pinMode(Airball2, OUTPUT);
  pinMode(Airrale, OUTPUT);
  //pinMode(Airball, INPUT);
  //pinMode(Airrale, INPUT);
}

void loop(){
  
//tryしたことの判定
bool leave = false;
  static uint32_t pre_time;
  if(digitalRead(Airball1) ==HIGH && digitalRead(Airrale) ==HIGH){
    leave = true;
  }else{
    leave = false;
  }
  
  if (millis()-pre_time >= 50){
    tf_msg.data = leave;
    ACtry.publish(&tf_msg);
    pre_time = millis();
  }
  nh.spinOnce();
  delay(1);
}



