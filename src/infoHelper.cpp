#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "infoHelper.h"

WiFiClient netClient;

extern char boardID[23];

void checkDeviceExist()
{
    HTTPClient http;
    JsonDocument doc;
    http.addHeader("X-Secret-Key", String(APPAPIKEY));
    String queryURL = String(APPAPI) + "/checkexist?u_id=" + String(boardID);

    Serial.println("Will connect " + queryURL);

    http.begin(netClient, queryURL.c_str());

    int httpResponseCode = http.GET();

    if(httpResponseCode == 204) //code no content => info not exist
    {
        Serial.println("code no content => info not exist");
        http.end();
        signInfo();
    }
    else if(httpResponseCode == 200){ //info exist, check firm_ver
        Serial.println("info exist, check firm_ver");
        JsonDocument doc;
        deserializeJson(doc, http.getString());
        if(String(APPVERSION) != doc["firm_ver"]){
            http.end();
            updateFirmver();
        }
    }
    else{
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println("Response " + payload);
        
    }
}

void signInfo()
{
    HTTPClient http;
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Secret-Key", String(APPAPIKEY));
    String queryURL = String(APPAPI) + "/data?u_id=" + String(boardID);
    Serial.println("Will connect " + queryURL);
    http.begin(netClient, queryURL.c_str());
    JsonDocument doc;
    doc["u_id"] = String(boardID);
    doc["device_type"] = String(APPDEVTYPE);
    doc["firm_ver"] = String(APPVERSION);

    String httpRequestData;
    serializeJson(doc, httpRequestData);
    Serial.print(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

void updateFirmver()
{
    HTTPClient http;
    http.addHeader("X-Secret-Key", String(APPAPIKEY));
    String queryURL = String(APPAPI) + "/firmware?u_id=" + String(boardID);
    http.begin(netClient, queryURL.c_str());
    http.addHeader("Content-Type", "application/json");
    JsonDocument doc;
    doc["firm_ver"] = String(APPVERSION);

    String httpRequestData;
    serializeJson(doc, httpRequestData);
    Serial.print(httpRequestData);
    int httpResponseCode = http.PUT(httpRequestData);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    }
    else
    {
        Serial.print("Error on sending PUT: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}



// void getNTP()
// {
//   configTime(this->gmtOffset_sec, this->daylightOffset_sec, this->ntpServer.c_str());
//   printLocalTime();
// }

void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// bool getExtraInfo()
// {
//     HTTPClient http;

//     JsonDocument doc;

//     String queryURL = String(APPAPI) + "/getExtInfo?key=" + String(APPAPIKEY) + "&code=" + String(boardID);

//     Serial.println("Will connect " + queryURL);

//     http.begin(netClient, queryURL.c_str());

//     int httpResponseCode = http.GET();

//     if (httpResponseCode > 0)
//     {
//         Serial.print("HTTP Response code: ");
//         Serial.println(httpResponseCode);
//         String payload = http.getString();

//         Serial.println("Response " + payload);

//         DeserializationError error = deserializeJson(doc, payload);

//         if (error)
//         {
//             Serial.print(F("deserializeJson() failed: "));
//             Serial.println(error.f_str());
//             http.end();
//             return false;
//         }
//     }
//     else
//     {
//         http.end();
//         return false;
//     }

//     http.end();

//     return true;
// }