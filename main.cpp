//g++ -Wall -Wextra -std=c++17 main.cpp -o Output -lcurl
//x86_64-w64-mingw32-g++ -std=c++17 -I/home/colinyuen/myLib/lib -o OutputTest.exe main.cpp -L/home/colinyuen/myLib/lib -lcurl -static-libgcc -static-libstdc++

#include <curl/curl.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <filesystem>
#include <locale>

using namespace std;

string API_Key = "trnsl.1.1.20200422T024223Z.8ed8c69f7072557a.e285aeb7a16dd7816a09810c641e040ae835269b";
filesystem::path msgoPath;
filesystem::directory_entry latestLog;
filesystem::file_time_type lastestUpdate;
int length;

//Finding Latest File & Reading
void readConfig();
void openLatestLog();
void readFile();

//Translation Part
static size_t writeCallback(void *data, size_t size, size_t nmemb, void* userp);
void translate(string text);
string trimBuffer(string buffer);
string getURL(const string text);

int main(){
    /*

        If(MSGO is not open){
            While MSGO is not open{
                Wait
            }
            openLatestLog();
        }
        
    */
    
    return 0;
}

void readConfig(){

}

void openLatestLog(){
    filesystem::path path = msgoPath / "Chat";
    for(const auto& entry : filesystem::directory_iterator(path)){
        latestLog = entry;
    }

    for(const auto& entry : filesystem::directory_iterator(path)){
        if(entry.last_write_time() > filesystem::last_write_time(latestLog.path())){
            latestLog = entry;
        }
    }
}

void readFile(){
    locale::global(locale(""));
    wifstream fin(/*latestLog.path()*/"test.log", ios::binary);
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, consume_header>));
    ostringstream out;
    string utf8_string;
    int prevLength = length;
    fin.seekg(0, fin.end);
    length = fin.tellg();
    fin.seekg(0, prevLength);
    for(wchar_t c; fin.get(c);){
        wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
        utf8_string.append(convert.to_bytes(c));
        if(utf8_string.find("：", 10) != string::npos){
            cout << utf8_string;
            utf8_string.clear();
        }
        if(utf8_string.find("\n") != string::npos){
            translate(utf8_string);
            utf8_string.clear();
        }
    }
}

static size_t writeCallback(void *data, size_t size, size_t nmemb, void* userp){
    ((string*)userp)->append((char*)data, size * nmemb);
    return size * nmemb;
}

void translate(string text){
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl){
        string readBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, getURL(text).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        cout << trimBuffer(readBuffer) << endl;
    }
    curl_global_cleanup();
}

string trimBuffer(string buffer){
    int op = buffer.find("[", 0);
    return buffer.substr(op + 2, buffer.find("}", op) - op - 6);
}

string getURL(const string text){
    CURL* curl = curl_easy_init();
    char* output;
    if(curl){
        output = curl_easy_escape(curl, text.c_str(), 0);
    }
    
    return "https://translate.yandex.net/api/v1.5/tr.json/translate?key=" 
    + API_Key 
    + "&text=" + string(output)
    + "&lang=ja-en"
    + "&[options=plain]";
}