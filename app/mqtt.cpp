/**
 * @file
 * @author  Patrick Jahns http://github.com/patrickjahns
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 *
 */
#include <RGBWWCtrl.h>


//#############################################################################
//MQTT Topics
Topics::Topics(){

//subscribe
  sub.E1.count = sub_e1;
  sub.E2.count = sub_e2;
  sub.E3.count = sub_e3;

  sub.E1.item[0] = "cmd";
  sub.E2.item[0][0] = "showSettings";

  sub.E1.item[1] = "color";
  sub.E2.item[1][0] = "setRAW";
  sub.E3.item[1][0][0] = "R";
  sub.E3.item[1][0][1] = "G";
  sub.E3.item[1][0][2] = "B";
  sub.E3.item[1][0][3] = "WW";
  sub.E3.item[1][0][4] = "CW";
  sub.E2.item[1][1] = "setHSV";
  sub.E3.item[1][1][0] = "H";
  sub.E3.item[1][1][1] = "S";
  sub.E3.item[1][1][2] = "V";
  sub.E3.item[1][1][3] = "CT";

  //         [2][2][5]

//publish
  pub.E1.count = pub_e1;
  pub.E2.count = pub_e2;
  pub.E3.count = pub_e3;

  pub.E1.item[0] = "network";
  pub.E2.item[0][0] = "connection";
  pub.E3.item[0][0][0] = "dhcp";
  pub.E3.item[0][0][1] = "ip";
  pub.E3.item[0][0][2] = "netmask";
  pub.E3.item[0][0][3] = "gateway";
  pub.E2.item[0][1] = "ap";
  pub.E3.item[0][1][0] = "secured";
  pub.E3.item[0][1][1] = "ssid";
  pub.E2.item[0][2] = "mqtt";
  pub.E3.item[0][2][0] = "enabled";
  pub.E3.item[0][2][1] = "server";
  pub.E3.item[0][2][2] = "port";
  pub.E3.item[0][2][3] = "username";
  pub.E2.item[0][3] = "udpserver";
  pub.E3.item[0][3][0] = "enabled";
  pub.E3.item[0][3][1] = "port";
  pub.E2.item[0][4] = "tcpserver";
  pub.E3.item[0][4][0] = "enabled";
  pub.E3.item[0][4][1] = "port";

  pub.E1.item[1] = "color";
  pub.E2.item[1][0] = "outputmode";
  pub.E2.item[1][1] = "hsv";
  pub.E3.item[1][1][0] = "model";
  pub.E3.item[1][1][1] = "red";
  pub.E3.item[1][1][2] = "yellow";
  pub.E3.item[1][1][3] = "green";
  pub.E3.item[1][1][4] = "cyan";
  pub.E3.item[1][1][5] = "blue";
  pub.E3.item[1][1][6] = "magenta";
  pub.E2.item[1][2] = "brightness";
  pub.E3.item[1][2][0] = "red";
  pub.E3.item[1][2][1] = "green";
  pub.E3.item[1][2][2] = "blue";
  pub.E3.item[1][2][3] = "ww";
  pub.E3.item[1][2][4] = "cw";
  pub.E2.item[1][3] = "colortemp";
  pub.E3.item[1][3][0] = "ww";
  pub.E3.item[1][3][1] = "cw";

  pub.E1.item[2] = "security";
  pub.E2.item[2][0] = "api_secured";

  pub.E1.item[3] = "restart";

  //         [4][5][7]
}

void ApplicationMQTTClient::publishSettings() {
	Serial.println("publishSettings");
	pub(0,0,0, String(app.cfg.network.connection.dhcp));
	pub(0,0,1, app.cfg.network.connection.ip.toString());
	pub(0,0,2, app.cfg.network.connection.netmask.toString());
	pub(0,0,3, app.cfg.network.connection.gateway.toString());

}

void ApplicationMQTTClient::onMessageReceived(String topic, String message) {

	Serial.print("incoming subscribe: ");
	  Serial.print(topic);
	  Serial.print(" | ");
	  Serial.println(message);

	  TdissectResult dissectResult;
	  dissectResult = dissectPayload(topic, message);

	  if (dissectResult.found){

		  Serial.print("dissectResult = "); Serial.println(dissectResult.itemPath);
		  TparameterResult par = dissectParameter(message);

	    if (dissectResult.itemPath == "0/0"){
	      Serial.println("showSettings");
		  if (message == "all"){
		    Serial.println("showSettings/all");
			publishSettings();
		  }
		}

//setOutputRaw
	    if (dissectResult.itemPath == "1/0"){
		  if (par.count == 5){
			  Serial.println("setRAW");
			  raw.r  = par.item[0].toInt();
			  raw.g  = par.item[1].toInt();
			  raw.b  = par.item[2].toInt();
			  raw.ww = par.item[3].toInt();
			  raw.cw = par.item[4].toInt();
			  app.rgbwwctrl.setOutputRaw(raw.r, raw.g, raw.b, raw.ww, raw.cw);
			  //app.rgbwwctrl.color.ColorStorage.
		  }
	    }
		if (dissectResult.itemPath == "1/0/0"){
		  Serial.println("setRAW/R");
		  raw.r = String(message).toInt();
		  app.rgbwwctrl.setOutputRaw(raw.r, raw.g, raw.b, raw.ww, raw.cw);
		}
		if (dissectResult.itemPath == "1/0/1"){
		  Serial.println("setRAW/G");
		  raw.g = String(message).toInt();
		  app.rgbwwctrl.setOutputRaw(raw.r, raw.g, raw.b, raw.ww, raw.cw);
		}
		if (dissectResult.itemPath == "1/0/2"){
		  Serial.println("setRAW/B");
		  raw.b = String(message).toInt();
		  app.rgbwwctrl.setOutputRaw(raw.r, raw.g, raw.b, raw.ww, raw.cw);
		}
		if (dissectResult.itemPath == "1/0/3"){
		  Serial.println("setRAW/WW");
		  raw.ww = String(message).toInt();
		  app.rgbwwctrl.setOutputRaw(raw.r, raw.g, raw.b, raw.ww, raw.cw);
		}
		if (dissectResult.itemPath == "1/0/4"){
		  Serial.println("setRAW/CW");
		  raw.cw = String(message).toInt();
		  app.rgbwwctrl.setOutputRaw(raw.r, raw.g, raw.b, raw.ww, raw.cw);
		}

//setHSV
	    if (dissectResult.itemPath == "1/1"){
	    	if (par.count == 4){
	    		Serial.println("setHSV");
	    		hsv.h  = par.item[0].toInt();
	    		hsv.s  = par.item[1].toInt();
	    		hsv.v  = par.item[2].toInt();
	    		hsv.ct = par.item[3].toInt();
	    		app.rgbwwctrl.setHSV(hsv, 5000, true);
	    	}
		}
	    if (dissectResult.itemPath == "1/1/0"){
		  Serial.println("setHSV/H");
		  hsv.h = String(message).toInt();
		  app.rgbwwctrl.setHSV(hsv);
		}
	    if (dissectResult.itemPath == "1/1/1"){
		  Serial.println("setHSV/S");
		  hsv.s = String(message).toInt();
		  app.rgbwwctrl.setHSV(hsv);
		}
	    if (dissectResult.itemPath == "1/1/2"){
		  Serial.println("setHSV/V");
		  hsv.v = String(message).toInt();
		  app.rgbwwctrl.setHSV(hsv);
		}
	    if (dissectResult.itemPath == "1/1/3"){
		  Serial.println("setHSV/CT");
		  hsv.ct = String(message).toInt();
		  app.rgbwwctrl.setHSV(hsv);
		}





	  }
}

//#############################################################################

ApplicationMQTTClient::ApplicationMQTTClient() {
	//mqtt = NULL;

	//mqtt = new MqttClient("192.168.1.203", 1883, MqttStringSubscriptionCallback(&ApplicationMQTTClient::onMessageReceived, this));

	//procTimer = new Timer();
	//procTimer->initializeMs(1000, TimerDelegate(&ApplicationMQTTClient::timer_checkConnection, this)).start();

/*
	colorR = new int(0);
	colorG = new int(0);
	colorB = new int(0);
	colorWW = new int(0);
	colorCW = new int(0);
*/
}


ApplicationMQTTClient::~ApplicationMQTTClient() {
	// cleanup before destroying object
	if(mqtt != NULL) {
		delete mqtt;
	}
}


void ApplicationMQTTClient::start(){
	Serial.println("Start MQTT");
	if (mqtt != NULL) {
		 delete mqtt;
	}

	//TODO: add settings from config
	mqtt = new MqttClient("192.168.1.203", 1883, MqttStringSubscriptionCallback(&ApplicationMQTTClient::onMessageReceived, this));

	if(!mqtt->setWill("last/will","The connection from this device is lost:(", 1, true)) {
		Serial.println("Unable to set the last will and testament. Most probably there is not enough memory on the device.");
	}
	mqtt->connect(app.cfg.network.ap.ssid);


//##################################################
// der hier will nicht...... :-(

	//mqtt->setCompleteDelegate(TcpClientCompleteDelegate(&ApplicationMQTTClient::checkMQTTDisconnect, this));

//##################################################
    String subcribe = app.cfg.network.ap.ssid + "/#";           //"ESP/#"; //
    Serial.print("subscribe: "); Serial.println(subcribe);
	mqtt->subscribe(subcribe);
	//pub(0,0,0, app.cfg.network.ap.ssid);
}

void ApplicationMQTTClient::stop() {
	 delete mqtt;
	 mqtt = NULL;

/*
	 delete colorR;
	 delete colorG;
	 delete colorB;
	 delete colorWW;
	 delete colorCW;
*/
}

bool ApplicationMQTTClient::isRunning() {
	return (mqtt != NULL);
}



//===> dissect incomming subscribe <-------------------------------------------
TdissectResult ApplicationMQTTClient::dissectPayload(String subTopic, String subValue){
  TdissectResult dissectResult;
  String Topics[4];
  for (int i = 0; i < 4; i++) {
    Topics[i] = NULL;
  }

  String str = subTopic;
  if (str.startsWith("/")) str.remove(0, 1);
  if (str.endsWith("/")) str.remove(str.length()-1,str.length());
  dissectResult.topic = str;
  dissectResult.value = subTopic;

  //Serial.print("dissectPayload subTopic = "); Serial.println(str);

  int index = -1;
  int i = 0;
  do{
   index = str.indexOf("/");
   if (index != -1){
      Topics[i] = str.substring(0, index);
      //Serial.println("Topic[" + String(i) + "] = " + Topics[i]);
	  str.remove(0, index +1);
	  i++;
	  if (str.indexOf("/") == -1 and str.length() > 0){    //best of the rest
	    index = -1;
		Topics[i] = str;
		//Serial.println("Topic[" + String(i) + "] = " + Topics[i]);
		i++;
	  }
	}else{
	}
  } while (index != -1);
  int depth = i;
  int countDepth = 0;
  //Serial.println("TreeDepth = " + String(i));

  //find item index
  String itemPath = "";
  bool found_e1 = false;
  if (depth > 1 and Topics[1] != NULL){
    for (int i = 0; i < topic.sub.E1.count; i++) {
	  if (topic.sub.E1.item[i] != NULL){
	    	//Serial.println(topic.sub.E1.item[i] + " / " + Topics[1]);
	    if (topic.sub.E1.item[i] == Topics[1]){
	    	//Serial.println("----> gefunden!");
	    	//Serial.println("");
          dissectResult.E1 = i;
		  itemPath = String(i);
		  dissectResult.found = true;
		  found_e1 = true;
		  countDepth++;
		  break;
        }else dissectResult.found = false;
	  }
    }
  }

  bool found_e2 = false;
  if (depth > 2 and Topics[2] != NULL and found_e1){
    for (int i = 0; i < topic.sub.E2.count; i++) {
	  if (topic.sub.E2.item[dissectResult.E1][i] != NULL){
	    	//Serial.println(topic.sub.E2.item[dissectResult.E1][i] + " / " + Topics[2]);
	    if (topic.sub.E2.item[dissectResult.E1][i] == Topics[2]){
	    	//Serial.println("----> gefunden!");
	    	//Serial.println("");
          dissectResult.E2 = i;
		  itemPath += "/";
		  itemPath += String(i);
		  dissectResult.found = true;
		  found_e2 = true;
		  countDepth++;
		  break;
        }else dissectResult.found = false;
	  }
    }
  }

  bool found_e3 = false;
  if (depth > 3 and Topics[3] != NULL and found_e1 and found_e2){
    for (int i = 0; i < topic.sub.E3.count; i++) {
	  if (topic.sub.E3.item[dissectResult.E1][dissectResult.E2][i] != NULL){
	    	//Serial.println(topic.sub.E3.item[dissectResult.E1][dissectResult.E2][i] + " / " + Topics[3]);
	    if (topic.sub.E3.item[dissectResult.E1][dissectResult.E2][i] == Topics[3]){
	    	//Serial.println("----> gefunden!");
	    	//Serial.println("");
          dissectResult.E3 = i;
		  itemPath += "/";
		  itemPath += String(i);
		  dissectResult.found = true;
		  found_e3 = true;
		  countDepth++;
		  break;
        }else dissectResult.found = false;
	  }
    }
  }
  if (depth == countDepth+1){
	  Serial.println("dissectResult found!");
	  dissectResult.itemPath = itemPath;

  }else{
	  dissectResult.found = false;
	  Serial.println("no dissectResult found!");
  }
  return dissectResult;
}

void checkMQTTDisconnect(TcpClient& client, bool flag) {
	Serial.println("#### checkMQTTDisconnect ");
}

//#####################################################################
//MQTT Routines
//===============================================================================
//  MQTT Control
//===============================================================================
//===> publish Topics <--------------------------------------------------------


void ApplicationMQTTClient::pub() {
	mqtt->publish("ESP/Sming", "Hello friends, from Internet of things :)"); // or publishWithQoS
}


bool ApplicationMQTTClient::pub(int e1, String Payload){
  String strTopic = buildE1(e1);
  mqtt->publish(strTopic, Payload);
}
bool ApplicationMQTTClient::pub(int e1, int e2, String Payload){
  String strTopic = buildE2(e1, e2);
  mqtt->publish(strTopic, Payload);
}
bool ApplicationMQTTClient::pub(int e1, int e2, int e3, String Payload){
  String strTopic = buildE3(e1, e2, e3);
  mqtt->publish(strTopic, Payload);
}
/*
bool ApplicationMQTTClient::pub(int e1, int e2, int e3, int e4, char* Payload){
  String strTopic = buildE4(e1, e2, e3, e4);
  mqtt_client.publish(strTopic.c_str(), Payload);
  mqtt_client.loop();
}
*/
//===> build Topics <----------------------------------------------------------


String ApplicationMQTTClient::buildE1(int e1){
  String strTopic = app.cfg.network.ap.ssid;  //TODO MQTT_DeviceName
  strTopic += "/";
  strTopic += topic.pub.E1.item[e1];
  return strTopic;
}
String ApplicationMQTTClient::buildE2(int e1, int e2){
  String strTopic = buildE1(e1);
  strTopic += "/";
  strTopic += topic.pub.E2.item[e1][e2];
  return strTopic;
}
String ApplicationMQTTClient::buildE3(int e1, int e2, int e3){
  String strTopic = buildE2(e1, e2);
  strTopic += "/";
  strTopic += topic.pub.E3.item[e1][e2][e3];
  return strTopic;
}
/*
String ApplicationMQTTClient::buildE4(int e1, int e2, int e3, int 43){
  String strTopic = buildE1(e1);
  strTopic += "/";
  strTopic += buildE2(e1, e2);
  strTopic += "/";
  strTopic += buildE3(e1, e2, e3);
  strTopic += "/";
  strTopic += topic.pub.E4.item[e1][e2][e3][e4];
  return strTopic;
}
*/


//#####################################################################
//MQTT helpers

TparameterResult ApplicationMQTTClient::dissectParameter(String Parameter){

	TparameterResult par;
	par.count = 0;
    for (int i = 0; i < 11; i++) {
    	par.item[i] = "";
    }

    String str = Parameter;

    int index = -1;
    int i = 0;

    do{
     index = str.indexOf(",");
     if (index != -1){
    	 par.item[i] = str.substring(0, index);
         //Serial.println("Parameter[" + String(i) + "] = " + par.item[i]);
         str.remove(0, index +1);
         i++;
         if (str.indexOf(",") == -1 and str.length() > 0){    //best of the rest
        	 index = -1;
        	 par.item[i] = str;
        	 //Serial.println("Parameter[" + String(i) + "] = " + par.item[i]);
        	 i++;
         }
  	  }else{
  	  }
    } while (index != -1);

    par.count = i;
    return par;

}


