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
#ifndef APP_MQTTCLIENT_H_
#define APP_MQTTCLIENT_H_

//#####################################################################
//MQTT Topics
typedef String topicField;

//publish struct
const int pub_e1 = 5;			//define TreeDepht here!!!
const int pub_e2 = 5;
const int pub_e3 = 8;
const int pub_e4 = 0;

struct Tpub_topicE1{
  topicField item[pub_e1];
	int count;
};
struct Tpub_topicE2{
  topicField item[pub_e1][pub_e2];
	int count;
};
struct Tpub_topicE3{
  topicField item[pub_e1][pub_e2][pub_e3];
	int count;
};

struct Tpub_Topic{
  Tpub_topicE1 E1;
  Tpub_topicE2 E2;
  Tpub_topicE3 E3;
};

//subscribe struct
const int sub_e1 = 5;			//define TreeDepht here!!!
const int sub_e2 = 5;
const int sub_e3 = 8;
const int sub_e4 = 0;

struct Tsub_topicE1{
  topicField item[sub_e1];
	int count;
};
struct Tsub_topicE2{
  topicField item[sub_e1][sub_e2];
	int count;
};
struct Tsub_topicE3{
  topicField item[sub_e1][sub_e2][sub_e3];
	int count;
};

struct Tsub_Topic{
  Tsub_topicE1 E1;
  Tsub_topicE2 E2;
  Tsub_topicE3 E3;
};


class Topics{
public:
Topics();
Tpub_Topic pub;
Tsub_Topic sub;
private:
};


struct TdissectResult{
  bool found = false;
  String topic = "";
	String value = "";
	String itemPath = "";
	int treeDepth = 0;
  int E1 = 0;
  int E2 = 0;
  int E3 = 0;
  int E4 = 0;
};

typedef String parameter;
struct TparameterResult{
  parameter item[10];
  int count = 0;
};



//#####################################################################
//MQTT Routines

//#####################################################################
//MQTT helpers




class ApplicationMQTTClient{

public:
	ApplicationMQTTClient();
	virtual ~ApplicationMQTTClient();

	void start();
	void stop();
	bool isRunning();
	void pub();

	bool pub(int e1, String Payload);
	bool pub(int e1, int e2, String Payload);
	bool pub(int e1, int e2, int e3, String Payload);

	void onMessageReceived(String topic, String message);
	void checkMQTTDisconnect(TcpClient& client, bool flag);
	void timer_checkConnection();

private:
	MqttClient* mqtt;

	bool _running = false;
	//Timer* procTimer;

	//MQTT-Control-----------------------
	Topics topic;
	String buildE1(int e1);
	String buildE2(int e1, int e2);
	String buildE3(int e1, int e2, int e3);
	TdissectResult dissectPayload(String subTopic, String subValue);
	void publishSettings();

	TparameterResult dissectParameter(String Parameter);
	HSVCT hsv;
	ChannelOutput raw;

};




#endif // APP_MQTTCLIENT_H_
