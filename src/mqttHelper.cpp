
#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char *ssid = "TTi Factory";
const char *password = "ttiFactory19@1";
const char *mqtt_server = "10.137.37.59";
const char *mqtt_user = "esp32_chamber";
const char *mqtt_pass = "4PRms4jc8WtXdx";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
extern char boardID[23];

void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        char connect_status[150];
        snprintf(connect_status, sizeof(connect_status), "{\"status\":\"%s\", \"client\": \"%s\"}", "Disconnected", boardID);
        if (client.connect(boardID, mqtt_user, mqtt_pass, ("/ConnectStatus/" + String(boardID)).c_str(), 1, true, connect_status))
        {
            Serial.println("MQTT connected");
            // Once connected, publish an announcement...
            snprintf(connect_status, sizeof(connect_status), "{\"status\":\"%s\", \"client\": \"%s\", \"appver\": \"%s\"}", "Connected", boardID, APPVERSION);
            client.publish(("/ConnectStatus/" + String(boardID)).c_str(), connect_status, true);
            // ... and resubscribe
            client.subscribe("inTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
void setup_mqtt()
{
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void mqttLoop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}

// void setup() {
//   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
//   Serial.begin(115200);
//   setup_wifi();
//   client.setServer(mqtt_server, 1883);
//   client.setCallback(callback);
// }

// void loop() {

//   if (!client.connected()) {
//     reconnect();
//   }
//   client.loop();

//   unsigned long now = millis();
//   if (now - lastMsg > 2000) {
//     lastMsg = now;
//     ++value;
//     snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
//     Serial.print("Publish message: ");
//     Serial.println(msg);
//     client.publish("outTopic", msg);
//   }
// }