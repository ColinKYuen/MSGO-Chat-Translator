//g++ main.cpp -o Output -lcurl 
#include <curl/curl.h>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

const string API_Key = "trnsl.1.1.20200422T024223Z.8ed8c69f7072557a.e285aeb7a16dd7816a09810c641e040ae835269b";

static size_t writeCallback(void *data, size_t size, size_t nmemb, void* userp);
void translate(string text);
string trimBuffer(string buffer);
string translateText(string text);
string getURL(string text);

int main(){
    //Things to Do - In Order
    //Either: Ask User to find MSGO / File search in program
    //Open Latest File
    //Go to end of the -> Loop until file is modified / updated -> Read Line (Print left of ":" & Translate Right side)
    translate("さっきだれか撃ってるようにみえた");
    return 0;
}
 
static size_t writeCallback(void *data, size_t size, size_t nmemb, void* userp){
    ((string*)userp)->append((char*)data, size * nmemb);
    return size * nmemb;
}

void translate(string text){
    CURL *curl;
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
    return buffer.substr(op + 1, buffer.find("]", op) - op - 1);
}

string getURL(string text){
    return "https://translate.yandex.net/api/v1.5/tr.json/translate?key=" 
    + API_Key 
    + "&text=" + text 
    + "&lang=ja-en"
    + "&[options=plain]";
}

/*
    Get the whole line
    Splice with :
    Print on the left as normal, translate then print the right
*/
/*
    fstream file;
    seekg(ios::end);
*/

/*
    Structure
        - Set Up Curl
        - Load Chat Log
        - Start Loop
*/