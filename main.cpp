//g++ -pedantic -Wall -Wextra std=c++14 main.cpp -o test1
#include <curl/curl.h>
#include <iostream>
#include <fstream>

using namespace std;

string API_Key = "trnsl.1.1.20200422T024223Z.8ed8c69f7072557a.e285aeb7a16dd7816a09810c641e040ae835269b";

void translate(string text);
string translateText(string text);

int main(){
//    translate("花");
    return 0;
}

void translate(string text){
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, getURL(text));

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
        }
        curl_easy_strerror(res));

        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

string getURL(string text){
    return "https://translate.yandex.net/api/v1.5/tr.json/translate?key=" 
    + API_Key 
    + "&text=" + text 
    + "&lang=jp-en"
    + "&[options=plain]";
}


// https://translate.yandex.net/api/v1.5/tr.json/translate?key=trnsl.1.1.20200422T024223Z.8ed8c69f7072557a.e285aeb7a16dd7816a09810c641e040ae835269b&text=花&lang=ja-en

/*
    Get the whole line
    Splice with :
    Print on the left as normal, translate then print the right
*/
/*
    fstream file;
    seekg(ios::end);
*/