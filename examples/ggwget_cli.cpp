#include "ggwget/Downloader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

namespace {

void print_usage(std::string_view program) {
    std::cerr << "Usage: " << program << " [-o <output_path>] <url>\n"
              << "  -o <output_path>   Write response to the provided file instead of stdout\n"
              << "  -h, --help         Show this message\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string output_path;
    std::string url;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg{argv[i]};
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        }

        if (arg == "-o") {
            if (i + 1 >= argc) {
                std::cerr << "error: -o requires an output path\n";
                return 1;
            }
            output_path = argv[++i];
            continue;
        }

        if (!arg.empty() && arg.front() == '-') {
            std::cerr << "error: unrecognised option: " << arg << "\n";
            return 1;
        }

        url = arg;
    }

    if (url.empty()) {
        std::cerr << "error: missing url\n";
        print_usage(argv[0]);
        return 1;
    }

#ifdef _WIN32
    if (output_path.empty()) {
        _setmode(_fileno(stdout), _O_BINARY);
    }
#endif

    try {
        if (output_path.empty()) {
            ggwget::download_to_stream(url, std::cout);
            std::cout.flush();
        } else {
            ggwget::download_to_file(url, output_path);
            std::cerr << "saved to "
                      << std::filesystem::path(output_path).lexically_normal().string() << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
