#include "ggwget/Downloader.hpp"

#include <curl/curl.h>

#include <fstream>
#include <stdexcept>
#include <string>

namespace ggwget {
namespace {

struct CurlGlobalInit {
    CurlGlobalInit() {
        const auto code = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (code != CURLE_OK) {
            throw std::runtime_error("curl_global_init failed");
        }
    }

    ~CurlGlobalInit() { curl_global_cleanup(); }
};

CurlGlobalInit& curl_guard() {
    static CurlGlobalInit guard;
    return guard;
}

size_t write_to_stream(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* stream = static_cast<std::ostream*>(userdata);
    const auto total = size * nmemb;
    stream->write(ptr, static_cast<std::streamsize>(total));
    if (!stream->good()) {
        return 0;
    }
    return total;
}

void perform_download(const std::string& url, std::ostream& output) {
    curl_guard();

    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("curl_easy_init failed");
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_stream);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

    const auto result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(result));
    }
}

}  // namespace

void download_to_stream(const std::string& url, std::ostream& output) {
    perform_download(url, output);
}

void download_to_file(const std::string& url, const std::filesystem::path& path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("unable to open file for writing: " + path.string());
    }

    perform_download(url, file);
}

}  // namespace ggwget
