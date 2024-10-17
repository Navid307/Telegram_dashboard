#include <curl/curl.h>
#include <json/json.h>
#include <regex>

#include <iostream>

#define MODULE_NAME "Telegram"

class Telegram : public IMessageService {
 public:
  Telegram() {
    this->token = BOT_TOKEN;
    this->chat_id = CHAT_ID;
  }

  void sendMessage(std::string message) override {
    std::string resp;
    
    // Replace space with %20 and next line with %0D%0A
    std::regex space("\\s");
    std::regex next_line("\\n");
    message = std::regex_replace(message, space, "%20");
    message = std::regex_replace(message, next_line, "%0D%0A");
    
    std::string url = "https://api.telegram.org/bot" + this->token +
                      "/sendMessage?chat_id=" + this->chat_id +
                      "&text=" + message;

    if (performCurlRequest(url, resp)) {
      Json::Value root;
      if (parseJsonResponse(resp, root)) {
        const Json::Value res = root["result"];
      }
    }
    curl_global_cleanup();
  }

  std::string getUpdate() override {
    std::string resp;
    std::string url = "https://api.telegram.org/bot" + this->token +
                      "/getUpdates?limit=1&offset=-1";
    if (performCurlRequest(url, resp)) {
      Json::Value root;
      if (parseJsonResponse(resp, root)) {
        const Json::Value res = root["result"];
        int update_id = res[0]["update_id"].asInt64();
        if (res[0]["update_id"].asInt64() != 0) {
          return res[0]["message"]["text"].asString();
        }
      }
    }
    return "";
  }

  void sendDocument(const std::string &documentPath,
                    const std::string &caption = "caption") override {
    CURL *curl;
    CURLcode res;

    std::string url = "https://api.telegram.org/bot" + this->token +
                      "/sendDocument?chat_id=" + this->chat_id +
                      "&caption=" + caption;

    curl_mime *form = NULL;
    curl_mimepart *field = NULL;
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
      form = curl_mime_init(curl);

      field = curl_mime_addpart(form);
      curl_mime_name(field, "document");
      curl_mime_filedata(field, documentPath.c_str());

      field = curl_mime_addpart(form);
      curl_mime_name(field, "submit");
      curl_mime_data(field, "send", CURL_ZERO_TERMINATED);

      headerlist = curl_slist_append(headerlist, buf);

      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

      res = curl_easy_perform(curl);
      if (res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

      curl_easy_cleanup(curl);

      curl_mime_free(form);
      curl_slist_free_all(headerlist);
    }
  }

 private:
  std::string token;
  std::string chat_id;

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
  }

  static bool performCurlRequest(const std::string &url,
                                 std::string &response) {
    CURL *curl = curl_easy_init();
    if (!curl) {
      std::cerr << "Failed to initialize CURL" << std::endl;
      return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
      return false;
    }

    return true;
  }

  static bool parseJsonResponse(const std::string &jsonResponse,
                                Json::Value &parsedRoot) {
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    std::string errs;

    bool parsingSuccessful = reader->parse(
        jsonResponse.c_str(), jsonResponse.c_str() + jsonResponse.size(),
        &parsedRoot, &errs);
    delete reader;

    if (!parsingSuccessful) {
      std::cerr << "Failed to parse JSON: " << errs << std::endl;
      return false;
    }

    return true;
  }

  static bool performCurlRequestMultipart(const std::string &url,
                                          std::string &response,
                                          const std::string &postData) {
    CURL *curl = curl_easy_init();
    if (!curl) {
      std::cerr << "Failed to initialize CURL" << std::endl;
      return false;
    }

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: multipart/form-data");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postData.size());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
      return false;
    }

    return true;
  }
};