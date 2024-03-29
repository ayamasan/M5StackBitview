#include <M5Stack.h>
#include "ArduinoJson.h"
#include <WiFi.h>
#include <HTTPClient.h>

// WIFI設定
const char *ssid = "xxxxxxx";
const char *password = "xxxxxxx";
// ビットコイン値段取得
const char *apiServer = "https://api.coindesk.com/v1/bpi/currentprice/JPY.json";

double bitcoin = 0.00538737;   // 所有ビットコイン

void setup() {
	// put your setup code here, to run once:
	M5.begin();
	M5.Lcd.fillScreen(0);       // 表示クリア
	M5.Lcd.setTextFont(4);      // フォント
	M5.Lcd.setTextColor(WHITE, BLACK);
	M5.Lcd.println(" WiFi connecting...");
	
	// WiFi接続
	WiFi.begin(ssid, password);
	int i = 0;
	while(WiFi.status() != WL_CONNECTED){
		delay(100);
		i++;
		if(i>100){  // 10secタイムアウト
			M5.Lcd.setTextColor(BLACK, RED);
			M5.Lcd.println(" ERROR : cannot connect to WiFi. ");
			break;
		}
	}
	// WiFi接続成功
	if(i <= 100){
		M5.Lcd.println(" Connected.");
	}
	
}

void loop() {
	// put your main code here, to run repeatedly:
	if((WiFi.status() == WL_CONNECTED)){
		HTTPClient http;
		
		// HTTP接続開始
		http.begin(apiServer);
		
		// リクエスト作成
		int httpCode = http.GET();
		
		// 返信
		if(httpCode > 0){
			// 応答取得
			String payload = http.getString();
			// json用メモリ確保
			DynamicJsonDocument doc(1024);
			// ペイロードをjson変換
			deserializeJson(doc, payload);
			JsonObject obj = doc.as<JsonObject>();
			// bpi.JPY
			JsonObject result = obj[String("bpi")][String("JPY")];
			// ビットコイン表示更新
			double rate = result[String("rate_float")];
			M5.Lcd.setCursor(0, 50);
			M5.Lcd.setTextColor(BLACK, GREEN);
			M5.Lcd.printf(" %d yen ", (int)(bitcoin * rate));
		}
		else{
			M5.Lcd.setTextColor(BLACK, RED);
			M5.Lcd.printf(" HTTP request error! ");
			//return;
		}
		M5.update();
		
		http.end();
		delay(5000);
	}
}
