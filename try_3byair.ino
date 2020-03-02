/*
 　・初期位置で、ボールを受け取る
 　 1.ボールを掴むエアーoff
 　・トライ位置につく
 　 2.レールを伸ばす
    3.ボールを離す
    4.ボールを離したよ案内(bool=true)
  ・初期位置に戻りながら、上の機構も元に戻す
    5.ボールを掴むエアーoff
    6.レールを元に戻す
 */
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>

//PWM
const int Airball = 5;
const int Airrale = 4;

//ノードハンドルの宣言 
ros::NodeHandle nh;

std_msgs::String chat;
std_msgs::Bool tf_msg;

ros::Publisher ACtry("bool", &tf_msg);//ボールをおいたかどうかの判断(true:put ball, false:Don't put ball)
ros::Publisher CHpub("ACmode", &chat);//Actuatorでのmode(start,ballcatch,try,ballleave)を表示(削除可)

//bool
bool leave = false;

void  Actuator(const std_msgs::Int32& try_msg){
  static uint32_t pre_time;
    if( try_msg.data == 0){//初期位置
       digitalWrite(Airball, LOW);
       digitalWrite(Airrale,LOW);
        chat.data = "start";
        leave = false;
    }else if( try_msg.data == 1){//初期位置でボールをつかむ
       digitalWrite(Airball, LOW);
      digitalWrite(Airrale,HIGH);
        chat.data = "balecatch";
        leave = false;
    }else if( try_msg.data == 2){//try
       digitalWrite(Airball, HIGH);
       digitalWrite(Airrale,HIGH);
        chat.data = "try";
        leave = false;
    }
    CHpub.publish(&chat);
     tf_msg.data =  leave;
   /*  if (millis()-pre_time >= 50)
  {
    tf_msg.data =  leave;
    pre_time = millis();
  }*/


}

// トピック名をtryとしてSubscriberをインスタンス化し、try呼び出す
ros::Subscriber<std_msgs::Int32> sub("try", &Actuator );

void setup()
{ 
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(CHpub);
  nh.advertise(ACtry);//うまく機能せず
 

  pinMode(Airball, OUTPUT);
  pinMode(Airrale, OUTPUT);
}

void loop()
{  
  nh.spinOnce();
   tf_msg.data =  leave;
  delay(1);
}
